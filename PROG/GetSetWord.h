/***************************************************************************/
/*                                                                         */
/*                              GetSetWord.h                               */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* Lecture et �criture entiers non align�s et little ou big endian         */
/*                                                                         */
/***************************************************************************/



#ifndef __GETSETWORD_H
#define __GETSETWORD_H



/*-------------------------------------------------------------------------*/
/* Gestion lecture et �criture de WORD, TBYTE (24 bits), DWORD et          */
/* non align�s en little endian ou big endian                              */
/*                                                                         */
/* Il y a plusieurs modes d'utilisation :                                  */
/*    1) source pr�vu pour toujours g�rer du little endian ou du big       */
/*       endian :                                                          */
/*          . les macros GET/SET_xxx_LE (little endian) ou GET/SET_xxx_BE  */
/*            (big endian) sont directement r�f�renc�es dans le source     */
/*    2) source r�utilisable qui g�re certaines fois du little endian et   */
/*       d'autres fois du big endian                                       */
/*          . la macro GSW_ENDIAN doit �tre d�finie soit � GSW_LITTLEENDIAN*/
/*            si on veut g�rer du little endian soit � GSW_BIGENDIAN si on */
/*            veut g�rer du big endian. Si la macro GSW_ENDIAN n'est pas   */
/*            d�finie avant l'inclusion de ce fichier, c'est du big endian */
/*            qui est g�r� par d�faut                                      */
/*          . les macros GET/SET_xxx sont utilis�es. Elles pointent sur les*/
/*            macros GET/SET_xxx_LE (little endian) ou GET/SET_xxx_BE (big */
/*            endian) selon la d�finition de la macro GSW_ENDIAN           */
/*    3) source qui g�re du little endian ou du big endian en fonction d'un*/
/*       contexte � l'ex�cution :                                          */
/*          . les macros GET/SET_xxx sont utilis�es                        */
/*          . le symbole <GSW_bLittleEndian> doit �tre un bool�en ou une   */
/*            expression valide dans le contexte courant de compilation    */
/*            avant l'appel des macros GET/SET_xxx                         */
/*          . les macros GET/SET_xxx sont �tendues en un op�rateur ternaire*/
/*            qui appelle GET/SET_xxx_LE (little endian) si                */
/*            <GSW_bLittleEndian> � TRUE ou GET/SET_xxx_BE (big endian) si */
/*            <GSW_bLittleEndian> � FALSE                                  */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* D�finitions g�n�rales                                                   */
/*-------------------------------------------------------------------------*/

typedef BYTE TBYTE [3] ;     /* entier 24 bits */

                             /* type endian pour endian fixe */
#define GSW_BIGENDIAN     1  /* big endian */
#define GSW_LITTLEENDIAN  2  /* little endian */


#ifndef GSW_ENDIAN               /* si endian non d�finie */
                                 /* big endian par d�faut */
   #define GSW_ENDIAN GSW_BIGENDIAN
#endif /* GSW_ENDIAN */

#if GSW_ENDIAN == GSW_BIGENDIAN  /* si big endian fixe */

                                 /* macros de base �tendues avec macro xxx_BE() */
   #define GET_WORD   GET_WORD_BE
   #define SET_WORD   SET_WORD_BE
   #define GET_TBYTE  GET_TBYTE_BE
   #define SET_TBYTE  SET_TBYTE_BE
   #define GET_DWORD  GET_DWORD_BE
   #define SET_DWORD  SET_DWORD_BE

#else                            /* little endian fixe */

                                 /* macros de base �tendues avec macro xxx_LE() */
   #define GET_WORD   GET_WORD_LE
   #define SET_WORD   SET_WORD_LE
   #define GET_TBYTE  GET_TBYTE_LE
   #define SET_TBYTE  SET_TBYTE_LE
   #define GET_DWORD  GET_DWORD_LE
   #define SET_DWORD  SET_DWORD_LE

#endif



/*-------------------------------------------------------------------------*/
/* Lecture/�criture WORD, TBYTE, DWORD non align�s en little               */
/* endian                                                                  */
/*-------------------------------------------------------------------------*/

typedef struct
{
   WORD w ;        /* entier 16 bits non align� */
} s_PackWord ;

typedef struct
{
   DWORD dw ;      /* entier 32 bits non align� */
} s_PackDWord ;

                   /* lecture WORD little endian non align� point� par <pwValue> */
