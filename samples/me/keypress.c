#if linux
#include	<stdio.h>
#include	<termios.h>
#include	<unistd.h>
struct  termios  ostate;                 /* saved tty state */
struct  termios  nstate;                 /* values for editor mode */

void main()
{
        /* Adjust output channel        */
        tcgetattr(1, &ostate);                       /* save old state */
        tcgetattr(1, &nstate);                       /* get base of new state */
	cfmakeraw(&nstate);
        tcsetattr(1, TCSADRAIN, &nstate);      /* set mode */

	while (1)
	{       int c;

		c = fgetc(stdin);
		printf("c = x%02x\r\n",c);
		if (c == 3)
		   break;
	}

        tcsetattr(1, TCSADRAIN, &ostate);	// return to original mode
}

#endif
#if _WIN32

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
    HANDLE hStdin;
    DWORD cNumRead, fdwMode, fdwSaveOldMode;
    INPUT_RECORD buf;
    int i;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
    {	printf("getstdhandle\n");
	exit(EXIT_FAILURE);
    }

    if (!GetConsoleMode(hStdin,&fdwSaveOldMode))
    {	printf("getconsolemode\n");
	exit(EXIT_FAILURE);
    }

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(hStdin,fdwMode))
    {	printf("setconsolemode\n");
	exit(EXIT_FAILURE);
    }

    for (i = 0; i < 20; )
    {
	if (!ReadConsoleInput(hStdin,&buf,1,&cNumRead))
	{   printf("ReadConsoleInput\n");
	    break;
	}

	switch (buf.EventType)
	{
	    case KEY_EVENT:
		printf("KEY_EVENT\n");
		printf("\tbKeyDown          = %d\n",buf.Event.KeyEvent.bKeyDown);
		printf("\twRepeatCount      = %d\n",buf.Event.KeyEvent.wRepeatCount);
		printf("\twVirtualKeyCode   = x%x\n",buf.Event.KeyEvent.wVirtualKeyCode);
		printf("\twVirtualScanCode  = x%x\n",buf.Event.KeyEvent.wVirtualScanCode);
		printf("\tAsciiChar         = x%x\n",buf.Event.KeyEvent.uChar.AsciiChar);
		printf("\tdwControlKeyState = x%lx\n",buf.Event.KeyEvent.dwControlKeyState);
		if (buf.Event.KeyEvent.uChar.AsciiChar == 'q')
		    goto Lexit;
		break;
	    case MOUSE_EVENT:
		printf("MOUSE_EVENT\n");
		printf("\tX = %d\n",buf.Event.MouseEvent.dwMousePosition.X);
		printf("\tY = %d\n",buf.Event.MouseEvent.dwMousePosition.Y);
		printf("\tdwButtonState = x%lx\n",buf.Event.MouseEvent.dwButtonState);
		printf("\tdwControlKeyState = x%lx\n",buf.Event.MouseEvent.dwControlKeyState);
		printf("\tdwEventFlags = x%lx\n",buf.Event.MouseEvent.dwEventFlags);
		break;
	    case WINDOW_BUFFER_SIZE_EVENT:
		printf("WINDOW_BUFFER_SIZE_EVENT\n");
		break;
	    case FOCUS_EVENT:
		printf("FOCUS_EVENT\n");
		break;
	    case MENU_EVENT:
		printf("MENU_EVENT\n");
		break;
	    default:
		printf("unknown event type\n");
		goto Lexit;
	}
    }

Lexit:
    if (!SetConsoleMode(hStdin,fdwSaveOldMode))
    {	printf("restore console mode\n");
	exit(EXIT_FAILURE);
    }
}

#endif
#if MSDOS

#include <stdio.h>
//#include <dos.h>

void main()
{   int c;

    while (1)
    {
        c = bdos(7,0) & 0xFF;
        if (c == 0)                             /* if extended code coming */
            c = bdos(7,0) << 8;
	c &= 0xFFFF;
	printf("c = x%04x\n",c);
	if (c == 'q')
	    break;
    }
}

#endif
