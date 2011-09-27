/*_ ed.h   Tue Mar 29 1988   Modified by: Walter Bright */
/*
 * This file is the general header file for all parts of the MicroEMACS
 * display editor. It contains definitions used by everyone, and it contains
 * the stuff you have to edit to create a version of the editor for a specific
 * operating system and terminal.
 */

#define AMIGA   0                       /* AmigaDOS, Lattice            */
#define ST520   0                       /* ST520, TOS                   */
#define MWC86   0
#define BSDUNIX 0                       /* BSD UN*X or Coherent         */
#define VMS     0                       /* VAX/VMS                      */
#define CPM     0                       /* CP/M-86                      */
#define HITACHI 0                       /* if Hitachi PC                */

#ifndef MSDOS                           /* Already defined for Lattice  */
#define MSDOS   0                       /* MS-DOS                       */
#endif

#ifndef _WIN32
#define _WIN32	0
#endif


/* Display options      */
#ifndef VT52
#define VT52    0                       /* VT52 terminal (Zenith).      */
#endif
#ifndef TERMCAP
#define TERMCAP 0			// Use TERMCAP
#endif
#ifndef NCURSES
#define NCURSES 0			// Use unix ncurses package
#endif
#define IBMPC   MSDOS                   /* IBM PC compatible display    */
#define ANSI    0                       /* using real ANSI terminal     */
#define ANSISYS 0                       /* using ANSI.SYS               */
#define VT100   0                       /* Handle VT100 style keypad.   */
#define LK201   0                       /* Handle LK201 style keypad.   */
#define RAINBOW 0                       /* Use Rainbow fast video.      */

#if __OS2__
#define MOUSE   0
#else
#define MOUSE   (MSDOS || _WIN32)       /* Use Microsoft Mouse          */
#endif

#if !MSDOS && !__OS2__
#define near
#define far
#define pascal
#endif

typedef unsigned short attchar_t;

#if BSDUNIX
#define memcpy(to,from,nbytes)  bcopy((from),(to),(nbytes))
#define memcmp(to,from,nbytes)  bcmp((from),(to),(nbytes))
extern char *memmove();
#endif

#if MSDOS || _WIN32 || VMS
#define filcmp  strcmpl
#else
#define filcmp  strcmp
#endif

#define MYIO  1                       /* Use my keybindings		*/

#define CVMVAS  1                       /* C-V, M-V arg. in screens.    */

#define NFILEN  80                      /* # of bytes, file name        */
#define NBUFN   80 /*16*/               /* # of bytes, buffer name      */
#define NLINE   256                     /* # of bytes, line             */
#define NKBDM   256                     /* # of strokes, keyboard macro */
#define NPAT    180                     /* # of bytes, pattern          */
#define HUGE    1000                    /* Huge number                  */

#define AGRAVE  0x60                    /* M- prefix,   Grave (LK201)   */
#define METACH  0x1B                    /* M- prefix,   Control-[, ESC  */
#define CTMECH  0x1C                    /* C-M- prefix, Control-\       */
#define EXITCH  0x1D                    /* Exit level,  Control-]       */
#define CTRLCH  0x1E                    /* C- prefix,   Control-^       */
#define HELPCH  0x1F                    /* Help key,    Control-_       */

/* Macro to give the number of elements in an array     */
#define arraysize(array)        (sizeof(array) / sizeof(array[0]))

