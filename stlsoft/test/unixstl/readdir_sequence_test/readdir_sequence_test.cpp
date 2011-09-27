
// Updated: 8th June 2004

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft_simple_algorithms.h>

stlsoft_ns_using(for_each_postinc)

#include <unixstl_readdir_sequence.h>

unixstl_ns_using(readdir_sequence)

#include <algorithm>

unixstl_ns_using_std(for_each)

/* ////////////////////////////////////////////////////////////////////////// */

void print_file_name(struct dirent const *entry)
{
	printf(" \"%s\"\n", entry->d_name);
}

int main(int argc, char *argv[])
{
	char const	*search_dir;

	if(argc < 2)
	{
		search_dir = ".";
	}
	else
	{
		search_dir = argv[1];
	}

	{
		printf("\nreaddir_sequence: with dots\n");

		readdir_sequence	sequence(search_dir, readdir_sequence::includeDots);

		for_each(sequence.begin(), sequence.end(), print_file_name);
	}

	{
		printf("\nreaddir_sequence, directories, without dots\n");

		readdir_sequence	                sequence(search_dir, readdir_sequence::directories);
		readdir_sequence::const_iterator	b  =  sequence.begin(); 
		readdir_sequence::const_iterator	e  =  sequence.end(); 

		for(; b != e; ++b)
		{
			readdir_sequence::const_iterator	c(b);
			readdir_sequence::const_iterator	d(c);

			c = d;

			print_file_name(*c);
		}
	}

	{
		printf("\nreaddir_sequence, files, without dots (post-increment)\n");

		readdir_sequence	sequence(search_dir, readdir_sequence::files);

		for_each_postinc(sequence.begin(), sequence.end(), print_file_name);
	}

	return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
