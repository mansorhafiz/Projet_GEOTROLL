/***************************************************************************/
/*                                                                         */
/*                               c_Version.c                               */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Commandes lecture appareil, version et date g�n�ration                  */
/*                                                                         */
/* - Ex�cution commande lecture appareil                                   */
/*      aWORD cvers_Exec80( BYTE * i_pbyRecData, aWORD i_wLenRecData,      */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/* - Ex�cution commande lecture version                                    */
/*      aWORD cvers_Exec81( BYTE * i_pbyRecData, aWORD i_wLenRecData,      */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/* - Ex�cution commande lecture date/heure g�n�ration logiciel             */
/*      aWORD cvers_Exec82( BYTE * i_pbyRecData, aWORD i_wLenRecData,      */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 18.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 18.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/


#include "Pic.h"

#define __VERSION

                           /* macros GET_<Type>() et SET_<Type>() en big endian */
#include "GetSetWord.h"



/*-------------------------------------------------------------------------*/
/* Organisations informations en Flash                                     */
/*-------------------------------------------------------------------------*/

#define CVERS_D_IND1  0x33  /* 1er indicateur pr�sence date/heure cr�ation */
#define CVERS_D_IND2  0xCC  /* 2�me indicateur pr�sence date/heure cr�ation */

typedef struct              /* organisation date/heure cr�ation en Flash */
{
   BYTE byInd1 ;            /* 1er indicateur pr�sence, CVERS_D_IND1 */
   BYTE byInd2 ;            /* 2�me indicateur pr�sence, CVERS_D_IND2 */
                            /* date/heure cr�ation */
   s_DateTime DateTimeCreate ;
} s_FlashDate ;

#define CVERS_C_IND1  0x5A  /* 1er indicateur pr�sence Crc16 */
#define CVERS_C_IND2  0xA5  /* 2�me indicateur pr�sence Crc16 */

typedef struct              /* organisation Crc16 en Flash */
{
   BYTE byInd1 ;            /* 1er indicateur pr�sence, CVERS_C_IND1 */
   BYTE byInd2 ;            /* 2�me indicateur pr�sence, CVERS_C_IND2 */
   WORD wCrc16 ;            /* Crc16 application */
} s_FlashCrc ;



/*-------------------------------------------------------------------------*/
/* Ex�cution commandes                                                     */
/*    - <i_pbyRecData> adresse donn�es associ�es � commande                */
/*    - <i_wLenRecData> taille donn�es associ�es � commande                */
/*    - <o_pbySendData> adresse pour retour donn�es associ�es � r�ponse    */
/*    - <i_wMaxSendData> taille maximum donn�es associ�es � r�ponse        */
/* Retour :                                                                */
/*    - taille donn�es associ�es � r�ponse, 0-n                            */
/*    - COM_S_ERRxxx (>= COM_S_ERROR) erreur ex�cution commande            */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Ex�cution commande lecture appareil                                     */
/*-------------------------------------------------------------------------*/

aWORD cvers_Exec80( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   char C rom* pszSrc ;
   BYTE * p ;

   wRet = 0 ;                             /* ex�cution correcte par d�faut */

   if ( i_wLenRecData != 0 )              /* si pr�sence param�tre */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {
      pszSrc = DEVICE ;                   /* cha�ne nom projet */
      p = o_pbySendData ;
      while ( *pszSrc != 0 )              /* boucle retour cha�ne */
      {
         if ( i_wMaxSendData == 0 )       /* si r�ponse pleine */
         {                                /* erreur buffer r�ponse pas assez grand */
            wRet = COM_S_ERRREPSIZE ;
            break ;
         }
         *p++ = *pszSrc++ ;               /* retour un caract�re */
      }
   }
   if ( wRet < COM_S_ERROR )
   {
      wRet = p - o_pbySendData ;          /* retour taille r�ponse */
   }

   return wRet ;
}



/*-------------------------------------------------------------------------*/
/* Ex�cution commande lecture version                                      */
/*-------------------------------------------------------------------------*/

typedef struct                            /* r�ponse commande Cmd-81h */
{
   char szVersion [4] ;                   /* version du logiciel, "vvrc" */
   WORD wCrc16 ;                          /* Crc 16 bits du logiciel en Flash */
} s_Resp01 ;

