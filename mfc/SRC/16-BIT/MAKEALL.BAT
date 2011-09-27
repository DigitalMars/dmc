@echo off
smake /nologo DLL=0 DEBUG=0 CODEVIEW=0 TARGET=R
if errorlevel 1 goto exit
smake /nologo DLL=0 DEBUG=1 CODEVIEW=1 TARGET=R
if errorlevel 1 goto exit

smake /nologo DLL=0 DEBUG=0 CODEVIEW=0 
if errorlevel 1 goto exit
smake /nologo DLL=0 DEBUG=1 CODEVIEW=1  
if errorlevel 1 goto exit

smake /nologo DLL=1 DEBUG=0 CODEVIEW=0 
if errorlevel 1 goto exit
smake /nologo DLL=1 DEBUG=1 CODEVIEW=1  
if errorlevel 1 goto exit

smake /nologo /f mfcdll.mak DEBUG=0 CODEVIEW=0
if errorlevel 1 goto exit
smake /nologo /f mfcdll.mak DEBUG=1 CODEVIEW=1 
if errorlevel 1 goto exit

smake /nologo /f mfcdb.mak DEBUG=0 CODEVIEW=0 
if errorlevel 1 goto exit
smake /nologo /f mfcdb.mak DEBUG=1 CODEVIEW=1 
if errorlevel 1 goto exit

smake /nologo /f mfcnet.mak DEBUG=0 CODEVIEW=0
if errorlevel 1 goto exit
smake /nologo /f mfcnet.mak DEBUG=1 CODEVIEW=1
if errorlevel 1 goto exit

smake /nologo /f mfcole.mak DEBUG=0 CODEVIEW=0
if errorlevel 1 goto exit
smake /nologo /f mfcole.mak DEBUG=1 CODEVIEW=1
if errorlevel 1 goto exit

:great
@echo off
echo ! 
echo ! 
echo ! 
echo !  *** SUCCESS ***
echo !  MFC 25 Static Libraries and DLLS Built Successfully!
echo ! 
echo ! 
echo ! 
goto end
:exit
@echo off
echo !  ****************
echo !  BUILD FAILED !!!
echo !  ****************
:end
rem 