#if MSDOS || _WIN32
#if HITACHI
#define PREFIX  0x80                    /* extended code is coming      */
#define SCAN    0x1000                  /* Scan code flag               */
#define UPKEY   (SCAN|0x78)             /* Up arrow key                 */
#define DNKEY   (SCAN|0x77)             /* Down arrow key               */
#define RTKEY   (SCAN|0x75)             /* Right arrow key              */
#define LTKEY   (SCAN|0x76)             /* Left arrow key               */
#define GOLDKEY F1KEY                   /* GOLD key                     */
#define F1KEY   (SCAN|0xD1)
#define F2KEY   (SCAN|0xD2)
#define F3KEY   (SCAN|0xD3)
#define F4KEY   (SCAN|0xD4)
#define F5KEY   (SCAN|0xD5)
#define F6KEY   (SCAN|0xD6)
#define F7KEY   (SCAN|0xD7)
#define F8KEY   (SCAN|0xD8)
#define F9KEY   (SCAN|0xD9)
#define F10KEY  (SCAN|0xDA)
#define InsKEY  (SCAN|0xB3)
#define CtrlHome        (0x0C)
#define CtrlEnd         (SCAN|0x6D)
#define DelKEY          (SCAN|0xB4)
#define CtrlRTKEY       (SCAN|0x65)
#define CtrlLFKEY       (SCAN|0x66)
#define HOMEKEY         (SCAN|0x68)     /* actually ctrl-^              */
#define ENDKEY          (SCAN|0x67)     /* actyally ctrl-v              */
#define PgUpKEY         (SCAN|0x79)
#define PgDnKEY         (SCAN|0x7A)
#else /* IBM PC keyboard */
#define UPKEY           0x4800          /* Up arrow key                 */
#define DNKEY           0x5000          /* Down arrow key               */
#define RTKEY           0x4D00          /* Right arrow key              */
#define LTKEY           0x4B00          /* Left arrow key               */
#define F1KEY           0x3B00
#define F2KEY           0x3C00
#define F3KEY           0x3D00
#define F4KEY           0x3E00
#define F5KEY           0x3F00
#define F6KEY           0x4000
#define F7KEY           0x4100
#define F8KEY           0x4200
#define F9KEY           0x4300
#define F10KEY          0x4400
#define F11KEY          0x5700
#define F12KEY          0x5800
#define AltF1KEY        0x6800
#define AltF2KEY        0x6900
#define AltF3KEY        0x6A00
#define AltF4KEY        0x6B00
#define AltF5KEY        0x6C00
#define AltF6KEY        0x6D00
#define AltF7KEY        0x6E00
#define AltF8KEY        0x6F00
#define AltF9KEY        0x7000
#define AltF10KEY       0x7100
#define ALTB            0x3000
#define ALTC            0x2E00
#define ALTD            0x2000
#define ALTE            0x1200
#define ALTF            0x2100
#define ALTH            0x2300
#define ALTM            0x3200
#define ALTX            0x2D00
#define ALTZ            0x2C00
#define InsKEY          0x5200
#define CtrlHome        0x7700
#define CtrlEnd         0x7500
#define DelKEY          0x5300
#define CtrlRTKEY       0x7400
#define CtrlLFKEY       0x7300
#define HOMEKEY         0x4700
#define ENDKEY          0x4F00
#define PgUpKEY         0x4900
#define PgDnKEY         0x5100

#define GOLDKEY         ALTF
#define MENU_BUTTON     ALTH

#endif /* IBM PC */
#else /* not MSDOS */
#if linux
#define UPKEY           0x4800          /* Up arrow key                 */
#define DNKEY           0x5000          /* Down arrow key               */
#define RTKEY           0x4D00          /* Right arrow key              */
#define LTKEY           0x4B00          /* Left arrow key               */
#define F1KEY           0x3B00
#define F2KEY           0x3C00
#define F3KEY           0x3D00
#define F4KEY           0x3E00
#define F5KEY           0x3F00
#define F6KEY           0x4000
#define F7KEY           0x4100
#define F8KEY           0x4200
#define F9KEY           0x4300
#define F10KEY          0x4400
#define AltF1KEY        0x6800
#define AltF2KEY        0x6900
#define AltF3KEY        0x6A00
#define AltF4KEY        0x6B00
#define AltF5KEY        0x6C00
#define AltF6KEY        0x6D00
#define AltF7KEY        0x6E00
#define AltF8KEY        0x6F00
#define AltF9KEY        0x7000
#define AltF10KEY       0x7100
#define ALTB            0x3000
#define ALTC            0x2E00
#define ALTD            0x2000
#define ALTE            0x1200
#define ALTF            0x2100
#define ALTH            0x2300
#define ALTM            0x3200
#define ALTX            0x2D00
#define ALTZ            0x2C00
#define InsKEY          0x5200
#define CtrlHome        0x7700
#define CtrlEnd         0x7500
#define DelKEY          0x5300
#define CtrlRTKEY       0x7400
#define CtrlLFKEY       0x7300
#define HOMEKEY         0x4700
#define ENDKEY          0x4F00
#define PgUpKEY         0x4900
#define PgDnKEY         0x5100

