/***
*clsid.h
*
*  Copyright (C) 1992, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
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
DEFINE_GUID(GUID_NULL, 0L, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

DEFINE_GUID(IID_IDispatch, 0x00020400L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(IID_IEnumVARIANT, 0x00020404L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(IID_IUnknown, 0x00000000L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(IID_IClassFactory, 0x00000001L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
# endif
#endif
