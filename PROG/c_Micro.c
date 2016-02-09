/***************************************************************************/
/*                                                                         */
/*                               c_Micro.c                                 */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Commandes micro-contr�leur                                              */
/*                                                                         */
/* - Ex�cution commande passage micro-contr�leur en mode transparent       */
/*      aWORD cmicro_Exec90( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Ex�cution commande �criture sorties micro-contr�leur                  */
/*      aWORD cmicro_Exec91( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/* - Ex�cution commande lecture entr�es micro-contr�leur                   */
/*      aWORD cmicro_Exec92( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Ex�cution commande inhibition watchdog micro-contr�leur               */
/*      aWORD cmicro_Exec93( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Ex�cution commande lecture d�tection reset micro-contr�leur           */
/*      aWORD cmicro_Exec94( BYTE * i_pbyRecData, aWORD i_wLenRecData,     */
/*                          BYTE * o_pbySendData, aWORD i_wMaxSendData ) ; */
/*                                                                         */
/* - Test si mode transparent demand�                                      */
/*      aBOOL cmicro_IsNeedTrans( DWORD * o_pdwBaudrate, BOOL * o_pbRtsCts,*/
/*                                aBOOL i_bReset ) ;                       */
/*                                                                         */
/***************************************************************************/
/* D�clarations :                                                          */
/***************************************************************************/
/* Cr�ation     : 18.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 18.11.2015  L. Darras                                    */
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
/* Ex�cution commande passage micro-contr�leur en mode transparent         */
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

   wRet = 0 ;                             /* ex�cution correcte par d�faut */

   if ( i_wLenRecData != sizeof(*pCmd) )  /* si mauvais nombre param�tres */
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
/* Ex�cution commande �criture sorties micro-contr�leur                    */
/*-------------------------------------------------------------------------*/

typedef struct                            /* commande Cmd-91h */
{
   BYTE byPort ;                          /* port */
   BYTE byMsk ;                           /* masque sorties */
   BYTE byOut ;                           /* �tats sorties */
} s_Cmd91 ;

aWORD cmicro_Exec91( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                     BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   s_Cmd91 * pCmd ;

   wRet = 0 ;                             /* ex�cution correcte par d�faut */

   if ( i_wLenRecData != sizeof(*pCmd) )  /* si mauvais nombre param�tres */
      wRet = COM_S_ERRSIZE ;              /* erreur taille commande */

   if ( wRet < COM_S_ERROR )
   {
      pCmd = (s_Cmd91 *)i_pbyRecData ;
      io_Write( pCmd->byPort, pCmd->byMsk, pCmd->byOut ) ;
   }

   return wRet ;
}



/*-------------------------------------------------------------------------*/
/* Ex�cution commande lecture entr�es micro-contr�leur                     */
/*-------------------------------------------------------------------------*/

typedef struct                            /* commande Cmd-92h */
{
   BYTE byPort ;                          /* port */
   BYTE byMsk ;                           /* masque entr�es */
} s_Cmd92 ;

aWORD cmicro_Exec92( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData )
{
   aWORD wRet ;
   s_Cmd92 *pCmd ;
   BYTE * p ;
   BYTE byIn ;

   wRet = 0 ;                             /* ex�cution correcte par d�faut */

   if ( i_wLenRecData != sizeof(*pCmd) )  /* si mauvais nombre param�tres */
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
      wRet = p - o_pbySendData ;          /* retour taille r�ponse */
   }

   return wRet ;
}



/*-------------------------------------------------------------------------*/
/* Test si mode transparent demand�                                        */
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

