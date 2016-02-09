/***************************************************************************/
/*                                                                         */
/*                                  Tempo.c                                */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Gestion des temporisations                                              */
/*                                                                         */
/* - Attente délai en ms                                                   */
/*    void tp_Delay( WORD i_wDelay )                                       */
/*                                                                         */
/* - Lecture base de temps en 1 ms                                         */
/*    WORD tp_Time1ms( void )                                              */
/*                                                                         */
/* - Lecture base de temps en 10 ms                                        */
/*    WORD tp_Time10ms( void )                                             */
/*                                                                         */
/* - Lecture base de temps en 100 ms                                       */
/*    WORD tp_Time100ms( void )                                            */
/*                                                                         */
/* - Calcul temps restant avec base de temps 1 ms                          */
/*    WORD tp_Tst1ms( WORD * o_pwStart, WORD i_wDuration )                 */
/*                                                                         */
/* - Calcul temps restant avec base de temps 10 ms                         */
/*    WORD tp_Tst10ms( WORD * o_pwStart, WORD i_wDuration )                */
/*                                                                         */
/* - Calcul temps restant avec base de temps 100 ms                        */
/*    WORD tp_Tst100ms( WORD * o_pwStart, WORD i_wDuration )               */
/*                                                                         */
/***************************************************************************/
/* Déclarations : Pic.h                                                    */
/***************************************************************************/
/* Création     : 19.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#include "Pic.h"



/*-------------------------------------------------------------------------*/
/* Attente délai en ms                                                     */
/*-------------------------------------------------------------------------*/

void tp_Delay( WORD i_wDelay )
{
   WORD wTime ;

   wTime = ti_Get1ms() ;
   while( ( ti_Get1ms() - wTime ) < i_wDelay ) ;
}



/*-------------------------------------------------------------------------*/
/* Lecture de l'heure en ms                                                */
/* Retourne une valeur s'incrementant toutes les ms                        */
/* Retourne 1 si le compteur vaut 0 au moment de l'appel                   */
/*-------------------------------------------------------------------------*/

WORD tp_Time1ms( void )
{
   WORD wTime ;

   wTime = ti_Get1ms() ;
   if ( wTime == 0 )
      wTime = 1 ;
   return wTime ;
}



/*-------------------------------------------------------------------------*/
/* Lecture de l'heure en 10 ms                                             */
/* Retourne une valeur s'incrementant toutes les 10 ms                     */
/* Retourne 1 si le compteur vaut 0 au moment de l'appel                   */
/*-------------------------------------------------------------------------*/

WORD tp_Time10ms( void )
{
   WORD wTime ;

   wTime = ti_Get10ms() ;
   if ( wTime == 0 )
      wTime = 1 ;
   return wTime ;
}



/*-------------------------------------------------------------------------*/
/* Lecture de l'heure en 100 ms                                            */
/* Retourne une valeur s'incrementant toutes les 100 ms                    */
/* Retourne 1 si le compteur vaut 0 au moment de l'appel                   */
/*-------------------------------------------------------------------------*/

WORD tp_Time100ms( void )
{
   WORD wTime ;

   wTime = ti_Get100ms() ;
   if ( wTime == 0 )
      wTime = 1 ;
   return wTime ;
}



/*-------------------------------------------------------------------------*/
/* Test du temps restant pour une temporisation en ms                      */
/* Retourne le nombre de ms pour arriver a la fin d'une temporisation      */
/* ayant debute a l'heure <*Start> et d'une longueur de <Duration> ms      */
/* Retourne 0 et met <*Start> a 0 si le temps écoulé depuis <*Start>       */
/* est plus grand que <Duration>. Retourne 0 si <*Start> est a 0           */
/*-------------------------------------------------------------------------*/

WORD tp_Tst1ms( WORD * o_pwStart, WORD i_wDuration )
{
   WORD wTime ;

   if ( *o_pwStart == 0 )           /* Test si tempo déjà terminée */
      return 0 ;                    /* Indication tempo terminée */

   wTime = ti_Get1ms() ;
   if ( wTime == 0 )
      wTime = 1 ;

   wTime -= *o_pwStart ;            /* Calcul temps écoulé depuis le debut */
   if ( wTime >= i_wDuration )      /* Test si temps écoulé superieur a duree */
   {
      *o_pwStart = 0 ;              /* Indication tempo terminée */
      return 0 ;
   }

   return i_wDuration - wTime ;     /* Temps restant pour arriver a duree */
}



/*-------------------------------------------------------------------------*/
/* Test du temps restant pour une temporisation en 10 ms                   */
/* Retourne le nombre de ms pour arriver a la fin d'une temporisation      */
/* ayant debute a l'heure <*Start> et d'une longueur de <Duration> *10ms   */
/* Retourne 0 et met <*Start> a 0 si le temps écoulé depuis <*Start>       */
/* est plus grand que <Duration>. Retourne 0 si <*Start> est a 0           */
/*-------------------------------------------------------------------------*/

WORD tp_Tst10ms( WORD * o_pwStart, WORD i_wDuration )
{
   WORD wTime ;

   if ( *o_pwStart == 0 )           /* Test si tempo déjà terminée */
      return 0 ;                    /* Indication tempo terminée */

   wTime = ti_Get10ms() ;
   if ( wTime == 0 )
      wTime = 1 ;

   wTime -= *o_pwStart ;            /* Calcul temps écoulé depuis le debut */
   if ( wTime >= i_wDuration )      /* Test si temps écoulé superieur a duree */
   {
      *o_pwStart = 0 ;              /* Indication tempo terminée */
      return 0 ;
   }

   return i_wDuration - wTime ;     /* Temps restant pour arriver a duree */
}



/*-------------------------------------------------------------------------*/
/* Test du temps restant pour une temporisation en 100 ms                  */
/* Retourne le nombre de ms pour arriver a la fin d'une temporisation      */
/* ayant debute a l'heure <*Start> et d'une longueur de <Duration> *100ms  */
/* Retourne 0 et met <*Start> a 0 si le temps écoulé depuis <*Start>       */
/* est plus grand que <Duration>. Retourne 0 si <*Start> est a 0           */
/*-------------------------------------------------------------------------*/

WORD tp_Tst100ms( WORD * o_pwStart, WORD i_wDuration )
{
   WORD wTime ;

   if ( *o_pwStart == 0 )           /* Test si tempo déjà terminée */
      return 0 ;                    /* Indication tempo terminée */

   wTime = ti_Get100ms() ;
   if( wTime == 0 )
      wTime = 1 ;

   wTime -= *o_pwStart ;            /* Calcul temps écoulé depuis le debut */
   if ( wTime >= i_wDuration )      /* Test si temps écoulé superieur a duree */
   {
      *o_pwStart = 0 ;              /* Indication tempo terminée */
      return 0 ;
   }

   return i_wDuration - wTime ;     /* Temps restant pour arriver a duree */
}

