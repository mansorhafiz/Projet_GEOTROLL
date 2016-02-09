/***************************************************************************/
/*                                                                         */
/*                                  InOut.C                                */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion entrées/sorties micro-contrôleur PIC                            */
/*                                                                         */
/* - Ecriture sorties micro-contrôleur PIC                                 */
/*      void io_Write( aBYTE i_byPort, aBYTE i_byMsk, aBYTE i_byOut) ;     */
/* - Lecture entrées micro-contrôleur PIC                                  */
/*      aBYTE io_Read( aBYTE i_byPort, aBYTE i_byMsk) ;                    */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 19.02.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.02.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include <p18f4550.h>
#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Un port du Pic est constitué de 3 registres :                           */
/*    - TRIS : configuration direction, =1 entrée, =0 sortie               */
/*    - PORT : lecture état ligne en entrée et écriture état ligne en      */
/*             sortie                                                      */
/*    - LAT : écriture état ligne en sortie et relecture état sortie       */
/* ATTENTION : lorsque patte configurée en sortie, même si la patte est à  */
/* 1, la lecture du registre PORT retourne 0. Le registre PORT doit être*  */
/* utilisé uniquement pour lire l'état des entrées. Pour manipuler les     */
/* sorties, il faut utiliser uniquement le registre LAT.                   */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* Définitions                                                             */
/*-------------------------------------------------------------------------*/

#define IO_MSK_A    0x00
#define IO_MSK_B    0x00            
#define IO_MSK_C    0x00		
#define IO_MSK_D    0x08		/* ligne led run en RD3 */
#define IO_MSK_E    0x00



/*-------------------------------------------------------------------------*/
/* Ecriture sorties micro-contrôleur PIC                                   */
/*    - <i_byPort> numéro port µC, 'A'-'E'                                 */
/*    - <i_byMsk> masque pour écriture sorties                             */
/*    - <i_byOut> états sorties à écrire                                   */
/*-------------------------------------------------------------------------*/

void io_Write( aBYTE i_byPort, aBYTE i_byMsk, aBYTE i_byOut)
{
   switch( i_byPort )               /* décodage port µC */
   {
      case 1 :                      /* port A */
         i_byMsk &= ~IO_MSK_A ;
         TRISA = TRISA & ~i_byMsk ;
         LATA = ( LATA & ~i_byMsk ) | ( i_byOut & i_byMsk ) ;
         break ;
      case 2 :                      /* port B */
         i_byMsk &= ~IO_MSK_B ;
         TRISB = TRISB & ~i_byMsk ;
         LATB = ( LATB & ~i_byMsk ) | ( i_byOut & i_byMsk ) ;
         break ;
      case 3 :                      /* port C */
         i_byMsk &= ~IO_MSK_C ;
         TRISC = TRISC & ~i_byMsk ;
         LATC = ( LATC & ~i_byMsk ) | ( i_byOut & i_byMsk ) ;
         break ;
      case 4 :                      /* port D */
         i_byMsk &= ~IO_MSK_D ;
         TRISD = TRISD & ~i_byMsk ;
         LATD = ( LATD & ~i_byMsk ) | ( i_byOut & i_byMsk ) ;
         break ;
      case 5 :                      /* port E */
         i_byMsk &= ~IO_MSK_E ;
         TRISE = TRISE & ~i_byMsk ;
         LATE = ( LATE & ~i_byMsk ) | ( i_byOut & i_byMsk ) ;
         break ;
      default :
         return ;
   }
}



/*-------------------------------------------------------------------------*/
/* Lecture entrées micro-contrôleur PIC                                    */
/*    - <i_byPort> numéro port µC, 'A'-'E'                                 */
/*    - <i_byMsk> masque pour lecture entrées                              */
/* Retour :                                                                */
/*    - états lus des entrées                                              */
/*-------------------------------------------------------------------------*/

aBYTE io_Read( aBYTE i_byPort, aBYTE i_byMsk )
{
   volatile near unsigned char Port ;

   switch( i_byPort )               /* décodage port µC */
   {
      case 1 :                      /* port A */
         i_byMsk &= ~IO_MSK_A ;
         TRISA = TRISA | i_byMsk ;
         Port = PORTA & i_byMsk ;
         break ;
      case 2 :                      /* port B */
         i_byMsk &= ~IO_MSK_B ;
         TRISB = TRISB | i_byMsk ;
         Port = PORTB & i_byMsk ;
         break ;
      case 3 :                      /* port C */
         i_byMsk &= ~IO_MSK_C ;
         TRISC = TRISC | i_byMsk ;
         Port = PORTC & i_byMsk ;
         break ;
      case 4 :                      /* port D */
         i_byMsk &= ~IO_MSK_D ;
         TRISD = TRISD | i_byMsk ;
         Port = PORTD & i_byMsk ;
         break ;
      case 5 :                      /* port E */
         i_byMsk &= ~IO_MSK_E ;
         TRISE = TRISE | i_byMsk ;
         Port = PORTE & i_byMsk ;
         break ;
      default :
         return 0 ;
   }
   return Port ;
}

