/***************************************************************************/
/*                                                                         */
/*                               c_Micro.c                                 */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Commandes micro-contrôleur                                              */
/*                                                                         */
/* - Exécution commande passage micro-contrôleur en mode transparent       */
/*      aWORD cmicro_Exec90( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Exécution commande écriture sorties micro-contrôleur                  */
/*      aWORD cmicro_Exec91( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/* - Exécution commande lecture entrées micro-contrôleur                   */
/*      aWORD cmicro_Exec92( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Exécution commande inhibition watchdog micro-contrôleur               */
/*      aWORD cmicro_Exec93( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Exécution commande lecture détection reset micro-contrôleur           */
/*      aWORD cmicro_Exec94( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Test si mode transparent demandé                                      */
/*      aBOOL cmicro_IsNeedTrans( DWORD * o_pdwBaudrate, BOOL * o_pbRtsCts,*/
/*                                aBOOL i_bReset ) ;                       */
/*                                                                         */
/***************************************************************************/
/* Déclarations :                                                          */
/***************************************************************************/
/* Création     : 18.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 18.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/

#include "Pic.h"
                        /* macros GET_<Type>() et SET_<Type>() en big endian */
#include "GetSetWord.h"



/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static DWORD l_dwBaudrate ;   /* baudrate communication */
static BOOL l_bRtsCts ;       /* gestion Rts/Cts */
static BOOL l_bNeedTrans ;    /* besoin mode transparent */



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
/* Exécution commande passage micro-contrôleur en mode transparent         */
/*-------------------------------------------------------------------------*/

typedef struct                            /* commande Cmd-90h */
{
   DWORD dwBaudrate ;                     /* baudrate */
   BOOL bRtsCts ;                         /* gestion Rts/Cts */
} s_Cmd90 ;

aWORD cmicro_Exec90( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                     BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   s_Cmd90 * pCmd ;

   wRet = 0 ;                             /* exécution correcte par défaut */

   if ( i_wLenRecData != sizeof(*pCmd) )  /* si mauvais nombre paramètres */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {
      pCmd = (s_Cmd90 *)i_pbyRecData ;
      SET_DWORD( &l_dwBaudrate, pCmd->dwBaudrate ) ;
      l_bRtsCts = pCmd->bRtsCts ;
      l_bNeedTrans = TRUE ;
   }

   return wRet ;
}



/*-------------------------------------------------------------------------*/
/* Exécution commande écriture sorties micro-contrôleur                    */
/*-------------------------------------------------------------------------*/

typedef struct                            /* commande Cmd-91h */
{
   BYTE byPort ;                          /* port */
   BYTE byMsk ;                           /* masque sorties */
   BYTE byOut ;                           /* états sorties */
} s_Cmd91 ;

aWORD cmicro_Exec91( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                     BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   s_Cmd91 * pCmd ;

   wRet = 0 ;                             /* exécution correcte par défaut */

   if ( i_wLenRecData != sizeof(*pCmd) )  /* si mauvais nombre paramètres */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {
      pCmd = (s_Cmd91 *)i_pbyRecData ;
      io_Write( pCmd->byPort, pCmd->byMsk, pCmd->byOut ) ;
   }

   return wRet ;
}



/*-------------------------------------------------------------------------*/
/* Exécution commande lecture entrées micro-contrôleur                     */
/*-------------------------------------------------------------------------*/

typedef struct                            /* commande Cmd-92h */
{
   BYTE byPort ;                          /* port */
   BYTE byMsk ;                           /* masque entrées */
} s_Cmd92 ;

aWORD cmicro_Exec92( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   s_Cmd92 *pCmd ;
   BYTE * p ;
   BYTE byIn ;

   wRet = 0 ;                             /* exécution correcte par défaut */

   if ( i_wLenRecData != sizeof(*pCmd) )  /* si mauvais nombre paramètres */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {
      p = o_pbySendData ;
      pCmd = (s_Cmd92 *)i_pbyRecData ;
      byIn = io_Read( pCmd->byPort, pCmd->byMsk) ;
      *p++ = byIn ;
   }

   if ( wRet < COM_S_ERROR )
   {
      wRet = p - o_pbySendData ;          /* retour taille réponse */
   }

   return wRet ;
}



/*-------------------------------------------------------------------------*/
/* Test si mode transparent demandé                                        */
/*-------------------------------------------------------------------------*/

aBOOL cmicro_IsNeedTrans( DWORD * o_pdwBaudrate, BOOL * o_pbRtsCts, aBOOL i_bReset )
{
   aBOOL bRet ;

   if ( l_bNeedTrans )                    /* si besoin mode transparent */
   {
      *o_pdwBaudrate = l_dwBaudrate ;
      *o_pbRtsCts = l_bRtsCts ;
      if ( i_bReset )
         l_bNeedTrans = FALSE ;           /* reset besoin mode transparent */
   }
   else
      bRet = FALSE ;

   return bRet ;
}

