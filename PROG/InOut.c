/***************************************************************************/
/*                                                                         */
/*                                  InOut.C                                */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion entr�es/sorties micro-contr�leur PIC                            */
/*                                                                         */
/* - Ecriture sorties micro-contr�leur PIC                                 */
/*      void io_Write( aBYTE i_byPort, aBYTE i_byMsk, aBYTE i_byOut) ;     */
/* - Lecture entr�es micro-contr�leur PIC                                  */
/*      aBYTE io_Read( aBYTE i_byPort, aBYTE i_byMsk) ;                    */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 19.02.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 19.02.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include <p18f4550.h>
#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Un port du Pic est constitu� de 3 registres :                           */
/*    - TRIS : configuration direction, =1 entr�e, =0 sortie               */
/*    - PORT : lecture �tat ligne en entr�e et �criture �tat ligne en      */
/*             sortie                                                      */
/*    - LAT : �criture �tat ligne en sortie et relecture �tat sortie       */
/* ATTENTION : lorsque patte configur�e en sortie, m�me si la patte est �  */
/* 1, la lecture du registre PORT retourne 0. Le registre PORT doit �tre*  */
/* utilis� uniquement pour lire l'�tat des entr�es. Pour manipuler les     */
/* sorties, il faut utiliser uniquement le registre LAT.                   */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* D�finitions                                                             */
/*-------------------------------------------------------------------------*/

#define IO_MSK_A    0x00
#define IO_MSK_B    0x00            
#define IO_MSK_C    0x00		
#define IO_MSK_D    0x08		/* ligne led run en RD3 */
#define IO_MSK_E    0x00



/*-------------------------------------------------------------------------*/
/* Ecriture sorties micro-contr�leur PIC                                   */
/*    - <i_byPort> num�ro port �C, 'A'-'E'                                 */
/*    - <i_byMsk> masque pour �criture sorties                             */
/*    - <i_byOut> �tats sorties � �crire                                   */
/*-------------------------------------------------------------------------*/

void io_Write( aBYTE i_byPort, aBYTE i_byMsk, aBYTE i_byOut)
{
   switch( i_byPort )               /* d�codage port �C */
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
/* Lecture entr�es micro-contr�leur PIC                                    */
/*    - <i_byPort> num�ro port �C, 'A'-'E'                                 */
/*    - <i_byMsk> masque pour lecture entr�es                              */
/* Retour :                                                                */
/*    - �tats lus des entr�es                                              */
/*-------------------------------------------------------------------------*/

aBYTE io_Read( aBYTE i_byPort, aBYTE i_byMsk )
{
   volatile near unsigned char Port ;

   switch( i_byPort )               /* d�codage port �C */
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

