@echo off
if "%1" == "" goto usage
if "%2" == "" goto usage
if "%3" == "" goto usage

if (%1)==(N) goto NODEBUG_SETUP
if (%1)==(n) goto NODEBUG_SETUP
if (%1)==(D) goto DEBUG_SETUP
if (%1)==(d) goto DEBUG_SETUP
:USAGE
echo.
echo Usage:
echo        SWITCH {N or D} MSTOOLSDIR WINDIR
echo.
echo where  N = switch to nodebug version of windows
echo        D = switch to debug version of windows
echo        MSTOOLSDIR = path to debug files e.g. c:\mstools
echo        WINDIR = path to windows directory e.g. c:\windows
echo.
goto DONE
:DEBUG_SETUP
echo Switching to debug system files...
copy %2\DEBUG\*.EXE %3\system
copy %2\DEBUG\*.DLL %3\system
copy %2\DEBUG\*.SYM %3\system
copy %2\DEBUG\*.VXD %3\system\VMM32
goto DONE
:NODEBUG_SETUP
echo Switching to nodebug system files...
copy %2\NODEBUG\*.EXE %3\system
copy %2\NODEBUG\*.DLL %3\system
copy %2\NODEBUG\*.SYM %3\system
copy %2\NODEBUG\*.VXD %3\system\VMM32
:DONE
