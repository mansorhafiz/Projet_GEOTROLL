/***************************************************************************/
/*                                                                         */
/*                               FrameCom.c                                */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Décodage/encodage trames pour dialogue sur port série                   */
/*                                                                         */
/* - Initialisation                                                        */
/*      void frmcom_Init( void ) ;                                         */
/*  - Dé-initialisation                                                    */
/*      void frmcom_UnInit( void ) ;                                       */
/*                                                                         */
/* - Lecture trame reçue                                                   */
/*      aRESULT frmcom_Receive( s_Frame * o_pFrm ) ;                       */
/* - Fin lecture trame reçue                                               */
/*      void frmcom_FreeReceive( void ) ;                                  */
/*                                                                         */
/* - Lecture trame pour émission                                           */
/*      aRESULT frmcom_GetSend( s_Frame * o_pFrm ) ;                       */
/* - Emission trame                                                        */
/*      aRESULT frmcom_Send( s_Frame * i_pFrm ) ;                          */
/* - Libération trame pour émission                                        */
/*      void frmcom_FreeSend( void ) ;                                     */
/*                                                                         */
/* - Tâche si necessaire pour dialogue sur port série                      */
/*      void frmcom_TaskCyc( void ) ;                                      */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 19.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Définitions                                                             */
/*-------------------------------------------------------------------------*/

#define FRMCOM_ESC         0x1B  /* synchro début trame */
#define FRMCOM_TYPECMD     0x40  /* synchro type commande */
#define FRMCOM_TYPERESP    0xC0  /* synchro type réponse */
#define FRMCOM_FIRSTCMD    0x80  /* code 1ère commande */

#define FRMCOM_DURMAXBYTE    15  /* durée maximum entre réception de 2 octets
                                    pour considérer qu'il s'agit de la même
                                    trame, 10 ms */

typedef enum                     /* phase réception */
{
   FRMCOM_PHR_CLOSE = 0,         /* canal communication fermé */
   FRMCOM_PHR_WAITESC,           /* attente ESC */
   FRMCOM_PHR_WAITTYPE,          /* attente type trame */
   FRMCOM_PHR_WAITCMD,           /* attente code commande */
   FRMCOM_PHR_WAITLEN,           /* attente longueur données */
   FRMCOM_PHR_RECDATA,           /* réception données et Crc8 */
   FRMCOM_PHR_RECEIVED,          /* trame reçue en attente lecture */
   FRMCOM_PHR_READED             /* trame lue, attente fin lecture */
} e_FramePhaseReceive ;

typedef enum                     /* phase émission */
{
   FRMCOM_PHS_CLOSE = 0,         /* canal communication fermé */
   FRMCOM_PHS_FREE,              /* émission disponible */
   FRMCOM_PHS_PREPARE            /* trame à émettre en cours préparation */
} e_FramePhaseSend ;

typedef struct                   /* trame reçue ou à émettre */
{
   char cEsc ;                   /* caractère syncho début trame, Esc */
   char cType ;                  /* caractère type trame, FRMCOM_TYPExxx */
   BYTE byCmdRep ;               /* code commande ou code réponse */
   BYTE byLenData ;              /* longueur des données */
   BYTE abyData [100] ;          /* données */
   BYTE byCrc8 ;                 /* Crc8 de <cEsc> à dernier octet données */
} s_FrameBuf ;

/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

e_FramePhaseReceive l_ePhRec ;   /* phase réception, FRMCOM_PHR_xxx */
BYTE l_byIdxRecData ;            /* index données trame reçue */
WORD l_wTmpMaxRec ;              /* tempo attente entre réception 2 octets, 10 ms */
BYTE l_byMaxLenReceive ;         /* longueur maximum données dans trame réception */
e_FramePhaseSend l_ePhSend ;     /* phase émission, FRMCOM_PHS_xxx */
BYTE l_byMaxLenSend ;            /* longueur maximum données dans trame émission */
s_FrameBuf l_Receive ;           /* trame reçue */
s_FrameBuf l_Send ;              /* trame à émettre */
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

   l_ePhRec = FRMCOM_PHR_WAITESC ;        /* attente réception ESC */
   l_ePhSend = FRMCOM_PHS_FREE ;          /* émission disponible */
   l_eCh = FRM_CH_USB ;

   ua_Open( 115200 ) ;                   /* ouverture port uart */
   
                                          /* longueur maximum réception */
   byMaxReceive = ua_GetMaxReceiveSize() ;
                                          /* limite à longueur trame */
   if ( byMaxReceive > sizeof(l_Receive) )
      byMaxReceive = sizeof(l_Receive) ;
                                          /* suppression encapsulation données */
   byMaxReceive -= sizeof(l_Receive) - sizeof(l_Receive.abyData) ;
   l_byMaxLenReceive = byMaxReceive ;     /* mémo. longueur maximum données réception */

   byMaxSend = ua_GetMaxSendSize() ;     /* longueur maximum émission */
   if ( byMaxSend > sizeof(l_Send) )      /* limite à longueur trame */
      byMaxSend = sizeof(l_Send) ;
                                          /* suppression encapsulation données */
   byMaxSend -= sizeof(l_Send) - sizeof(l_Send.abyData) ;
   l_byMaxLenSend = byMaxSend ;           /* mémo. longueur maximum données émission */
}



