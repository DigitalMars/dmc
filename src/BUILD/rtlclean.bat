@echo off

if exist *.bro del *.bro
if exist *.dpd del *.dpd
if exist *.lib del *.lib
if exist ..\..\redist32\sccrt70.lib del ..\..\redist32\sccrt70.lib

call cleandir snn
call cleandir snd
call cleandir sccrt70
call cleandir snnuni
call cleandir snduni
call cleandir sdl
call cleandir sds
call cleandir sdm
call cleandir sdc
call cleandir sdx
call cleandir swl
call cleandir sws
call cleandir swm
call cleandir swc
call cleandir dlluni
