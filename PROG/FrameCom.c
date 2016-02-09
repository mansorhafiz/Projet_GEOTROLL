/***************************************************************************/
/*                                                                         */
/*                               FrameCom.c                                */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* D�codage/encodage trames pour dialogue sur port s�rie                   */
/*                                                                         */
/* - Initialisation                                                        */
/*      void frmcom_Init( void ) ;                                         */
/*  - D�-initialisation                                                    */
/*      void frmcom_UnInit( void ) ;                                       */
/*                                                                         */
/* - Lecture trame re�ue                                                   */
/*      aRESULT frmcom_Receive( s_Frame * o_pFrm ) ;                       */
/* - Fin lecture trame re�ue                                               */
/*      void frmcom_FreeReceive( void ) ;                                  */
/*                                                                         */
/* - Lecture trame pour �mission                                           */
/*      aRESULT frmcom_GetSend( s_Frame * o_pFrm ) ;                       */
/* - Emission trame                                                        */
/*      aRESULT frmcom_Send( s_Frame * i_pFrm ) ;                          */
/* - Lib�ration trame pour �mission                                        */
/*      void frmcom_FreeSend( void ) ;                                     */
/*                                                                         */
/* - T�che si necessaire pour dialogue sur port s�rie                      */
/*      void frmcom_TaskCyc( void ) ;                                      */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 19.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* D�finitions                                                             */
/*-------------------------------------------------------------------------*/

#define FRMCOM_ESC         0x1B  /* synchro d�but trame */
#define FRMCOM_TYPECMD     0x40  /* synchro type commande */
#define FRMCOM_TYPERESP    0xC0  /* synchro type r�ponse */
#define FRMCOM_FIRSTCMD    0x80  /* code 1�re commande */

#define FRMCOM_DURMAXBYTE    15  /* dur�e maximum entre r�ception de 2 octets
                                    pour consid�rer qu'il s'agit de la m�me
                                    trame, 10 ms */

typedef enum                     /* phase r�ception */
{
   FRMCOM_PHR_CLOSE = 0,         /* canal communication ferm� */
   FRMCOM_PHR_WAITESC,           /* attente ESC */
   FRMCOM_PHR_WAITTYPE,          /* attente type trame */
   FRMCOM_PHR_WAITCMD,           /* attente code commande */
   FRMCOM_PHR_WAITLEN,           /* attente longueur donn�es */
   FRMCOM_PHR_RECDATA,           /* r�ception donn�es et Crc8 */
   FRMCOM_PHR_RECEIVED,          /* trame re�ue en attente lecture */
   FRMCOM_PHR_READED             /* trame lue, attente fin lecture */
} e_FramePhaseReceive ;

typedef enum                     /* phase �mission */
{
   FRMCOM_PHS_CLOSE = 0,         /* canal communication ferm� */
   FRMCOM_PHS_FREE,              /* �mission disponible */
   FRMCOM_PHS_PREPARE            /* trame � �mettre en cours pr�paration */
} e_FramePhaseSend ;

typedef struct                   /* trame re�ue ou � �mettre */
{
   char cEsc ;                   /* caract�re syncho d�but trame, Esc */
   char cType ;                  /* caract�re type trame, FRMCOM_TYPExxx */
   BYTE byCmdRep ;               /* code commande ou code r�ponse */
   BYTE byLenData ;              /* longueur des donn�es */
   BYTE abyData [100] ;          /* donn�es */
   BYTE byCrc8 ;                 /* Crc8 de <cEsc> � dernier octet donn�es */
} s_FrameBuf ;

/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

