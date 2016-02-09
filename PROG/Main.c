/***************************************************************************/
/*                                                                         */
/*                                 Main.c                                  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Programme principal                                                     */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 19.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/

#ifndef __VERSION                /* si pas inclusion pour symboles version */

#include "Define.h"              /*Définitions globales : types, macros, structures et constantes*/
#include "Hard.h"                /*Definiton entrée sortie, horloge, etc*/
#include "Pic.h"                 /* Définitions et fonctions prototypes pour le logiciel de test*/

#endif  /* __VERSION */



#ifndef __VERSION                /* si pas inclusion pour symboles version */



/*-------------------------------------------------------------------------*/
/* Fonctions prototypes                                                    */
/*-------------------------------------------------------------------------*/

static void Init( void ) ;



/*-------------------------------------------------------------------------*/
/* Programme principal                                                     */
/*-------------------------------------------------------------------------*/

void main(void)
{
   Init() ;                      /* init oscillateur */
   int_Init() ;                  /* init interruptions */
   ti_Init() ;                   /* init timer */
   runled_Init() ;               /* init led run */

   frmcom_Init() ;               /* init communication avec Uart PC */

   for ( ; ; )
   {
      for ( ; ; )
      {
         runled_TaskCyc() ;      /* traitement clignotement led run */
         frmcom_TaskCyc() ;
         com_ProcessCmd() ;      /* traitement commandes */
      }
   }
}



/*=========================================================================*/

/*-------------------------------------------------------------------------*/
/* Initialisation principale                                               */
/*-------------------------------------------------------------------------*/

static void Init( void )
{
                              /* configuration oscillateur */
   OSCCONbits.IDLEN = 0 ;     /* Device enters sleep mode on SLEEP instruction*/
                              /* sélection oscillateur primaire par SCS1:0 */
   OSCCONbits.SCS1 = 0 ;
   OSCCONbits.SCS0 = 0 ;

   ADCON1 = 0x0F ;            /* PCFG[3:0] à 1 pour considérer toutes entrées numériques */

}

#endif