#define GOLDKEY         ALTF
#define MENU_BUTTON     ALTH

#else
#define UPKEY   0x81                    /* Up arrow key                 */
#define DNKEY   0x82                    /* Down arrow key               */
#define RTKEY   0x83                    /* Right arrow key              */
#define LTKEY   0x84                    /* Left arrow key               */
#define GOLDKEY 0x85                    /* GOLD key                     */

#if TERMCAP
#define PF2KEY  0x86                    /* PF2 key (Vt100 keypad)       */
#define PF3KEY  0x87                    /* PF3 key (Vt100 keypad)       */
#define PF4KEY  0x88                    /* PF4 key (Vt100 keypad)       */
#define F0KEY   0x89                    /* 0 key (Vt100 keypad)         */
#define F1KEY   0x8A                    /* 1 key (Vt100 keypad)         */
#define F2KEY   0x8B                    /* 2 key (Vt100 keypad)         */
#define F3KEY   0x8C                    /* 3 key (Vt100 keypad)         */
#define F4KEY   0x8D                    /* 4 key (Vt100 keypad)         */
#define F5KEY   0x8E                    /* 5 key (Vt100 keypad)         */
#define F6KEY   0x8F                    /* 6 key (Vt100 keypad)         */
#define F7KEY   0x90                    /* 7 key (Vt100 keypad)         */
#define F8KEY   0x91                    /* 8 key (Vt100 keypad)         */
#define F9KEY   0x92                    /* 9 key (Vt100 keypad)         */
#define FDOTKEY 0x93                    /* DOT key (Vt100 keypad)       */
#define FMINUSKEY 0x94                  /* MINUS key (Vt100 keypad)     */
#define FCOMMAKEY 0x95                  /* COMMA key (Vt100 keypad)     */
#define FENTERKEY 0x96                  /* ENTER key (Vt100 keypad)     */
#define SCROLLUPKEY 0x97
#define SCROLLDNKEY 0x98
#define PAGEUPKEY       0x99
#define PAGEDNKEY       0x9A
#endif /* TCAP */
#endif
#endif /* not MSDOS */

#define CTRL(c) ((c)&0x1F)              /* Control flag, or'ed in       */

#define META    METACH                  /* Meta flag, or'ed in          */
#define CTLX    CTRL('X')               /* ^X flag, or'ed in            */
#define GOLD    GOLDKEY                 /* Another Meta flag, or'ed in  */

#define FALSE   0                       /* False, no, bad, etc.         */
#define TRUE    1                       /* True, yes, good, etc.        */
#define ABORT   2                       /* Death, ^G, abort, etc.       */

#define FIOSUC  0                       /* File I/O, success.           */
#define FIOFNF  1                       /* File I/O, file not found.    */
#define FIOEOF  2                       /* File I/O, end of file.       */
#define FIOERR  3                       /* File I/O, error.             */

#define CFCPCN  0x0001                  /* Last command was C-P, C-N    */
#define CFKILL  0x0002                  /* Last command was a kill      */

/*
 * There is a window structure allocated for every active display window. The
 * windows are kept in a big list, in top to bottom screen order, with the
 * listhead at "wheadp". Each window contains its own values of dot and mark.
 * The flag field contains some bits that are set by commands to guide
 * redisplay; although this is a bit of a compromise in terms of decoupling,
 * the full blown redisplay is just too expensive to run for every input
 * character. 
 */
