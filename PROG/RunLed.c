/***************************************************************************/
/*                                                                         */
/*                                RunLed.c                                 */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion led Run                                                         */
/*                                                                         */
/* - Initialisation                                                        */
/*      void runled_Init( void ) ;                                         */
/*                                                                         */
/* - Traitement cyclique                                                   */
/*      void runled_TaskCyc( void ) ;                                      */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 19.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Hard.h"
#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* D�finitions                                                             */
/*-------------------------------------------------------------------------*/

#define RUNLED_T_NORMAL       100      /* p�riode clign. �tat normal, 10ms */



/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static BOOL l_bOnBlink ;               /* �tat clignotement led - True si allum�e */
static WORD l_wTmpBlink ;



/*-------------------------------------------------------------------------*/
/* Fonctions prototypes                                                    */
/*-------------------------------------------------------------------------*/

static void runled_LedOff( void ) ;
static void runled_LedOn( void ) ;



/*-------------------------------------------------------------------------*/
/* Initialisation                                                          */
/*-------------------------------------------------------------------------*/

void runled_Init( void )
{
   O_LED_RUN_DIR = 0 ;                 /* O_LED_RUN en sortie */

   l_wTmpBlink = tp_Time10ms() ;       /* lancement tempo */
   l_bOnBlink = TRUE ;                 /* pour commencer leds �teintes */
}



/*-------------------------------------------------------------------------*/
/* Traitement cyclique                                                     */
/*-------------------------------------------------------------------------*/

void runled_TaskCyc( void )
{
   if ( tp_Tst10ms( &l_wTmpBlink, RUNLED_T_NORMAL ) == 0 )
   {
      if ( l_bOnBlink )                /* si led allum�e, on �teint */
      {
         l_bOnBlink = FALSE ;
         runled_LedOff() ;
      }
      else                             /* si led �teinte, on allume */
      {
         l_bOnBlink = TRUE ;
         runled_LedOn() ;
      }
      l_wTmpBlink = tp_Time10ms() ;    /* relancement tempo */
   }
}



/*=========================================================================*/

/*-------------------------------------------------------------------------*/
/* Led Off                                                                 */
/*-------------------------------------------------------------------------*/

static void runled_LedOff( void )
{                                      /* �teint led */
   O_LED_RUN = 0 ;
}



/*-------------------------------------------------------------------------*/
/* Led On                                                                  */
/*-------------------------------------------------------------------------*/

static void runled_LedOn( void )
{                                      /* allume led */
   O_LED_RUN = 1 ;
}

