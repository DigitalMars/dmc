
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft.h>

#include <winstl.h>

#include <winstl_current_directory.h>

winstl_ns_using(current_directory_a)

int main(int /* argc */, char ** /*argv*/)
{
	current_directory_a	curr_dir;

	printf("Current directory: %s\n", static_cast<char const*>(curr_dir));

    return 0;
}
