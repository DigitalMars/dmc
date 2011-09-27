
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#include <winstl.h>

#include <winstl_module_filename.h>

int main(int /* argc */, char ** /*argv*/)
{
	winstl_ns_using(module_filename_a)
	winstl_ns_using(module_filename_w)

	module_filename_a	mf_a;

	fprintf(stdout, "Module filename: %s\n", static_cast<char const*>(mf_a));

	module_filename_w	mf_w;

	fwprintf(stdout, L"Module filename: %s\n", static_cast<wchar_t const*>(mf_w));

    return 0;
}
