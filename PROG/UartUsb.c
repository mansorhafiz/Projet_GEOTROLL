/***************************************************************************/
/*                                                                         */
/*                                 UartUsb.c                               */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion communication avec Uart port USB                                */
/*                                                                         */
/* - Ouverture port communication                                          */
/*      void uausb_Open( void ) ;                                          */
/* - Fermeture port communication                                          */
/*      void uausb_Close( void ) ;                                         */
/*                                                                         */
/* - Test si detection Break                                               */
/*      aBOOL uausb_IsBreak( aBOOL i_bReset ) ;                            */
/*                                                                         */
/* - Lecture nombre octets recus                                           */
/*      aBYTE uausb_GetReceiveSize( void ) ;                               */
/* - Reception buffer octets                                               */
/*      aBYTE uausb_Receive( void * o_pvBuf, aBYTE i_byMaxSize ) ;         */
/* - Lecture longueur maximum buffer reception                             */
/*      aBYTE uausb_GetMaxReceiveSize( void ) ;                            */
/*                                                                         */
/* - Lecture place disponible pour emission                                */
/*      aBYTE uausb_GetSendSize( void ) ;                                  */
/* - Emission buffer octets                                                */
/*      aRESULT uausb_Send( void * i_pvBuf, aBYTE i_bySize ) ;             */
/* - Lecture longueur maximum buffer pouvant etre emis                     */
/*      aBYTE uausb_GetMaxSendSize( void ) ;                               */
/* - Test si emission terminee                                             */
/*      aBOOL uausb_IsSendEmpty( void ) ;                                  */
/*                                                                         */
/* - Traitement interruption                                               */
/*      void uausb_ProcessIt( void ) ;                                     */
/*                                                                         */
/***************************************************************************/
/* Declarations : Pic.h                                                    */
/***************************************************************************/
/* Creation     : 25.11.2015  L. Darras        Version 1.0                 */
/* Modification :                                                          */
/***************************************************************************/

#include "CDC\system\usb\usb.h"

#include "Hard.h"
#include "Define.h"
#include "CDC\io_cfg.h"



/*-------------------------------------------------------------------------*/
/* Definitions                                                             */
/*-------------------------------------------------------------------------*/

#define UAUSB_SENDSIZE    50              /* taille buffer emission */

#define UAUSB_RECSIZE     50              /* taille buffer reception */


/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static BYTE l_byPtrSendIn ;          /* index ecriture dans buffer emission */
static BYTE l_byPtrSendOut ;         /* index lecture dans buffer emission */
static BYTE volatile l_byNbSend ;    /* nombre caractere dans buffer emission */
static BYTE l_abyBufSend [UAUSB_SENDSIZE] ; /* buffer emission */

static BYTE l_byPtrRecIn ;           /* index ecriture dans buffer reception */
static BYTE l_byPtrRecOut ;          /* index lecture dans buffer reception */
static BYTE volatile l_byNbRec ;    /* nombre caractere dans buffer reception */
static BYTE l_abyBufRec [UAUSB_RECSIZE] ;   /* buffer reception */

static BOOL l_bIsBreak ;

// Ring buffers for transfering data to and from the UART ISR:
//  - (Head pointer == Tail pointer) is defined as an empty FIFO
//  - (Head pointer == Tail pointer - 1), accounting for wraparound,
//    is defined as a completely full FIFO.  As a result, the max data
//    in a FIFO is the buffer size - 1.
static BYTE vUARTRXFIFO[65];
static BYTE vUARTTXFIFO[17];
static volatile BYTE *RXHeadPtr = vUARTRXFIFO, *RXTailPtr = vUARTRXFIFO;
static volatile BYTE *TXHeadPtr = vUARTTXFIFO, *TXTailPtr = vUARTTXFIFO;


/*-------------------------------------------------------------------------*/
/* Ouverture port communication usb                                        */
/*-------------------------------------------------------------------------*/

void uausb_Open( void )
{
                                 /* initialisation variables locales */
   l_byPtrSendIn = 0 ;
   l_byPtrSendOut = 0 ;
   l_byNbSend = 0 ;
   l_byPtrRecIn = 0 ;
   l_byPtrRecOut = 0 ;
   l_byNbRec = 0 ;

    mInitializeUSBDriver() ;            /* initialisation port USB */
}



/*-------------------------------------------------------------------------*/
/* Fermeture port communication usb                                        */
/*-------------------------------------------------------------------------*/

void uausb_Close( void )
{
}



/*-------------------------------------------------------------------------*/
/* Test si reception Break sur port communication usb                      */
/*    - <i_bReset> TRUE mise a 0 indicateur apres lecture                  */
/* Retour :                                                                */
/*    - TRUE Break detecte depuis test precedent                           */
/*    - FALSE pas de Break detecte                                         */
/*-------------------------------------------------------------------------*/

