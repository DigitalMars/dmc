Sample: Communications API Function Demonstration

Summary:

The COMM sample application is designed to demonstrate the
basics of using the Win32 communications API functions while
maintaining a common code base with Win16 code.

More Information:

The COMM program performs communications using the Windows
functions OpenFile, ReadFile, SetCommState, SetCommMask,
WaitCommEvent, WriteFile and CloseFile.

This sample creates a background thread to watch for COMM
receiver events and posts a notification message to the main
terminal window.  Foreground character processing is written
to the communications port.

Simple TTY character translation is performed and a screen
buffer is implemented for use as the I/O window.

Overlapped file I/O techniques are demonstrated.

How To Use:

The baud rate, data bits, stop bits, parity, port, RTS/CTS
handshaking, DTR/DSR handshaking, and XON/XOFF handshaking
can be changed under the Settings menu item.

Once the communications settings are setup, the Action menu
item can be selected to connect or disconnect the TTY
program.


