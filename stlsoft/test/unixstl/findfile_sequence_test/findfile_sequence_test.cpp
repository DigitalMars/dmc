
// Updated: 16th June 2004

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

//#define	UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS

#include <stdio.h>

#include <unixstl_findfile_sequence.h>

unixstl_ns_using(findfile_sequence)

#include <algorithm>

unixstl_ns_using_std(for_each)

/* ////////////////////////////////////////////////////////////////////////// */

void print_file_name(char const *fileName)
{
	printf(" \"%s\"\n", fileName);
}

extern "C" void __stdcall Sleep(unsigned long);

int main(int argc, char *argv[])
{
	char const	*pattern	=	NULL;
	char const	*search_dir	=	NULL;

//	::Sleep(100000);

	if(argc < 2)
	{
		// This just allows easy testing on Win32.
#ifdef WIN32
		pattern = "*.*";
#else
		pattern = "*";
#endif /* WIN32 */
	}
	else
	{
		pattern = argv[1];
	}

	if(argc < 3)
	{
		search_dir = ".";
	}
	else
	{
		search_dir = argv[2];
	}

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
	try
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
	{

		{
			printf("\nfindfile_sequence, without dots, unsorted\n");

			findfile_sequence	sequence(search_dir, pattern, findfile_sequence::noSort |  findfile_sequence::includeDots);

			for_each(sequence.begin(), sequence.end(), print_file_name);
		}

		{
			printf("\nfindfile_sequence, files, sorted\n");

			findfile_sequence	sequence(search_dir, pattern, findfile_sequence::files);

			for_each(sequence.begin(), sequence.end(), print_file_name);
		}

		{
			printf("\nfindfile_sequence, files, unsorted, absolute-path\n");

			findfile_sequence	sequence(search_dir, pattern, findfile_sequence::files | findfile_sequence::absolutePath);

			for_each(sequence.begin(), sequence.end(), print_file_name);
		}

		{
			printf("\nfindfile_sequence: with dots, directories only, directories marked, sorted, reverse iteration\n");

			findfile_sequence	sequence(pattern, findfile_sequence::directories |  findfile_sequence::includeDots |  findfile_sequence::markDirs);

			for_each(sequence.rbegin(), sequence.rend(), print_file_name);
		}

		{
			printf("\nfindfile_sequence: without dots, unsorted, accessed via indexing\n");

			findfile_sequence	sequence(pattern);

			for(findfile_sequence::size_type i = 0; i < sequence.size(); ++i)
			{
				if(i % 2)
				{
					printf(" %u: \"%s\"\n", i, sequence[i]);
				}
				else
				{
					printf(" %u: \"%s\"\n", i, const_cast<findfile_sequence const&>(sequence)[i]);
				}
			}
		}
	}
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
	catch(unixstl_ns_qual(glob_sequence_exception) &gx)
	{
		printf("Exception caught: %s (glob(): %d; errno: %d)\n", gx.what(), gx.get_globstatus(), gx.get_errno());
	}
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */

	return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
