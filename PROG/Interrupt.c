/***************************************************************************/
/*                                                                         */
/*                                Interrupt.c                              */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion interruptions                                                   */
/*                                                                         */
/* - Initialisation interruptions                                          */
/*      void int_Init( void ) ;                                            */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 19.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "p18cxxx.h"
#include "Pic.h"



/***************************************************************************/
/* Fonctions prototypes                                                    */
/***************************************************************************/

void HighInterrupt( void ) ;
void LowInterrupt( void ) ;



/***************************************************************************/
/* Initialisation interruptions                                            */
/***************************************************************************/

void int_Init( void )
{
   RCONbits.IPEN = 1 ;        /* validation niveaux priorit� sur interruptions */

   INTCONbits.GIEH = 1 ;      /* validation interruptions haute priorit� */
   INTCONbits.GIEL = 1 ;      /* validation interruptions faible priorit� */

}



/*=========================================================================*/

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
   _asm
      GOTO HighInterrupt
   _endasm
}
#pragma code



#pragma code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
   _asm
      GOTO LowInterrupt
   _endasm
}
#pragma code



#pragma interrupt HighInterrupt
void HighInterrupt( void )
{
   BYTE byCar ;

   if ( ( PIR1 & PIE1 & 0x30 ) != 0 )
      ua1_ProcessIt() ;       /* traitement interruptions UART 1 */

}



#pragma interruptlow LowInterrupt
void LowInterrupt( void )
{
   if ( ( PIR1 & PIE1 & 0x04 ) != 0 )
      ti_ProcessIt() ;        /* traitement interruptions timer */
}

