/***************************************************************************/
/*                                                                         */
/*                                 Uart1.c                                 */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion communication avec Uart 1 ( PC )                                */
/*                                                                         */
/* - Ouverture port communication                                          */
/*      void ua1_Open( DWORD i_dwBaudRate ) ;                              */
/* - Fermeture port communication                                          */
/*      void ua1_Close( void ) ;                                           */
/*                                                                         */
/* - Lecture nombre octets reçus                                           */
/*      aBYTE ua1_GetReceiveSize( void ) ;                                 */
/* - Réception buffer octets                                               */
/*      aBYTE ua1_Receive( void * o_pvBuf, aBYTE i_byMaxSize ) ;           */
/* - Lecture longueur maximum buffer réception                             */
/*      aBYTE ua1_GetMaxReceiveSize( void ) ;                              */
/*                                                                         */
/* - Lecture place disponible pour émission                                */
/*      aBYTE ua1_GetSendSize( void ) ;                                    */
/* - Emission buffer octets                                                */
/*      aRESULT ua1_Send( void * i_pvBuf, aBYTE i_bySize ) ;               */
/* - Lecture longueur maximum buffer pouvant être émis                     */
/*      aBYTE ua1_GetMaxSendSize( void ) ;                                 */
/*                                                                         */
/* - Traitement interruption                                               */
/*      void ua1_ProcessIt( void ) ;                                       */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 12.02.2009  L. Darras        Version 1.0                 */
/* Vérifié      : 26.03.2009  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Hard.h"
#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Définitions                                                             */
/*-------------------------------------------------------------------------*/

#define UA1_SENDSIZE    50                /* taille buffer émission */

#define UA1_RECSIZE     50                /* taille buffer réception */
                                          /* niveau buffer réception pour gestion RTS */
#define UA1_RECRTS      (UA1_RECSIZE/4)



/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static BYTE l_byPtrSendIn ;               /* index écriture dans buffer émission */
static BYTE l_byPtrSendOut ;              /* index lecture dans buffer émission */
static BYTE volatile l_byNbSend ;         /* nombre caractère dans buffer émission */
static BYTE l_abyBufSend [UA1_SENDSIZE] ; /* buffer émission */

static BYTE l_byPtrRecIn ;                /* index écriture dans buffer réception */
static BYTE l_byPtrRecOut ;               /* index lecture dans buffer réception */
static BYTE volatile l_byNbRec ;          /* nombre caractère dans buffer réception */
static BYTE l_abyBufRec [UA1_RECSIZE] ;   /* buffer réception */



/*-------------------------------------------------------------------------*/
/* Ouverture port communication 1                                          */
/*    - <i_dwBaudRate> vitesse dialogue, baud                              */
/*      Contrôle RTS/CTS toujours valide :                                 */
/*      Ligne RTS : sortie, active en TTL à 0(+12V), et inactive à 1(-12V).*/
/*          + Activée à l'initialisation.                                  */
/*          + Inactivée pendant l'interruption de réception si buffer      */
/*            rempli au 3/4, et re-activée lors de lecture buffer si       */
/*            occupation buffer devient infèrieure à 1/4.                  */
/*      Ligne CTS : entrée, active en TTL à 0(+12V), et inactive à 1(-12V).*/
/*          + Transmission UART uniquement si ligne CTS active             */
/*-------------------------------------------------------------------------*/

