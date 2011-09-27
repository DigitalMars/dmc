@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by DAOTABLE.HPJ. >"hlp\DAOTable.hm"
echo. >>"hlp\DAOTable.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\DAOTable.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\DAOTable.hm"
echo. >>"hlp\DAOTable.hm"
echo // Prompts (IDP_*) >>"hlp\DAOTable.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\DAOTable.hm"
echo. >>"hlp\DAOTable.hm"
echo // Resources (IDR_*) >>"hlp\DAOTable.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\DAOTable.hm"
echo. >>"hlp\DAOTable.hm"
echo // Dialogs (IDD_*) >>"hlp\DAOTable.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\DAOTable.hm"
echo. >>"hlp\DAOTable.hm"
echo // Frame Controls (IDW_*) >>"hlp\DAOTable.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\DAOTable.hm"
REM -- Make help for Project DAOTABLE


echo Building Win32 Help files
start /wait hcrtf -x "hlp\DAOTable.hpj"
echo.
if exist Debug\nul copy "hlp\DAOTable.hlp" Debug
if exist Debug\nul copy "hlp\DAOTable.cnt" Debug
if exist Release\nul copy "hlp\DAOTable.hlp" Release
if exist Release\nul copy "hlp\DAOTable.cnt" Release
echo.


