/*_ disp.c   Fri May 26 1995   Modified by: Walter Bright */
/*
 * _WIN32 console version of disp package.
 * It compiles into nothing if not a WIN32 device.
 */

#if     _WIN32

#include	<windows.h>
#include        <stdio.h>
#include        <stdlib.h>
#include	<disp.h>

static HANDLE hStdout;			// console output handle
static CONSOLE_SCREEN_BUFFER_INFO csbi;
static CONSOLE_CURSOR_INFO cci;

typedef unsigned short attchar_t;

disp_t disp_state;

WORD attributes;

/*****************************************
 * Handle errors.
 */

static void disp_error(const char *p)
{
    printf("%s\n",p);
    exit(EXIT_FAILURE);
}

/****************************************
 */

void disp_open()
{
    if (disp_inited)
	return;
    disp_inited = 1;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE)
    {	disp_error("getstdhandle\n");
	return;
    }

    if (!GetConsoleScreenBufferInfo(hStdout,&csbi))
    {	disp_error("getconsolescreenbufferinfo\n");
	return;
    }

    disp_numrows = csbi.dwSize.Y;
    disp_numcols = csbi.dwSize.X;
    disp_cursorrow = csbi.dwCursorPosition.Y;
    disp_cursorcol = csbi.dwCursorPosition.X;
    attributes = csbi.wAttributes;
    disp_mono = 0;
    disp_snowycga = 0;
    disp_mode = 3;			// 80*25 color text mode
    disp_inited = 1;
    disp_ega = 1;
    disp_base = 0;
    //disp_offset = 0;
    disp_state.handle = (void *) hStdout;

    if (!GetConsoleCursorInfo(hStdout,&cci))
    {	disp_error("getconsolecursorinfo\n");
    }
}

/****************************************
 */

void disp_close()
{
    if (disp_inited)
    {
	disp_inited = 0;

	// Back to original text attribute
	if (!SetConsoleTextAttribute(hStdout,csbi.wAttributes))
	{
	    disp_error("setconsolecursorinfo\n");
	    // error
	}

	if (!SetConsoleCursorInfo(hStdout,&cci))
	{   disp_error("restore cursor info\n");
	    return;
	}
    }
}

/****************************************
 */

static void win32_setlines(int lines)
{
    COORD coord;
    CONSOLE_CURSOR_INFO cci;

#if 0
    static char format[] = "mode con lines=%d";
    char string[sizeof(format) + sizeof(int) * 3];

    sprintf(string,format,lines);
    system(string);
#elif 1
    {	SMALL_RECT sr;

	if (!GetConsoleScreenBufferInfo(hStdout,&csbi))
	{   disp_error("getconsolescreenbufferinfo\n");
	    return;
	}
	if (csbi.dwSize.Y == lines)
	    return;

	coord.X = 80;
	coord.Y = lines;
	sr.Left = 0;
	sr.Top = 0;
	sr.Right = 79;
	sr.Bottom = lines - 1;
	if (lines == 25)
	{
	    if (!SetConsoleWindowInfo(hStdout,TRUE,&sr))
	    {
		disp_error("disp_reset43 2\n");
		// error
	    }
	    if (!SetConsoleScreenBufferSize(hStdout,coord))
	    {
		disp_error("disp_reset43 1\n");
		// error
	    }
	}
	else
	{
	    if (!SetConsoleScreenBufferSize(hStdout,coord))
	    {
		disp_error("disp_reset43 1\n");
		// error
	    }
	    if (!SetConsoleWindowInfo(hStdout,TRUE,&sr))
	    {
		disp_error("disp_reset43 2\n");
		// error
	    }
	}
    }
#else
    coord.X = 80;
    coord.Y = lines;
    if (!SetConsoleScreenBufferSize(hStdout,coord))
    {
	disp_error("disp_reset43\n");
	// error
    }
#endif

    if (!GetConsoleScreenBufferInfo(hStdout,&csbi))
    {	disp_error("getconsolescreenbufferinfo\n");
	return;
    }

    disp_numrows = csbi.dwSize.Y;
    disp_numcols = csbi.dwSize.X;
    disp_cursorrow = csbi.dwCursorPosition.Y;
    disp_cursorcol = csbi.dwCursorPosition.X;
}