e_FramePhaseReceive l_ePhRec ;   /* phase r�ception, FRMCOM_PHR_xxx */
BYTE l_byIdxRecData ;            /* index donn�es trame re�ue */
WORD l_wTmpMaxRec ;              /* tempo attente entre r�ception 2 octets, 10 ms */
BYTE l_byMaxLenReceive ;         /* longueur maximum donn�es dans trame r�ception */
e_FramePhaseSend l_ePhSend ;     /* phase �mission, FRMCOM_PHS_xxx */
BYTE l_byMaxLenSend ;            /* longueur maximum donn�es dans trame �mission */
s_FrameBuf l_Receive ;           /* trame re�ue */
s_FrameBuf l_Send ;              /* trame � �mettre */
e_FrameCh l_eCh ;                /* canal */


/*-------------------------------------------------------------------------*/
/* Fonctions prototypes                                                    */
/*-------------------------------------------------------------------------*/

static void ua_Open( DWORD i_dwBaudRate) ;
static void ua_Close( void ) ;

static aBYTE ua_Receive( void * o_pvBuf, aBYTE i_byMaxSize ) ;
static aBYTE ua_GetMaxReceiveSize( void ) ;

static aRESULT ua_Send( void C* i_pvBuf, aBYTE i_bySize ) ;
static aBYTE ua_GetMaxSendSize( void ) ;

static void frmcom_ProcessReceive( void ) ;



/*-------------------------------------------------------------------------*/
/* Initialisation                                                          */
/*-------------------------------------------------------------------------*/

void frmcom_Init( void )
{
   aBYTE byMaxReceive ;
   aBYTE byMaxSend ;

   l_ePhRec = FRMCOM_PHR_WAITESC ;        /* attente r�ception ESC */
   l_ePhSend = FRMCOM_PHS_FREE ;          /* �mission disponible */
   l_eCh = FRM_CH_USB ;

   ua_Open( 115200 ) ;                   /* ouverture port uart */
   
                                          /* longueur maximum r�ception */
   byMaxReceive = ua_GetMaxReceiveSize() ;
                                          /* limite � longueur trame */
   if ( byMaxReceive > sizeof(l_Receive) )
      byMaxReceive = sizeof(l_Receive) ;
                                          /* suppression encapsulation donn�es */
   byMaxReceive -= sizeof(l_Receive) - sizeof(l_Receive.abyData) ;
   l_byMaxLenReceive = byMaxReceive ;     /* m�mo. longueur maximum donn�es r�ception */

   byMaxSend = ua_GetMaxSendSize() ;     /* longueur maximum �mission */
   if ( byMaxSend > sizeof(l_Send) )      /* limite � longueur trame */
      byMaxSend = sizeof(l_Send) ;
                                          /* suppression encapsulation donn�es */
   byMaxSend -= sizeof(l_Send) - sizeof(l_Send.abyData) ;
   l_byMaxLenSend = byMaxSend ;           /* m�mo. longueur maximum donn�es �mission */
}



/*-------------------------------------------------------------------------*/
/* De-initialisation                                                       */
/*-------------------------------------------------------------------------*/

void frmcom_UnInit( void )
{

   l_ePhRec = FRMCOM_PHR_CLOSE ;          /* canal communication ferm� */
   l_ePhSend = FRMCOM_PHS_CLOSE ;

   ua_Close( ) ;                       /* fermeture port uart */
}



/*-------------------------------------------------------------------------*/
/* Lecture trame re�ue                                                     */
/*    - <o_pFrm> adresse pour retour trame lue                             */
/* Retour :                                                                */
/*    - OK trame re�ue, besoin appel frmcom_FreeReceive() apr�s fin        */
/*         traitement trame                                                */
/*    - ERR pas de trame re�ue ou identificateur canal incorrect           */
/*-------------------------------------------------------------------------*/

