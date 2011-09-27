# Microsoft Developer Studio Project File - Name="ccvui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ccvui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ccvui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ccvui.mak" CFG="ccvui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ccvui - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ccvui - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ccvui - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ole32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ccvui - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ole32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ccvui - Win32 Release"
# Name "ccvui - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\ccvui.cpp
# End Source File
# Begin Source File

SOURCE=..\ccvuiDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "STLSoft Header Files"

# PROP Default_Filter ""
# Begin Group "STLSoft"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\stlsoft.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_allocator_base.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_auto_buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_cccap_msvc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_constraints.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_string_access.h
# End Source File
# End Group
# Begin Group "COMSTL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\comstl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\comstl_enum_sequence.h
# End Source File
# Begin Source File

SOURCE=..\..\..\comstl_initialisers.h
# End Source File
# Begin Source File

SOURCE=..\..\..\comstl_memory_functions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\comstl_olestring_functions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\comstl_refcount_functions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\comstl_string_access.h
# End Source File
# Begin Source File

SOURCE=..\..\..\comstl_value_policies.h
# End Source File
# End Group
# Begin Group "WinSTL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\winstl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\research\winstl_char_conversions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_processheap_allocator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_string_access.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_window_access.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_window_enable_scope.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\ccvui.h
# End Source File
# Begin Source File

SOURCE=..\ccvuiDlg.h
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\ccvui.ico
# End Source File
# Begin Source File

SOURCE=..\res\ccvui.ico
# End Source File
# Begin Source File

SOURCE=..\ccvui.rc
# End Source File
# End Group
# End Target
# End Project
