Sample:

Demonstration of Setting Console Text Color Attributes

Summary:

The CONSOLEC sample illustrates the use of the
SetConsoleTextAttribute and GetConsoleScreenBufferInfo
functions to set the console text color attributes.

More Information:

This sample also functions as a console window text color
setting utility.  The command syntax for the utility is as
follows:

     COLOR FOREGROUND [BACKGROUND]

where FOREGROUND and BACKGROUND are the new text color
selections for the current console.  If the utility is
invoked without any options, the utility's syntax and a
table of the possible color choices is displayed.  The
BACKGROUND selection is optional and thus the FOREGROUND
text color may be soley changed.

Possible colors are: BLACK, BLUE, GREEN, CYAN, RED, MAGENTA,
YELLOW and WHITE.  Each can be selected as the FOREGROUND
or the BACKGROUND color.  Selection of the same color for
both the FOREGROUND and the BACKGROUND is not permitted.
The color options are not case sensitive and only the first
unique characters are necessary to select the color.  For
example:

     COLOR BLU W

will select BLUE on WHITE text color attributes.

The text color attribute changes will only affect new
console output.  Thus, text in the console buffer before the
utility is invoked will retain their original color
attributes.
