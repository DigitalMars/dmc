
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>
#include <wchar.h>

#include <stlsoft.h>

#include <winstl.h>

#include <winstl_system_directory.h>

winstl_ns_using(system_directory_a)
winstl_ns_using(system_directory_w)

int main(int /* argc */, char ** /*argv*/)
{
	{
		system_directory_a	sys_dir;

		 printf( "System directory: %s\n", static_cast<char const*>(sys_dir));
	}

	{
		system_directory_a	sys_dir;

		 printf( "System directory: %s\n", static_cast<char const*>(sys_dir));
	}

	{
		system_directory_w	sys_dir;

		wprintf(L"System directory: %s\n", static_cast<wchar_t const*>(sys_dir));
	}

	{
		system_directory_w	sys_dir;

		wprintf(L"System directory: %s\n", static_cast<wchar_t const*>(sys_dir));
	}

	return 0;
}
