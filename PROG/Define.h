/***************************************************************************/
/*                                                                         */
/*                                Define.h                                 */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* D�finitions globales : types, macros, structures et constantes          */
/*                                                                         */
/***************************************************************************/



#ifndef __DEFINE_H
#define __DEFINE_H



/*-------------------------------------------------------------------------*/
/* Types de base                                                           */
/*-------------------------------------------------------------------------*/

#ifndef __TYPES                    /* types standard */
#define __TYPES

typedef unsigned long int DWORD ;  /* dw  32 bits non sign� */
typedef signed long int SDWORD ;   /* sdw 32 bits sign� */

typedef unsigned short int WORD ;  /* w   16 bits non sign� */
typedef WORD aWORD ;               /* w      param�tre/variable locale */
typedef signed short int SWORD ;   /* sw  16 bits sign� */
typedef SWORD aSWORD ;             /* sw     param�tre/variable locale */

typedef unsigned char BYTE ;       /* by   8 bits non sign� */
typedef BYTE aBYTE ;               /* by     param�tre/variable locale */
typedef signed char SBYTE ;        /* sby  8 bits sign� */
typedef SBYTE aSBYTE ;             /* sby    param�tre/variable locale */

typedef unsigned char BOOL ;       /* b   bool�en */
typedef BOOL aBOOL ;               /* b      param�tre/variable locale */

#endif /* __TYPES */

#ifndef __TYPES_EX                 /* types �tendus */
#define __TYPES_EX

typedef DWORD DATETIME ;           /* dt  32 bits non sign� pour stockage date et
                                          heure, nombre secondes depuis 01.01.1970 */

typedef BYTE RESULT ;              /* r    8 bits non sign� pour retour statut
                                          ex�cution, OK, ERR, xx_R_yyy */
typedef BYTE aRESULT ;             /* r   param�tre/variable locale */

#endif /* __TYPES_EX */



/*-------------------------------------------------------------------------*/
/* Minimum/maximum types de base                                           */
/*-------------------------------------------------------------------------*/

#define DWORD_MAX   ((DWORD)0xffffffffuL)           /* 32 bits */
#define SDWORD_MIN  ((SDWORD)0x80000000L)
#define SDWORD_MAX  ((SDWORD)0x7fffffffL)

#define WORD_MAX    ((WORD)0xffffu)                 /* 16 bits */
#define SWORD_MIN   ((SWORD)0x8000)
#define SWORD_MAX   ((SWORD)0x7fff)

#define BYTE_MAX    ((BYTE)0xffu)                   /*  8 bits */
#define SBYTE_MIN   ((SBYTE)0x80)
#define SBYTE_MAX   ((SBYTE)0x7f)



/*-------------------------------------------------------------------------*/
/* Constantes et macros de base                                            */
/*-------------------------------------------------------------------------*/

#ifndef FALSE
#define FALSE  0                 /* bool�en faux (BOOL) */
#endif

#ifndef TRUE
#define TRUE   1                 /* bool�en vrai (BOOL) */
#endif

#ifndef OK
#define OK     0                 /* statut correct (RESULT) */
#endif

#ifndef ERR
#define ERR    1                 /* statut erreur (RESULT) */
#endif

#ifndef NULL
#define NULL   0                 /* pointeur nul */
#endif

#define C const                  /* objet constant */

                                 /* nombre �l�ments d'un tableau */
#define ARRAY_SIZE( Array )  ( sizeof(Array) / sizeof((Array)[0]) )

                                 /* pointeur apr�s dernier �l�ment tableau */
#define ARRAY_END( Array )   ( &(Array)[ARRAY_SIZE(Array)] )

                                 /* taille d'un champ d'une structure */
#define FIELD_SIZE( Struct, Field ) ( sizeof( ((Struct *)0)->Field ) )

                                 /* offset d'un champ dans une structure */
#define OFFSETOF( Struct, Field )   ( (DWORD)( (&((Struct *)0)->Field ) ) )

                                 /* contrainte alignement type ou variable */
#define ALIGNOF( Type )      ( __ALIGNOF__(Type) )

                                 /* alignement pointeur <p> pour type <Type> */
#define ALIGN( p, Type ) \
   (void *)( ( (DWORD)(p) + ( ALIGNOF(Type) - 1 ) ) & ~( ALIGNOF(Type) - 1 ) )

                                 /* test si pointeur <p> align� sur type <Type> */