/****************************************
 */

void __cdecl disp_set43()
{
    win32_setlines(50);
}

/****************************************
 */

void __cdecl disp_reset43()
{
    win32_setlines(25);
}

/****************************************
 */

void __cdecl disp_setcursortype(int i)
{   CONSOLE_CURSOR_INFO cci;

    cci.bVisible = TRUE;
    cci.dwSize = i;
    if (!SetConsoleCursorInfo(hStdout,&cci))
    {
	disp_error("setconsolecursorinfo\n");
	// error
    }
}

/****************************************
 */

void __cdecl disp_setattr(int attr)
{
    if (!SetConsoleTextAttribute(hStdout,attr))
    {
	disp_error("setconsolecursorinfo\n");
	// error
    }
}

/***************************************
 */

int __cdecl disp_getattr()
{
    CONSOLE_SCREEN_BUFFER_INFO cs;

    if (!GetConsoleScreenBufferInfo(hStdout,&cs))
    {	disp_error("getconsolescreenbufferinfo\n");
	return;
    }
    return cs.wAttributes;
}

/***************************************
 */

int __cdecl disp_getmode()
{
    return 3;
}

/***************************************
 */

void __cdecl disp_move(int row,int col)
{   COORD coord;

    coord.X = col;
    coord.Y = row;
    if (!SetConsoleCursorPosition(hStdout,coord))
    {
	disp_error("setconsolecursorposition\n");
	// error
    }
    disp_cursorrow = row;
    disp_cursorcol = col;
}

/*******************************
 * Bring everything up-to-date.
 */

void __cdecl disp_flush(void)
{
    CONSOLE_SCREEN_BUFFER_INFO cs;

    if (!GetConsoleScreenBufferInfo(hStdout,&cs))
    {	disp_error("getconsolescreenbufferinfo\n");
	return;
    }

    disp_numrows = cs.dwSize.Y;
    disp_numcols = cs.dwSize.X;
    disp_cursorrow = cs.dwCursorPosition.Y;
    disp_cursorcol = cs.dwCursorPosition.X;
}

/***************************************
 */

void __cdecl disp_eeol(void)
{   DWORD cWritten;
    COORD coord;
    CONSOLE_CURSOR_INFO cci;

    disp_flush();
    coord.Y = disp_cursorrow;
    coord.X = disp_cursorcol;
    if (!FillConsoleOutputCharacter(hStdout,' ',
	disp_numcols - disp_cursorcol,coord,&cWritten))
    {
	disp_error("fillconsoleoutputcharacter\n");
	// error
    }
    if (!FillConsoleOutputAttribute(hStdout,disp_getattr(),
	disp_numcols - disp_cursorcol,coord,&cWritten))
    {
	disp_error("fillconsoleoutputattribute\n");
	// error
    }
}

/***************************************
 */

void __cdecl disp_eeop(void)
{   DWORD cWritten;
    COORD coord;
    CONSOLE_CURSOR_INFO cci;
    int n;

    disp_flush();
    coord.Y = disp_cursorrow;
    coord.X = disp_cursorcol;
    n = disp_numcols - disp_cursorcol;
    n += (disp_numrows - disp_cursorrow - 1) * disp_numcols;
    if (!FillConsoleOutputCharacter(hStdout,' ',n,coord,&cWritten))
    {
	disp_error("fillconsoleoutputcharacter\n");
	// error
    }
    if (!FillConsoleOutputAttribute(hStdout,disp_getattr(),
	n,coord,&cWritten))
    {
	disp_error("fillconsoleoutputattribute\n");
	// error
    }
}

/***************************************
 */

void __cdecl disp_startstand(void)
{
    disp_setattr(0x77 ^ attributes);			// inverse video
}

/***************************************
 */

void __cdecl disp_endstand(void)
{
    disp_setattr(attributes);			// white chars on black background
}

/***************************************
 */

