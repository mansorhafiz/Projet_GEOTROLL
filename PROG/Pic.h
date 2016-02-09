/***************************************************************************/
/*                                                                         */
/*                                  Pic.h                                  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Définitions et fonctions prototypes pour le logiciel de test            */
/*                                                                         */
/***************************************************************************/
/* Création     : 19.11.2015  L. Darras        Version 1.0                 */
/* Vérifié      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#ifndef __PIC_H
#define __PIC_H

#include "Define.h"



/*-------------------------------------------------------------------------*/
/* Définitions                                                             */
/*-------------------------------------------------------------------------*/

#define VERSION    "01"          /* numéro de version (2 car) */

#define REVISION   "0"           /* numéro de révision (1 car) */

#define CORRECT    " "           /* indice de correction (1 car) */

#define DEVICE     "GEOTROLL"    /* type appareil : carte GEOTROLL */

                                 /* bits caractéristiques */
#define FEAT_CMDREAD   0x0001    /* présence commande lecture */

                                 /* liste bits caractéristiques complémentaires */
#define FEATURE    ( FEAT_CMDREAD )

#define BOOT_FLASHBASE        0x100000  /* adresse base Flash */
#define BOOT_FLASHSIZE        0x040000  /* taille Flash (256 ko) */

#define BOOT_SIZE               0x2000  /* taille  Boot (8 ko) */
                                        /* adresse numéro série dans secteur Boot */
#define BOOT_SERIALNUMBER_ADDR  ( BOOT_FLASHBASE + \
                                ( BOOT_SIZE - 16 - BOOT_SERIALNUMBER_SIZE ) )
#define BOOT_SERIALNUMBER_SIZE      32  /* taille numéro série dans secteur Boot */

                                        /* adresse entête application */
#define BOOT_APPHEADER_ADDR     ( BOOT_FLASHBASE + BOOT_SIZE + 0xA0 )



/*-------------------------------------------------------------------------*/
/* Codes commande, réponse et erreur                                       */
/*-------------------------------------------------------------------------*/

                                      /* identificateur trame reçue */
#define COM_C_DEVICE            0x80  /* lecture appareil */
#define COM_C_VERSION           0x81  /* lecture version */
#define COM_C_DATECREATE        0x82  /* lecture date/heure génération */
#define COM_C_RESTART           0x83  /* redémarrage Boot */
#define COM_C_APPLI             0x84  /* passage à l'application */
#define COM_C_SERIALNUMBER      0x85  /* lecture numéro série carte */
#define COM_C_FEATURE           0x86  /* lecture caractéristiques */

#define COM_C_LSTZONES          0x89  /* lecture liste des zones */
#define COM_C_STARTENDPROGRAM   0x8A  /* début/fin programmation zone */
#define COM_C_PROGRAMDATA       0x8B  /* programmation données */
#define COM_C_READDATA          0x8C  /* lecture données */

                                      /* identificateur trame émise */
                                      /* code réponse d'une commande */
#define COM_RESP( byCmd )       ( byCmd & ~0x80 )
#define COM_R_ERROR             0x7F  /* erreur */


#define COM_S_ERROR           0x8000  /* 1er code retour erreur */
                                      /* définition code erreur */
#define _ER(Code)             ( COM_S_ERROR + (Code) )
#define COM_S_ERRUNKNOWN   _ER(0x01)  /* code commande inconnu */
#define COM_S_ERRMODE      _ER(0x02)  /* commande interdite dans ce mode de
                                         fonctionnement */
#define COM_S_ERRSTATE     _ER(0x03)  /* commande interdite dans cet état */
#define COM_S_ERRSIZE      _ER(0x04)  /* nombre incorrect de paramètres dans commande */
#define COM_S_ERRLIMIT     _ER(0x05)  /* paramètre hors limites dans commande */
#define COM_S_ERRREPSIZE   _ER(0x06)  /* réponse trop grande pour ce canal */