aRESULT frmcom_Receive( s_Frame * o_pFrm )
{
   aRESULT rRet ;

                                          /* si canal ouvert et trame pas re�ue */
   if ( l_ePhRec != FRMCOM_PHR_CLOSE && l_ePhRec != FRMCOM_PHR_RECEIVED &&
        l_ePhRec != FRMCOM_PHR_READED )
   {
      frmcom_ProcessReceive() ;           /* traitement r�ception trame */
   }

                                          /* si trame re�ue */
   if ( l_ePhRec == FRMCOM_PHR_RECEIVED )
   {                                      /* indication trame lue */
      l_ePhRec = FRMCOM_PHR_READED ;
                                          /* retour code commande */
      o_pFrm->byCmdRep = l_Receive.byCmdRep ;
                                          /* retour longueur donn�es */
      o_pFrm->byLen = l_Receive.byLenData ;
                                          /* retour adresse donn�es */
      o_pFrm->pbyData = l_Receive.abyData ;

      rRet = OK ;                         /* trame re�ue */
   }
   else                                   /* pas trame re�ue */
      rRet = ERR ;

   return rRet ;
}



/*-------------------------------------------------------------------------*/
/* Fin lecture trame re�ue                                                 */
/*-------------------------------------------------------------------------*/

void frmcom_FreeReceive( void )
{
   if ( l_ePhRec == FRMCOM_PHR_READED )
      {                                   /* retour � attente synchro d�but trame */
         l_ePhRec = FRMCOM_PHR_WAITESC ;
      }
}



/*-------------------------------------------------------------------------*/
/* Lecture trame pour �mission                                             */
/*    - <o_pFrm> adresse pour retour trame pr�par�e pour �mission          */
/* Retour :                                                                */
/*    - OK trame correctement pr�par�e pour �mission                       */
/*    - ERR pas de trame disponible pour �mission ou identificateur canal  */
/*          incorrect                                                      */
/* Note : <o_pFrm->byLen> indique la longueur maximum des donn�es de la    */
/* trame, c'est � dire la taille du buffer point� par <o_pFrm->pbyData>.   */
/*-------------------------------------------------------------------------*/

aRESULT frmcom_GetSend( s_Frame * o_pFrm )
{
   aRESULT rRet ;

                                          /* si trame �mission disponible */
   if ( l_ePhSend == FRMCOM_PHS_FREE )
   {                                      /* trame �mission pr�par�e */
      l_ePhSend = FRMCOM_PHS_PREPARE ;

      o_pFrm->byCmdRep = 0 ;              /* code r�ponse inconnu */
                                          /* longueur maximum des donn�es */
      o_pFrm->byLen = l_byMaxLenSend ;
                                          /* adresse pour donn�es � �mettre */
      o_pFrm->pbyData = l_Send.abyData ;

      rRet = OK ;
   }
   else                                   /* trame �mission non disponible */
      rRet = ERR ;

   return rRet ;
}



/*-------------------------------------------------------------------------*/
/* Emission trame                                                          */
/*    - <i_pFrm> trame � �mettre                                           */
/* Retour :                                                                */
/*    - OK trame en cours �mission                                         */
/*    - ERR probl�me �mission : pas de trame � �mettre pr�par�e,           */
/*          identificateur canal incorrect ou pas assez de place dans      */
/*          buffer �mission                                                */
/* Note : la trame � �mettre <i_pFrm> doit obligatoiremnet �tre obtenue    */
/* par un appel � frmcom_GetSend().                                        */
/*-------------------------------------------------------------------------*/

aRESULT frmcom_Send( s_Frame C* i_pFrm )
{
   aRESULT rRet ;
   aBYTE byLen ;
   aBYTE byCrc8 ;

                                          /* si trame pr�par�e */
   if ( l_ePhSend == FRMCOM_PHS_PREPARE )
   {                                      /* trame �mission disponible */
      l_ePhSend = FRMCOM_PHS_FREE ;

      l_Send.cEsc = FRMCOM_ESC ;          /* octet synchro d�but trame */
                                          /* type r�ponse */
      l_Send.cType = FRMCOM_TYPERESP ;
                                          /* code r�ponse */
      l_Send.byCmdRep = i_pFrm->byCmdRep ;
      byLen = i_pFrm->byLen ;
      l_Send.byLenData = byLen ;          /* longueur donn�es */
                                          /* calcul et insertion Crc8 */
      byCrc8 = crc_Calc8( (BYTE *)&l_Send.cEsc, byLen + 4 ) ;
      l_Send.abyData[byLen] = byCrc8 ;

                                          /* �mission proprement dite trame */
      rRet = ua_Send( (BYTE C*)(&l_Send.cEsc), byLen + 5 ) ;
   }
   else                                   /* pas de trame � �mettre pr�par�e */
      rRet = ERR ;

   return rRet ;
}



