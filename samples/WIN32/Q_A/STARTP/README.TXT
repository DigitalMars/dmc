Summary:

    This sample demonstrates how to start a new process with various
    process attributes. It is a functional replacement for the
    "start" command.

More Information:

Starts a specified program, batch, or command file.
STARTP [options] [program] [parameters]

  /Ttitle     Title to display in window title bar. Quote the
              entire paramter to include spaces in the title
  /Dpath      Starting directory
  /l|/h|/r    Set priority to low/high/realtime
  /min|/max   Start window minimized/maximized
  /w          Wait for started process to end before returning
              control to the command processor. This option starts
              the process synchronously
  /c          Use current console instead of creating a new console
  /b          Start detached with no console at all
  /p          Create new process group for ^c, ^break events
  /s          Run Win16 app in separate VDM
  program     A batch file or program to run as either a GUI
              application or a console application
  parameters  These are the parameters passed to the program

Note that the priority parameters may have no effect if the child
process changes its own priority.
