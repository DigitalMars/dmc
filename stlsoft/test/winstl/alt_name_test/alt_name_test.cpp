// alt_name_test.cpp : Defines the entry point for the console application.
//

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <winstl_findfile_sequence.h>

winstl_ns_using(findfile_sequence_a)

int main(int /* argc */, char* /* argv */[])
{
//	findfile_sequence_a						files("c:\\documents and settings\\*.*");
	findfile_sequence_a						files("\\", "*.*");
	findfile_sequence_a::const_iterator		begin	=	files.begin();
	findfile_sequence_a::const_iterator		end		=	files.end();

	for(; begin != end; ++begin)
	{
		printf("%s\n", (*begin).get_filename());
		printf("%s\n", (*begin).get_short_filename());
		printf("%s\n", (*begin).get_path());
		printf("\n");
	}

	return 0;
}
