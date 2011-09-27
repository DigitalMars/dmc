# Microsoft Developer Studio Project File - Name="whereis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=whereis - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "whereis.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "whereis.mak" CFG="whereis - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "whereis - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "whereis - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "whereis - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_STLSOFT_COMPILE_VERBOSE" /YX /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "whereis - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_STLSOFT_COMPILE_VERBOSE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "whereis - Win32 Release"
# Name "whereis - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\whereis.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
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

SOURCE=..\..\..\stlsoft_cstring_maker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_limit_traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_malloc_allocator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_meta.h
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

SOURCE=..\..\..\stlsoft_size_traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_string_access.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_string_tokeniser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stlsoft_type_traits.h
# End Source File
# End Group
# Begin Group "WinSTL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\winstl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_current_directory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_environment_variable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_filesystem_traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_findfile_sequence.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_functionals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_functions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_processheap_allocator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_string_access.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_system_version.h
# End Source File
# Begin Source File

SOURCE=..\..\..\winstl_windows_ident.h
# End Source File
# End Group
# End Group
# End Group
# End Target
# End Project
