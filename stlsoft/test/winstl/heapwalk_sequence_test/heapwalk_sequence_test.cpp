// HeapWalk_test.cpp : Defines the entry point for the console application.
//

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <stlsoft.h>

#include <winstl.h>
#include <winstl_heapwalk_sequence.h>

winstl_ns_using(heapwalk_sequence)

int main(int /* argc */, char* /* argv */[])
{
	heapwalk_sequence					heap_blocks;
	heapwalk_sequence::const_iterator	begin	=	heap_blocks.begin();
	heapwalk_sequence::const_iterator	end		=	heap_blocks.end();

	for(; begin != end; ++begin)
	{
		heapwalk_sequence::const_reference	entry	=	*begin;

		printf("Block size: %lu\n", entry.cbData);

#if 0 // Enable this to demonstrate Forward Iterator concept capability
		for(heapwalk_sequence::const_iterator it = begin; it != end; ++it)
		{
			heapwalk_sequence::const_reference	entry	=	*it;

			printf("  Block size: %lu\n", entry.cbData);
		}
#endif /* 0 */
	}

	return 0;
}
