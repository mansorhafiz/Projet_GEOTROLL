/***************************************************************************/
/*                                                                         */
/*                               Communic.c                                */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion des communications sur les diff�rents canaux                    */
/*                                                                         */
/* - Traitement commande sur un canal                                      */
/*      void com_ProcessCmd( void );                                       */
/*                                                                         */
/* - D�finition donn�es compl�mentaires description erreur                 */
/*      void com_SetDataError( void C* i_pvBuf, aWORD i_wSize ) ;          */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 18.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 18.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Tableau fonctions ex�cution commande                                    */
/*-------------------------------------------------------------------------*/


#define COM_FIRSTCMD  0x80  /* code 1�re commande */

               /* fonction ex�cution commande
                      - <i_pbyRecData> adresse donn�es associ�es � commande
                      - <i_wLenRecData> longueur donn�es associ�es � commande
                      - <o_pbySendData> adresse pour retour donn�es associ�es �
                                        r�ponse, peut �tre identique �
                                        <i_pbyRecData> si commande ne demande
                                        pas buffer pour �mission
                      - <i_wMaxSendData> taille maximum donn�es associ�es � r�ponse */
typedef aWORD (* f_ExecCmd)( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                             BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;

typedef struct              /* description commande */
{
   f_ExecCmd pFunc ;        /* fonction ex�cution commande */
} s_DescCmd ;

static s_DescCmd C rom k_aDescCmd [] =
{
     { cvers_Exec80   },    /* 80: lecture appareil */
     { cvers_Exec81   },    /* 81: lecture version */
     { cvers_Exec82   },    /* 82: lecture date/heure g�n�ration */
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
     { cmicro_Exec90   },   /* 90: passage micro-contr�leur en mode transparent */
     { cmicro_Exec91   },   /* 91: �criture sorties micro-contr�leur */
     { cmicro_Exec92   },   /* 92: lecture entr�es micro-contr�leur */
     { NULL },   		    /* 93 */
     { NULL }    		    /* 94 */
} ;



/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static BYTE l_bySizeDataError ;        /* taille donn�es compl�mentaires pour erreur */
static BYTE l_abyDataError [20] ;      /* buffer donn�es compl�mentaires pour erreur */



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

                                         /* demande trame pour �mission */
   rRet = frmcom_GetSend( &SendFrame ) ;
   if ( rRet == OK )                     /* si trame pour �mission disponible */
   {                                     /* lecture trame re�ue */
      rRet = frmcom_Receive( &RecFrame ) ;
      if ( rRet == OK )                  /* si trame re�ue */
      {
         l_bySizeDataError = 0 ;         /* suppression donn�es compl�mentaires erreur */
         wRet = 0 ;

         byCmd = RecFrame.byCmdRep ;     /* code comande re�u */
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
            pFunc = pDesc->pFunc ;       /* fonction ex�cution commande */
            if ( pFunc == NULL )         /* si absence fonction ex�cution */
            {
               wRet = COM_S_ERRUNKNOWN ; /* commande inconnue */
            }
         }

         if ( wRet < COM_S_ERROR )
         {
                                         /* code r�ponse de la commande */
            SendFrame.byCmdRep = COM_RESP( byCmd ) ;

                                         /* appel fonction ex�cution commande */
            wRet = (*pFunc)( RecFrame.pbyData, RecFrame.byLen,
                             SendFrame.pbyData, SendFrame.byLen ) ;
         }

         if ( wRet >= COM_S_ERROR )      /* si erreur ex�cution */
         {                               /* code r�ponse erreur */
            SendFrame.byCmdRep = COM_R_ERROR ;
                                         /* donn�es r�ponse erreur */
            SendFrame.pbyData[0] = byCmd ;
            SendFrame.pbyData[1] = wRet - COM_S_ERROR ;
            wRet = 2 ;                   /* taille standard r�ponse erreur */

            bySizeDataError = l_bySizeDataError ;
            if ( bySizeDataError != 0 )  /* si donn�es compl�mentaires erreur */
            {                            /* taille maximum �mission */
               byMaxSendData = SendFrame.byLen ;
               if ( byMaxSendData < wRet )
                  byMaxSendData = 0 ;    /* plus de place pour �mission */
               else
                  byMaxSendData -= wRet ;/* place restante apr�s taille standard */
                                         /* limite taille donn�es compl�mentaires */
                                         /* erreur � place restante pour �mission */
               if ( bySizeDataError > byMaxSendData )
                  bySizeDataError = byMaxSendData ;
               wRet += bySizeDataError ; /* taille totale r�ponse */

               if ( bySizeDataError != 0 )
               {
                  pbyDest = &SendFrame.pbyData[2] ;
                  pbySrc = l_abyDataError ;
                  do                     /* recopie donn�es erreur */
                  {
                     --bySizeDataError ;
                     pbyDest[bySizeDataError] = pbySrc[bySizeDataError] ;
                  } while ( bySizeDataError != 0 ) ;
               }
            }
         }

         SendFrame.byLen = wRet ;        /* insertion taille r�ponse */
                                         /* �mission r�ponse */
         frmcom_Send( &SendFrame ) ;

         frmcom_FreeReceive( ) ;         /* lib�ration trame re�ue */
      }
      else                               /* pas de trame re�ue */
      {
         frmcom_FreeSend( ) ;            /* lib�ration trame pour �mission */
      }
   }
}



/*-------------------------------------------------------------------------*/
/* D�finition donn�es compl�mentaires description erreur                   */
/*    - <i_pvBuf> adresse buffer avec donn�es compl�mentaires              */
/*    - <i_wSize> taille donn�es compl�mentaires pour erreur               */
/*-------------------------------------------------------------------------*/

void com_SetDataError( void C* i_pvBuf, aWORD i_wSize )
{
   BYTE C* pbySrc ;
   BYTE * pbyDest ;

   if ( i_wSize > sizeof(l_abyDataError) )
      i_wSize = sizeof(l_abyDataError) ;   /* limite � taille buffer erreur */
   l_bySizeDataError = i_wSize ;           /* m�mo. taille donn�es erreur */

   if ( i_wSize != 0 )                     /* si pr�sence donn�es erreur */
   {
      pbyDest = l_abyDataError ;
      pbySrc = i_pvBuf ;
      do                                   /* recopie donn�es erreur */
      {
         --i_wSize ;
         pbyDest[i_wSize] = pbySrc[i_wSize] ;
      } while ( i_wSize != 0 ) ;
   }
}


