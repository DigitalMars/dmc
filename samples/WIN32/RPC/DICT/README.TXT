/*************************************************************/
/**                                                         **/
/**                    Microsoft RPC Examples               **/
/**                    Dictionary Application               **/
/**             Copyright(c) Microsoft Corp. 1992-1995      **/
/**                                                         **/
/*************************************************************/

Overview:
---------

This demo is a an example a non trivial local application to RPC.
The local program is a character oriented "play" program, which lets a
user insert, delete, and iterate over a dictionary.  The dictionary is
based on splay trees, which are printed out in a format described below.

We remoted the basic program by taking the interface to the dictionary
(given in dict0.h) and creating an interface to a remote dictionary,
(given in replay.idl) which is remoted using RPC.

Following the local dictionary interface. which is minimal and uniform,
the remote interface is also uniform.  However, since the local dictionary
is generic (can store any item type, using void* pointers to items),
the interface needed to change to deal with predefined (Record) type
items in order to be remoted using the Microsoft RPC system.  Also,
since the local implementation allows a "peek" at the root of the tree
using a DICT_CURR_ITEM macro, the whole interface achanged needed a to
change.

Remote dictionaries are represented by context handles to demonstrate
the use of [context_handle].  Context handles are currently initialized
using a global primitive handle [implicit_handle].

State was added to remote dictionaries in order to allow sharing them,
and maintain reference counts.  At this time, however, access to shared
dictionaries is *not* serialized!  By default each client gets his own
copy of the dictionaries.  To use shared dictionaries, start the client
using the -s switch.

The use of context to maintain state is also demonstrated by
differenciating between a private iterator, activated by "n" for "next"
and "p" for "previous", and a global shared iterator activated by "N"
for "Next" and "P" for previous.  To start iterators, use "h" - go to
the "head" of the dictionary, or "t" - go to the "tail" of the dictionary.
The private iterator can be reset to DICT_CURR_ITEM using "r".

Building on Windows NT or Windows 95
------------------------------------

The following environment variables should be set for you already.
  set CPU=i386
  set INCLUDE=%SDKROOT%\h
  set LIB=%SDKROOT%\lib
  set PATH=%SDKROOT%\system32;%SDKROOT%\bin;

Where %SDKROOT% is the root directory for the 32-bit Windows SDK.

For mips, set CPU=mips
For alpha, set CPU=alpha

Build the sample distributed application:
  nmake cleanall
  nmake

Building the client application for MS-DOS
------------------------------------------

After installing the Microsoft Visual C/C++ version 1.50 development
environment and the 16-bit RPC SDK on a Windows NT or Windows 95
computer, you can build the sample client application from Windows NT
or Windows 95.

  nmake -f makefile.dos cleanall
  nmake -f makefile.dos

This builds the client application client.exe.

You may also execute the Microsoft Visual C/C++ compiler under MS-DOS.
This requires a two step build process.

  Step One: Compile the .IDL files under Windows NT or Windows 95
     nmake -a -f makefile.dos replay.h

  Step Two: Compile the C sources (stub and application) under MS-DOS.
     nmake -f makefile.dos

Using the program:
------------------

To use the local dictionary example, type "play".

To use the remote versiom, type "server" on the server side.
On the client side, for each client use "client" for a fresh,
private copy of the dictionary, or "client -s" for a shared copy
of the dictionary.  The following command line options are
available on the client side.

Options:
--------

The -? displays the following message:

Usage : client [-s] [-n <server>] [-v <view_option>]

The -s option is used for using a shared version of the dictionary.
    If none exist, a fresh shared version is created.

The -v<option> is used for getting different views of the dictionary:
    -v normal (default) - default view of the dictionary
    (see explanation below)
    -v flat - Flat printout
    -v rev (or -v reverse) - reversese the tree to print left subtree first
    followed by the node, followed by the right subtree, at every level

The -n <server_name> is used for specifying a server machine.  Without it
    the server is assumed to run on the same machine.

    If you use the -n option you have to make sure that the server is
    ready to receive calls; that is you have to start the server, by typing

    -> server

    to the console, even if the server is your own machine!

