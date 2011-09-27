

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft.h>

#include <winstl.h>

#include <winstl_absolute_path.h>

winstl_ns_using(absolute_path_a)

#include <winstl_current_directory.h>

winstl_ns_using(current_directory_a)

#include <winstl_windows_directory.h>

winstl_ns_using(windows_directory_a)


static void dump_path(char const *path)
{
	absolute_path_a	abs_path(path);

	printf("%s => %s\n", path, static_cast<char const *>(abs_path));
}

int main(int /* argc */, char ** /*argv*/)
{
	dump_path(windows_directory_a());
	dump_path("\\");
	dump_path(current_directory_a());
	dump_path(".");

    return 0;
}
