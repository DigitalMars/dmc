// findfile_sequence_test.cpp
//
// Updated: 1st May 2004

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stlsoft.h>

#include <winstl.h>

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER == 1100
# pragma warning(disable : 4550) // VC 5 is a bit dim
#endif /* _MSC_VER == 1100 */

//#define _WINSTL_FINDFILE_ITERATOR_IS_FORWARD

#include <stlsoft_simple_algorithms.h>

#include <winstl_findfile_sequence.h>

winstl_ns_using(findfile_sequence_a)
winstl_ns_using(findfile_sequence_w)

#include <winstl_current_directory.h>

winstl_ns_using(current_directory_a)
winstl_ns_using(current_directory_w)

#include <winstl_windows_directory.h>

winstl_ns_using(windows_directory_a)
winstl_ns_using(windows_directory_w)

#include <stdio.h>
#include <wchar.h>

#include <functional>
#include <algorithm>

static void dump_path(char const *path);

// Because of the difficulties that Borland and GNU compilers have with
// these definitions, this functional looks extremely complex. When
// writing for a single compiler, or for a set that includes the 'better'
// compilers (Intel, Metrowerks, Comeau, Digital Mars) it would not be
// an issue.

struct print_path
{
public:
	void operator ()(winstl_ns_qual(findfile_sequence_a)::value_type const &value)
	{
		fprintf(stdout, "%s\n", (const char *)value);
	}
	void operator ()(winstl_ns_qual(findfile_sequence_w)::value_type const &value)
	{
		fwprintf(stdout, L"%s\n", (const wchar_t *)value);
	}
};


void dump_path(char const *path)
{
	printf("%s\n", path);
}

int main(int /* argc */, char ** /* argv */)
{
	{
		current_directory_a	current_dir;
		findfile_sequence_a	files(current_dir, "*.*", findfile_sequence_a::files | findfile_sequence_a::includeDots);

		printf("Current files (ANSI, via implicit conversion to char const *):\n");
		stlsoft_ns_qual(for_each_preinc)(files.begin(), files.end(), dump_path);

		printf("Current files (ANSI, via sequence::value_type):\n");
		stlsoft_ns_qual(for_each_preinc)(files.begin(), files.end(), print_path());

		printf("Current files (ANSI, via implicit conversion to char const *, post-increment operator):\n");
		stlsoft_ns_qual(for_each_postinc)(files.begin(), files.end(), dump_path);

		printf("Current files (ANSI, via sequence::value_type, post-increment operator):\n");
		stlsoft_ns_qual(for_each_postinc)(files.begin(), files.end(), print_path());
	}

	{
		current_directory_w	current_dir;
		findfile_sequence_w	files(current_dir, L"*.*", findfile_sequence_w::files);

		printf("Current files (Unicode, via sequence::value_type):\n");
		stlsoft_ns_qual(for_each_preinc)(files.begin(), files.end(), print_path());
	}

	{
		windows_directory_a	win_dir;
		findfile_sequence_a	directories(win_dir, "*.*", findfile_sequence_a::directories);

		printf("Windows directory directories (ANSI, via implicit conversion to char const *):\n");
		stlsoft_ns_qual(for_each_preinc)(directories.begin(), directories.end(), dump_path);
	}

	{
		windows_directory_w	win_dir;
		findfile_sequence_w	files(win_dir, L"*.*", findfile_sequence_w::files);

		printf("Windows directory files (Unicode, via sequence::value_type):\n");
		stlsoft_ns_qual(for_each_preinc)(files.begin(), files.end(), print_path());
	}

	// Test operator ++(int) - for Tanemura

	{
		windows_directory_w	win_dir;
		findfile_sequence_w	files(win_dir, L"*.exe", findfile_sequence_w::files);

		printf("Windows directory files (Unicode, via sequence::value_type, post-increment operator):\n");
		findfile_sequence_w::const_iterator	begin	=	files.begin();
		findfile_sequence_w::const_iterator	end		=	files.end();

		for(; begin != end; )
		{
			print_path()(*begin++);
		}
	}

	{
		windows_directory_a	win_dir;
		findfile_sequence_a	files(win_dir, "*.exe", findfile_sequence_a::files);

		printf("Windows directory files (ANSI, copying iterators):\n");
		findfile_sequence_a::const_iterator	begin	=	files.begin();
		findfile_sequence_a::const_iterator	end		=	files.end();

		for(; begin != end; ++begin)
		{
			print_path()(*begin);

			findfile_sequence_a::const_iterator	next(begin);

			if(++next != end)
			{
				print_path()(*next);
			}
		}
	}

	{
		findfile_sequence_a	files(".", ".", findfile_sequence_a::directories);

		printf("Current directory directories (ANSI):\n");
		findfile_sequence_a::const_iterator	begin	=	files.begin();
		findfile_sequence_a::const_iterator	end		=	files.end();

		for(; begin != end; ++begin)
		{
			print_path()(*begin);
		}
	}

	return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
