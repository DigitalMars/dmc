# This is a part of the Microsoft Foundation Classes C++ library.
# Copyright (C) 1992-1995 Microsoft Corporation
# All rights reserved.
#
# This source code is only intended as a supplement to the
# Microsoft Foundation Classes Reference and related
# electronic documentation provided with the library.
# See these sources for detailed information regarding the
# Microsoft Foundation Classes product.

# MFC42[LANG].DLL is a DLL which contains language specific resources
# There should be a l.[LANG] and a ..\include\l.[LANG] directories
#   before attempting this build.
# These directories should contain the language specific MFC .rc
#   files used to build this DLL.
# The resulting DLL contains no code, just the MFC standard resources.
#
# The following macros are used to control the build process:
#   LANG=<3 character language identifier>
#       This is used to uniquely identify the DLL.  It is the standard
#       3 character language abbreviation retrieved via GetLocaleInfo.
#
#   LANGID=<4 character string indicating language ID>
#       This is used to construct a correct version resource.
#       The LANGID is specified in hex.
#
#   CP=<decimal codepage>
#   CPHEX=<hex codepage>
#       This codepage matches the last 4 digits of the LANGCODE,
#       except that they are in decimal where as the LANGCODE is
#       specified in hex.
#
# Examples:
#   // build for LANG=ENGLISH
#   nmake LANG=ENU LANGID=0409 /f mfcintl.mak
#
#   // build for LANG=FRENCH
#   nmake LANG=FRA LANGID=040C /f mfcintl.mak
#
#   // build for LANG=JAPANESE
#   nmake LANG=JPN LANGID=0411 CP=932 CPHEX=03A4 /f mfcintl.mak
#       (Note: you must have codepage 932 installed)
#

# Name of this makefile for use in recursion
MAKNAME=mfcintl.mak

!ifndef CP
# Default to "Windows, Multilingual" codepage (ANSI)
CP=1252
CPHEX=04E4
!endif

TARG=MFC42$(LANG)
RC_DEFINES=/DLANG=$(LANG)
LFLAGS=/noentry /dll /base:0x5FD00000
LINK32=link

# Default PLATFORM depending on host environment
!ifndef PLATFORM
!ifndef PROCESSOR_ARCHITECTURE
!error PLATFORM must be set to intended target
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "x86"
PLATFORM=INTEL
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "ALPHA"
PLATFORM=ALPHA
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "MIPS"
PLATFORM=MIPS
!endif
!if "$(PROCESSOR_ARCHITECTURE)" == "PPC"
PLATFORM=PPC
!endif
!endif

!if "$(PLATFORM)" == "INTEL"
LFLAGS=$(LFLAGS) /machine:i386
!endif
!if "$(PLATFORM)" == "MIPS"
LFLAGS=$(LFLAGS) /machine:mips
!endif
!if "$(PLATFORM)" == "ALPHA"
LFLAGS=$(LFLAGS) /machine:alpha
!endif
!if "$(PLATFORM)" == "PPC"
LFLAGS=$(LFLAGS) /machine:ppc
!endif

!ifdef RELEASE # Release VERSION info
RC_DEFINES=$(RC_DEFINES) /DRELEASE
LFLAGS=$(LFLAGS) /release
!endif

RC_DEFINES=$(RC_DEFINES) \
	/DLANGCODE=\"$(LANGID)$(CPHEX)\" /DLANGID=0x$(LANGID) /DCODEPAGE=$(CP)
RC_CODEPAGE=/c$(CP) /l$(LANGID)

dll_goal: $(TARG).dll

#############################################################################
# Build target

$(TARG).res: mfcintl.rc build_.h
	rc /r $(RC_CODEPAGE) $(RC_DEFINES) /fo $(TARG).res mfcintl.rc

$(TARG).dll: $(TARG).res
	$(LINK32) $(LFLAGS) /out:$(TARG).DLL $(TARG).res
	-del $(TARG).res

#############################################################################
# Predefined locales

# Chinese (PRC)
chs:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=CHS LANGID=0804 CP=936 CPHEX=03A8

# Chinese (Traditional)
cht:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=CHT LANGID=0404 CP=950 CPHEX=03B6

# English (American)
enu:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=ENU LANGID=0409

# French (Standard)
fra:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=FRA LANGID=040C

# German (Standard)
deu:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=DEU LANGID=0407

# Italian (standard)
ita:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=ITA LANGID=0410

# Korean
kor:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=KOR LANGID=0412 CP=949 CPHEX=03B5

# Japanese
jpn:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=JPN LANGID=0411 CP=932 CPHEX=03A4

# Spanish (Castilian)
esp:
	$(MAKE) -$(MAKEFLAGS) -f $(MAKNAME) LANG=ESP LANGID=040A

#############################################################################