/*-------------------------------------------------------------------------*/
/* De-initialisation                                                       */
/*-------------------------------------------------------------------------*/

void frmcom_UnInit( void )
{

   l_ePhRec = FRMCOM_PHR_CLOSE ;          /* canal communication fermé */
   l_ePhSend = FRMCOM_PHS_CLOSE ;

   ua_Close( ) ;                       /* fermeture port uart */
}



/*-------------------------------------------------------------------------*/
/* Lecture trame reçue                                                     */
/*    - <o_pFrm> adresse pour retour trame lue                             */
/* Retour :                                                                */
/*    - OK trame reçue, besoin appel frmcom_FreeReceive() après fin        */
/*         traitement trame                                                */
/*    - ERR pas de trame reçue ou identificateur canal incorrect           */
/*-------------------------------------------------------------------------*/

aRESULT frmcom_Receive( s_Frame * o_pFrm )
{
   aRESULT rRet ;

                                          /* si canal ouvert et trame pas reçue */
   if ( l_ePhRec != FRMCOM_PHR_CLOSE && l_ePhRec != FRMCOM_PHR_RECEIVED &&
        l_ePhRec != FRMCOM_PHR_READED )
   {
      frmcom_ProcessReceive() ;           /* traitement réception trame */
   }

                                          /* si trame reçue */
   if ( l_ePhRec == FRMCOM_PHR_RECEIVED )
   {                                      /* indication trame lue */
      l_ePhRec = FRMCOM_PHR_READED ;
                                          /* retour code commande */
      o_pFrm->byCmdRep = l_Receive.byCmdRep ;
                                          /* retour longueur données */
      o_pFrm->byLen = l_Receive.byLenData ;
                                          /* retour adresse données */
      o_pFrm->pbyData = l_Receive.abyData ;

      rRet = OK ;                         /* trame reçue */
   }
   else                                   /* pas trame reçue */
      rRet = ERR ;

   return rRet ;
}



/*-------------------------------------------------------------------------*/
/* Fin lecture trame reçue                                                 */
/*-------------------------------------------------------------------------*/

void frmcom_FreeReceive( void )
{
   if ( l_ePhRec == FRMCOM_PHR_READED )
      {                                   /* retour à attente synchro début trame */
         l_ePhRec = FRMCOM_PHR_WAITESC ;
      }
}



/*-------------------------------------------------------------------------*/
/* Lecture trame pour émission                                             */
/*    - <o_pFrm> adresse pour retour trame préparée pour émission          */
/* Retour :                                                                */
/*    - OK trame correctement préparée pour émission                       */
/*    - ERR pas de trame disponible pour émission ou identificateur canal  */
/*          incorrect                                                      */
/* Note : <o_pFrm->byLen> indique la longueur maximum des données de la    */
/* trame, c'est à dire la taille du buffer pointé par <o_pFrm->pbyData>.   */
/*-------------------------------------------------------------------------*/

aRESULT frmcom_GetSend( s_Frame * o_pFrm )
{
   aRESULT rRet ;

                                          /* si trame émission disponible */
   if ( l_ePhSend == FRMCOM_PHS_FREE )
   {                                      /* trame émission préparée */
      l_ePhSend = FRMCOM_PHS_PREPARE ;

      o_pFrm->byCmdRep = 0 ;              /* code réponse inconnu */
                                          /* longueur maximum des données */
      o_pFrm->byLen = l_byMaxLenSend ;
                                          /* adresse pour données à émettre */
      o_pFrm->pbyData = l_Send.abyData ;

      rRet = OK ;
   }
   else                                   /* trame émission non disponible */
      rRet = ERR ;

   return rRet ;
}



/*-------------------------------------------------------------------------*/
/* Emission trame                                                          */
/*    - <i_pFrm> trame à émettre                                           */
/* Retour :                                                                */
/*    - OK trame en cours émission                                         */
/*    - ERR problème émission : pas de trame à émettre préparée,           */
/*          identificateur canal incorrect ou pas assez de place dans      */
/*          buffer émission                                                */
/* Note : la trame à émettre <i_pFrm> doit obligatoiremnet être obtenue    */
/* par un appel à frmcom_GetSend().                                        */
/*-------------------------------------------------------------------------*/