typedef struct  WINDOW {
        struct  WINDOW *w_wndp;         /* Next window                  */
        struct  BUFFER *w_bufp;         /* Buffer displayed in window   */
        struct  LINE *w_linep;          /* Top line in the window       */
        struct  LINE *w_dotp;           /* Line containing "."          */
        short   w_doto;                 /* Byte offset for "."          */
        struct  LINE *w_markp;          /* Line containing "mark"       */
        short   w_marko;                /* Byte offset for "mark"       */
        char    w_toprow;               /* Origin 0 top row of window   */
        char    w_ntrows;               /* # of rows of text in window  */
        char    w_force;                /* If NZ, forcing row.          */
        char    w_flag;                 /* Flags.                       */
        short   w_startcol;             /* starting column              */
}       WINDOW;

#define WFFORCE 0x01                    /* Window needs forced reframe  */
#define WFMOVE  0x02                    /* Movement from line to line   */
#define WFEDIT  0x04                    /* Editing within a line        */
#define WFHARD  0x08                    /* Better do a full display     */
#define WFMODE  0x10                    /* Update mode line.            */

/* !=0 means marking    */
#define window_marking(wp)      ((wp)->w_markp != NULL)

/*
 * Seperate kbufp[] buffers for cut, word, line and char deletes...
 */
#define DCUTBUF 0
#define DLINEBUF 1
#define DWORDBUF 2
#define DCHARBUF 3
#define DSPECBUF 4      /* maximum number of temp buffers */

/*
 * Text is kept in buffers. A buffer header, described below, exists for every
 * buffer in the system. The buffers are kept in a big list, so that commands
 * that search for a buffer by name can find the buffer header. There is a
 * safe store for the dot and mark in the header, but this is only valid if
 * the buffer is not being displayed (that is, if "b_nwnd" is 0). The text for
 * the buffer is kept in a circularly linked list of lines, with a pointer to
 * the header line in "b_linep".
 */
typedef struct  BUFFER {
        struct  BUFFER *b_bufp;         /* Link to next BUFFER          */
        struct  LINE *b_dotp;           /* Link to "." LINE structure   */
        short   b_doto;                 /* Offset of "." in above LINE  */
        struct  LINE *b_markp;          /* The same as the above two,   */
        short   b_marko;                /* but for the "mark"           */
        struct  LINE *b_linep;          /* Link to the header LINE      */
        char    b_nwnd;                 /* Count of windows on buffer   */
        char    b_flag;                 /* Flags                        */
        char    b_fname[NFILEN];        /* File name                    */
        char    b_bname[NBUFN];         /* Buffer name                  */
}       BUFFER;

#define BFTEMP  0x01                    /* Internal temporary buffer    */
#define BFCHG   0x02                    /* Changed since last write     */
#define BFRDONLY 0x04                   /* Buffer is read only          */
#define BFNOCR  0x08                    /* last line in buffer has no   */
                                        /* trailing CR                  */

/*
 * The starting position of a region, and the size of the region in
 * characters, is kept in a region structure.  Used by the region commands.
 */
typedef struct  {
    struct  LINE *r_linep;      /* Origin LINE address                  */
    unsigned r_offset;          /* Origin LINE offset (not in col mode) */
    unsigned r_size;            /* Length in characters (approx in col mode) */
    unsigned r_nlines;          /* Number of lines                      */
    unsigned r_leftcol;         /* Left column for column cut           */
    unsigned r_rightcol;        /* And right column                     */
} REGION;

/*
 * All text is kept in circularly linked lists of "LINE" structures. These
 * begin at the header line (which is the blank line beyond the end of the
 * buffer). This line is pointed to by the "BUFFER". Each line contains the
 * number of bytes in the line (the "used" size), the size of the text array,
 * and the text. The end of line is not stored as a byte; it's implied. Future
 * additions will include update hints, and a list of marks into the line.
 */
typedef struct  LINE {
        struct  LINE *l_fp;             /* Link to the next line        */
        struct  LINE *l_bp;             /* Link to the previous line    */
        short   l_size;                 /* Allocated size               */
        short   l_used;                 /* Used size                    */
        char    l_text[1];              /* A bunch of characters.       */
}       LINE;

