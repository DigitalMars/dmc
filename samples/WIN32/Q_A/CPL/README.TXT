Sample: Control Panel Applet Sample

Summary:

A sample demonstrating the creation of a Control Panel Applet.

More Information:

The following code sample shows the CPlApplet function for a DLL containing
three Control Panel applications that set preferences for a component stereo
system attached to the computer. The sample uses an application-defined
StereoApplets array that contains three structures, each corresponding to
one of the Control Panel applications. Each structure contains all the
information required by the CPL_NEWINQUIRE message, as well as the dialog
box template and dialog box procedure required by the CPL_DBLCLK message.
The code demonstrates how to fill the structures in the StereoApplets array.

To install your new DLL, simply copy the .CPL file into your SYSTEM32
directory and the Control Panel will automatically load the DLL upon 
startup.

Additional reference words:

CPIApplet