aRESULT frmcom_Send( s_Frame C* i_pFrm )
{
   aRESULT rRet ;
   aBYTE byLen ;
   aBYTE byCrc8 ;

                                          /* si trame préparée */
   if ( l_ePhSend == FRMCOM_PHS_PREPARE )
   {                                      /* trame émission disponible */
      l_ePhSend = FRMCOM_PHS_FREE ;

      l_Send.cEsc = FRMCOM_ESC ;          /* octet synchro début trame */
                                          /* type réponse */
      l_Send.cType = FRMCOM_TYPERESP ;
                                          /* code réponse */
      l_Send.byCmdRep = i_pFrm->byCmdRep ;
      byLen = i_pFrm->byLen ;
      l_Send.byLenData = byLen ;          /* longueur données */
                                          /* calcul et insertion Crc8 */
      byCrc8 = crc_Calc8( (BYTE *)&l_Send.cEsc, byLen + 4 ) ;
      l_Send.abyData[byLen] = byCrc8 ;

                                          /* émission proprement dite trame */
      rRet = ua_Send( (BYTE C*)(&l_Send.cEsc), byLen + 5 ) ;
   }
   else                                   /* pas de trame à émettre préparée */
      rRet = ERR ;

   return rRet ;
}



/*-------------------------------------------------------------------------*/
/* Libération trame pour émission                                          */
/* Note : si lecture trame à émettre avec frmcom_GetSend() et que par la   */
/* suite il n'y a pas de trame à émettre et donc pas appel à frmcom_Send(),*/
/* besoin appel cette fonction avant de pouvoir faire un nouvel appel à    */
/* frmcom_GetSend().                                                       */
/*-------------------------------------------------------------------------*/

void frmcom_FreeSend( void)
{
   if ( l_ePhSend == FRMCOM_PHS_PREPARE )
   {                                      /* retour à trame émission disponible */
      l_ePhSend = FRMCOM_PHS_FREE ;
   }
}



/*=========================================================================*/

/*-------------------------------------------------------------------------*/
/* Traitement réception sur canal 1                                        */
/*-------------------------------------------------------------------------*/