/*-------------------------------------------------------------------------*/
/* Lib�ration trame pour �mission                                          */
/* Note : si lecture trame � �mettre avec frmcom_GetSend() et que par la   */
/* suite il n'y a pas de trame � �mettre et donc pas appel � frmcom_Send(),*/
/* besoin appel cette fonction avant de pouvoir faire un nouvel appel �    */
/* frmcom_GetSend().                                                       */
/*-------------------------------------------------------------------------*/

void frmcom_FreeSend( void)
{
   if ( l_ePhSend == FRMCOM_PHS_PREPARE )
   {                                      /* retour � trame �mission disponible */
      l_ePhSend = FRMCOM_PHS_FREE ;
   }
}



/*=========================================================================*/

/*-------------------------------------------------------------------------*/
/* Traitement r�ception sur canal 1                                        */
/*-------------------------------------------------------------------------*/

static void frmcom_ProcessReceive( void)
{
   e_FramePhaseReceive ePhRec ;
   aBYTE bySize ;
   aBYTE byIdxRecData ;
   aBYTE byCrc8 ;

   ePhRec = l_ePhRec ;                    /* phase r�ception */

   for ( ; ; )
   {
      switch ( ePhRec )                   /* selon phase r�ception */
      {
         case FRMCOM_PHR_WAITESC :        /* attente ESC */
                                          /* r�ception ESC */
            bySize = ua_Receive( &l_Receive.cEsc, 1 ) ;
            if ( bySize == 1 )            /* si r�ception correcte */
            {                             /* si r�ception ESC */
               if ( l_Receive.cEsc == FRMCOM_ESC )
               {                          /* passage � attente type trame */
                  ePhRec = FRMCOM_PHR_WAITTYPE ;
               }
            }
            break ;

         case FRMCOM_PHR_WAITTYPE :       /* attente type trame */
                                          /* r�ception type trame */
            bySize = ua_Receive( &l_Receive.cType, 1 ) ;
            if ( bySize == 1 )            /* si r�ception correcte */
            {                             /* si r�ception type commande */
               if ( l_Receive.cType == FRMCOM_TYPECMD )
               {                          /* passage � attente code commande */
                  ePhRec = FRMCOM_PHR_WAITCMD ;
               }
                                          /* si r�ception ESC */
               else if ( l_Receive.cType == FRMCOM_ESC )
               {                          /* init ESC dans trame */
                  l_Receive.cEsc = FRMCOM_ESC ;
                                          /* reste � attente type trame */
               }
               else                       /* pas r�ception type commande */
               {                          /* retour � attente ESC */
                  ePhRec = FRMCOM_PHR_WAITESC ;
               }
            }
            break ;

         case FRMCOM_PHR_WAITCMD :        /* attente code commande */
                                          /* r�ception code commande */
            bySize = ua_Receive( &l_Receive.byCmdRep, 1 ) ;
            if ( bySize == 1 )            /* si r�ception correcte */
            {                             /* si code commande correct */
               if ( l_Receive.byCmdRep >= FRMCOM_FIRSTCMD )
               {                          /* passage � attente longueur donn�es */
                  ePhRec = FRMCOM_PHR_WAITLEN ;
               }
                                          /* si r�ception ESC */
               else if ( l_Receive.byCmdRep == FRMCOM_ESC )
               {                          /* retour � attente type trame */
                  ePhRec = FRMCOM_PHR_WAITTYPE ;
                                          /* init ESC dans trame */
                  l_Receive.cEsc = FRMCOM_ESC ;
               }
               else                       /* code commande incorrect */
               {                          /* retour � attente ESC */
                  ePhRec = FRMCOM_PHR_WAITESC ;
               }
            }
            break ;

         case FRMCOM_PHR_WAITLEN :        /* attente longueur donn�es */
                                          /* r�ception longueur donn�es */
            bySize = ua_Receive( &l_Receive.byLenData, 1 ) ;
            if ( bySize == 1 )            /* si r�ception correcte */
            {                             /* si longueur donn�es pas trop grande */
               if ( l_Receive.byLenData <=
                    sizeof(l_Receive.abyData) )
               {                          /* init index donn�es � recevoir */
                  l_byIdxRecData = 0 ;
                                          /* passage � r�ception donn�es */
                  ePhRec = FRMCOM_PHR_RECDATA ;
               }
               else                       /* longueur donn�es trop grande */
               {                          /* retour � attente ESC */
                  ePhRec = FRMCOM_PHR_WAITESC ;
               }
            }
            break ;

         case FRMCOM_PHR_RECDATA :        /* r�ception donn�es */
                                          /* index donn�es � recevoir */
            byIdxRecData = l_byIdxRecData ;
                                          /* reception donn�es */
            bySize = ua_Receive( &l_Receive.abyData[byIdxRecData],
                                        (l_Receive.byLenData+1) - byIdxRecData ) ;
            if ( bySize != 0 )            /* si r�ception au moins un octet de donn�es */
            {                             /* mise � jour index */
               byIdxRecData += bySize ;
                                          /* si r�ception donn�es termin�e */
               if ( byIdxRecData == l_Receive.byLenData+1 )
               {                          /* calcul Crc8 octets re�us */
                  byCrc8 = crc_Calc8( (BYTE *)&l_Receive.cEsc, l_Receive.byLenData + 5 ) ;

                  if ( byCrc8 == 0 )      /* si Crc8 correct */
                  {                       /* trame re�ue correctement */
                     ePhRec = FRMCOM_PHR_RECEIVED ;
                     bySize = 0 ;         /* arr�t traitement */
                  }
                  else                    /* Crc8 incorrect */
                  {                       /* retour � attente ESC */
                     ePhRec = FRMCOM_PHR_WAITESC ;
                  }
               }
               else                       /* m�mo. nouvel index */
                  l_byIdxRecData = byIdxRecData ;
            }
            break ;

         default :                        /* phase inconnue */
            bySize = 0 ;                  /* arr�t traitement */
            break ;
      }

      if ( bySize == 0 )                  /* si pas re�u de caract�re */
         break ;                          /* arr�t traitement */
                                          /* relance tempo entre r�ception 2 octets */
      l_wTmpMaxRec = tp_Time10ms() ;
   } while ( bySize != 0 ) ;
                                          /* si trame en cours r�ception */
   if ( ePhRec != FRMCOM_PHR_WAITESC &&
        ePhRec != FRMCOM_PHR_RECEIVED &&
        ePhRec != FRMCOM_PHR_READED )
   {                                      /* si d�passement dur�e max entre 2 octets */
      if ( tp_Tst10ms( &l_wTmpMaxRec, FRMCOM_DURMAXBYTE ) == 0 )
      {                                   /* retour � attente ESC */
         ePhRec = FRMCOM_PHR_WAITESC ;
      }
   }
   l_ePhRec = ePhRec ;                    /* m�mo nouvelle phase r�ception */
}

