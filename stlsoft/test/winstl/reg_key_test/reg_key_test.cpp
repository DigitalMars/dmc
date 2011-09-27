
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stdio.h>

#include <winstl.h>
#include <winstl_reg_key.h>

winstl_ns_using(reg_key_a)

int main(int /* argc */, char ** /*argv*/)
{
	reg_key_a	keyRoot(HKEY_CLASSES_ROOT, "");
	reg_key_a	key(keyRoot, "dspfile");

	printf("Name: %s\n", key.name().c_str());
	printf("Class: %s\n", key.reg_class().c_str());
	printf("#sub-keys: %u\n", key.num_sub_keys());
	printf("#values: %u\n", key.num_values());

    return 0;
}
