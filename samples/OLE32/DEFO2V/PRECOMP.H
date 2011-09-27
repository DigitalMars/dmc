#ifndef _PRECOMP_H_
#define _PRECOMP_H_

#define STRICT

// Disable "unrefereced formal parameter"
#pragma warning( disable : 4100 )

// Disable "Note: Creating new precompiled header"
#pragma warning( disable : 4699 )

// Disable "4706: assignment within conditional expression"
#pragma warning( disable : 4706 )

// Disable "4201: nonstandard...nameless struct/union"
#pragma warning( disable : 4201 )

// DIsable "C4514: unreferenced inline function has been removed
#pragma warning( disable : 4514 )

#ifdef WIN32
#define EXPORT
#else
#define EXPORT __export
#endif

#ifdef _UNICODE
#define UNICODE         // UNICODE is used by Windows headers
#endif

#ifdef WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef INC_OLE2
    #define INC_OLE2
    #endif
#else
    #include <memory.h>
#endif



#include <windows.h>
#include <windowsx.h>

#ifdef WIN32
    #include <tchar.h>
#endif

#ifdef _UNICODE
#else
#ifndef WIN32
 #define LPTSTR      LPSTR
 #define LPCTSTR     LPCSTR
 #define _T(a_)      a_
 #define _T(a_)      a_
#endif
#endif

#ifndef WIN32
#include <ole2.h>
#include <olenls.h>
#include <ver.h>
#include <compobj.h>
#include <variant.h>
#include <dispatch.h>
#else
#include <oleauto.h>
#endif

#endif // _PRECOMP_H_
