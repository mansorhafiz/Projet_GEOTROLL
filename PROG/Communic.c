/***************************************************************************/
/*                                                                         */
/*                               Communic.c                                */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion des communications sur les différents canaux                    */
/*                                                                         */
/* - Traitement commande sur un canal                                      */
/*      void com_ProcessCmd( void );                                       */
/*                                                                         */
/* - Définition données complémentaires description erreur                 */
/*      void com_SetDataError( void C* i_pvBuf, aWORD i_wSize ) ;          */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 18.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 18.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Tableau fonctions exécution commande                                    */
/*-------------------------------------------------------------------------*/


#define COM_FIRSTCMD  0x80  /* code 1ère commande */

               /* fonction exécution commande
                      - <i_pbyRecData> adresse données associées à commande
                      - <i_wLenRecData> longueur données associées à commande
                      - <o_pbySendData> adresse pour retour données associées à
                                        réponse, peut être identique à
                                        <i_pbyRecData> si commande ne demande
                                        pas buffer pour émission
                      - <i_wMaxSendData> taille maximum données associées à réponse */
typedef aWORD (* f_ExecCmd)( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                             BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;

typedef struct              /* description commande */
{
   f_ExecCmd pFunc ;        /* fonction exécution commande */
} s_DescCmd ;

static s_DescCmd C rom k_aDescCmd [] =
{
     { cvers_Exec80   },    /* 80: lecture appareil */
     { cvers_Exec81   },    /* 81: lecture version */
     { cvers_Exec82   },    /* 82: lecture date/heure génération */
     { NULL },              /* 83 */
     { NULL },              /* 84 */
     { NULL },              /* 85 */
     { NULL },              /* 86 */
     { NULL },              /* 87 */
     { NULL },              /* 88 */
     { NULL },              /* 89 */
     { NULL },              /* 8A */
     { NULL },              /* 8B */
     { NULL },              /* 8C */
     { NULL },              /* 8D */
     { NULL },              /* 8E */
     { NULL },              /* 8F */
     { cmicro_Exec90   },   /* 90: passage micro-contrôleur en mode transparent */
     { cmicro_Exec91   },   /* 91: écriture sorties micro-contrôleur */
     { cmicro_Exec92   },   /* 92: lecture entrées micro-contrôleur */
     { NULL },   		    /* 93 */
     { NULL }    		    /* 94 */
} ;



/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static BYTE l_bySizeDataError ;        /* taille données complémentaires pour erreur */
static BYTE l_abyDataError [20] ;      /* buffer données complémentaires pour erreur */



/*-------------------------------------------------------------------------*/
/* Traitement commande sur un canal                                        */
/*-------------------------------------------------------------------------*/

void com_ProcessCmd( void )
{
   aRESULT rRet ;
   s_Frame SendFrame ;
   s_Frame RecFrame ;
   aBYTE byCmd ;
   aBYTE byIdxCmd ;
   aWORD wRet ;
   s_DescCmd C rom * pDesc ;
   f_ExecCmd pFunc ;
   aBYTE bySizeDataError ;
   aBYTE byMaxSendData ;
   BYTE * pbySrc ;
   BYTE * pbyDest ;

                                         /* demande trame pour émission */
   rRet = frmcom_GetSend( &SendFrame ) ;
   if ( rRet == OK )                     /* si trame pour émission disponible */
   {                                     /* lecture trame reçue */
      rRet = frmcom_Receive( &RecFrame ) ;
      if ( rRet == OK )                  /* si trame reçue */
      {
         l_bySizeDataError = 0 ;         /* suppression données complémentaires erreur */
         wRet = 0 ;

         byCmd = RecFrame.byCmdRep ;     /* code comande reçu */
                                         /* code commande 0-n (au lieu 80h-n) */
         byIdxCmd = byCmd - COM_FIRSTCMD ;
                                         /* si code commande incorrect */
         if ( byIdxCmd >= ARRAY_SIZE(k_aDescCmd) )
         {
            wRet = COM_S_ERRUNKNOWN ;    /* commande inconnue */
         }

         if ( wRet < COM_S_ERROR )
         {                               /* adresse description commande */
            pDesc = &k_aDescCmd[byIdxCmd] ;
            pFunc = pDesc->pFunc ;       /* fonction exécution commande */
            if ( pFunc == NULL )         /* si absence fonction exécution */
            {
               wRet = COM_S_ERRUNKNOWN ; /* commande inconnue */
            }
         }

         if ( wRet < COM_S_ERROR )
         {
                                         /* code réponse de la commande */
            SendFrame.byCmdRep = COM_RESP( byCmd ) ;

                                         /* appel fonction exécution commande */
            wRet = (*pFunc)( RecFrame.pbyData, RecFrame.byLen,
                             SendFrame.pbyData, SendFrame.byLen ) ;
         }

         if ( wRet >= COM_S_ERROR )      /* si erreur exécution */
         {                               /* code réponse erreur */
            SendFrame.byCmdRep = COM_R_ERROR ;
                                         /* données réponse erreur */
            SendFrame.pbyData[0] = byCmd ;
            SendFrame.pbyData[1] = wRet - COM_S_ERROR ;
            wRet = 2 ;                   /* taille standard réponse erreur */

            bySizeDataError = l_bySizeDataError ;
            if ( bySizeDataError != 0 )  /* si données complémentaires erreur */
            {                            /* taille maximum émission */
               byMaxSendData = SendFrame.byLen ;
               if ( byMaxSendData < wRet )
                  byMaxSendData = 0 ;    /* plus de place pour émission */
               else
                  byMaxSendData -= wRet ;/* place restante après taille standard */
                                         /* limite taille données complémentaires */
                                         /* erreur à place restante pour émission */
               if ( bySizeDataError > byMaxSendData )
                  bySizeDataError = byMaxSendData ;
               wRet += bySizeDataError ; /* taille totale réponse */

               if ( bySizeDataError != 0 )
               {
                  pbyDest = &SendFrame.pbyData[2] ;
                  pbySrc = l_abyDataError ;
                  do                     /* recopie données erreur */
                  {
                     --bySizeDataError ;
                     pbyDest[bySizeDataError] = pbySrc[bySizeDataError] ;
                  } while ( bySizeDataError != 0 ) ;
               }
            }
         }

         SendFrame.byLen = wRet ;        /* insertion taille réponse */
                                         /* émission réponse */
         frmcom_Send( &SendFrame ) ;

         frmcom_FreeReceive( ) ;         /* libération trame reçue */
      }
      else                               /* pas de trame reçue */
      {
         frmcom_FreeSend( ) ;            /* libération trame pour émission */
      }
   }
}



/*-------------------------------------------------------------------------*/
/* Définition données complémentaires description erreur                   */
/*    - <i_pvBuf> adresse buffer avec données complémentaires              */
/*    - <i_wSize> taille données complémentaires pour erreur               */
/*-------------------------------------------------------------------------*/

void com_SetDataError( void C* i_pvBuf, aWORD i_wSize )
{
   BYTE C* pbySrc ;
   BYTE * pbyDest ;

   if ( i_wSize > sizeof(l_abyDataError) )
      i_wSize = sizeof(l_abyDataError) ;   /* limite à taille buffer erreur */
   l_bySizeDataError = i_wSize ;           /* mémo. taille données erreur */

   if ( i_wSize != 0 )                     /* si présence données erreur */
   {
      pbyDest = l_abyDataError ;
      pbySrc = i_pvBuf ;
      do                                   /* recopie données erreur */
      {
         --i_wSize ;
         pbyDest[i_wSize] = pbySrc[i_wSize] ;
      } while ( i_wSize != 0 ) ;
   }
}