static void frmcom_ProcessReceive( void)
{
   e_FramePhaseReceive ePhRec ;
   aBYTE bySize ;
   aBYTE byIdxRecData ;
   aBYTE byCrc8 ;

   ePhRec = l_ePhRec ;                    /* phase réception */

   for ( ; ; )
   {
      switch ( ePhRec )                   /* selon phase réception */
      {
         case FRMCOM_PHR_WAITESC :        /* attente ESC */
                                          /* réception ESC */
            bySize = ua_Receive( &l_Receive.cEsc, 1 ) ;
            if ( bySize == 1 )            /* si réception correcte */
            {                             /* si réception ESC */
               if ( l_Receive.cEsc == FRMCOM_ESC )
               {                          /* passage à attente type trame */
                  ePhRec = FRMCOM_PHR_WAITTYPE ;
               }
            }
            break ;

         case FRMCOM_PHR_WAITTYPE :       /* attente type trame */
                                          /* réception type trame */
            bySize = ua_Receive( &l_Receive.cType, 1 ) ;
            if ( bySize == 1 )            /* si réception correcte */
            {                             /* si réception type commande */
               if ( l_Receive.cType == FRMCOM_TYPECMD )
               {                          /* passage à attente code commande */
                  ePhRec = FRMCOM_PHR_WAITCMD ;
               }
                                          /* si réception ESC */
               else if ( l_Receive.cType == FRMCOM_ESC )
               {                          /* init ESC dans trame */
                  l_Receive.cEsc = FRMCOM_ESC ;
                                          /* reste à attente type trame */
               }
               else                       /* pas réception type commande */
               {                          /* retour à attente ESC */
                  ePhRec = FRMCOM_PHR_WAITESC ;
               }
            }
            break ;

         case FRMCOM_PHR_WAITCMD :        /* attente code commande */
                                          /* réception code commande */
            bySize = ua_Receive( &l_Receive.byCmdRep, 1 ) ;
            if ( bySize == 1 )            /* si réception correcte */
            {                             /* si code commande correct */
               if ( l_Receive.byCmdRep >= FRMCOM_FIRSTCMD )
               {                          /* passage à attente longueur données */
                  ePhRec = FRMCOM_PHR_WAITLEN ;
               }
                                          /* si réception ESC */
               else if ( l_Receive.byCmdRep == FRMCOM_ESC )
               {                          /* retour à attente type trame */
                  ePhRec = FRMCOM_PHR_WAITTYPE ;
                                          /* init ESC dans trame */
                  l_Receive.cEsc = FRMCOM_ESC ;
               }
               else                       /* code commande incorrect */
               {                          /* retour à attente ESC */
                  ePhRec = FRMCOM_PHR_WAITESC ;
               }
            }
            break ;

         case FRMCOM_PHR_WAITLEN :        /* attente longueur données */
                                          /* réception longueur données */
            bySize = ua_Receive( &l_Receive.byLenData, 1 ) ;
            if ( bySize == 1 )            /* si réception correcte */
            {                             /* si longueur données pas trop grande */
               if ( l_Receive.byLenData <=
                    sizeof(l_Receive.abyData) )
               {                          /* init index données à recevoir */
                  l_byIdxRecData = 0 ;
                                          /* passage à réception données */
                  ePhRec = FRMCOM_PHR_RECDATA ;
               }
               else                       /* longueur données trop grande */
               {                          /* retour à attente ESC */
                  ePhRec = FRMCOM_PHR_WAITESC ;
               }
            }
            break ;

         case FRMCOM_PHR_RECDATA :        /* réception données */
                                          /* index données à recevoir */
            byIdxRecData = l_byIdxRecData ;
                                          /* reception données */
            bySize = ua_Receive( &l_Receive.abyData[byIdxRecData],
                                        (l_Receive.byLenData+1) - byIdxRecData ) ;
            if ( bySize != 0 )            /* si réception au moins un octet de données */
            {                             /* mise à jour index */
               byIdxRecData += bySize ;
                                          /* si réception données terminée */
               if ( byIdxRecData == l_Receive.byLenData+1 )
               {                          /* calcul Crc8 octets reçus */
                  byCrc8 = crc_Calc8( (BYTE *)&l_Receive.cEsc, l_Receive.byLenData + 5 ) ;

                  if ( byCrc8 == 0 )      /* si Crc8 correct */
                  {                       /* trame reçue correctement */
                     ePhRec = FRMCOM_PHR_RECEIVED ;
                     bySize = 0 ;         /* arrêt traitement */
                  }
                  else                    /* Crc8 incorrect */
                  {                       /* retour à attente ESC */
                     ePhRec = FRMCOM_PHR_WAITESC ;
                  }
               }
               else                       /* mémo. nouvel index */
                  l_byIdxRecData = byIdxRecData ;
            }
            break ;

         default :                        /* phase inconnue */
            bySize = 0 ;                  /* arrêt traitement */
            break ;
      }

      if ( bySize == 0 )                  /* si pas reçu de caractère */
         break ;                          /* arrêt traitement */
                                          /* relance tempo entre réception 2 octets */
      l_wTmpMaxRec = tp_Time10ms() ;
   } while ( bySize != 0 ) ;
                                          /* si trame en cours réception */
   if ( ePhRec != FRMCOM_PHR_WAITESC &&
        ePhRec != FRMCOM_PHR_RECEIVED &&
        ePhRec != FRMCOM_PHR_READED )
   {                                      /* si dépassement durée max entre 2 octets */
      if ( tp_Tst10ms( &l_wTmpMaxRec, FRMCOM_DURMAXBYTE ) == 0 )
      {                                   /* retour à attente ESC */
         ePhRec = FRMCOM_PHR_WAITESC ;
      }
   }
   l_ePhRec = ePhRec ;                    /* mémo nouvelle phase réception */
}

/*=========================================================================*/

/*-------------------------------------------------------------------------*/
/* Ouverture port communication                                            */
/*    - <i_dwBaudRate> vitesse dialogue, baud                              */
/*      Contrôle RTS/CTS toujours valide :                                 */
/*      Ligne RTS : sortie, active en TTL à 0(+12V), et inactive à 1(-12V).*/
/*          + Activée à l'initialisation.                                  */
/*          + Inactivée pendant l'interruption de réception si buffer      */
/*            rempli au 3/4, et re-activée lors de lecture buffer si       */
/*            occupation buffer devient infèrieure à 1/4.                  */
/*      Ligne CTS : entrée, active en TTL à 0(+12V), et inactive à 1(-12V).*/
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
/* Réception buffer octets sur port communication                          */
/*    - <o_pvBuf> adresse buffer pour retour octets reçus                  */
/*    - <i_byMaxSize> nombre maximum octets à transférer dans <o_pvBuf>    */
/* Retour :                                                                */
/*    - nombre octets transférés dans <o_pvBuf>                            */
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
/* Lecture longueur maximum buffer pouvant être reçu sur port com          */
/* Retour :                                                                */
/*    - taille buffer réception                                             */
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
/*    - <i_pvBuf> buffer octets à émettre                                  */
/*    - <i_bySize> nombre octets à émettre                                 */
/* Retour :                                                                */
/*    - OK émission correcte                                               */
/*    - ERR pas assez de place dans buffer émission                        */
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
/* Lecture longueur maximum buffer pouvant être émis sur port com          */
/* Retour :                                                                */
/*    - taille buffer émission                                             */
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
