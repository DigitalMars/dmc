#ifndef _TOFILE_H_
#define _TOFILE_H_

#if defined(WIN32) && !defined(_UNICODE)
    #define DoTypeInfoToFile    DoTypeInfoToFileA
    #define DoTypeLibToFile     DoTypeLibToFileA
#endif

BOOL DoTypeInfoToFile( HWND hwnd, LPTYPEINFO lpTypeInfo )  ;
BOOL DoTypeLibToFile( HWND hwnd, LPTYPELIB lpTypeLib ) ;

#endif // _TOFILE_H_
