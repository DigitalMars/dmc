////////////////////////////////////////////////////////////////////////////////
//
// Created:
// Updated: 7th June 2004


// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>
#include <wchar.h>

#include <stlsoft.h>

#include <winstl.h>


#include <winstl_windows_directory.h>

winstl_ns_using(basic_windows_directory)
winstl_ns_using(windows_directory_w)
winstl_ns_using(windows_directory_a)

int main(int /* argc */, char ** /*argv*/)
{
	{
		windows_directory_a	win_dir;

		 printf( "Windows directory (ANSI):    %s\n", static_cast<char const*>(win_dir));
	}

	{
		windows_directory_w	win_dir;

		wprintf(L"Windows directory (Unicode): %s\n", static_cast<wchar_t const*>(win_dir));
	}

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
