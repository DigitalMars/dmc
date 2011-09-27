
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#ifdef __DMC__
#define processheap_allocator                   _pha
#define reg_traits                              _rt
#define basic_reg_key                           _brk
#define basic_reg_key_sequence_const_iterator   _brksci
#endif /* __DMC__ */

#include <winstl.h>
#include <winstl_reg_key_sequence.h>

#include <stdio.h>

#include <algorithm>
#include <functional>

winstl_ns_using(ws_char_a_t)
winstl_ns_using(reg_key_a)
winstl_ns_using(reg_key_sequence_a)


static void iterate_recurse(reg_key_sequence_a::const_iterator from, reg_key_sequence_a::const_iterator to, int depth);


struct dump_key
	: public winstl_ns_qual_std(unary_function)<reg_key_a const &, void>
{
	void operator ()(reg_key_a const key)
	{
		printf("%s\n", key.name().c_str());
	}
};


int main(int /* argc */, char ** /*argv*/)
{
	reg_key_sequence_a	keys(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft");

	printf("Iterating non-recursively (forward):\n");
	winstl_ns_qual_std(for_each)(keys.begin(), keys.end(), dump_key());

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    printf("Iterating non-recursively (backward):\n");
	winstl_ns_qual_std(for_each)(keys.rbegin(), keys.rend(), dump_key());
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

	printf("Iterating recursively:\n");
	iterate_recurse(keys.begin(), keys.end(), 1);

    return 0;
}


void iterate_recurse(reg_key_sequence_a::const_iterator from, reg_key_sequence_a::const_iterator to, int depth)
{
	ws_char_a_t	prefix[255];

	memset(prefix, ' ', sizeof(prefix));
	prefix[depth] = '\0';

	for(; from != to; ++from)
	{
		const reg_key_a		&key = *from;
		reg_key_sequence_a	keys(key);

		printf("%s, %s\n", prefix, key.name().c_str());

		iterate_recurse(keys.begin(), keys.end(), 1 + depth);
	}
}

/* ////////////////////////////////////////////////////////////////////////// */


#if 0
"basic_reg_key_sequence_const_iterator<>::basic_reg_key_sequence_const_iterator<>(struct HKEY__ *)"

#endif /* 0 */