#define lforw(lp)       ((lp)->l_fp)
#define lback(lp)       ((lp)->l_bp)
#define lgetc(lp, n)    ((lp)->l_text[(n)]&0xFF)
#define lputc(lp, n, c) ((lp)->l_text[(n)]=(c))
#define llength(lp)     ((lp)->l_used)

/*
 * The editor communicates with the display using a high level interface. A
 * "TERM" structure holds useful variables, and indirect pointers to routines
 * that do useful operations. The low level get and put routines are here too.
 * This lets a terminal, in addition to having non standard commands, have
 * funny get and put character code too. The calls might get changed to
 * "termp->t_field" style in the future, to make it possible to run more than
 * one terminal type.
 */  
typedef struct  {
        short   t_nrow;                 /* Number of rows.              */
        short   t_ncol;                 /* Number of columns.           */
        int     (*t_open)();            /* Open terminal at the start.  */
        int     (*t_close)();           /* Close terminal at end.       */
        int     (*t_getchar)();         /* Get character from keyboard. */
        int     (*t_putchar)();         /* Put character to display.    */
        int     (*t_flush)();           /* Flush output buffers.        */
        int     (*t_move)();            /* Move the cursor, origin 0.   */
        int     (*t_eeol)();            /* Erase to end of line.        */
        int     (*t_eeop)();            /* Erase to end of page.        */
        int     (*t_beep)();            /* Beep.                        */
        int     (*t_standout)();        /* Start standout mode          */
        int     (*t_standend)();        /* End standout mode            */
        int     (*t_scrollup)();        /* Scroll the screen up         */
        int     (*t_scrolldn)();        /* Scroll the screen down       */
                                        /* Note: scrolling routines do  */
                                        /*  not save cursor position.   */
}       TERM;

extern  char    column_mode;            /* !=0 if in column cut mode    */
extern  int     fillcol;                /* Fill column                  */
extern  int     currow;                 /* Cursor row                   */
extern  int     thisflag;               /* Flags, this command          */
extern  int     lastflag;               /* Flags, last command          */
extern  int     curgoal;                /* Goal for C-P, C-N            */
extern  int     markcol;                /* starting column for column cut */
extern  int     mpresf;                 /* Stuff in message line        */
extern  int     sgarbf;                 /* State of screen unknown      */
extern  WINDOW  *curwp;                 /* Current window               */
extern  BUFFER  *curbp;                 /* Current buffer               */
extern  WINDOW  *wheadp;                /* Head of list of windows      */
extern  BUFFER  *bheadp;                /* Head of list of buffers      */
extern  BUFFER  *blistp;                /* Buffer for C-X C-B           */
extern  short   kbdm[];                 /* Holds kayboard macro data    */
extern  short   *kbdmip;                /* Input pointer for above      */
extern  short   *kbdmop;                /* Output pointer for above     */
extern  char    pat[];                  /* Search pattern               */
extern  TERM    term;                   /*(Terminal information.        */
extern  int     mouse;                  /* TRUE if mouse is installed   */
extern	int	hardtabsize;		// hardware tab size

extern  BUFFER  *buffer_find();               /* Lookup a buffer by name      */
extern  WINDOW  *wpopup();              /* Pop up window creation       */
extern  LINE    *line_realloc();        /* Allocate a line              */

extern void mlwrite(const char *,...);

/********************************
 * All configuration parameters are stored in this struct.
 */

extern struct CONFIG
{
    unsigned modeattr;          /* for mode line                */
    unsigned normattr;          /* for normal text              */
    unsigned eolattr;           /* for end of line              */
    unsigned markattr;          /* for selected text            */
    unsigned tabchar;           /* char to use for tab display  */
} config;

/**************
 * George M. Jones      {cbosgd,ihnp4}!osu-eddie!george
 * Work Phone:          george@ohio-state.csnet
 * (614) 457-8600       CompuServe: 70003,2443
 */

#if __DMC__
#include        <stdlib.h>
#else
extern  char    *getenv();
extern  char    *itoa();
extern  char    *malloc();
extern  char    *calloc();
extern  char    *realloc();
#endif

int display_recalc();

