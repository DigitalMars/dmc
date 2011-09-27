
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <winstl.h>

#include <winstl_system_version.h>

#include <stdio.h>


int main(int /* argc */, char ** /*argv*/)
{
    winstl_ns_using(system_version)

	printf("system_version_test\n\n");
	printf("Operating system type:    %s\n", system_version::winnt() ? "NT" : system_version::win9x() ? "9x" : "win32s");
	printf("Operating system version: %u.%u\n", system_version::major(), system_version::minor());
	printf("Operating system build#:  %u\n", system_version::build_number());

    return 0;
}
