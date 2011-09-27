// findvolume_sequence_test.cpp
//
// Updated: 22nd April 2004

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stlsoft.h>

#include <winstl.h>

#include <stlsoft_simple_algorithms.h>

#include <winstl_findvolume_sequence.h>

winstl_ns_using(findvolume_sequence_a)
winstl_ns_using(findvolume_sequence_w)

#include <stdio.h>
#include <wchar.h>

#include <functional>
#include <algorithm>

#if !defined(_WIN32_WINNT) || \
    (_WIN32_WINNT < 0x0500)

static FARPROC GetKernel32ProcOrTerminate(LPSTR procName)
{
	// We're incrementing KERNEL32's reference count, but that's ok,
	// because it's KERNEL32. Not to be recommended normally.
	FARPROC	fp	=	::GetProcAddress(::LoadLibrary("Kernel32"), procName);

	return (fp != NULL)  ? fp : (::ExitProcess(1), FARPROC(0));
}

HANDLE WINAPI FindFirstVolumeA(LPSTR lpszVolumeName, DWORD cchBufferLength)
{
	return (*reinterpret_cast<HANDLE (WINAPI *)(LPSTR , DWORD )>(GetKernel32ProcOrTerminate("FindFirstVolumeA")))(lpszVolumeName, cchBufferLength);
}

HANDLE WINAPI FindFirstVolumeW(LPWSTR lpszVolumeName, DWORD cchBufferLength)
{
	return (*reinterpret_cast<HANDLE (WINAPI *)(LPWSTR , DWORD )>(GetKernel32ProcOrTerminate("FindFirstVolumeW")))(lpszVolumeName, cchBufferLength);
}

BOOL WINAPI FindNextVolumeA(HANDLE hFindVolume, LPSTR lpszVolumeName, DWORD cchBufferLength)
{
	return (*reinterpret_cast<BOOL (WINAPI *)(HANDLE, LPSTR , DWORD )>(GetKernel32ProcOrTerminate("FindNextVolumeA")))(hFindVolume, lpszVolumeName, cchBufferLength);
}

BOOL WINAPI FindNextVolumeW(HANDLE hFindVolume, LPWSTR lpszVolumeName, DWORD cchBufferLength)
{
	return (*reinterpret_cast<BOOL (WINAPI *)(HANDLE, LPWSTR , DWORD )>(GetKernel32ProcOrTerminate("FindNextVolumeW")))(hFindVolume, lpszVolumeName, cchBufferLength);
}

BOOL WINAPI FindVolumeClose(HANDLE hFindVolume)
{
	return (*reinterpret_cast<BOOL (WINAPI *)(HANDLE)>(GetKernel32ProcOrTerminate("FindVolumeClose")))(hFindVolume);
}

#endif /* !_WIN32_WINNT || (_WIN32_WINNT < 0x0500) */


// Because of the difficulties that Borland and GNU compilers have with
// these definitions, this functional looks extremely complex. When
// writing for a single compiler, or for a set that includes the 'better'
// compilers (Intel, Metrowerks, Comeau, Digital Mars) it would not be
// an issue.

struct print_path
{
public:
	void operator ()(winstl_ns_qual(findvolume_sequence_a)::value_type const &value)
	{
		fprintf(stdout, "%s\n", (const char *)value);
	}
	void operator ()(winstl_ns_qual(findvolume_sequence_w)::value_type const &value)
	{
		fwprintf(stdout, L"%s\n", (const wchar_t *)value);
	}
};

int main(int /* argc */, char ** /*argv*/)
{
	winstl_ns_qual(findvolume_sequence_a)	volumes_a;
    winstl_ns_qual(findvolume_sequence_w)	volumes_w;

	printf("Current volumes (ANSI):\n");
	stlsoft_ns_qual(for_each_preinc)(volumes_a.begin(), volumes_a.end(), print_path());
	printf("Current volumes (Unicode):\n");
    stlsoft_ns_qual(for_each_postinc)(volumes_w.begin(), volumes_w.end(), print_path());

    return 0;
}
