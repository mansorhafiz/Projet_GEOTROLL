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
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 19.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Hard.h"
#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Définitions                                                             */
/*-------------------------------------------------------------------------*/

#define RUNLED_T_NORMAL       100      /* pèriode clign. état normal, 10ms */



/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static BOOL l_bOnBlink ;               /* état clignotement led - True si allumée */
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
   l_bOnBlink = TRUE ;                 /* pour commencer leds éteintes */
}



/*-------------------------------------------------------------------------*/
/* Traitement cyclique                                                     */
/*-------------------------------------------------------------------------*/

void runled_TaskCyc( void )
{
   if ( tp_Tst10ms( &l_wTmpBlink, RUNLED_T_NORMAL ) == 0 )
   {
      if ( l_bOnBlink )                /* si led allumée, on éteint */
      {
         l_bOnBlink = FALSE ;
         runled_LedOff() ;
      }
      else                             /* si led éteinte, on allume */
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
{                                      /* éteint led */
   O_LED_RUN = 0 ;
}



/*-------------------------------------------------------------------------*/
/* Led On                                                                  */
/*-------------------------------------------------------------------------*/

static void runled_LedOn( void )
{                                      /* allume led */
   O_LED_RUN = 1 ;
}

