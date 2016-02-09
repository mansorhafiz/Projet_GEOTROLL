@echo off

   rem --- Construction complète application

   rem --- Usage : build [-]
   rem ---                construction pour carte PIC 4550

if [%1]==[##] goto Process

set PROCESSOR=18f4550

%0 ## %PROCESSOR% c_Version Main Config Communic c_Micro FrameCom InOut Crc Interrupt Timer Tempo Uart1 UartUsb RunLed
goto :EOF


:Process
shift
set PROCESSOR=%1
set LKRFILE=%PROCESSOR%.lkr
echo proc %PROCESSOR%
set PROJECT=Pic

   rem --- Effacement anciens fichiers sorties
del /s *.hex

   rem --- Effacement anciens fichiers générés par compilateur ou linker
del /s *.o *.map *.lst *.err *.cod >nul

   rem --- Effacement anciens fichiers générés par débugger MPLAB
del /s *.mcs *.cof

   rem --- Compilation ---
   rem options compilation :
   rem   + -p pour préciser microcontrôleur cible
   rem   + -i pour ajouter répertoire include
   rem   + -fo pour création fichiers objets .o sous OBJ\
   rem   + -O+ pour autoriser toutes optimisations
   rem   + -Opa- pour inhiber optimisation d'abstraction procédurale

shift
set _OBJS=
:Next
mcc18 -p%PROCESSOR% -i%INCLUDE% -fo=OBJ\%1.o -O+ -Opa- %1.c

if errorlevel 1 goto :EOF
set _OBJS=%_OBJS% OBJ\%1.o
shift
if not [%1]==[] goto Next

   rem --- Link ---
   rem options link :
   rem   + /l pour ajouter librairie
   rem   + /o pour préciser format fichier sortie .hex
   rem   + /w pour ne pas générer de fichier .cod et .lst
   rem   + /d pour ne pas générer fichier de listing


mplink %_OBJS% %LKRFILE% /l%LIB% /o %PROJECT%.hex /w /d