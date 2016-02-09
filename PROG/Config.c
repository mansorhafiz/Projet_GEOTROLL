/***************************************************************************/
/*                                                                         */
/*                               Config.c                                  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Param�trage registres configuration Pic                                 */
/*                                                                         */
/***************************************************************************/
/* Cr�ation     : 18.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 18.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/

/*-------------------------------------------------------------------------*/
/* Param�trage selon type de micro                                         */
/*-------------------------------------------------------------------------*/

#ifdef __18F4550              /* si carte avec 18F4550 */

                              /* CONFIG1L */
					/* - USBDIV : clock source from OSC1/OSC2 */
					/* - CPUDIV : OSC /2 */
					/* - PLLDIV : entr�e 20MHz, donc div
                                             par 5 pour obtenir 4MHz avant PLL */
#pragma config USBDIV = 2, CPUDIV = OSC1_PLL2, PLLDIV = 5

                              /* CONFIG1H */
                              /* - IESO : validation lancement appli en vitesse double
                                          pour r�duire temps lancement */
                              /* - FCMEN : autorisation micro de switch de clock
                                           si clock externe d�faillant */
                              /* - FOSC : s�lection oscillateur HS avec PLL (HSPLL_HS)*/
#pragma config IESO = ON, FCMEN = ON, FOSC = HSPLL_HS

                              /* CONFIG2L */
					/* - VREGEN : validation regulateur alim USB */
					/* - BORV : reset level at 2.7V */
					/* - BOR : brown own reset enabled */
					/* - PWRT : attente de 62ms avant d�marrage */
#pragma config VREGEN = ON, BORV = 2, BOR = ON, PWRT = ON

                              /* CONFIG2H */
                              /* - WDTPS : configuration de la p�riode du watchdog */
                              /* - WDT : d�validation watchdog pour pouvoir le
                                         contr�ler logiciellement par bit SWDTEN de WDTCON */
#pragma config WDTPS = 1, WDT = OFF

                              /* CONFIG3H */
                              /* - CCP2MX : ECCP2/P2A multiplex� avec RC1 */
#pragma config CCP2MX = ON
                              /* CONFIG4L */
                              /* - DEBUG : d�validation mode debugger -> RB6
                                           et RB7 utilis�es en E/S */
                              /* - XINST : validation jeu instructions �tendues
                                           pour optimisation code */
                              /* - STVREN : validation reset micro si pile en overflow */
#pragma config DEBUG = OFF, XINST = OFF, STVREN = ON

                              /* CONFIG5L */
                              /* - CP0 : configuration program en mode non prot�g� */
                              /* - CP1 : configuration program en mode non prot�g� */
                              /* - CP2 : configuration program en mode non prot�g� */
                              /* - CP3 : configuration program en mode non prot�g� */
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF

					/* CONFIG5H */
					/* - CPD : data eeprom code non used */
					/* - CPB : boot block code non used */
#pragma config CPD = OFF, CPB = OFF

					/* CONFIG6L */
					/* - WRT3 : Block write protection non used */
					/* - WRT2 : Block write protection non used */
					/* - WRT1 : Block write protection non used */
					/* - WRT0 : Block write protection non used */
#pragma config WRT3 = OFF, WRT2 = OFF, WRT1 = OFF, WRT0 = OFF

					/* CONFIG6H */
					/* EBTR3 : Block table read protection non used */
					/* EBTR2 : Block table read protection non used */
					/* EBTR1 : Block table read protection non used */
					/* EBTR0 : Block table read protection non used */
#pragma config EBTR3 = OFF, EBTR2 = OFF, EBTR1 = OFF, EBTR0 = OFF

#endif   /* __18F4550 */

