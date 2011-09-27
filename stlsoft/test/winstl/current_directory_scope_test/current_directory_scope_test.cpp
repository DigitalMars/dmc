
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft.h>
#include <winstl.h>

#include <winstl_current_directory.h>

winstl_ns_using(current_directory_a)

#include <winstl_system_directory.h>

winstl_ns_using(system_directory_a)

#include <winstl_windows_directory.h>

winstl_ns_using(windows_directory_a)

#include <winstl_current_directory_scope.h>

winstl_ns_using(current_directory_scope_a)


int main(int /* argc */, char ** /*argv*/)
{
	{	current_directory_a	curr_dir;
		printf(" Current directory: %s\n", static_cast<char const *>(curr_dir)); }

	printf("Moving up one\n");
	{
		current_directory_scope_a	scope("..");

		{	current_directory_a	curr_dir;
			printf(" Current directory: %s\n", static_cast<char const *>(curr_dir)); }
	}
	{	current_directory_a	curr_dir;
		printf(" Moved back\n Current directory: %s\n", static_cast<char const *>(curr_dir)); }

	printf("Moving to system directory\n");
	{
		system_directory_a			sys_dir;
		current_directory_scope_a	scope(sys_dir);

		{	current_directory_a	curr_dir;
			printf(" Current directory: %s\n", static_cast<char const *>(curr_dir)); }
	}
	{	current_directory_a	curr_dir;
		printf(" Moved back\n Current directory: %s\n", static_cast<char const *>(curr_dir)); }

	printf("Moving to windows directory\n");
	{
		windows_directory_a			win_dir;
		current_directory_scope_a	scope(win_dir);

		{	current_directory_a	curr_dir;
			printf(" Current directory: %s\n", static_cast<char const *>(curr_dir)); }
	}
	{	current_directory_a	curr_dir;
		printf(" Moved back\n Current directory: %s\n", static_cast<char const *>(curr_dir)); }

    return 0;
}