Program structure:
==================

Basic application:
------------------

  dict0.h + dict0.c - These files contain a basic generic dictionary
    implementation, based on Self Adjusting Trees (Splay trees),
    invented by Sleator and Tarjan.  In the remote dictionary
    application (replay) the generic dictionary code runs on the server,
    which maintains a data base to be shared by clients.

    Note that the generic dictionary data structures contain untyped
    pointers ((void)*), and thus require a special layer of typed trees
    (strongly typed tree nodes) containing items of RPC-able types
    in order to convert the local dictionary package and distribute it
    using the Microsoft RPC system, and particularly the MIDL compiler.

    If you are interested in RPC (as opposed to splay trees) treat
    the generic dictionary as a "black box", and ignore the implementation
    details.  Otherwise, you are probably looking at the wrong README
    file (and should read Tarjan and Sleators paper first anyhow...)

Local dictionary:
-----------------

  play.h + play.c - These files contain a simple character oriented
    interactive demo / test program for the dictionary.

Remote dictionary:
------------------

  replay.idl - The Interface Definition Language (IDL) file contains:

    o type definitions for the remotable objects required (such as the
      Record item type, RecordTreeNode, etc)

    o the context handle definition (a (void)* in this case) providing
      a handle on a remote dictionary

    o the signatures for the remote dictionary operations

  replay.acf - An Attribute Configuration File (ACF), used here to
    specify the initial binding method.  We use implicit_binding to
    bind to the server initially, and to initialize the context handle.

  replay.c - This file contains the implementation of the remote, or
    virtual, dictionary (VDict_...) operations.  These are required
    only on the server side, and are replaced by caller stubs
    (in replay_c.c) on the client side.

Utlities:
---------

  util0.h + util0.c - These files contain utility routines used by both
    the client and server applications allocate and to free dictionaries
    and trees, as well as pretty-print routines, used to print trees on
    both sides.  See description of the user interface portion below for
    further discussion of the tree printing format.

Client side:
------------

  client.c - This file contains the driver of the client side program.
    The client binds to the server, initializes a dictionary, and activates
    a context handle for the dictionary on the server side.  It then goes
    into a test loop which prints the dictionary tree and prompts the user
    for additional input by printing the following usage message:

Usage:
Type a single character, followed by an optional key as follows:

    i <key> :: Insert <key> into dictionary
    d <key> :: Delete <key> from dictionary
    f <key> :: Find <key> in dictionary
    N       :: next of current item in dictionary
    P       :: previous of current item in dictionary
    n       :: Next of local current item in dictionary
    p       :: Previous of local current item in dictionary
    h       :: Head (first item) of dictionary
    t       :: Tail (last item) of dictionary
    ?       :: Print this message
    q       :: Quit

where <key> is <integer> <string>

next op (i d x f n N p P h t ? q):

Server side:
------------

  server.c - This file contains a pretty standard server main loop for
    the remote dictionary "replay" demo.

Tree print format:
------------------

The tree printing routine prints the keys in a binary search tree using
the following "prinTree" recursive algorithm:

    prinTree (right subtree, with current indentation + one "tab");
    print the key at the root (with current indentation);
    prinTree (left subtree, with current indentation + one "tab");

Keys are a pair of the form <integer, string>.  Assume the following tree:


                           -----------
                           |0 : jack |
                           -----------
                          /           \
                     ----------      -----------
                     |0 : don |      |0 : jane |
                     ----------      -----------
                     /       \
                    /         \
               -----------  ----------
               |0 : adam |  |0 : eve |
               -----------  ----------

The above print algorithm would print it as follows:


        0 : jane
    0 : jack
            0 : eve
        0 : don
            0 : adam

The left child of a node is printed on the first line following the node
with is indented by one more tab then the given node.
The right child of a node is printed on the last line preceding the node
with is indented by one more tab then the given node.
If you tilt your hand to the left while examining the printed tree format
it closely resembles the "traditional" format of a rooted binary tree.

Known Limitations:
------------------

Shared dictionaries are unprotected, so serialized access to the
dictionary is assumed.  Use at your own risk :)
