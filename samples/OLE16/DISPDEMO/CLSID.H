/***
*clsid.h
*
*  Copyright (C) 1992, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*  This file defines the CLSIDs referenced by the IDispatch demo app.
*
*Implementation Notes:
*
*****************************************************************************/

DEFINE_GUID(CLSID_CPoly,  0x00020462, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(CLSID_CPoly2, 0x00020464, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);


#ifdef _MAC
DEFINE_GUID(CLSID_TMS,  0x00020206, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(CLSID_TMSi, 0x00020207, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(CLSID_SR2,  0x00010001, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
#endif

#ifdef INITGUID
# ifdef _MAC
DEFINE_GUID(GUID_NULL, 0L, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DEFINE_GUID(IID_IDispatch, 0x00020400L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
DEFINE_GUID(IID_IUnknown, 0x00000000L, 0, 0, 0xC0, 0, 0, 0, 0, 0, 0, 0x46);
# endif
#endif