aWORD cvers_Exec81( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   s_Resp01 * pResp ;
   s_FlashCrc C* pCrc ;
   aWORD wCrc16 ;

   REFPARM(i_pbyRecData) ;

   wRet = 0 ;                             /* ex�cution correcte par d�faut */

   if ( i_wLenRecData != 0 )              /* si pr�sence param�tre(s) */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {                                      /* si pas assez de place pour r�ponse */
      if ( i_wMaxSendData < sizeof(*pResp) )
         wRet = COM_S_ERRREPSIZE ;        /* erreur buffer r�ponse pas assez grand */
   }

   if ( wRet < COM_S_ERROR )
   {
      pResp = (s_Resp01 *)o_pbySendData ;

                                          /* retour version logiciel */
      pResp->szVersion[0] = VERSION[0] ;
      pResp->szVersion[1] = VERSION[1] ;
      pResp->szVersion[2] = REVISION[0] ;
      pResp->szVersion[3] = CORRECT[0] ;

                                          /* adresse Crc16 en Flash */
      pCrc = (s_FlashCrc C*)( BOOT_FLASHBASE + ( BOOT_SIZE - 4 ) ) ;
                                          /* si Crc16 pr�sent en Flash */
      if ( pCrc->byInd1 == CVERS_C_IND1 && pCrc->byInd2 == CVERS_C_IND2 )
         wCrc16 = pCrc->wCrc16 ;          /* Crc16 logiciel */
      else
         wCrc16 = 0 ;                     /* Crc16 � 0 si absent */
                                          /* retour Crc16 */
      SET_WORD( &pResp->wCrc16, wCrc16 ) ;

      wRet = sizeof(*pResp) ;             /* retour taille r�ponse */
   }

   return wRet ;
}


/*-------------------------------------------------------------------------*/
/* Ex�cution commande lecture date/heure g�n�ration logiciel               */
/*-------------------------------------------------------------------------*/

typedef struct                            /* r�ponse commande Cmd-82h */
{
   BYTE byDay ;                           /* jour, 1-31 */
   BYTE byMonth ;                         /* mois, 1-12 */
   WORD wYear ;                           /* ann�e, 2008-2099 */
   BYTE byHour ;                          /* heure, 0-23 */
   BYTE byMin ;                           /* minute, 0-59 */
   BYTE bySec ;                           /* seconde, 0-59 */
} s_Resp02 ;

aWORD cvers_Exec82( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   s_Resp02 * pResp ;
   s_FlashDate C* pDate ;
   aWORD wYear ;

   REFPARM(i_pbyRecData) ;

   wRet = 0 ;                             /* ex�cution correcte par d�faut */

   if ( i_wLenRecData != 0 )              /* si pr�sence param�tre(s) */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {                                      /* si pas assez de place pour r�ponse */
      if ( i_wMaxSendData < sizeof(*pResp) )
         wRet = COM_S_ERRREPSIZE ;        /* erreur buffer r�ponse pas assez grand */
   }

   if ( wRet < COM_S_ERROR )
   {
      pResp = (s_Resp02 *)o_pbySendData ;
                                          /* adresse date/heure cr�ation en Flash */
      pDate = (s_FlashDate C*)( BOOT_FLASHBASE + ( BOOT_SIZE - 16 ) ) ;
                                          /* si date/heure cr�ation pr�sente en Flash */
      if ( pDate->byInd1 == CVERS_D_IND1 && pDate->byInd2 == CVERS_D_IND2 )
      {                                   /* retour date/heure logiciel */
         pResp->byDay = pDate->DateTimeCreate.byDay ;
         pResp->byMonth = pDate->DateTimeCreate.byMonth ;
         wYear = pDate->DateTimeCreate.wYear ;
         pResp->byHour = pDate->DateTimeCreate.byHour ;
         pResp->byMin = pDate->DateTimeCreate.byMin ;
         pResp->bySec = pDate->DateTimeCreate.bySec ;
      }
      else                                /* date/heure cr�ation absente */
      {
         pResp->byDay = 0 ;               /* retour date/heure � 0 */
         pResp->byMonth = 0 ;
         wYear = 0 ;
         pResp->byHour = 0 ;
         pResp->byMin = 0 ;
         pResp->bySec = 0 ;
      }
      SET_WORD( &pResp->wYear, wYear ) ;

      wRet = sizeof(*pResp) ;             /* retour taille r�ponse */
   }

   return wRet ;
}