#define GET_WORD_LE( pwValue ) \
   ( ((s_PackWord *)(pwValue))->w )

                   /* �criture WORD little endian non align� <wValue> �
                      l'adresse <pwValue> */
#define SET_WORD_LE( pwValue, wValue ) \
   ((s_PackWord *)(pwValue))->w = (wValue)

                   /* lecture TBYTE little endian non align� point� par <ptbyValue> */
#define GET_TBYTE_LE( ptbyValue ) \
   MAKEDWORD_B( ((BYTE C*)(ptbyValue))[0], ((BYTE C*)(ptbyValue))[1], \
                ((BYTE C*)(ptbyValue))[2], 0 )

                   /* �criture TBYTE little endian non align� <dwValue> �
                      l'adresse <pybyValue> */
#define SET_TBYTE_LE( pybyValue, dwValue ) \
   ((BYTE *)(pybyValue))[0] = LOBYTE( LOWORD( dwValue ) ), \
   ((BYTE *)(pybyValue))[1] = HIBYTE( LOWORD( dwValue ) ), \
   ((BYTE *)(pybyValue))[2] = LOBYTE( HIWORD( dwValue ) )

                   /* lecture DWORD little endian non align� point� par <pdwValue> */
#define GET_DWORD_LE( pdwValue ) \
   ( ((s_PackDWord *)(pdwValue))->dw )

                   /* �criture DWORD little endian non align� <dwValue> �
                      l'adresse <pdwValue> */
#define SET_DWORD_LE( pdwValue, dwValue ) \
   ((s_PackDWord *)(pdwValue))->dw = (dwValue)



/*-------------------------------------------------------------------------*/
/* Lecture/�criture WORD, TBYTE, DWORD et non align�s en big endian        */
/*                                                                         */
/*-------------------------------------------------------------------------*/

                   /* lecture WORD big endian non align� point� par <pwValue> */
#define GET_WORD_BE( pwValue ) \
   MAKEWORD( ((BYTE C*)(pwValue))[1], ((BYTE C*)(pwValue))[0] )

                   /* �criture WORD big endian non align� <wValue> �
                      l'adresse <pwValue> */
#define SET_WORD_BE( pwValue, wValue ) \
   ((BYTE *)(pwValue))[0] = HIBYTE( wValue ), \
   ((BYTE *)(pwValue))[1] = LOBYTE( wValue )

                   /* lecture TBYTE big endian non align� point� par <ptbyValue> */
#define GET_TBYTE_BE( ptbyValue ) \
   MAKEDWORD_B( ((BYTE C*)(ptbyValue))[2], ((BYTE C*)(ptbyValue))[1], \
                ((BYTE C*)(ptbyValue))[0], 0 )

                   /* �criture TBYTE big endian non align� <dwValue> �
                      l'adresse <pybyValue> */
#define SET_TBYTE_BE( pybyValue, dwValue ) \
   ((BYTE *)(pybyValue))[0] = LOBYTE( HIWORD( dwValue ) ), \
   ((BYTE *)(pybyValue))[1] = HIBYTE( LOWORD( dwValue ) ), \
   ((BYTE *)(pybyValue))[2] = LOBYTE( LOWORD( dwValue ) )

                   /* lecture DWORD big endian non align� point� par <pdwValue> */
#define GET_DWORD_BE( pdwValue ) \
   MAKEDWORD_B( ((BYTE C*)(pdwValue))[3], ((BYTE C*)(pdwValue))[2],  \
                ((BYTE C*)(pdwValue))[1], ((BYTE C*)(pdwValue))[0] )

                   /* �criture DWORD big endian non align� <dwValue> �
                      l'adresse <pdwValue> */
#define SET_DWORD_BE( pdwValue, dwValue ) \
   ((BYTE *)(pdwValue))[0] = HIBYTE( HIWORD( dwValue ) ), \
   ((BYTE *)(pdwValue))[1] = LOBYTE( HIWORD( dwValue ) ), \
   ((BYTE *)(pdwValue))[2] = HIBYTE( LOWORD( dwValue ) ), \
   ((BYTE *)(pdwValue))[3] = LOBYTE( LOWORD( dwValue ) )



#endif  /* __GETSETWORD_H */