#define ISALIGN( p, Type ) \
   ( ( (DWORD)(p) & ( ALIGNOF(Type) - 1 ) ) == 0 )

                                 /* alignement pointeur <p> pour taille <Size> */
#define ALIGNSIZE( p, Size ) \
   (void *)( ( (DWORD)(p) + ( (Size) - 1 ) ) & ~( (Size) - 1 ) )

                                 /* test si pointeur <p> align� sur taille <Size> */
#define ISALIGNSIZE( p, Size ) \
   ( ( (DWORD)(p) & ( (Size) - 1 ) ) == 0 )

                                 /* 8 bits poids faible de 16 bits */
#define LOBYTE( wValue )  ( (BYTE)( (WORD)(wValue) & 0xff ) )
                                 /* 8 bits poids fort de 16 bits */
#define HIBYTE( wValue )  ( (BYTE)( (WORD)(wValue) >> 8 ) )
                                 /* formation 16 bits avec 2 x 8 bits */
#define MAKEWORD( byLow, byHigh ) \
   ( ( (WORD)( (BYTE)(byHigh) ) << 8 ) | (WORD)( (BYTE)(byLow) ) )

                                 /* 16 bits poids faible de 32 bits */
#define LOWORD( dwValue )  ( (WORD)( (DWORD)(dwValue) & 0xffff ) )
                                 /* 16 bits poids fort de 32 bits */
#define HIWORD( dwValue )  ( (WORD)( (DWORD)(dwValue) >> 16 ) )
                                 /* formation 32 bits avec 2 x 16 bits */
#define MAKEDWORD( wLow, wHigh ) \
   ( ( (DWORD)( (WORD)(wHigh) ) << 16 ) | (DWORD)( (WORD)(wLow) ) )
                                 /* formation 32 bits avec 4 x 8 bits */
#define MAKEDWORD_B( byLow, byMidLow, byMidHigh, byHigh ) \
   ( ( (DWORD)( (BYTE)(byHigh) ) << 24 ) | ( (DWORD)( (BYTE)(byMidHigh) ) << 16 ) | \
     ( (DWORD)( (BYTE)(byMidLow) ) << 8 ) | (DWORD)( (BYTE)(byLow) ) )

                                 /* 32 bits poids faible de 64 bits */
#define LODWORD( qwValue )  ( (DWORD)( (QWORD)(qwValue) & 0xffffffff ) )
                                 /* 32 bits poids fort de 64 bits */
#define HIDWORD( qwValue )  ( (DWORD)( (QWORD)(qwValue) >> 32 ) )
                                 /* formation 64 bits avec 2 x 32 bits */
#define MAKEQWORD( dwLow, dwHigh ) \
   ( ( (QWORD)( (DWORD)(dwHigh) ) << 32 ) | (QWORD)( (DWORD)(dwLow) ) )

                                 /* suppression warning param�tre non utilis� */
#define REFPARM( Parm ) \
   if (Parm)
#define REFPARM2( Parm1, Parm2 ) \
   if (Parm1) ; if (Parm2)
#define REFPARM3( Parm1, Parm2, Parm3 ) \
   if (Parm1) ; if (Parm2) ; if (Parm3)

#ifdef __ICCARM__
                       /* fonction avec instructions ARM localis�e en Flash */
#define ARMFUNC    __arm __interwork
                       /* fonction avec instructions ARM localis�e en Ram */
#define ARMRAMFUNC __arm __interwork __ramfunc
                       /* fonction avec instructions Thumb localis�e en Flash */
#define THUMBFUNC  __thumb __interwork
                       /* variable non initialis�e avant appel main() */
#define NOINIT     __no_init

#else
#define ARMFUNC
#define ARMRAMFUNC
#define THUMBFUNC
#define NOINIT
#endif



/*-------------------------------------------------------------------------*/
/* Types g�n�raux                                                          */
/*-------------------------------------------------------------------------*/

typedef struct          /* date et heure */
{
   BYTE byDay ;         /* jour du mois (1-31) */
   BYTE byMonth ;       /* mois de l'ann�e (1-12) */
   WORD wYear ;         /* ann�e (1990-2050) */
   BYTE byHour ;        /* heure du jour (0-23) */
   BYTE byMin ;         /* minute dans l'heure (0-59) */
   BYTE bySec ;         /* seconde dans la minute (0-59) */
} s_DateTime ;



#endif /* __DEFINE_H */
