// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// Updated: 23rd December 2003
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDAFX_H__AB1F6DB7_9BE6_4D58_A0AB_7195149D4DDE__INCLUDED_)
#define AFX_STDAFX_H__AB1F6DB7_9BE6_4D58_A0AB_7195149D4DDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#if defined(_MSC_VER) && \
    _MSC_VER >= 1200
# include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif /* _MSC_VER >= 1200 */
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


////////////////////////////////////////////////////////////////////////////////

#include <stlsoft.h>
#include <comstl.h>

#include <comstl_enum_sequence.h>

comstl_ns_using(input_cloning_policy)
comstl_ns_using(enum_simple_sequence)

#include <comstl_value_policies.h>

comstl_ns_using(GUID_policy)

#include <comstl_initialisers.h>

comstl_ns_using(com_initialiser)

#include <comstl_string_access.h>

stlsoft_ns_using(c_str_ptr_a)

#include <winstl.h>

#include <winstl_char_conversions.h>

winstl_ns_using(w2a)

#include <winstl_window_enable_scope.h>

winstl_ns_using(window_enable_scope)

#ifndef __ICatRegistrar_INTERFACE_DEFINED__
# include <comcat.h>
#endif /* __ICatRegistrar_INTERFACE_DEFINED__ */


////////////////////////////////////////////////////////////////////////////////


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AB1F6DB7_9BE6_4D58_A0AB_7195149D4DDE__INCLUDED_)