#define COM_S_ERRZONE      _ER(0x20)  /* numéro zone incorrecte */
#define COM_S_ERRADDR      _ER(0x21)  /* adresse en dehors zone en cours
                                         programmation/lecture */
#define COM_S_ERRUNLOCK    _ER(0x22)  /* problème déverrouillage */
#define COM_S_ERRLOCK      _ER(0x23)  /* problème verrouillage */
#define COM_S_ERRERASE     _ER(0x24)  /* problème effacement */
#define COM_S_ERRNOERASE   _ER(0x25)  /* octet non effacé avant programmation */
#define COM_S_ERRPROGRAM   _ER(0x26)  /* problème programmation */
#define COM_S_ERRVERIFY    _ER(0x27)  /* mauvaise valeur octet après programmation */
#define COM_S_ERRREADFLASH _ER(0x28)  /* problème lecture Flash */



/*-------------------------------------------------------------------------*/
/* Programme principal                                            (Main.c) */
/*-------------------------------------------------------------------------*/

void main( void ) ;



/*-------------------------------------------------------------------------*/
/* Gestion canaux communication                           (FrameChannel.c) */                   /*Fichier pas trouvé!!*/
/*-------------------------------------------------------------------------*/

typedef struct                 /* description trame */
{
   BYTE byCmdRep ;             /* code commande reçu ou code réponse à émettre */
   BYTE byLen ;                /* longueur données commande reçu, longueur
                                  maximum données réponse et longueur données
                                  réponse à émettre */
   BYTE * pbyData ;            /* adresse données commande reçue ou données
                                  réponse à émettre */
} s_Frame ;

typedef struct                 /* informations canal */
{
   WORD wMaxDataCmd ;          /* nombre maximum octets de données dans une commande */
   WORD wMaxDataRep ;          /* nombre maximum octets de données dans une réponse */
} s_FrmInfos ;



/*-------------------------------------------------------------------------*/
/* Gestion des communications sur les différents canaux       (Communic.c) */
/*-------------------------------------------------------------------------*/

void com_ProcessCmd( void ) ;

void com_SetDataError( void C* i_pvBuf, aWORD i_wSize ) ;



/*-------------------------------------------------------------------------*/
/* Commandes lecture appareil, version et date génération    (c_Version.c) */
/*-------------------------------------------------------------------------*/

