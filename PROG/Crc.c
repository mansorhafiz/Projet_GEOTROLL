/***************************************************************************/
/*                                                                         */
/*                                  Crc.c                                  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Calcul Crc 8 bits et 16 bits                                            */
/*                                                                         */
/* - Calcul Crc 8 bits d'un buffer                                         */
/*      aBYTE crc_Calc8( BYTE * i_pbyBuf, DWORD i_dwSize ) ;               */
/* - Mise à jour Crc 8 bits avec un buffer                                 */
/*      aBYTE crc_Update8( BYTE * i_pbyBuf, DWORD i_dwSize,                */
/*                         aBYTE i_byCrc ) ;                               */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                   */
/***************************************************************************/
/* Création     : 19.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Calcul Crc 8 bits d'un buffer                                           */
/*    - <i_pbyBuf> adresse buffer                                          */
/*    - <i_dwSize> nombre octets dans buffer                               */
/* Retour :                                                                */
/*    - Crc8 calculé                                                       */
/* Calcul Crc avec polynôme : x8 + x5 + x4 + 1 (0x31)                      */
/*-------------------------------------------------------------------------*/

aBYTE crc_Calc8( BYTE * i_pbyBuf, DWORD i_dwSize )
{
   return crc_Update8( i_pbyBuf, i_dwSize, 0 ) ;
}



/*-------------------------------------------------------------------------*/
/* Mise à jour Crc 8 bits avec un buffer                                   */
/*    - <i_pbyBuf> adresse buffer                                          */
/*    - <i_dwSize> nombre octets dans buffer                               */
/*    - <i_byCrc> valeur courante Crc8                                     */
/* Retour :                                                                */
/*    - Crc8 mis à jour                                                    */
/* Calcul Crc avec polynôme : x8 + x5 + x4 + 1 (0x31)                      */
/*-------------------------------------------------------------------------*/

ARMFUNC aBYTE crc_Update8( BYTE * i_pbyBuf, DWORD i_dwSize, aBYTE i_byCrc )
{
   aBYTE byVal ;

   while ( i_dwSize != 0 )        /* parcours du buffer */
   {
      byVal = *i_pbyBuf++ ;       /* lecture d'un octet */

      byVal ^= i_byCrc ;          /* mise à jour Crc8 avec octet */
      i_byCrc = 0 ;
      if ( byVal & 0x01 )
         i_byCrc ^= 0x5E ;
      if ( byVal & 0x02 )
         i_byCrc ^= 0xBC ;
      if ( byVal & 0x04 )
         i_byCrc ^= 0x61 ;
      if ( byVal & 0x08 )
         i_byCrc ^= 0xC2 ;
      if ( byVal & 0x10 )
         i_byCrc ^= 0x9D ;
      if ( byVal & 0x20 )
         i_byCrc ^= 0x23 ;
      if ( byVal & 0x40 )
         i_byCrc ^= 0x46 ;
      if ( byVal & 0x80 )
         i_byCrc ^= 0x8C ;
      --i_dwSize ;
   }

   return i_byCrc ;
}
