/***************************************************************************/
/*                                                                         */
/*                                 Hard.h                                  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Définitions concernant l'utilisation du hard                            */
/*                                                                         */
/***************************************************************************/
/* Création     : 19.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#ifndef __HARD_H
#define __HARD_H



#include "p18cxxx.h"



/*-------------------------------------------------------------------------*/
/* Horloges système                                                        */
/*-------------------------------------------------------------------------*/

#define FOSC      48000000             /* 48 MHz */
#define PRESCALER 1                    /* valeur prescaler */
#define FREQ      1000                 /* 1000 Hz pour période 1ms */


/*-------------------------------------------------------------------------*/
/* Affectation des entrées/sorties                                         */
/*-------------------------------------------------------------------------*/

/* ----- Port A ----- */

/* ----- Port B ----- */

/* ----- Port C ----- */

#define O_UART1_RTS_DIR       TRISCbits.TRISC0  /* Direction output RTS UART1 */
#define O_UART1_RTS           PORTCbits.RC0     /* output RTS UART1 */
#define I_UART1_CTS_DIR       TRISCbits.TRISC1  /* Direction input CTS UART1 */
#define I_UART1_CTS           PORTCbits.RC1     /* input CTS UART1 */
#define O_UART1_TX_DIR        TRISCbits.TRISC6  /* Direction output TX UART1 */
#define I_UART1_RX_DIR        TRISCbits.TRISC7  /* Direction input RX UART1 */

/* ----- Port D ----- */
                                                /* led run en RD3 */
#define O_LED_RUN_DIR         TRISDbits.TRISD3  /* Direction output led run */
#define O_LED_RUN             PORTDbits.RD3     /* output led run */

/* ----- Port E ----- */


#endif /* __HARD_H */