void ua1_Open( DWORD i_dwBaudRate )
{
                                 /* initialisation variables locales */
   l_byPtrSendIn = 0 ;
   l_byPtrSendOut = 0 ;
   l_byNbSend = 0 ;
   l_byPtrRecIn = 0 ;
   l_byPtrRecOut = 0 ;
   l_byNbRec = 0 ;
                                 /* configuration Entrées / Sorties UART */

   O_UART1_TX_DIR = 0 ;          /* ligne TX UART donc en sortie */
   I_UART1_RX_DIR = 1 ;          /* ligne RX UART donc en entrée */

                                 /* configuration UART */
   TXSTA = 0 ;                  /* Reset USART */
   RCSTA = 0 ;
                                 /* initialisation registre TXSTA */
   TXSTAbits.TX9 = 0 ;          /* mode 8 bits */
   TXSTAbits.TXEN = 1 ;         /* validation transmission */
   TXSTAbits.SYNC = 0 ;         /* mode asynchrone */
   TXSTAbits.SENDB = 0 ;        /* pas d'envoi de break */
   TXSTAbits.BRGH = 1 ;         /* sélection high baudrate */

   BAUDCONbits.BRG16 = 1 ;      /* validation registre baudrate 16 bits */

   switch ( i_dwBaudRate )       /* pour SYNC=0, BRGH=1, BRG16=1, FOSC = 48MHz, */
                                 /* Baudrate = FOSC/(4*(n+1)), */
                                 /* avec n = (SPBRGH<<8 + SPBRG))*/
   {
      case 115200 :
         SPBRG = 0x67 ;
         SPBRGH = 0 ;
         break ;
      case 57600 :
         SPBRG = 0xCF ;
         SPBRGH = 0 ;
         break ;
      case 19200 :
         SPBRG = 0x70 ;
         SPBRGH = 0x02 ;
         break ;
      case 9600 :
      default :                  /* baudrate 9600 par défaut */
         SPBRG = 0xE1 ;
         SPBRGH = 0x04 ;
         break ;
   }

   RCSTAbits.SPEN = 1 ;         /* validation Uart */
   RCSTAbits.RX9 = 0 ;          /* mode 8 bits */
   RCSTAbits.CREN = 1 ;         /* validation réception continu */

                                 /* configuration Interruptions UART */
   IPR1bits.TXIP = 1 ;           /* Interruption transmission UART1 priorité haute */
   IPR1bits.RCIP = 1 ;           /* Interruption réception UART1 priorité haute */

   PIR1bits.TXIF = 0 ;
   PIR1bits.RCIF = 0 ;
   PIE1bits.RCIE = 1 ;           /* validation interruption sur réception */

}



/*-------------------------------------------------------------------------*/
/* Fermeture port communication 1                                          */
/*-------------------------------------------------------------------------*/

void ua1_Close( void )
{
   PIE1bits.RCIE = 0 ;           /* désactivation interruption réception UART */
   PIE1bits.TXIE = 0 ;           /* désactivation interruption émission UART */
   RCSTAbits.CREN = 0 ;         /* désactivation réception continue UART */
   RCSTAbits.SPEN = 0 ;         /* désactivation réception UART */
   TXSTAbits.TXEN = 0 ;         /* désactivation émission UART */

   O_UART1_TX_DIR = 1 ;          /* on repositionne lignes RX/TX en entrées par défaut */
   I_UART1_RX_DIR = 1 ;
}



/*-------------------------------------------------------------------------*/
/* Lecture nombre octets reçus sur port communication 1                    */
/* Retour :                                                                */
/*    - nombre octets reçus en attente et donc pas encore lus avec         */
/*      ua1_Receive()                                                      */
/*-------------------------------------------------------------------------*/

aBYTE ua1_GetReceiveSize( void )
{
   return l_byNbRec ;
}



/*-------------------------------------------------------------------------*/
/* Réception buffer octets sur port communication 1                        */
/*    - <o_pvBuf> adresse buffer pour retour octets reçus                  */
/*    - <i_byMaxSize> nombre maximum octets à transférer dans <o_pvBuf>    */
/* Retour :                                                                */
/*    - nombre octets transférés dans <o_pvBuf>                            */
/*    - 0 si port communication pas ouvert                                 */
/*-------------------------------------------------------------------------*/

aBYTE ua1_Receive( void * o_pvBuf, aBYTE i_byMaxSize )
{
   BYTE * p ;
   aBYTE bySize ;                /* nombre octets recus */
   aBYTE byNbRec ;

   bySize = 0 ;
   p = o_pvBuf ;
   while ( i_byMaxSize > 0 )
   {
      if ( l_byNbRec > 0 )
      {
         *p = l_abyBufRec[l_byPtrRecOut] ;
         ++p ;
         ++l_byPtrRecOut ;
         if ( l_byPtrRecOut >= (BYTE)sizeof(l_abyBufRec) )
            l_byPtrRecOut = 0 ;  /* index lecture revient au début buffer */

            /* Note : il faut absolument décrémenter <l_byNbRec> AVANT de */
            /* valider les interruptions.                                  */

         ++bySize ;
         --l_byNbRec ;           /* décrémentation nombre cars recus */
         --i_byMaxSize ;
      }
      else
         break ;
   }

   byNbRec = l_byNbRec ;
   if ( byNbRec <= UA1_RECRTS )
   {
      O_UART1_RTS = 0 ;          /* re-validation réception par signal RTS actif */
   }

   return bySize ;
}



/*-------------------------------------------------------------------------*/
/* Lecture longueur maximum buffer pouvant être reçu sur port com 1        */
/* Retour :                                                                */
/*    - taille buffer réception                                             */
/*-------------------------------------------------------------------------*/

