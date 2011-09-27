Sample: DDEML API Demonstration

Summary:

The Install SDK sample provides an example of how to use the DDEML
API's to add groups and program items to the Program Manager. The 
program can be run either in interactive or batch mode and the
search path can be specified on the command line.

More Information:

The program can be started from the command line. The following
flags can be used.

   (-/)(sS) <path> Specify the search path. Currently this must be
   single path. The application searches for an environmental variable
   "MSTOOLS". If this variable is not found then the default value is
   c:\mstools\samples.

   (-/)g(G) <name> Specify the name of the group that the items 
   should be added to. The default value is 'Sample Applications'.

   (-/)b(B) Specifies that the program should run in batch mode.
   The program will find all of the .exe files in the specified
   path, searching recursively. It will then create the specified
   group and add all of the found executables to the group. The
   program will then exit.

   (-/)(iI) Specifies interactive mode. This is the default. The
   program will find all of the exe files in the specified 
   path, searching recursively. The names will be displayed in
   a list with the name that will be use displayed in an additional
   list. A drop down combo box will display a list of the currently
   available groups. The user can type in a new group name if
   desired.

 If the program finds more than 50 executables then after adding 50
items to a group a new group is created using the name currently 
selected in the Program Manager Group list with "Part <n>" appended
where <n> starts at 2 and is incremented after every new group is
added.
