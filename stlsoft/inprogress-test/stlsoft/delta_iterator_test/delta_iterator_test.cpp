// delta_iterator_test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>

#include <stlsoft_delta_iterator.h>

char s_sz[] = "Hello World\n";

#include <vector>
#include <string>

typedef	std::vector<std::string>	string_vector_t;

int main(int argc, char* argv[])
{
	typedef	stlsoft::delta_iterator<const char, 3>					char_it_3_t;
	typedef stlsoft::delta_iterator<string_vector_t::iterator, 2>	sv_it_2_t;

	string_vector_t	sv;

	sv.push_back("12");
	sv.push_back("-110");
	sv.push_back("17");
	sv.push_back("19");
	sv.push_back("33");
	sv.push_back("-2391");
	sv.push_back("4096");
	sv.push_back("1");

	{
		char_it_3_t	begin(s_sz);
		char_it_3_t	end(s_sz, stlsoft_num_elements(s_sz) - 1);

		for(; begin < end; ++begin)
		{
			printf(begin);
		}
	}

#if 0
	{
		sv_it_2_t	begin(sv.begin());
		sv_it_2_t	begin(sv.end());

		for(; begin < end; ++begin)
		{
			printf("%s\n", (*begin).c_str());
		}
	}
#endif /* 0 */

	return 0;
}
