@echo off
%1 < cpl.cpp > compilera.asm 
Bin\fasm compilera.asm
compilera.exe < cpl.cpp > compilerb.asm
fc compilera.asm compilerb.asm
rem compilera.exe < cpl.cpp
pause