attchar_t __cdecl disp_peekw(int row,int col)
{   attchar_t attchar;
    CHAR_INFO sb;
    COORD sbsize;
    COORD sbcoord;
    SMALL_RECT sdrect;

    sbsize.X = 1;
    sbsize.Y = 1;
    sbcoord.X = 0;
    sbcoord.Y = 0;
    sdrect.Left = col;
    sdrect.Top = row;
    sdrect.Right = col;
    sdrect.Bottom = row;

    if (!ReadConsoleOutput(hStdout,&sb,sbsize,sbcoord,&sdrect))
    {
	disp_error("readconsoleoutput\n");
	// error
    }
    attchar = sb.Char.AsciiChar & 0xFF;
    attchar |= sb.Attributes << 8;
    return attchar;
}

/***************************************
 */

void __cdecl disp_pokew(int row,int col,attchar_t attchar)
{
    CHAR_INFO sb;
    COORD sbsize;
    COORD sbcoord;
    SMALL_RECT sdrect;

    sbsize.X = 1;
    sbsize.Y = 1;
    sbcoord.X = 0;
    sbcoord.Y = 0;
    sdrect.Left = col;
    sdrect.Top = row;
    sdrect.Right = col;
    sdrect.Bottom = row;

    sb.Char.AsciiChar = attchar & 0xFF;
    sb.Attributes = (attchar >> 8) & 0xFF;

    if (!WriteConsoleOutput(hStdout,&sb,sbsize,sbcoord,&sdrect))
    {
	disp_error("writeconsoleoutput\n");
	// error
    }
}

/*********************************
 * Read an area from the display, putting the result in buffer.
 * Where:
 *	buffer	Pointer to array of unsigneds. Into each unsigned
 *		will be placed the corresponding attr/char word.
 *		The words are stored sequentially from left to right
 *		and top to bottom. The number of unsigneds required
 *		is ((brow - trow + 1) * (rcol - lcol + 1)).
 *	trow,lcol	Top left corner
 *	brow,rcol	Bottom right corner
 */

void disp_peekbox(attchar_t *buffer,
		unsigned trow, unsigned lcol,
		unsigned brow, unsigned rcol)
{   unsigned r,c;

    for (r = trow; r <= brow; r++)
	for (c = lcol; c <= rcol; c++)
	    *buffer++ = disp_peekw(r,c);
}

/****************************
 * The inverse of disp_peekbox().
 */

void disp_pokebox(attchar_t *buffer,
		unsigned trow, unsigned lcol,
		unsigned brow, unsigned rcol)
{   unsigned r,c;

    for (r = trow; r <= brow; r++)
	for (c = lcol; c <= rcol; c++)
	    disp_pokew(r,c,*buffer++);
}

/***********************************
 * Fill a box with the specified attribute and character.
 */

void disp_fillbox(unsigned attrchar,
		unsigned trow, unsigned lcol,
		unsigned brow, unsigned rcol)
{
    unsigned x;

    for (; trow <= brow; trow++)
	for (x = lcol; x <= rcol; x++)
	    disp_pokew(trow,x,attrchar);
}

/**********************************
 * Draw a box on the screen.
 * The box must be at least 2 chars wide and 2 chars high.
 *	style		0 = double line
 *			1 = single line
 *			2 = solid
 *			3 = double horizontal, single vertical
 *			4 = single horizontal, double vertical
 *	attr		Attribute byte to use
 *	trow,lcol	Upper left corner
 *	brow,rcol	Lower right corner
 */

