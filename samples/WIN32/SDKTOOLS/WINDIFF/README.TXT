Sample: Windiff

Summary:

Windiff compares directories or files giving a graphic comparison.

Full command line syntax:

WINDIFF [paths] [saveoption]

paths

   path
       to compare what's at path with what's in the current directory
    
   path1 path2
       to compare what's at path1 with what's at path2

   Note: path can always be relative or absolute, net or local, file or 
   directory.


saveoption = -slrd savefile

   where slrd is any combination of these four letters to write the
   names of files that are

       s the same in both paths
       l only in the left hand path
       r only in the right hand path
       d in both paths, but the two files are different

More Information:

Colors

    RED background designates text from LEFT FILE
    YELLOW background designates text from RIGHT FILE
    BLUE text designates a moved line
    BLACK text is used for everything else


File menu

    Compare files...
        Uses the File Open dialog for each of two files to be compared.
    Compare directories
        Opens a to dialog to allow entry of two directory names for comparison.
    Close
        Closes the current files.
    Abort
        Will be greyed unless an operation is in progress.  Then allows
        that operation to be terminated before completion.
    Save File List
        Allows the list of files which are (the same, different, only in left
        or only in right) to be saved.
    Copy Files...
        Opens a dialog allowing you to write the files to a disk.
    Print
        Prints out the results of the compare.

View menu

    Outline
        Show lists of files.
    Expand
        Show comparison of selected files.
    Picture
        Show picture as well as selected files.
    Previous Change (F7)
        Skip to previous point of difference in the file.
    Next change (F8)
        Skip to next point of difference in the file.

Expand menu

    Left file only
        Show only lines from left hand file (but coloured so as to highlight
        changed lines).
    Right file only
        Show only lines from right hand file (but coloured so as to highlight
        changed lines).
    Both files (default)
        Show a merge of both files. All the lines in the left hand file are
        shown in the order in which they occur in that file, likewise for
        the right hand file.  Lines which are ONLY in the left hand file
        are shown in red.  Lines which are ONLY in the right hand file
        are shown in yellow.
    Left line numbers
        Line numbers are shown, based on the left hand file.
    Right line numbers
        Line numbers are shown, based on the right hand file.
    Right line numbers
        Line numbers are shown, based on the right hand file.
    No line numbers
        Line numbers are turned off.

Options menu

    Ignore blanks
        Blanks are ignored in the expanded view, so that lines which
        differ only in white space are shjown as identical.
    Show Identical Files
        Include files which are identical in each path in outline mode.
    Show Left-Only Files
        Include files which occur only in the left hand path in outline mode.
    Show Right-Only Files
        Include files which occur only in the right hand path in outline mode.
    Show Different Files
        Include files which occur in both paths, but which are not the same
        in outline mode.

Help

    About
        Displays version information about WINDIFF.




