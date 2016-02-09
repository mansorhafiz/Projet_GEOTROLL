/***************************************************************************/
/*                                                                         */
/*                               c_Version.c                               */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Commandes lecture appareil, version et date génération                  */
/*                                                                         */
/* - Exécution commande lecture appareil                                   */
/*      aWORD cvers_Exec80( BYTE * i_pbyRecData, aWORD i_wLenRecData,      */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/* - Exécution commande lecture version                                    */
/*      aWORD cvers_Exec81( BYTE * i_pbyRecData, aWORD i_wLenRecData,      */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/* - Exécution commande lecture date/heure génération logiciel             */
/*      aWORD cvers_Exec82( BYTE * i_pbyRecData, aWORD i_wLenRecData,      */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 18.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 18.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/


#include "Pic.h"

#define __VERSION

                           /* macros GET_<Type>() et SET_<Type>() en big endian */
#include "GetSetWord.h"



/*-------------------------------------------------------------------------*/
/* Organisations informations en Flash                                     */
/*-------------------------------------------------------------------------*/

#define CVERS_D_IND1  0x33  /* 1er indicateur présence date/heure création */
#define CVERS_D_IND2  0xCC  /* 2ème indicateur présence date/heure création */

typedef struct              /* organisation date/heure création en Flash */
{
   BYTE byInd1 ;            /* 1er indicateur présence, CVERS_D_IND1 */
   BYTE byInd2 ;            /* 2ème indicateur présence, CVERS_D_IND2 */
                            /* date/heure création */
   s_DateTime DateTimeCreate ;
} s_FlashDate ;

#define CVERS_C_IND1  0x5A  /* 1er indicateur présence Crc16 */
#define CVERS_C_IND2  0xA5  /* 2ème indicateur présence Crc16 */

typedef struct              /* organisation Crc16 en Flash */
{
   BYTE byInd1 ;            /* 1er indicateur présence, CVERS_C_IND1 */
   BYTE byInd2 ;            /* 2ème indicateur présence, CVERS_C_IND2 */
   WORD wCrc16 ;            /* Crc16 application */
} s_FlashCrc ;



/*-------------------------------------------------------------------------*/
/* Exécution commandes                                                     */
/*    - <i_pbyRecData> adresse données associées à commande                */
/*    - <i_wLenRecData> taille données associées à commande                */
/*    - <o_pbySendData> adresse pour retour données associées à réponse    */
/*    - <i_wMaxSendData> taille maximum données associées à réponse        */
/* Retour :                                                                */
/*    - taille données associées à réponse, 0-n                            */
/*    - COM_S_ERRxxx (>= COM_S_ERROR) erreur exécution commande            */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Exécution commande lecture appareil                                     */
/*-------------------------------------------------------------------------*/

aWORD cvers_Exec80( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   char C rom* pszSrc ;
   BYTE * p ;

   wRet = 0 ;                             /* exécution correcte par défaut */

   if ( i_wLenRecData != 0 )              /* si présence paramètre */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {
      pszSrc = DEVICE ;                   /* chaîne nom projet */
      p = o_pbySendData ;
      while ( *pszSrc != 0 )              /* boucle retour chaîne */
      {
         if ( i_wMaxSendData == 0 )       /* si réponse pleine */
         {                                /* erreur buffer réponse pas assez grand */
            wRet = COM_S_ERRREPSIZE ;
            break ;
         }
         *p++ = *pszSrc++ ;               /* retour un caractère */
      }
   }
   if ( wRet < COM_S_ERROR )
   {
      wRet = p - o_pbySendData ;          /* retour taille réponse */
   }

   return wRet ;
}



/*-------------------------------------------------------------------------*/
/* Exécution commande lecture version                                      */
/*-------------------------------------------------------------------------*/

typedef struct                            /* réponse commande Cmd-81h */
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

   wRet = 0 ;                             /* exécution correcte par défaut */

   if ( i_wLenRecData != 0 )              /* si présence paramètre(s) */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {                                      /* si pas assez de place pour réponse */
      if ( i_wMaxSendData < sizeof(*pResp) )
         wRet = COM_S_ERRREPSIZE ;        /* erreur buffer réponse pas assez grand */
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
                                          /* si Crc16 présent en Flash */
      if ( pCrc->byInd1 == CVERS_C_IND1 && pCrc->byInd2 == CVERS_C_IND2 )
         wCrc16 = pCrc->wCrc16 ;          /* Crc16 logiciel */
      else
         wCrc16 = 0 ;                     /* Crc16 à 0 si absent */
                                          /* retour Crc16 */
      SET_WORD( &pResp->wCrc16, wCrc16 ) ;

      wRet = sizeof(*pResp) ;             /* retour taille réponse */
   }

   return wRet ;
}


/*-------------------------------------------------------------------------*/
/* Exécution commande lecture date/heure génération logiciel               */
/*-------------------------------------------------------------------------*/

typedef struct                            /* réponse commande Cmd-82h */
{
   BYTE byDay ;                           /* jour, 1-31 */
   BYTE byMonth ;                         /* mois, 1-12 */
   WORD wYear ;                           /* année, 2008-2099 */
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

   wRet = 0 ;                             /* exécution correcte par défaut */

   if ( i_wLenRecData != 0 )              /* si présence paramètre(s) */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {                                      /* si pas assez de place pour réponse */
      if ( i_wMaxSendData < sizeof(*pResp) )
         wRet = COM_S_ERRREPSIZE ;        /* erreur buffer réponse pas assez grand */
   }

   if ( wRet < COM_S_ERROR )
   {
      pResp = (s_Resp02 *)o_pbySendData ;
                                          /* adresse date/heure création en Flash */
      pDate = (s_FlashDate C*)( BOOT_FLASHBASE + ( BOOT_SIZE - 16 ) ) ;
                                          /* si date/heure création présente en Flash */
      if ( pDate->byInd1 == CVERS_D_IND1 && pDate->byInd2 == CVERS_D_IND2 )
      {                                   /* retour date/heure logiciel */
         pResp->byDay = pDate->DateTimeCreate.byDay ;
         pResp->byMonth = pDate->DateTimeCreate.byMonth ;
         wYear = pDate->DateTimeCreate.wYear ;
         pResp->byHour = pDate->DateTimeCreate.byHour ;
         pResp->byMin = pDate->DateTimeCreate.byMin ;
         pResp->bySec = pDate->DateTimeCreate.bySec ;
      }
      else                                /* date/heure création absente */
      {
         pResp->byDay = 0 ;               /* retour date/heure à 0 */
         pResp->byMonth = 0 ;
         wYear = 0 ;
         pResp->byHour = 0 ;
         pResp->byMin = 0 ;
         pResp->bySec = 0 ;
      }
      SET_WORD( &pResp->wYear, wYear ) ;

      wRet = sizeof(*pResp) ;             /* retour taille réponse */
   }

   return wRet ;
}

