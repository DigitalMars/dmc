# Microsoft Developer Studio Project File - Name="lnunique" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=lnunique - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lnunique.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lnunique.mak" CFG="lnunique - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lnunique - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "lnunique - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lnunique - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\..\..\..\..\dev\include\stlsoft\\" /D "WIN32" /D "NDEBUG" /D "STRICT" /D "_CONSOLE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /version:1.0 /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "lnunique - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\..\..\dev\include\stlsoft\\" /D "WIN32" /D "_DEBUG" /D "STRICT" /D "_CONSOLE" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /version:1.0 /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "lnunique - Win32 Release"
# Name "lnunique - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\lnunique.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Standard Header Files"

# PROP Default_Filter ""
# End Group
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

SOURCE=..\..\..\stlsoft_char_traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_constraints.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_meta.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_new_allocator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_sap_cast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_sign_traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_simple_algorithms.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_simple_string.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_size_traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_type_traits.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Version Files"

# PROP Default_Filter "rcv"
# End Group
# End Target
# End Project
