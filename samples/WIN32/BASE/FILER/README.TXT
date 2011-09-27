                       FILER.EXE - File I/O Sample
                       ===========================

SUMMARY:

    Filer.exe is a basic file management applet, ala File Manager or
    Norton CommanderTM.  It demonstrates many of the new File I/O API,
    and their related algorithms, such as drive enumeration, .EXE Version
    Information retrieval and directory tree expansion.  It also
    demonstrates many intensive user algorithms, such as child managment,
    subclassing, synchronization and control window management.


DESCRIPTION:

    Filer presents the user with two configurable child windows, each of
    which the user may associate with a drive from the drives available on
    the system.

    A Drive Toolbar describes the available drives on the system. Users
    select drives from the toolbar or the Drives menu.  A function toolbar
    also corresponds to the file I/O functions in the File menu.  A command
    line window at the bottom of the app will spawn a command shell with
    the command given, and the option of keeping or destroying the command
    window after the given command completes.

    Each of these Drive child windows contains a Directory ListBox, and a
    File ListBox, with which the user may browse through the files on the
    selected drive.

    Filer gives the user the option to Open (execute/edit), Copy, Delete,
    Move, and Rename files, as well as Make and Remove Directories, and
    display version information embedded in Win32 files.  The active Drive
    child acts as the source, and the inactive Drive child acts as the
    default destination of file I/O operations.

    The Drive children may also be configured as side by side or above and
    below one another, and the File and Directory Listboxes in each may swap
    positions.  The user may opt to fully expand the Directory trees.

    All features of Filer may be selected from the Mouse, Keyboard, or by
    Menu Items.

FUTURE ENHANCEMENTS(?):

    -Directory copy, move, and delete.
    -Font choice.
    -Online Help.
    -Save configuration.
    -File associations.
    -Network drive functions.
    -File size, last change information;  view/modify file attributes.
    -Total size of given single/multiple selection of files.
