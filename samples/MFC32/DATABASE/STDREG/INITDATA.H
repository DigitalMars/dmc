// initdata.h -- data used to populate the Student Registration tables
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

struct CCourseData
{
	char*   m_CourseID;
	char*   m_CourseTitle;
	int     m_Hours;
};
CCourseData courseData[] =
{
		"MATH101", "Algegra", 4,
		"MATH201", "Calculus I", 4,
		"MATH202", "Calculus II", 4,
};


struct CDynabindSectionData
{
	char*   m_CourseID;
	char*   m_SectionNo;
	char*   m_InstructorID;
	char*   m_RoomNo;
	char*   m_Schedule;
	int     m_Capacity;
	char*   m_LabRoomNo;
	char*   m_LabSchedule;
};
CDynabindSectionData dynabindSectionData[] =
{
	"MATH101", "1", "KLAUSENJ", "KEN-12",   "MWF10-11",  40, "WEATH-101",  "MWF11-12",
	"MATH101", "2", "ROGERSN",  "WIL-1088", "TTH3:30-5", 15, "JMA-2048",   "TTH2:30-3",
	"MATH201", "1", "ROGERSN",  "WIL-1034", "MWF2-3",    20, "WIL-1024",   "TTH1-2",
	"MATH201", "2", "SMITHJ",   "WIL-1054", "MWF3-4",    25, "WIL-1048",   "MWF4-5",
	"MATH202", "1", "KLAUSENJ", "WIL-1054", "MWF9-10",   20, "WIL-2000",   "TTH1-2:30",
	"MATH202", "2", "ROGERSN",  "KEN-12",   "TTH9-30:11",15, "KEN-30",     "TTH8:30-9:30",
	"MATH202", "3", "KLAUSENJ", "WIL-2033", "TTH3-4:30", 15, "WIL-2001",   "TTH2-3",
};


struct CEnrollmentData
{
	long    m_StudentID;
	char*   m_CourseID;
	char*   m_SectionNo;
	char*   m_Grade;
};
CEnrollmentData enrollmentData[] =
{
	1001, "MATH101", "1", "A",
	1002, "MATH101", "1", "B",
	1003, "MATH101", "1", "C",
	1004, "MATH101", "1", "A",
	1005, "MATH201", "2", "B",
	1006, "MATH201", "2", "A",
	1007, "MATH201", "2", "C",
	1008, "MATH202", "1", "C",
	1009, "MATH202", "1", "B",
	1010, "MATH202", "1", "A",
	1011, "MATH202", "2", "B",
	1012, "MATH202", "2", "B",
	1013, "MATH202", "3", "A",
	1014, "MATH202", "3", "C",
	1015, "MATH202", "3", "A",
	1016, "MATH202", "3", "B",
};


struct CInstructorData
{
	char*   m_InstructorID;
	char*   m_Name;
	char*   m_RoomNo;
};
CInstructorData instructorData[] =
{
	"KLAUSENJ", "Klausen, Jim",  "HAN-171",
	"ROGERSN",  "Robers, Nancy", "HAN-163",
	"SMITHJ",   "Smith, Jane",   "HAN-155",
};


struct CSectionData
{
	char*   m_CourseID;
	char*   m_SectionNo;
	char*   m_InstructorID;
	char*   m_RoomNo;
	char*   m_Schedule;
	int     m_Capacity;
};
CSectionData sectionData[] =
{
	"MATH101", "1", "KLAUSENJ",  "KEN-12",   "MWF10-11",   40,
	"MATH101", "2", "ROGERSN",   "WIL-1088", "TTH3:30-5",  15,
	"MATH201", "1", "ROGERSN",   "WIL-1034", "MWF2-3",     20,
	"MATH201", "2", "SMITHJ",    "WIL-1054", "MWF3-4",     25,
	"MATH202", "1", "KLAUSENJ",  "WIL-1054", "MWF9-10",    20,
	"MATH202", "2", "ROGERSN",   "KEN-12",   "TTH9:30-11", 15,
	"MATH202", "3", "KLAUSENJ",  "WIL-2033", "TTH3-4:30",  15,
};

struct CStudentData
{
	long    m_StudentID;
	char*   m_Name;
	int     m_GradYear;
};
CStudentData studentData[] =
{
	1001, "Smith, Randy",      96,
	1002, "Maples, Alex",      95,
	1003, "Jones, Thomas",     95,
	1004, "Shannon, Eric",     96,
	1005,  "Foster, Susan",    96,
	1006, "Jefferson, Nancy",  95,
	1007, "Turner, Bob",       96,
	1008, "Holm, David",       95,
	1009, "Reynolds, Don",     96,
	1010, "Taylor, Robert",    95,
	1011, "Karr, Dave",        96,
	1012, "Tannant, Tim",      96,
	1013, "Marcus, Susan",     95,
	1014, "Butterfield, Rita", 96,
	1015, "Amon, Craig",       95,
	1016, "Anderson, Sandra",  96,
	1017, "Cooper, Linda",     95,
};
