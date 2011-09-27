@echo off

:start2
rem ***************************************************************************
rem If there are no arguments, restart, building all libraries.
rem ***************************************************************************
if not "%1"=="" goto start
buildlib SWL SNN SDL SDX SWS SDS SWC SWM SDC SDM SND IOSOLD

rem ***************************************************************************
rem Set buildlib_path to how to get to the SRC directory.
rem ***************************************************************************
 
:start
rem ***************************************************************************
rem Top of loop. Shift through each argument.
rem ***************************************************************************

:loop
shift

rem ***************************************************************************
rem Make sure the argument is a valid library name. Echo a message to stdout
rem if it isn't, otherwise jump to set parameters for that
rem library.
rem ***************************************************************************

if "%0" == "" goto done
if "%0" == "SWL" goto %0
if "%0" == "swl" goto %0
if "%0" == "SWC" goto %0
if "%0" == "swc" goto %0
if "%0" == "SWM" goto %0
if "%0" == "swm" goto %0
if "%0" == "SWS" goto %0
if "%0" == "sws" goto %0
if "%0" == "SNN" goto %0
if "%0" == "snn" goto %0
if "%0" == "SDL" goto %0
if "%0" == "sdl" goto %0
if "%0" == "SDC" goto %0
if "%0" == "sdc" goto %0
if "%0" == "SDM" goto %0
if "%0" == "sdm" goto %0
if "%0" == "SDS" goto %0
if "%0" == "sds" goto %0
if "%0" == "SDX" goto %0
if "%0" == "sdx" goto %0
if "%0" == "SND" goto %0
if "%0" == "snd" goto %0
if "%0" == "SCCRT70" goto %0
if "%0" == "sccrt70" goto %0
if "%0" == "iosold" goto %0
if "%0" == "IOSOLD" goto %0

echo *** Parameter "%0" is not a valid library name
goto loop

:swl
smake /nologo /fswl.mak
goto loop

:swc
smake /nologo /fswc.mak
goto loop

:swm
smake /nologo /fswm.mak
goto loop

:sws
smake /nologo /fsws.mak
goto loop

:snn
smake /nologo /fsnn.mak
goto loop

:snd
smake /nologo /fsnd.mak
goto loop

:sccrt70
smake /nologo /fsccrt70.mak
goto loop

:sdl
smake /nologo /fsdl.mak
goto loop

:sdc
smake /nologo /fsdc.mak

:sdm
smake /nologo /fsdm.mak
goto loop

:sds
smake /nologo /fsds.mak
goto loop

:sdx
smake /nologo /fsdx.mak
goto loop

:iosold
smake /nologo /fmkiosold.mak
goto loop

:done


