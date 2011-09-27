@echo off
REM Batch file to install and register OLE Messaging on Windows 95.
REM Usage: olemess mstools-dir windows-dir
REM        olemess e:\mstools c:\windows
REM The mstools-dir parameter must point to the SDK CD, not to
REM your local MSTOOLS directory. Necessary files are available
REM only on the CD.

if "%1"=="" goto BadParameter
if "%2"=="" goto BadParameter

if NOT EXIST %1\mapi\win95\nodebug\usa\mdisp32.exe goto BadSource
if NOT EXIST %2\system\mapi32.dll goto BadDest

copy %1\mapi\win95\nodebug\usa\mdisp32.exe %2\system >nul
copy %1\mapi\win95\nodebug\usa\mdisp32.tlb %2\system >nul
copy %1\mapi\win95\nodebug\usa\mdisp.tlb   %2\system   >nul

regedit /s %1\mapi\cfg\mdisp32.reg
echo.
echo OLE Messaging installed
echo.
goto Done

:BadDest
echo The WINDOWS directory parameter (%2) does not contain a MAPI installation.
echo Please install MAPI before installing OLE Messaging
echo.
goto Done

:BadSource
echo The MSTOOLS directory parameter (%1) does not contain a valid Win 32 SDK
echo.
goto Done

:BadParameter
echo Usage: olemess mstools-directory windows-directory
echo  e.g.: olemess e:\mstools c:\windows
echo The mstools-dir parameter must point to the SDK CD, not to
echo your local MSTOOLS directory. Necessary files are available
echo only on the CD.
echo.

:Done