/*=========================================================================*/

/*-------------------------------------------------------------------------*/
/* Ouverture port communication                                            */
/*    - <i_dwBaudRate> vitesse dialogue, baud                              */
/*      Contr�le RTS/CTS toujours valide :                                 */
/*      Ligne RTS : sortie, active en TTL � 0(+12V), et inactive � 1(-12V).*/
/*          + Activ�e � l'initialisation.                                  */
/*          + Inactiv�e pendant l'interruption de r�ception si buffer      */
/*            rempli au 3/4, et re-activ�e lors de lecture buffer si       */
/*            occupation buffer devient inf�rieure � 1/4.                  */
/*      Ligne CTS : entr�e, active en TTL � 0(+12V), et inactive � 1(-12V).*/
/*          + Transmission UART uniquement si ligne CTS active             */
/*-------------------------------------------------------------------------*/

void ua_Open( DWORD i_dwBaudRate)
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_Open();
            break;
        default:
            ua1_Open(i_dwBaudRate);
            break;
    }
}


/*-------------------------------------------------------------------------*/
/* Fermeture port communication                                            */
/*-------------------------------------------------------------------------*/

void ua_Close( void )
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_Close();
            break;
        default:
            ua1_Close();
            break;
    }
}


/*-------------------------------------------------------------------------*/
/* R�ception buffer octets sur port communication                          */
/*    - <o_pvBuf> adresse buffer pour retour octets re�us                  */
/*    - <i_byMaxSize> nombre maximum octets � transf�rer dans <o_pvBuf>    */
/* Retour :                                                                */
/*    - nombre octets transf�r�s dans <o_pvBuf>                            */
/*    - 0 si port communication pas ouvert                                 */
/*-------------------------------------------------------------------------*/

