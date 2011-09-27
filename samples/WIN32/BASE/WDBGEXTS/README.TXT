Sample: Creating a WinDbg Extension


Summary:

The purpose of the WDBGEXTS sample is to demonstrate how to create a WinDbg
extension. This is a port of a sample demonstrating how to create an NTSD
extension.

Extensions are DLL entry points. The arguments passed to an extension are:

    HANDLE hCurrentProcess - Supplies a handle to the current process (at
        the time the extension is called).

    HANDLE hCurrentThread - Supplies a handle to the current thread (at
        the time the extension is called).

    DWORD CurrentPc - Supplies the current pc at the time the extension is
        called.

    PWINDBG_EXTENSION_APIS lpExtensionApis - Supplies the address of the
        functions callable by this extension.

    LPSTR lpArgumentString - Supplies the command-line arguments for the
        extension.

The type PWINDBG_EXTENSION_APIS is defined in \mstools\h\wdbgexts.h.

Note that in the makefile the -Gz option is specified to compiler in order
to ensure that __stdcall is used.

More Information:

The following is a description of the exported functions

    igrep()

        Searches the instruction stream for a pattern.

    str()

	Given a pointer to a string, it prints out the string, its length,
	and its location in memory.

In order to use the commands contained in WDBGEXTS.DLL, make sure that the
DLL is placed in a directory which is on the PATH.

The syntax for the commands is as follows


    !wdbgexts.igrep [pattern [expression] ]

    !wdbgexts.str [string]
