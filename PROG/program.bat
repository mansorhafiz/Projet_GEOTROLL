@echo off

   rem --- Usage : program [SourceFile]
   rem ---    SourceFile : fichier � programmer 

set PROCESSEUR=PIC18F4550
set SOURCEFILE= %1

   rem --- programmation PIC par sonde PicKit 3
   rem options :
   rem   + /P : s�lection du micro-contr�leur cible
   rem   + /F : s�lection du fichier source � programmer
   rem   + /J : affichage pourcentage progression
   rem   + /E : effacement flash avant programmation
   rem   + /M : programmation compl�te micro-contr�leur
   rem   + /R : lancement appli apr�s programmation avec MCU aliment� en externe

pk2cmd.exe /P%PROCESSEUR% /F%SOURCEFILE% /J /E /M /R