aBYTE ua1_GetMaxReceiveSize( void )
{
   return (BYTE)sizeof(l_abyBufRec) ;
}



/*-------------------------------------------------------------------------*/
/* Lecture place disponible pour émission sur port communication 1         */
/* Retour :                                                                */
/*    - nombre octets disponible dans buffer émission                      */
/*-------------------------------------------------------------------------*/

aBYTE ua1_GetSendSize( void )
{
   return ( (BYTE)sizeof(l_abyBufSend) - l_byNbSend ) ;
}



/*-------------------------------------------------------------------------*/
/* Emission buffer octets sur port communication 1                         */
/*    - <i_pvBuf> buffer octets à émettre                                  */
/*    - <i_bySize> nombre octets à émettre                                 */
/* Retour :                                                                */
/*    - OK émission correcte                                               */
/*    - ERR pas assez de place dans buffer émission                        */
/*-------------------------------------------------------------------------*/

aRESULT ua1_Send( void C* i_pvBuf, aBYTE i_bySize )
{
   BYTE C* p ;
   aBYTE byFreeSize ;

   byFreeSize = (BYTE)sizeof(l_abyBufSend) - l_byNbSend ;

   if ( i_bySize > byFreeSize )
      return ERR ;

   p = i_pvBuf ;
   while ( i_bySize > 0 )
   {
      l_abyBufSend[l_byPtrSendIn] = *p ;
      ++p ;
      ++l_byPtrSendIn ;
      if ( l_byPtrSendIn >= (BYTE)sizeof(l_abyBufSend) )
         l_byPtrSendIn = 0 ;        /* index écriture revient au début buffer */

         /* Note : il faut absolument incrémenter <l_byNbSend> AVANT de */
         /* valider les interruptions.                                  */

      ++l_byNbSend ;                /* incrémentation nombre cars à envoyer */
      PIE1bits.TXIE = 1 ;           /* validation interruption sur transmission */

      --i_bySize ;
   }

   return OK ;
}



/*-------------------------------------------------------------------------*/
/* Lecture longueur maximum buffer pouvant être émis sur port com 1        */
/* Retour :                                                                */
/*    - taille buffer émission                                             */
/*-------------------------------------------------------------------------*/

aBYTE ua1_GetMaxSendSize( void )
{
   return (BYTE)sizeof(l_abyBufSend) ;
}



/*-------------------------------------------------------------------------*/
/* Traitement interruption UART 1                                          */
/*-------------------------------------------------------------------------*/

void ua1_ProcessIt( void )
{
   aBOOL bCTSHigh ;

   if ( PIR1 & PIE1 & 0x10 )           /* si TXIE ET TXIF actifs */
   {
      PIR1bits.TXIF = 0 ;
                                       /* traitement interruption émission UART */
      if ( l_byNbSend > 0 )            /* si au moins 1 caractère à envoyer */
      {
         if ( ! I_UART1_CTS )          /* émission si gestion CTS et CTS actif */
         {
            TXREG = l_abyBufSend[l_byPtrSendOut] ;
            ++l_byPtrSendOut ;

            if ( l_byPtrSendOut >= (BYTE)sizeof(l_abyBufSend) )
               l_byPtrSendOut = 0 ;    /* index lecture remis au début buffer */
            --l_byNbSend ;             /* décrementation nb cars à envoyer */
         }
         else
            TXSTAbits.TXEN = 0 ;      /* dévalidation transmission UART1 */
      }
      else
         PIE1bits.TXIE = 0 ;           /* dévalidation interruption sur transmission UART1 */
   }

   if ( PIR1 & PIE1 & 0x20 )           /* si RCIE ET RCIF actifs */
   {
      PIR1bits.RCIF = 0 ;
      if ( l_byNbRec < UA1_RECSIZE )   /* si place dans buffer */
      {                                /* lecture caractère et stockage */
         l_abyBufRec[l_byPtrRecIn] = RCREG ;
         ++l_byPtrRecIn ;
         if ( l_byPtrRecIn >= (BYTE)sizeof(l_abyBufRec) )
            l_byPtrRecIn = 0 ;         /* index écriture remis au début buffer */
         ++l_byNbRec ;
      }
      else                             /* buffer plein */
      {
         RCREG ;                      /* lecture caractère pour acq. interruption */
      }

      if ( l_byNbRec >= ( UA1_RECSIZE - UA1_RECRTS ) )
      {
         O_UART1_RTS = 1 ;             /* RTS inactif */
      }
   }
}
