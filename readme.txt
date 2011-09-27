	Digital Mars C/C++ Compilers
	----------------------------

Downloaded Version
------------------

The documentation is available online at:

	www.digitalmars.com

Much more software (over 300Mb) comes with the CD version, available at:

	www.digitalmars.com/shop.html


Installing the Downloaded Version
---------------------------------

Digital Mars has a different view of installation than most software products.
It doesn't need registry entries, environment variable settings, nor does it
need to install anything in the \windows\system directory.

Consequently, the compiler can be run without any need for an install.
Just run the programs in \dm\bin. This is most convenient
for cases where a particular version of the compiler is needed, or to just try
it out.

Most of the non-IDDE Digital Mars programs are console programs.
A command prompt window can be created on XP by
clicking on [Start][Command Prompt].

Installing is as simple as going to the root directory of the hard disk and
unzipping the downloaded dm???c.zip file:

	unzip dm???c.zip

The compiler is \dm\bin\dmc.exe. Too make it even more convenient, add

	c:\dm\bin

to the PATH environment variable setting (c: is the drive it is installed on).

To compile a simple hello.c program:

	\dm\bin\dmc hello

will compile and link to create hello.exe.

To use <iostream> and other STL code, download
ftp://ftp.digitalmars.com/Digital_Mars_C++/STL/stlport.zip
as well. Be sure and use the switch:

	-I\dm\stlport\stlport

so that the compiler can find <iostream>.

Do not install into a subdirectory with spaces, tabs,
+ signs, commas, or # characters in the subdirectory or path names.

Full CD Version
---------------

Documentation is provided in HTML format, which means it is easiest
to read it in a browser, such as Netscape or Explorer.

To read the documentation, bring up the browser, and type in:

	g:\dm\index.html

where g is the drive letter of the cdrom drive the Digital Mars CD is in.

C++ Seminars
------------

If you want to hone your C++ programming skills, check out the
Extraordinary C++ seminar at http://www.astoriaseminar.com
