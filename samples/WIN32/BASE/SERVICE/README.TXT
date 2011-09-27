Sample: Simple Service


SUMMARY:

    The Simple Service sample demonstrates how to create and
    install a service.

    In this particular case, the service merely opens a named
    pipe of the name, which defaults to \\.\pipe\simple.  And
    waits for traffic.  If it receives anything, it surrounds
    the input with:

            Hello! [<input goes here>]

    and sends it back down the pipe to the client.

    The service can be Started, and Stopped from either the
    services control panel, the net start/stop command line,
    or by using the Service Controller utility ( see below )

    The service also provides command line parameters which
    install, remove, or run (debug) the service as a console app.

    To aid in writing/debugging service, the
    SDK contains a utility (MSTOOLS\BIN\SC.EXE) that
    can be used to control, configure, or obtain service status.
    SC displays complete status for any service/driver
    in the service database, and allows any of the configuration
    parameters to be easily changed at the command line.
    For more information on SC.EXE, type SC at the command line.


TO USE:

    To install the service, first compile everything, and then:

        simple -install

    Now, let's look at SC's command line parameters:

        sc

    Next, all you have to do is start the service, either using the
    "net start" method, the control panel Services applet, or:

        sc start simpleservice

    Verify that the service has entered the RUNNING state:

        sc query simpleservice

    Once the service has been started, you can use the CLIENT
    program to verify that it really is working, using the syntax:

            client

    which should return the response:

            Hello! [World]

    If, after playing with the sample you wish to remove the service,
    simply say:

            Simple -remove

    Other: You may change the name of the pipe by specifying "-pipe <pipename>"
    as startup parameters for both CLIENT and SIMPLE.  The string passed in by
    CLIENT may be changed by specifying "-string <string>".

    

MORE INFO:

    The use of the SERVICE.H header file and the accompanying SERVICE.C
    file simplifies the process of writting a service.  You as a developer
    simply need to follow the TODO's outlined in th header file, and 
    implement the ServiceStart() and ServiceStop() functions for your service.

    There is no need to modify the code in SERVICE.C.  Just add SERVICE.C
    to your project and link with the following librarys:

    libcmt.lib kernel32.lib advapi.lib shell32.lib

    This code also supports UNICODE.  Be sure to compile both SERVICE.C and
    and code #include "service.h" with the same Unicode setting.

    Upon completion, your code will have the following command line interface

    <service exe> -?                to display this list
    <service exe> -install          to install the service
    <service exe> -remove           to remove the service
    <service exe> -debug <params>   to run as a console app for debugging

    Note: This code also implements Ctrl+C and Ctrl+Break handlers
          when using the debug option.  These console events cause
          your ServiceStop routine to be called



Additional reference words:

CloseServiceHandle, InitializeSecurityDescriptor,
SetSecurityDescriptorDacl, SetServiceStatus, OpenSCManager,
StartServiceCtrlDispatcher, RegisterEventSource,
DeregisterEventSource, RegisterServiceCtrlHandler,
SetConsoleCtrlHandler