aWORD cvers_Exec80( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;
aWORD cvers_Exec81( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;
aWORD cvers_Exec82( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;


/*-------------------------------------------------------------------------*/
/* Commandes micro-contrôleur                                  (c_Micro.c) */
/*-------------------------------------------------------------------------*/

aWORD cmicro_Exec90( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                     BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;
aWORD cmicro_Exec91( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                     BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;
aWORD cmicro_Exec92( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                     BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;
aBOOL cmicro_IsNeedTrans( DWORD * o_pdwBaudrate, BOOL * o_pbRtsCts,
                          aBOOL i_bReset ) ;



/*-------------------------------------------------------------------------*/
/* Décodage/encodage trames pour dialogue sur port série      (FrameCom.c) */
/*-------------------------------------------------------------------------*/

typedef enum _e_FrameCh_        /* identificateurs canaux communication {FRM_CH_xxx} */
{
   FRM_CH_NULL = 0,             /* aucun canal ou canal inconnu */
   FRM_CH_RS232,                /* liaison série RS232 */
   FRM_CH_USB,                  /* port série virtuel sur Usb */
   FRM_CH_LAST                  /* numéro (+1) dernier canal */
} e_FrameCh ;
                                /* numéro 1er canal communication */
#define FRM_CH_FIRST      FRM_CH_RS232

void frmcom_Init( void ) ;
void frmcom_UnInit( void ) ;

aRESULT frmcom_Receive( s_Frame * o_pFrm ) ;
void frmcom_FreeReceive( void ) ;

aRESULT frmcom_GetSend( s_Frame * o_pFrm ) ;
aRESULT frmcom_Send( s_Frame C* i_pFrm ) ;
void frmcom_FreeSend( void ) ;

void frmcom_TaskCyc(void);


/*-------------------------------------------------------------------------*/
/* Gestion interruptions                                  (Interruption.c) */
/*-------------------------------------------------------------------------*/

void int_Init( void ) ;



/*-------------------------------------------------------------------------*/
/* Gestion communication avec Uart 1 ( PC )                      (Uart1.c) */
/*-------------------------------------------------------------------------*/

void ua1_Open( DWORD i_dwBaudRate) ;
void ua1_Close( void ) ;

aBYTE ua1_GetReceiveSize( void ) ;
aBYTE ua1_Receive( void * o_pvBuf, aBYTE i_byMaxSize ) ;
aBYTE ua1_GetMaxReceiveSize( void ) ;

aBYTE ua1_GetSendSize( void ) ;
aRESULT ua1_Send( void C* i_pvBuf, aBYTE i_bySize ) ;
aBYTE ua1_GetMaxSendSize( void ) ;
aBOOL ua1_IsSendEmpty( void ) ;

void ua1_ProcessIt( void ) ;
void ua1_ProcessCTS( void ) ;



/*-------------------------------------------------------------------------*/
/* Gestion communication avec Uart Usb                         (UartUsb.c) */
/*-------------------------------------------------------------------------*/

void uausb_Open(void);
void uausb_Close(void);

aBYTE uausb_GetReceiveSize(void);
aBYTE uausb_Receive(void * o_pvBuf, aBYTE i_byMaxSize);
aBYTE uausb_GetMaxReceiveSize(void);

aBYTE uausb_GetSendSize(void);
aRESULT uausb_Send(void C* i_pvBuf, aBYTE i_bySize);
aBYTE uausb_GetMaxSendSize(void);
aBOOL uausb_IsSendEmpty(void);

void uausb_TaskCyc(void);



/*-------------------------------------------------------------------------*/
/* Gestion led run                                              (RunLed.c) */
/*-------------------------------------------------------------------------*/

void runled_Init( void ) ;

void runled_TaskCyc( void ) ;



/*-------------------------------------------------------------------------*/
/* Gestion entrées/sorties micro-contrôleur PIC                  (InOut.c) */
/*-------------------------------------------------------------------------*/

void io_Write( aBYTE i_byPort, aBYTE i_byMsk, aBYTE i_byOut) ;
aBYTE io_Read( aBYTE i_byPort, aBYTE i_byMsk) ;



/*-------------------------------------------------------------------------*/
/* Calcul Crc 8 bits et 16 bits                                    (Crc.c) */
/*-------------------------------------------------------------------------*/

aBYTE crc_Calc8( BYTE * i_pbyBuf, DWORD i_dwSize ) ;
ARMFUNC aBYTE crc_Update8( BYTE * i_pbyBuf, DWORD i_dwSize, aBYTE i_byCrc ) ;



/*-------------------------------------------------------------------------*/
/* Gestion tempo                                                 (Tempo.c) */
/*-------------------------------------------------------------------------*/

void tp_Delay( WORD i_wDelay ) ;

WORD tp_Time1ms( void ) ;
WORD tp_Time10ms( void ) ;
WORD tp_Time100ms( void ) ;

WORD tp_Tst1ms( WORD * o_pwStart, WORD i_wDuration ) ;
WORD tp_Tst10ms( WORD * o_pwStart, WORD i_wDuration ) ;
WORD tp_Tst100ms( WORD * o_pwStart, WORD i_wDuration ) ;



/*-------------------------------------------------------------------------*/
/* Gestion timer                                                 (Timer.c) */
/*-------------------------------------------------------------------------*/

void ti_Init( void ) ;
void ti_UnInit( void ) ;

WORD ti_Get1ms( void ) ;
WORD ti_Get10ms( void ) ;
WORD ti_Get100ms( void ) ;

void ti_ProcessIt( void ) ;

#endif  /* __PIC_H */
