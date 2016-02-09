/***************************************************************************/
/*                                                                         */
/*                                 Main.c                                  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Programme principal                                                     */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 19.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/

#ifndef __VERSION                /* si pas inclusion pour symboles version */

#include "Define.h"              /*D�finitions globales : types, macros, structures et constantes*/
#include "Hard.h"                /*Definiton entr�e sortie, horloge, etc*/
#include "Pic.h"                 /* D�finitions et fonctions prototypes pour le logiciel de test*/

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
                              /* s�lection oscillateur primaire par SCS1:0 */
   OSCCONbits.SCS1 = 0 ;
   OSCCONbits.SCS0 = 0 ;

   ADCON1 = 0x0F ;            /* PCFG[3:0] � 1 pour consid�rer toutes entr�es num�riques */

}

#endif
