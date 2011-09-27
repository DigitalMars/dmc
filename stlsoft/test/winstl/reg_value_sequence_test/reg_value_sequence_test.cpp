
// Updated: 22nd April 2004

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#define	UNICODE
#define	_UNICODE
#define __TEXT(x)	L ## x

#ifdef __DMC__x
#define basic_reg_value                             _brv
#define basic_reg_value_sequence_const_iterator     _brvsci
#endif /* __DMC__ */

#include <winstl.h>

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER == 1100
# pragma warning(disable : 4550) // VC 5 is a bit dim
#endif /* _MSC_VER == 1100 */

#include <winstl_reg_value.h>
#include <winstl_reg_value_sequence.h>

#include <stdio.h>
#include <tchar.h>

#include <algorithm>

winstl_ns_using(reg_value_a)
winstl_ns_using(reg_value_w)
winstl_ns_using(reg_value)
winstl_ns_using(reg_key_a)
winstl_ns_using(reg_key_w)
winstl_ns_using(reg_key)
winstl_ns_using(reg_value_sequence_a)
winstl_ns_using(reg_value_sequence_w)
winstl_ns_using(reg_value_sequence)

static void dump_key(reg_value const &value);
static void dump_bin(reg_value const &value);

int main(int /* argc */, char ** /*argv*/)
{
	reg_value_a	value;
	reg_value_w	valuew;

    value.type();
    value.name();
    value.value_sz();
    value.value_expand_sz();
    value.value_dword();
    value.value_binary();
#if 0
    value.value_dword_littleendian();
    value.value_dword_bigendian();
#endif /* 0 */

//	reg_key								key(HKEY_CLASSES_ROOT, __TEXT("CLSID"));
	reg_key								key(HKEY_CURRENT_USER, __TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer"));

	reg_value_sequence					values(key);

	printf("values: %lu values\n\n", values.size());

	winstl_ns_qual_std(for_each)(values.begin(), values.end(), dump_key);

	printf("\n\n");

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
	winstl_ns_qual_std(for_each)(values.rbegin(), values.rend(), dump_key);
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

    return 0;
}

void dump_key(reg_value const &value)
{
	_tprintf(__TEXT("%s: "), value.name().c_str());
	switch(value.type())
	{
		case	REG_SZ:						_tprintf(__TEXT("[\"%s\"]"), value.value_sz().c_str()); break;
		case	REG_EXPAND_SZ:				_tprintf(__TEXT("[\"%s\"]"), value.value_expand_sz().c_str()); break;
		case	REG_BINARY:					dump_bin(value); break;
		case	REG_DWORD_LITTLE_ENDIAN:	_tprintf(__TEXT("[%lu]"), value.value_dword()); break;
		case	REG_DWORD_BIG_ENDIAN:		_tprintf(__TEXT("[%lu]"), value.value_dword()); break;
		default:							_tprintf(__TEXT("")); break;
	}
	printf("\n");
}

static void dump_bin(reg_value const &value)
{
	winstl_ns_using(ws_byte_t);
	winstl_ns_using(reg_blob);

	reg_value::blob_type	blob(value.value_binary());

	reg_value::blob_type::const_iterator	begin	=	blob.begin();
	reg_value::blob_type::const_iterator	end		=	blob.end();

	printf("[ ");
	for(int i = 0; begin != end; ++begin)
	{
		printf("%02x ", *begin);

		if(0 == (++i % 8))
		{
			printf("\n  ");
		}
	}
	printf("]\n");
}

/* ////////////////////////////////////////////////////////////////////////// */
