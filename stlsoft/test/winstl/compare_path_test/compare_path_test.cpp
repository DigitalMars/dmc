
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <windows.h>
#include <stdio.h>

#include <winstl.h>

#include <winstl_functionals.h>



int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr, "USAGE: compare_path_test <path1> <path2>\n\n");
	}
	else
	{
		winstl_ns_using(compare_path)

		char const			*path1	=	argv[1];
		char const			*path2	=	argv[2];
		compare_path<char>	cpf;

		if(cpf(path1, path2))
		{
			fprintf(stdout, "\"%s\" == \"%s\"\n", path1, path2);
		}
		else
		{
			fprintf(stdout, "\"%s\" != \"%s\"\n", path1, path2);
		}
	}

    return 0;
}
