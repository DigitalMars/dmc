/* lcapi32.h - internal definitions for Locale A, W Win32(s) API routines. */

#ifndef __LCAPI32_H
#define __LCAPI32_H

# if __cplusplus
extern "C" {
# endif

typedef unsigned int size_t;

#define CPYERR		(size_t) -1
#define APIERR		0
#define CMPLT		1
#define CMPEQ		2
#define CMPGT		3

#define DNOTHING	0
#define DETECT_A	1
#define DETECT_W	2

# ifdef	_UNICODE	/* Current UNICODE Support */
#define __tCreateFile(a,b,c,d,e,f,g)	__wCreateFile(0,a,b,c,d,e,f,g)
#define __tDeleteFile(a)		__wDeleteFile (0,a)
#define __tMoveFile(a,b)		__wMoveFile (0,a,b)
#define __tGetFileAttributes(a)		__wGetFileAttributes(0,a)
#define __tSetFileAttributes(a,b)	__wSetFileAttributes(0,a,b)
#define __tCreateDirectory(a,b)		__wCreateDirectory(0,a,b)
#define __tRemoveDirectory(a)		__wRemoveDirectory(0,a)
#define __tFindFirstFile(a,b)		__wFindFirstFile(0,a,b)
#define __tFindNextFile(a,b)		__wFindNextFile(0,a,b)
#define __tGetCurrentDirectory(a,b)	__wGetCurrentDirectory(0,a,b)
#define __tSetCurrentDirectory(a)	__wSetCurrentDirectory(0,a)
#define __tCreateProcess(a,b,c,d,e,f,g,h,i,j)	__wCreateProcess(0,a,b,c,d,e,f,g,h,i,j)
#define __tGetCommandLine()		__wGetCommandLine(0)
#define __tGetModuleFileName(a,b,c)	__wGetModuleFileName(0,a,b,c)
# else			/* SingleByte and MultiByte Character Support */
#define __tCreateFile(a,b,c,d,e,f,g)	CreateFileA(a,b,c,d,e,f,g)
#define __tDeleteFile(a)		DeleteFileA(a)
#define __tMoveFile(a,b)		MoveFileA(a,b)
#define __tGetFileAttributes(a)		GetFileAttributesA(a)
#define __tSetFileAttributes(a,b)	SetFileAttributesA(a,b)
#define __tCreateDirectory(a,b)		CreateDirectoryA(a,b)
#define __tRemoveDirectory(a)		RemoveDirectoryA(a)
#define __tFindFirstFile(a,b)		FindFirstFileA(a,b)
#define __tFindNextFile(a,b)		FindNextFileA(a,b)
#define __tGetCurrentDirectory(a,b)	GetCurrentDirectoryA(a,b)
#define __tSetCurrentDirectory(a)	SetCurrentDirectoryA(a)
#define __tCreateProcess(a,b,c,d,e,f,g,h,i,j)	CreateProcessA(a,b,c,d,e,f,g,h,i,j)
#define __tGetCommandLine		GetCommandLineA
#define __tGetModuleFileName		GetModuleFileNameA
# endif

#define GETCMAIN(x)	(char *) malloc ((x) * sizeof (char))
#define GETWMAIN(x)	(wchar_t *) malloc ((x) * sizeof (wchar_t))
#define WtoMcs(a,b,c,d)	WideCharToMultiByte(cPage,WC_COMPOSITECHECK|WC_SEPCHARS,a,b,c,d,NULL,NULL)
#define MtoWlc(a,b,c,d)	MultiByteToWideChar(cPage,__locale_chars,a,b,c,d)
#define MtoWcm(a,b,c,d)	MultiByteToWideChar(cPage,MB_PRECOMPOSED,a,b,c,d)
#define WtoMcsSZ(a,b)	WtoMcs(a,b,NULL,0)
#define MtoWlcSZ(a,b)	MtoWlc(a,b,NULL,0)
#define MtoWcmSZ(a,b)	MtoWcm(a,b,NULL,0)

HANDLE __cdecl __wCreateFile(UINT, LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
BOOL   __cdecl __wDeleteFile(UINT, LPCWSTR);
BOOL   __cdecl __wMoveFile(UINT, LPCWSTR, LPCWSTR);
DWORD  __cdecl __wGetFileAttributes(UINT, LPCWSTR);
BOOL   __cdecl __wSetFileAttributes(UINT, LPCWSTR, DWORD);
BOOL   __cdecl __wCreateDirectory(UINT, LPCWSTR, LPSECURITY_ATTRIBUTES);
BOOL   __cdecl __wRemoveDirectory(UINT, LPCWSTR);
HANDLE __cdecl __wFindFirstFile(UINT, LPCWSTR, LPWIN32_FIND_DATAW);
BOOL   __cdecl __wFindNextFile(UINT, HANDLE, LPWIN32_FIND_DATAW);
DWORD  __cdecl __wGetCurrentDirectory(UINT, DWORD, LPWSTR);
BOOL   __cdecl __wSetCurrentDirectory(UINT, LPCWSTR);
BOOL   __cdecl __wCreateProcess(UINT, LPCWSTR, LPCWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCWSTR, LPSTARTUPINFO, LPPROCESS_INFORMATION);
LPWSTR __cdecl __wGetCommandLine(UINT);
DWORD  __cdecl __wGetModuleFileName(UINT, HMODULE, LPWSTR, DWORD);

#define __aCompareString(a,b,c,d,e,f,g)	CompareStringA(b,c,d,e,f,g)
#define __aGetLocaleInfo(a,b,c,d,e)	GetLocaleInfoA(b,c,d,e)
#define __aGetStringType(a,b,c,d,e,f)	GetStringTypeA(b,c,d,e,f)
#define __aLCMapString(a,b,c,d,e,f,g)	LCMapStringA(b,c,d,e,f,g)

int    __cdecl __wCompareString(UINT, LCID, DWORD, LPCWSTR, int, LPCWSTR, int);
int    __cdecl __wGetLocaleInfo(UINT, LCID, LCTYPE, LPWSTR, int);
BOOL   __cdecl __wGetStringType(UINT, LCID, DWORD, LPCWSTR, int, LPWORD);
int    __cdecl __wLCMapString(UINT, LCID, DWORD, LPCWSTR, int, LPWSTR, int);

# if __cplusplus
}
# endif

#endif

