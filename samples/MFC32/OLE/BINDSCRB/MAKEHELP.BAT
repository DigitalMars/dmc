@echo off
REM -- First make map file from App Studio generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by scribble.HPJ. >hlp\scribble.hm
echo. >>hlp\scribble.hm
echo // Commands (ID_* and IDM_*) >>hlp\scribble.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\scribble.hm
echo. >>hlp\scribble.hm
echo // Prompts (IDP_*) >>hlp\scribble.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\scribble.hm
echo. >>hlp\scribble.hm
echo // Resources (IDR_*) >>hlp\scribble.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\scribble.hm
echo. >>hlp\scribble.hm
echo // Dialogs (IDD_*) >>hlp\scribble.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\scribble.hm
echo. >>hlp\scribble.hm
echo // Frame Controls (IDW_*) >>hlp\scribble.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\scribble.hm
REM -- Make help for Project scribble
call hc31 scribble.hpj
if exist windebug copy scribble.hlp windebug
if exist winrel copy scribble.hlp winrel
if exist macdebug copy scribble.hlp macdebug
if exist macrel copy scribble.hlp macrel
echo.
