/***************************************************************************/
/*                                                                         */
/*                                  Pic.h                                  */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* D�finitions et fonctions prototypes pour le logiciel de test            */
/*                                                                         */
/***************************************************************************/
/* Cr�ation     : 19.11.2015  L. Darras        Version 1.0                 */
/* V�rifi�      : 19.11.2015  L. Darras                                    */
/* Modification :                                                          */
/***************************************************************************/



#ifndef __PIC_H
#define __PIC_H

#include "Define.h"



/*-------------------------------------------------------------------------*/
/* D�finitions                                                             */
/*-------------------------------------------------------------------------*/

#define VERSION    "01"          /* num�ro de version (2 car) */

#define REVISION   "0"           /* num�ro de r�vision (1 car) */

#define CORRECT    " "           /* indice de correction (1 car) */

#define DEVICE     "GEOTROLL"    /* type appareil : carte GEOTROLL */

                                 /* bits caract�ristiques */
#define FEAT_CMDREAD   0x0001    /* pr�sence commande lecture */

                                 /* liste bits caract�ristiques compl�mentaires */
#define FEATURE    ( FEAT_CMDREAD )

#define BOOT_FLASHBASE        0x100000  /* adresse base Flash */
#define BOOT_FLASHSIZE        0x040000  /* taille Flash (256 ko) */

#define BOOT_SIZE               0x2000  /* taille  Boot (8 ko) */
                                        /* adresse num�ro s�rie dans secteur Boot */
#define BOOT_SERIALNUMBER_ADDR  ( BOOT_FLASHBASE + \
                                ( BOOT_SIZE - 16 - BOOT_SERIALNUMBER_SIZE ) )
#define BOOT_SERIALNUMBER_SIZE      32  /* taille num�ro s�rie dans secteur Boot */

                                        /* adresse ent�te application */
#define BOOT_APPHEADER_ADDR     ( BOOT_FLASHBASE + BOOT_SIZE + 0xA0 )



/*-------------------------------------------------------------------------*/
/* Codes commande, r�ponse et erreur                                       */
/*-------------------------------------------------------------------------*/

                                      /* identificateur trame re�ue */
#define COM_C_DEVICE            0x80  /* lecture appareil */
#define COM_C_VERSION           0x81  /* lecture version */
#define COM_C_DATECREATE        0x82  /* lecture date/heure g�n�ration */
#define COM_C_RESTART           0x83  /* red�marrage Boot */
#define COM_C_APPLI             0x84  /* passage � l'application */
#define COM_C_SERIALNUMBER      0x85  /* lecture num�ro s�rie carte */
#define COM_C_FEATURE           0x86  /* lecture caract�ristiques */

#define COM_C_LSTZONES          0x89  /* lecture liste des zones */
#define COM_C_STARTENDPROGRAM   0x8A  /* d�but/fin programmation zone */
#define COM_C_PROGRAMDATA       0x8B  /* programmation donn�es */
#define COM_C_READDATA          0x8C  /* lecture donn�es */

                                      /* identificateur trame �mise */
                                      /* code r�ponse d'une commande */
#define COM_RESP( byCmd )       ( byCmd & ~0x80 )
#define COM_R_ERROR             0x7F  /* erreur */


#define COM_S_ERROR           0x8000  /* 1er code retour erreur */
                                      /* d�finition code erreur */
#define _ER(Code)             ( COM_S_ERROR + (Code) )
#define COM_S_ERRUNKNOWN   _ER(0x01)  /* code commande inconnu */
#define COM_S_ERRMODE      _ER(0x02)  /* commande interdite dans ce mode de
                                         fonctionnement */
#define COM_S_ERRSTATE     _ER(0x03)  /* commande interdite dans cet �tat */
#define COM_S_ERRSIZE      _ER(0x04)  /* nombre incorrect de param�tres dans commande */
#define COM_S_ERRLIMIT     _ER(0x05)  /* param�tre hors limites dans commande */
#define COM_S_ERRREPSIZE   _ER(0x06)  /* r�ponse trop grande pour ce canal */

#define COM_S_ERRZONE      _ER(0x20)  /* num�ro zone incorrecte */
#define COM_S_ERRADDR      _ER(0x21)  /* adresse en dehors zone en cours
                                         programmation/lecture */
#define COM_S_ERRUNLOCK    _ER(0x22)  /* probl�me d�verrouillage */
#define COM_S_ERRLOCK      _ER(0x23)  /* probl�me verrouillage */
#define COM_S_ERRERASE     _ER(0x24)  /* probl�me effacement */
#define COM_S_ERRNOERASE   _ER(0x25)  /* octet non effac� avant programmation */
#define COM_S_ERRPROGRAM   _ER(0x26)  /* probl�me programmation */
#define COM_S_ERRVERIFY    _ER(0x27)  /* mauvaise valeur octet apr�s programmation */
#define COM_S_ERRREADFLASH _ER(0x28)  /* probl�me lecture Flash */



/*-------------------------------------------------------------------------*/
/* Programme principal                                            (Main.c) */
/*-------------------------------------------------------------------------*/

void main( void ) ;



/*-------------------------------------------------------------------------*/
/* Gestion canaux communication                           (FrameChannel.c) */                   /*Fichier pas trouv�!!*/
/*-------------------------------------------------------------------------*/

typedef struct                 /* description trame */
{
   BYTE byCmdRep ;             /* code commande re�u ou code r�ponse � �mettre */
   BYTE byLen ;                /* longueur donn�es commande re�u, longueur
                                  maximum donn�es r�ponse et longueur donn�es
                                  r�ponse � �mettre */
   BYTE * pbyData ;            /* adresse donn�es commande re�ue ou donn�es
                                  r�ponse � �mettre */
} s_Frame ;

typedef struct                 /* informations canal */
{
   WORD wMaxDataCmd ;          /* nombre maximum octets de donn�es dans une commande */
   WORD wMaxDataRep ;          /* nombre maximum octets de donn�es dans une r�ponse */
} s_FrmInfos ;



/*-------------------------------------------------------------------------*/
/* Gestion des communications sur les diff�rents canaux       (Communic.c) */
/*-------------------------------------------------------------------------*/

void com_ProcessCmd( void ) ;

void com_SetDataError( void C* i_pvBuf, aWORD i_wSize ) ;



/*-------------------------------------------------------------------------*/
/* Commandes lecture appareil, version et date g�n�ration    (c_Version.c) */
/*-------------------------------------------------------------------------*/

aWORD cvers_Exec80( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;
aWORD cvers_Exec81( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;
aWORD cvers_Exec82( BYTE * i_pbyRecData, aWORD i_wLenRecData,
                    BYTE * o_pbySendData, aWORD i_wMaxSendData ) ;


/*-------------------------------------------------------------------------*/
/* Commandes micro-contr�leur                                  (c_Micro.c) */
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
/* D�codage/encodage trames pour dialogue sur port s�rie      (FrameCom.c) */
/*-------------------------------------------------------------------------*/

typedef enum _e_FrameCh_        /* identificateurs canaux communication {FRM_CH_xxx} */
{
   FRM_CH_NULL = 0,             /* aucun canal ou canal inconnu */
   FRM_CH_RS232,                /* liaison s�rie RS232 */
   FRM_CH_USB,                  /* port s�rie virtuel sur Usb */
   FRM_CH_LAST                  /* num�ro (+1) dernier canal */
} e_FrameCh ;
                                /* num�ro 1er canal communication */
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
/* Gestion entr�es/sorties micro-contr�leur PIC                  (InOut.c) */
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