void disp_box(int style,int attr,
		unsigned trow, unsigned lcol,
		unsigned brow, unsigned rcol)
{   unsigned i;
    unsigned attchar;
    static struct
    {	unsigned char ul,ur,ll,lr;	/* upper left, etc.		*/
	unsigned char vert,horz;	/* vertical and horizontal segments */
    } *b,boxchar[] =
    {
	{ 0xC9,0xBB,0xC8,0xBC,0xBA,0xCD },	/* double line		*/
	{ 0xDA,0xBF,0xC0,0xD9,0xB3,0xC4 },	/* single line		*/
	{ 0xDB,0xDB,0xDB,0xDB,0xDB,0xDB },	/* solid box		*/
	{ 213, 184, 212, 190, 179, 205  },	/* double hor, single ver */
	{ 214, 183, 211, 189, 186, 196  },	/* single hor, double ver */
    };

    b = &boxchar[style];
    attr *= 256;

    /* Do corners	*/
    disp_pokew(trow,lcol,attr + b->ul);
    disp_pokew(trow,rcol,attr + b->ur);
    disp_pokew(brow,lcol,attr + b->ll);
    disp_pokew(brow,rcol,attr + b->lr);

    /* Do horizontal segments	*/
    attchar = attr + b->horz;
    for (i = lcol + 1; i < rcol; i++)
    {	disp_pokew(trow,i,attchar);
	disp_pokew(brow,i,attchar);
    }

    /* Do vertical segments	*/
    attchar = attr + b->vert;
    for (i = trow + 1; i < brow; i++)
    {	disp_pokew(i,lcol,attchar);
	disp_pokew(i,rcol,attchar);
    }
}

/*************************************
 */

int __cdecl disp_putc(int c)
{   DWORD count;

    if (!WriteConsole(hStdout,&c,1,&count,NULL))
    {
	disp_error("writeconsole\n");
	c = EOF;
    }
    else
    {
	CONSOLE_SCREEN_BUFFER_INFO cs;

	if (!GetConsoleScreenBufferInfo(hStdout,&cs))
	{   disp_error("getconsolescreenbufferinfo\n");
	    return EOF;
	}

	disp_numrows = cs.dwSize.Y;
	disp_numcols = cs.dwSize.X;
	disp_cursorrow = cs.dwCursorPosition.Y;
	disp_cursorcol = cs.dwCursorPosition.X;
    }
    return c;
}

/****************************
 * A printf() that uses the disp package.
 */

static int disp2_putc(int c,void *fp)
{
    return disp_putc(c);
}

int disp_printf(char *format,...)
{   extern int _pformat(int (__cdecl *)(int,void *),void *,const char *,va_list);

    return _pformat((int (__cdecl *)(int,void *))disp2_putc,NULL,format,(va_list)&(format) + sizeof(format));
}

/*********************************
 * Send a string to display.
 */

void disp_puts(const char *p)
{   DWORD count;

    if (!WriteConsole(hStdout,p,strlen(p),&count,NULL))
    {
	disp_error("writeconsole\n");
	// error
    }
}

/****************************************
 * Scroll a region of the display.
 *	lines > 0:	scroll up
 *	lines == 0:	blank window
 *	lines < 0:	scroll down
 */

void disp_scroll(int lines, unsigned ulrow, unsigned ulcol, unsigned lrrow,
                 unsigned lrcol, unsigned attr)
{
  attchar_t *p, buf[256];  /* Should be greater than any screen width. */
  attchar_t val;
  int height = lrrow - ulrow + 1;
  int row, incr;

  val = (disp_getattr() << 8) + ' ';
  if (lines != 0)
  {
    int end_row;

    if(lines > 0) /* Scroll up. */
    {
      row = ulrow;
      end_row = lrrow;
      incr = 1;
    }
    else          /* Scroll down. */
    {
      row = lrrow;
      end_row = ulrow;
      incr = -1;
    }
    row += lines - incr;
    /* Do the scroll. */
    do
    {
      row += incr;
      disp_peekbox(buf,row,ulcol,row,lrcol);
      disp_pokebox(buf,row - lines,ulcol,row - lines,lrcol);
    } while(row != end_row);

    lines *= incr;  /* Make lines > 0. */
    incr *= -1;     /* Change the sign for the attribute fill. */
  }
  else
  {
    lines = lrrow - ulrow + 1;
    row = ulrow;
    incr = 1;
  }

  p = buf + (lrcol - ulcol) + 1;
  do
  {	*p = val;
  } while (p-- > buf);

  /* Fill in the attribute on the blank lines. */
  do
  {
    disp_pokebox(buf,row,ulcol,row,lrcol);
    row += incr;
  } while(--lines);
}

void disp_showcursor()
{
}

void disp_hidecursor()
{
}

#endif /* _WIN32 */
