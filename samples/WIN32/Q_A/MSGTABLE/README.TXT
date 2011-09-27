Sample: Constructing and Using a Message Table Resource

Summary:

This sample demonstrates how to contruct and use a message table
resource in your application. The sample message table (messages.mc)
for the message compiler (MC.EXE) shows the format of a typical
message table source file. Note that some of the entries for each
message are optional but are shown for demonstration purposes. See
the documentation in the tools.hlp file on the message table
compiler for more info on which fields are optional.

More Information:

This particular sample shows how to create a resource-only DLL that
contains nothing but the message table resource. The DLL source file
(messages.c) has a stub entry point that simply returns TRUE. This
is due to the requirement that a DLL has at least one entry point,
so we need to have one for our resource-only DLL.

The msgtest.exe executable shows how to load the message table DLL,
how to extract the message text from the resource with the
FormatMessage API, and how to decode the various bits in the message
ID. Note that the messages.h include file to the executable is
created during the make process by the message compiler (mc.exe) as
it is creating the binary resource file from the message table
source file.

The makefile should come in very handy in showing how to set up your
dependancy rules for your message table source files and message
table DLL.

This sample could be easily modified to link the message table
resource directly into your executable rather than into a
stand-alone DLL; there is no requirement that the message table
resouce be located only in a DLL, though this is the common case.
