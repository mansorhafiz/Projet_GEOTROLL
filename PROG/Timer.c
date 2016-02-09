/***************************************************************************/
/*                                                                         */
/*                                 Timer.c                                 */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion timer                                                           */
/*                                                                         */
/* - Initialisation timer                                                  */
/*      void ti_Init( void ) ;                                             */
/* - De-initialisation timer                                               */
/*      void ti_UnInit( void ) ;                                           */
/*                                                                         */
/* - Lecture compteur de ms                                                */
/*      WORD ti_Get1ms( void ) ;                                           */
/*                                                                         */
/* - Lecture compteur de 10 ms                                             */
/*      WORD ti_Get10ms( void ) ;                                          */
/*                                                                         */
/* - Lecture compteur de 100 ms                                            */
/*      WORD ti_Get100ms( void ) ;                                         */
/*                                                                         */
/* - Demande inhibition watchdog                                           */
/*      void ti_InhibWatchdog( void ) ;                                    */
/*                                                                         */
/* - Lecture �tat demande inhibition watchdog depuis dernier reset         */
/*      BOOL ti_IsInhibWatchdog( void ) ;                                  */
/*                                                                         */
/* - Traitement interruption                                               */
/*      void ti_ProcessIt( void ) ;                                        */
/*                                                                         */
/***************************************************************************/
/* D�clarations : Pic.h                                                    */
/***************************************************************************/
/* Cr�ation     : 19.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Hard.h"
#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* D�finitions                                                             */
/*-------------------------------------------------------------------------*/

                                       /* d�finition valeur prescaler TMR1 */
#define T_TMR     ( ( FOSC/4 )/( PRESCALER*FREQ ) )
#define T_TMR1H   ( T_TMR >> 8 )
#define T_TMR1L   ( T_TMR & 0x00FF )



/*-------------------------------------------------------------------------*/
/* Variables                                                               */
/*-------------------------------------------------------------------------*/

static WORD volatile l_t_wTime1ms ;    /* 'heure' � 1 ms */
static WORD volatile l_t_wTime10ms ;   /* 'heure' � 10 ms */
static WORD volatile l_t_wTime100ms ;  /* 'heure' � 100 ms */

static BYTE l_byInc10ms ;              /* compteur pour incr�ment 10 ms */
static BYTE l_byInc100ms ;             /* compteur pour incr�ment 100 ms */
static BOOL l_bWDInhibAsk ;            /* �tat demande inhibition watchdog */
                                       /* �tat d�tection demande inhibition */
                                       /* watchdog depuis dernier reset */
static BOOL l_bWDInhibAskFromLastReset ;


/*-------------------------------------------------------------------------*/
/* Initialisation timer                                                    */
/*-------------------------------------------------------------------------*/

void ti_Init( void )
{
   l_t_wTime1ms = 0 ;
   l_byInc10ms = 10 ;
   l_byInc100ms = 10 ;

   T1CONbits.RD16 = 1 ;       /* validation R/W timer 1 en 1 op�ration de 16 bits */
   T1CONbits.TMR1CS = 0 ;     /* s�lection horloge interne */
                              /* valeur prescaler 1:1 */
   T1CONbits.T1CKPS1 = 0 ;
   T1CONbits.T1CKPS0 = 0 ;

   CCP1CON = 0x0B ;           /* configuration mode comparaison + raz timer si comparaison */
                              /* affectation valeur comparaison */
   CCPR1H = T_TMR1H ;
   CCPR1L = T_TMR1L ;

   IPR1bits.CCP1IP = 0 ;      /* interruption compare timer 1 en priorit� faible */
   PIR1bits.CCP1IF = 0 ;      /* initialisation flag interruption compare timer 1 */
   PIE1bits.CCP1IE = 1 ;      /* validation interruption compare timer 1 */

   T1CONbits.TMR1ON = 1 ;     /* validation timer 1 */
}



/*-------------------------------------------------------------------------*/
/* De-initialisation timer                                                 */
/*-------------------------------------------------------------------------*/

void ti_UnInit( void )
{
   PIE1bits.CCP1IE = 0 ;      /* d�-validation interruption compare timer 1 */
   T1CONbits.TMR1ON = 0 ;     /* d�-validation timer 1 */
}



/*-------------------------------------------------------------------------*/
/* Lecture compteur de ms                                                  */
/*-------------------------------------------------------------------------*/

WORD ti_Get1ms( void )
{
   return l_t_wTime1ms ;
}



/*-------------------------------------------------------------------------*/
/* Lecture compteur de 10 ms                                               */
/*-------------------------------------------------------------------------*/

WORD ti_Get10ms( void )
{
   return l_t_wTime10ms ;
}



/*-------------------------------------------------------------------------*/
/* Lecture compteur de 100 ms                                              */
/*-------------------------------------------------------------------------*/

WORD ti_Get100ms( void )
{
   return l_t_wTime100ms ;
}



/*-------------------------------------------------------------------------*/
/* Traitement interruption                                                 */
/*-------------------------------------------------------------------------*/

void ti_ProcessIt( void )
{
   aBYTE byInc10ms ;
   aBYTE byInc100ms ;

   PIR1bits.CCP1IF = 0 ;         /* re-init flag interruption compare timer 1 */

   ++l_t_wTime1ms ;
   byInc10ms = l_byInc10ms - 1 ;          /* d�c. compteur inc. 10 ms */
   if ( byInc10ms == 0 )                  /* si 10 x 1 ms */
   {
      ++l_t_wTime10ms ;                   /* incr�mentation 'heure' � 10 ms */
      byInc100ms = l_byInc100ms - 1 ;     /* d�c. compteur inc. 100 ms */
      if ( byInc100ms == 0 )              /* si 10 x 10 ms */
      {                                   /* incr�mentation 'heure' � 100 ms */
         ++l_t_wTime100ms ;
         byInc100ms = 10 ;                /* r�-init compteur inc. 100 ms */
      }
      l_byInc100ms = byInc100ms ;

      byInc10ms = 10 ;                    /* r�-init compteur inc. 10 ms */
   }
   l_byInc10ms = byInc10ms ;
}


