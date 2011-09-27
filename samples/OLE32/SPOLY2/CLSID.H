/*** 
*clsid.h
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  This file defines the CLSIDs
*
*Implementation Notes:
*
*****************************************************************************/

DEFINE_GUID(CLSID_CPoly2, 0x00020464, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(CLSID_CPoint2, 0x00020465, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);


#ifdef INITGUID
# ifdef _MAC
/* Define this under PPC Metroworks */
#  if !defined(_MW_BUILD) 
DEFINE_GUID(IID_IDispatch, 0x00020400L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(IID_IEnumVARIANT, 0x00020404L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
#  endif
DEFINE_GUID(GUID_NULL, 0L, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DEFINE_GUID(IID_IUnknown, 0x00000000L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(IID_IClassFactory, 0x00000001L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
# endif
#endif

