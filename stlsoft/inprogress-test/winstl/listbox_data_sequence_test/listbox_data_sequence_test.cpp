

#include <winstl.h>

#include <string>

using std::string;


#include <research/winstl_listbox_data_sequence.h>


int main(int /* argc */, char ** /*argv*/)
{
	winstl_ns_using(listbox_data_sequence)

	HWND	hwndListBox	=	::CreateWindowExA(0, "LISTBOX", "", LBS_SORT, 0, 0, 0, 0, 0, 0, 0, 0);

	SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)"String 3");
	SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)"String 2");
	SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)"String 45");
	SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)"String 1");

	listbox_data_sequence					lbs(hwndListBox);
	listbox_data_sequence::const_iterator	begin	=	lbs.begin();
	listbox_data_sequence::const_iterator	end		=	lbs.end();

	for(; begin != end; ++begin)
	{
		printf("%s\n", (const char*)(*begin).c_str());
	}

    return 0;
}
