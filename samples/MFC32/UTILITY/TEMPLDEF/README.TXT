========================================================================
	  MICROSOFT FOUNDATION CLASS LIBRARY : TEMPLDEF EXAMPLE PROGRAM
========================================================================

This directory contains a utility that implements a subset of the
C++ 3.0 syntax for templates.  This application is a DOS utility that
requires the MFC library MAFXCR.LIB or the debug variant MAFXCRD.LIB.

The syntax of the templates implemented and the operation of the tool
are described in the technical note, MFC\DOC\TN004.TXT.   Please
refer to that note for a discussion of the templates.

This directory also contains the files required to build the collection
variants used by the MFC Library.

AFXCOLL1.H and AFXCOLL1.INL are the header file stub that prefix
the generated NEWCOLL.H and NEWCOLL.INL files.  AFXCOLL2.H and
AFXCOLL2.INL are similar files appended at the end of the generated
NEWCOLL.H and NEWCOLL.INL files.

ARRAY.CTT is the template for the array collection shape.

LIST.CTT is the template for the list collection shape.

MAP.CTT is the template for the map collection shape.

MAP_S.CTT is the template for the map collections that use
CString class objects as either the KEY or VALUE of a map.

MKCOLL.BAT is a DOS batch file that will construct the various
collection shapes and variants that are used in the MFC
library.  The MFC\SRC subdirectory already contains
expanded versions of the varieties used in the core
MFC library, so the use of this tool is not required to build
the library.

You should consider using these templates as a guide for developing
your own generic abstract data types.

NOTE: This tool is an example program and the syntax it supports
may change in the future.  It is provided as a sample application only.
