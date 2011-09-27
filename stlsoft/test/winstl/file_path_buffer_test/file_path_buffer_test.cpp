
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <winstl.h>

#include <winstl_file_path_buffer.h>

#include <stdio.h>
#include <wchar.h>

int main(int /* argc */, char ** /*argv*/)
{
	winstl_ns_qual(file_path_buffer_a)	path1;
	winstl_ns_qual(file_path_buffer_w)	path2;
	winstl_ns_qual(file_path_buffer_a)	path3;

	::GetWindowsDirectoryA(path1, path1.size());
	::GetSystemDirectoryW(path2, path2.size());
	::GetCurrentDirectoryA(path3.size(), path3);

	 printf( "Windows directory: %s\n", (char*)path1);
	wprintf(L"System directory:  %s\n", (wchar_t*)path2);
	 printf( "Current directory: %s\n", (char*)path3);

    return 0;
}