aBOOL uausb_IsBreak( aBOOL i_bReset )
{
   aBOOL bRet ;

   bRet = l_bIsBreak ;
   if ( bRet && i_bReset )
      l_bIsBreak = FALSE ;    /* re-initialisation */

   return bRet ;
}



/*-------------------------------------------------------------------------*/
/* Lecture nombre octets recus sur port communication usb                  */
/* Retour :                                                                */
/*    - nombre octets recus en attente et donc pas encore lus avec         */
/*      uausb_Receive()                                                    */
/*-------------------------------------------------------------------------*/

aBYTE uausb_GetReceiveSize( void )
{
   return l_byNbRec ;
}



/*-------------------------------------------------------------------------*/
/* Reception buffer octets sur port communication usb                      */
/*    - <o_pvBuf> adresse buffer pour retour octets reeus                  */
/*    - <i_byMaxSize> nombre maximum octets a transferer dans <o_pvBuf>    */
/* Retour :                                                                */
/*    - nombre octets transferes dans <o_pvBuf>                            */
/*    - 0 si port communication pas ouvert                                 */
/*-------------------------------------------------------------------------*/

aBYTE uausb_Receive( void * o_pvBuf, aBYTE i_byMaxSize )
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

   return bySize ;
}



/*-------------------------------------------------------------------------*/
/* Lecture longueur maximum buffer pouvant etre recu sur port com usb      */
/* Retour :                                                                */
/*    - taille buffer reception                                            */
/*-------------------------------------------------------------------------*/

aBYTE uausb_GetMaxReceiveSize( void )
{
   return (BYTE)sizeof(l_abyBufRec) ;
}



/*-------------------------------------------------------------------------*/
/* Lecture place disponible pour emission sur port communication usb       */
/* Retour :                                                                */
/*    - nombre octets disponible dans buffer emission                      */
/*-------------------------------------------------------------------------*/

aBYTE uausb_GetSendSize( void )
{
   return ((BYTE)sizeof(l_abyBufSend) - l_byNbSend) ;
}



/*-------------------------------------------------------------------------*/
/* Emission buffer octets sur port communication usb                       */
/*    - <i_pvBuf> buffer octets a emettre                                  */
/*    - <i_bySize> nombre octets a emettre                                 */
/* Retour :                                                                */
/*    - OK emission correcte                                               */
/*    - ERR pas assez de place dans buffer emission                        */
/*-------------------------------------------------------------------------*/

aRESULT uausb_Send( void C* i_pvBuf, aBYTE i_bySize )
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

      ++l_byNbSend ;                /* incrémentation nombre cars à envoyer */

      --i_bySize ;
   }

   return OK ;
}



/*-------------------------------------------------------------------------*/
/* Lecture longueur maximum buffer pouvant etre emis sur port com usb      */
/* Retour :                                                                */
/*    - taille buffer emission                                             */
/*-------------------------------------------------------------------------*/

aBYTE uausb_GetMaxSendSize( void )
{
   return (BYTE)sizeof(l_abyBufSend) ;
}



/*-------------------------------------------------------------------------*/
/* Test si emission terminee                                               */
/* Retour :                                                                */
/*    - TRUE si emission terminee, FALSE si non                            */
/*-------------------------------------------------------------------------*/

aBOOL uausb_IsSendEmpty( void )
{
    return FALSE ;
}



/*-------------------------------------------------------------------------*/
/* Tache                                                                   */
/*-------------------------------------------------------------------------*/

void uausb_TaskCyc( void )
{
    byte c, byCnt, byCntRead ;
    BYTE abyBufRec [UAUSB_RECSIZE] ;   /* buffer reception */

    USBCheckBusStatus();                    // Must use polling method
    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 // Interrupt or polling method

    CDCTxService();

    byCntRead = getsUSBUSART((char *)abyBufRec, UAUSB_RECSIZE) ;
    if (byCntRead > 0)                 /* if one byte read at least */
    {
        for ( c=0 ; c<byCntRead ; c++ )
        {            
            l_abyBufRec[l_byPtrRecIn] = abyBufRec[c] ; /* add the byte to buffer */
            l_byPtrRecIn += 1 ;
            if ( l_byPtrRecIn >= (BYTE)sizeof(l_abyBufRec) )
                l_byPtrRecIn = 0 ;         /* index écriture remis au début buffer */
            ++l_byNbRec ;
        }
    }

    if (l_byNbSend > 0)
    {
         if (mUSBUSARTIsTxTrfReady())
         {
             mUSBUSARTTxRam((byte*)(&l_abyBufSend[l_byPtrSendOut]), 1);
             ++l_byPtrSendOut ;
             if ( l_byPtrSendOut >= (BYTE)sizeof(l_abyBufSend) )
                l_byPtrSendOut = 0 ;    /* index lecture remis au début buffer */
             --l_byNbSend ;             /* décrementation nb cars à envoyer */
         }
    }
}

