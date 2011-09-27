

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <winstl.h>

#include <winstl_system_info.h>

#include <stdio.h>

int main(int /* argc */, char ** /*argv*/)
{
	printf("system_info_test\n\n");
	printf("Number of processors:   %u\n", winstl_ns_qual(system_info)::number_of_processors());
	printf("Page size:              %u bytes\n", winstl_ns_qual(system_info)::page_size());
	printf("Allocation granularity: %u bytes\n", winstl_ns_qual(system_info)::allocation_granularity());

    return 0;
}