aBYTE ua_Receive( void * o_pvBuf, aBYTE i_byMaxSize )
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_Receive( o_pvBuf, i_byMaxSize );
            break;
        default:
            ua1_Receive( o_pvBuf, i_byMaxSize );
            break;
    }
}



/*-------------------------------------------------------------------------*/
/* Lecture longueur maximum buffer pouvant �tre re�u sur port com          */
/* Retour :                                                                */
/*    - taille buffer r�ception                                             */
/*-------------------------------------------------------------------------*/

aBYTE ua_GetMaxReceiveSize( void )
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_GetMaxReceiveSize();
            break;
        default:
            ua1_GetMaxReceiveSize();
            break;
    }
}


/*-------------------------------------------------------------------------*/
/* Emission buffer octets sur port communication                           */
/*    - <i_pvBuf> buffer octets � �mettre                                  */
/*    - <i_bySize> nombre octets � �mettre                                 */
/* Retour :                                                                */
/*    - OK �mission correcte                                               */
/*    - ERR pas assez de place dans buffer �mission                        */
/*-------------------------------------------------------------------------*/

aRESULT ua_Send( void C* i_pvBuf, aBYTE i_bySize )
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_Send( i_pvBuf, i_bySize );
            break;
        default:
            ua1_Send( i_pvBuf, i_bySize );
            break;
    }
}


/*-------------------------------------------------------------------------*/
/* Lecture longueur maximum buffer pouvant �tre �mis sur port com          */
/* Retour :                                                                */
/*    - taille buffer �mission                                             */
/*-------------------------------------------------------------------------*/

aBYTE ua_GetMaxSendSize( void )
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_GetMaxSendSize();
            break;
        default:
            ua1_GetMaxSendSize();
            break;
    }
}


/*-------------------------------------------------------------------------*/
/* Test si emission terminee                                               */
/* Retour :                                                                */
/*    - TRUE si emission terminee, FALSE si non                            */
/*-------------------------------------------------------------------------*/

aBOOL ua_IsSendEmpty( void )
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_IsSendEmpty();
            break;
        default:
            break;
    }
}


/*-------------------------------------------------------------------------*/
/* Traitement cyclique                                                     */
/*-------------------------------------------------------------------------*/

void frmcom_TaskCyc(void)
{
    switch (l_eCh)
    {
        case FRM_CH_USB :
            uausb_TaskCyc();
            break;
        default:    // FRM_CH_RS232 par interruption
            break;
    }
}
