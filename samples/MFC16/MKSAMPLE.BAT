@echo off
REM This is a part of the Microsoft Foundation Classes C++ library.
REM Copyright (C) 1992-1993 Microsoft Corporation
REM All rights reserved.
REM
REM This source code is only intended as a supplement to the
REM Microsoft Foundation Classes Reference and Microsoft
REM QuickHelp documentation provided with the library.
REM See these sources for detailed information regarding the
REM

echo.
echo Microsoft Foundation Classes -- Sample Applications (batch build)
echo.
echo This MS-DOS Batch file will build most of the Windows/MFC sample
echo applications.  Use of the batch file will save time and disk space
echo if you are interested in seeing all of the working samples as you 
echo learn the Microsoft Foundation Classes.
echo If you are modifying a sample or using it as a basis for your 
echo own application, then the use of the Integrated Development and 
echo Debugging Environment is recommended.
echo.
echo.

set __DEBUG=1
if "%1"=="DEBUG" goto doit
if "%1"=="debug" goto doit

set __DEBUG=0

if "%1"=="RETAIL" goto doit
if "%1"=="retail" goto doit

if "%1"=="CLEAN" goto doit_clean
if "%1"=="clean" goto doit_clean

echo Please specify DEBUG or RETAIL or CLEAN on the command line.
echo.
echo     DEBUG will build executables with symbolic information,
echo         diagnostics, and no optimizations (large and slow).
echo     RETAIL will build ship quality executables that
echo         are fully optimized (small and fast).
echo     CLEAN will remove all compiler generated files (object
echo         files, executables, etc.)  Use this to switch between
echo         DEBUG and RETAIL.
echo.

goto end

:doit
shift

echo.
echo NOTE: Since this builds all the sample programs it could take a 
echo little while.  Type Ctrl-C now if you wish to build them later.  
echo You can stop the build at any time by typing Ctrl-C (several times) 
echo and answer 'Y' to terminate.
echo.
pause

:doit_clean

if "%1"=="CLEAN" erase STDAFX??.SYM
if "%1"=="CLEAN" erase STDAFX??.OBJ

@echo on

cd CALCDRIV
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\CATALOG
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\DYNABIND
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\CHKBOOK
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\CTRLBARS
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\CTRLTEST
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\DIBLOOK
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\DLLHUSK
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\DRAWCLI
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\HELLO
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\HELLOAPP
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\HIERSVR
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\MDI
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\MULTIPAD
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\OCLIENT
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\PROPDLG
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\SPEAKN
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\SUPERPAD
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\TRACER
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\TRACKER
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\VBCHART
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\VBCIRCLE
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\VIEWEX
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\MAKEHM
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\TEMPLDEF
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\DLLTRACE
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\AUTOCLIK
cd STEP1
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP2
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP3
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..
cd ..\ENROLL                      
cd STEP1
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP2
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP3
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP4
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..
cd ..\CONTAIN
cd STEP1
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP2
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..
cd ..\SCRIBBLE
cd STEP0
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP1
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP2
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP3
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP4
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP5
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP6
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..\STEP7
smake /nologo "DEBUG=%__DEBUG%" %1
cd ..

@echo off
cd ..
:end
set __DEBUG=
@echo on
