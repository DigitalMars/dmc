/*_ main.c   Wed Apr 12 1989 */
/* $Header$ */
/*
 * This program is in public domain; written by Dave G. Conroy.
 * This file contains the main driving routine, and some keyboard processing
 * code, for the MicroEMACS screen editor.
 *
 * REVISION HISTORY:
 *
 * 1.0  Steve Wilhite, 30-Nov-85
 *      - Removed the old LK201 and VT100 logic. Added code to support the
 *        DEC Rainbow keyboard (which is a LK201 layout) using the the Level
 *        1 Console In ROM INT. See "rainbow.h" for the function key definitions.
 *
 * 2.0  George Jones, 12-Dec-85
 *      - Ported to Amiga.
 *
 * Later versions - Walter Bright, Bjorn Benson
 *	- Ported to linux
 *	- Ported to Win32 (compile with Digital Mars C compiler, www.digitalmars.com)
 *	- Ported to DOS32
 */
#include        <stdio.h>
#include        <time.h>
#include        "ed.h"

#if IBMPC || _WIN32
#include        <disp.h>
#endif

#if     VMS
#include        <ssdef.h>
#define GOOD    (SS$_NORMAL)
#endif

#ifndef GOOD
#define GOOD    0
#endif

int     currow;                         /* Working cursor row           */
int     fillcol;                        /* Current fill column          */
int     thisflag;                       /* Flags, this command          */
int     lastflag;                       /* Flags, last command          */
int     curgoal;                        /* Goal column                  */
int     markcol;                        /* starting column for column cut */
int     mouse;                          /* TRUE if we have a mouse      */
BUFFER  *curbp;                         /* Current buffer               */
WINDOW  *curwp;                         /* Current window               */
BUFFER  *bheadp;                        /* BUFFER listhead              */
WINDOW  *wheadp;                        /* WINDOW listhead              */
BUFFER  *blistp;                        /* Buffer list BUFFER           */
short   kbdm[NKBDM] = {CTLX|')'};       /* Macro                        */
short   *kbdmip;                        /* Input  for above             */
short   *kbdmop;                        /* Output for above             */
char    pat[NPAT];                      /* Pattern                      */
char    insertmode = 1;                 /* insert/overwrite mode        */
static char *progname;                  /* this program name            */

extern  int     basic_nextline();       /* Move to next line            */
extern  int     basic_setmark();        /* Set mark                     */

extern  int     random_setfillcol();    /* Set fill column.             */
extern  int     random_showcpos();      /* Show the cursor position     */
extern  int     random_twiddle();       /* Twiddle characters           */
extern  int     random_tab();           /* Insert tab                   */
extern  int     random_hardtab();       // Set hardware tabs
extern  int     random_newline();       /* Insert CR-LF                 */
extern  int     random_indent();        /* Insert CR-LF, then indent    */
extern  int     random_incindent();     /* increase indentation level   */
extern  int     random_decindent();     /* decrease indentation level   */
extern  int     random_opttab();        /* optimize tabbing in line     */
extern  int     random_openline();      /* Open up a blank line         */
extern  int     random_deblank();       /* Delete blank lines           */
extern  int     random_quote();         /* Insert literal               */
extern  int     random_forwdel();       /* Forward delete               */
extern  int     random_backdel();       /* Backward delete              */
extern  int     random_kill();          /* Kill forward                 */
extern  int     random_yank();          /* Yank back from killbuffer.   */
extern  int     random_undelchar();     /* Undelete a character         */

extern  int     region_togglemode();    /* Toggle column region mode    */
extern  int     region_kill();          /* Kill region.                 */
extern  int     region_copy();          /* Copy region to kill buffer.  */

extern  int     window_next();          /* Move to the next window      */
extern  int     window_prev();          /* Move to the previous window  */
extern  int     window_only();          /* Make current window only one */
extern  int     window_split();         /* Split current window         */
extern  int     window_mvdn();          /* Move window down             */
extern  int     window_mvup();          /* Move window up               */
extern  int     window_enlarge();       /* Enlarge display window       */
extern  int     window_shrink();        /* Shrink window                */
extern  int     window_reposition();    /* Reposition window            */
extern  int     window_refresh();       /* Refresh the screen           */

extern  int     toggleinsert();         /* Toggle insert/overwrite mode */
extern  int     line_overwrite();       /* Write char in overwrite mode */
extern  int     ctrlg();                /* Abort out of things          */
extern  int     quit();                 /* Quit                         */
extern  int     main_saveconfig();      /* Save configuration           */
extern  int     ctlxlp();               /* Begin macro                  */
extern  int     ctlxrp();               /* End macro                    */
extern  int     macrotoggle();          /* Start/End macro		*/
extern  int     ctlxe();                /* Execute macro                */
extern  int     filenext();             /* Edit next file               */
extern  int     fileread();             /* Get a file, read only        */
extern  int     filevisit();            /* Get a file, read write       */
extern  int     filewrite();            /* Write a file                 */
extern  int     fileunmodify();         /* Turn off buffer changed bits */
extern  int     filesave();             /* Save current file            */
extern  int     filename();             /* Adjust file name             */
extern  int     getcol();               /* Get current column           */
extern  int     gotobol();              /* Move to start of line        */
extern  int     forwchar();             /* Move forward by characters   */
extern  int     gotoeol();              /* Move to end of line          */
extern  int     backchar();             /* Move backward by characters  */
extern  int     forwline();             /* Move forward by lines        */
extern  int     backline();             /* Move backward by lines       */
extern  int     forwpage();             /* Move forward by pages        */
extern  int     backpage();             /* Move backward by pages       */
extern  int     gotobob();              /* Move to start of buffer      */
extern  int     gotoeob();              /* Move to end of buffer        */
extern  int     gotoline();             /* Move to line number          */
extern  int     removemark();           /* Remove mark                  */
extern  int     swapmark();             /* Swap "." and mark            */
extern  int     forwsearch();           /* Search forward               */
extern  int     backsearch();           /* Search backwards             */
extern  int     search_paren();         /* Toggle over parentheses      */
extern  int     listbuffers();          /* Display list of buffers      */
extern  int     usebuffer();            /* Switch a window to a buffer  */
extern  int     buffer_next();          /* Switch to next buffer        */
extern  int     killbuffer();           /* Make a buffer go away.       */
extern  int     word_select();          /* Select word                  */
extern  int     word_back();            /* Backup by words              */
extern  int     word_forw();            /* Advance by words             */
extern  int     misc_upper();           /* Upper case word/region       */
extern  int     misc_lower();           /* Lower case word/region       */
extern  int     capword();              /* Initial capitalize word.     */
extern  int     delfword();             /* Delete forward word.         */
extern  int     delbword();             /* Delete backward word.        */
extern  int     spawncli();             /* Run CLI in a subjob.         */
extern  int     spawn();                /* Run a command in a subjob.   */
extern  int     spawn_filter();         /* Filter buffer through program */
extern  int     spawn_pipe();           /* Run program and gather output */
extern  int     quickexit();            /* low keystroke style exit.    */
extern  int     delwind();              /* Delete a window              */
extern  int     filemodify();           /* Write modified files         */
extern  int     normexit();             /* Write modified files and exit*/
extern  int     replacestring();        /* Search and replace           */
extern  int     queryreplacestring();   /* Query search and replace     */
#if IBMPC || _WIN32
extern  int     win32toggle43();	/* Toggle 43 line mode          */
extern  int     ibmpctoggle43();        /* Toggle 43 line mode          */
extern  int     display_norm_fg();
extern  int     display_norm_bg();
extern  int     display_mode_fg();
extern  int     display_mode_bg();
extern  int     display_mark_fg();
extern  int     display_mark_bg();
extern  int     display_eol_bg();
#endif
#if MYIO
extern  int     Dignore();              /* do nothing                   */
extern  int     Dsearch();              /* Search                       */
extern  int     Dsearchagain();         /* Search for the same string   */
extern  int     Ddelline();             /* Delete a line                */
extern  int     Dundelline();           /* Undelete a line              */
extern  int     Ddelword();             /* Delete a word                */
extern  int     Ddelbword();            /* Delete a word (backwards)    */
extern  int     Dundelword();           /* Undelete a word              */
extern  int     Dadvance();             /* Set into advance mode        */
extern  int     Dbackup();              /* Set into backup mode         */
extern  int     Dpause();               /* Pause the program (UNIX only)*/
extern  int     Dinsertdate();          /* File and date stamp          */
extern  int     Dcppcomment();		/* convert to // comment	*/
extern  int     Dinsertfile();          /* Insert a file                */
#endif

/*
 * Command table.
 * This table  is *roughly* in ASCII order, left to right across the
 * characters of the command. This expains the funny location of the
 * control-X commands.
 */

typedef struct  {
        short   k_code;                 /* Key code                     */
        int     (*k_fp)();              /* Routine to handle it         */
}       KEYTAB;

KEYTAB  keytab[] = {
        /* Definitions common to all versions   */
        CTRL('@'),              ctrlg, /*basic_setmark,*/
        CTRL('A'),              gotobol,
        CTRL('B'),              backchar,
        CTRL('C'),              quit,
        CTRL('D'),              random_forwdel,
        CTRL('E'),              gotoeol,
        CTRL('F'),              forwchar,
        CTRL('G'),              ctrlg,
        CTRL('H'),              random_backdel,
        CTRL('I'),              random_tab,
        CTRL('J'),              Ddelline,
        CTRL('K'),              random_kill,
        CTRL('L'),              window_refresh,
        CTRL('M'),              random_newline,
        CTRL('N'),              forwline,
        CTRL('O'),              random_openline,
        CTRL('P'),              backline,
        CTRL('Q'),              random_quote,   /* Often unreachable    */
        CTRL('R'),              backsearch,
        CTRL('S'),              forwsearch,     /* Often unreachable    */
        CTRL('T'),              random_twiddle,
        CTRL('V'),              forwpage,
        CTRL('W'),              search_paren,
        CTRL('Y'),              random_yank,
        0x7F,                   random_backdel,
#if 0
        /* Unused definitions from original microEMACS */
        CTRL('C'),              spawncli,       /* Run CLI in subjob.   */
        CTRL('J'),              random_indent,
        CTRL('W'),              region_kill,
        CTRL('Z'),              quickexit,      /* quick save and exit  */
#endif
#if MSDOS || _WIN32
        CTRL('Z'),              spawncli,      /* Run CLI in subjob.   */
#endif
#if IBMPC || _WIN32
        F2KEY,                  Dsearchagain,
        F3KEY,                  search_paren,
        F4KEY,                  Dsearch,
        F5KEY,                  basic_nextline,
        F6KEY,                  window_next,
        F7KEY,                  basic_setmark,
        F8KEY,                  region_copy,
        F9KEY,                  region_kill,
        F10KEY,                 random_yank,
	F11KEY,			ctlxe,
	F12KEY,			macrotoggle,
        AltF1KEY,               display_norm_bg,
        AltF2KEY,               display_norm_fg,
        AltF3KEY,               display_mode_bg,
        AltF5KEY,               display_mark_fg,
        AltF6KEY,               display_mark_bg,
        AltF4KEY,               display_mode_fg,
        AltF7KEY,               display_eol_bg,
        AltF9KEY,               random_decindent,
        AltF10KEY,              random_incindent,
        ALTB,                   buffer_next,
        ALTC,                   main_saveconfig,
#if IBMPC
        ALTE,                   ibmpctoggle43,
#elif _WIN32
        ALTE,                   win32toggle43,
#endif
	ALTX,			normexit,
        ALTZ,                   spawn_pipe,
        RTKEY,                  forwchar,
        LTKEY,                  backchar,
        DNKEY,                  forwline,
        UPKEY,                  backline,
        InsKEY,                 toggleinsert,
        DelKEY,                 random_forwdel,
        PgUpKEY,                backpage,
        PgDnKEY,                forwpage,
        HOMEKEY,                window_mvup,
        ENDKEY,                 window_mvdn,
        CtrlRTKEY,              word_forw,
        CtrlLFKEY,              word_back,
        CtrlHome,               gotobob,
        CtrlEnd,                gotoeob,
#endif
#if linux
        F2KEY,                  Dsearchagain,
        F3KEY,                  search_paren,
        F4KEY,                  Dsearch,
        F5KEY,                  basic_nextline,
        F6KEY,                  window_next,
        F7KEY,                  basic_setmark,
        F8KEY,                  region_copy,
        F9KEY,                  region_kill,
        F10KEY,                 random_yank,
        AltF9KEY,               random_decindent,
        AltF10KEY,              random_incindent,
        ALTB,                   buffer_next,
        ALTC,                   main_saveconfig,
	ALTX,			normexit,
        ALTZ,                   spawn_pipe,
        RTKEY,                  forwchar,
        LTKEY,                  backchar,
        DNKEY,                  forwline,
        UPKEY,                  backline,
        InsKEY,                 toggleinsert,
        DelKEY,                 random_forwdel,
        PgUpKEY,                backpage,
        PgDnKEY,                forwpage,
        HOMEKEY,                window_mvup,
        ENDKEY,                 window_mvdn,
        CtrlRTKEY,              word_forw,
        CtrlLFKEY,              word_back,
        CtrlHome,               gotobob,
        CtrlEnd,                gotoeob,
#elif TERMCAP
        RTKEY,                  forwchar,
        LTKEY,                  backchar,
        DNKEY,                  forwline,
        UPKEY,                  backline,
        F0KEY,                  random_yank,
        F2KEY,                  gotoeol,
        F4KEY,                  Dadvance,
        F5KEY,                  basic_nextline,
        F6KEY,                  gotoeol,
        F7KEY,                  basic_setmark,
        F8KEY,                  region_copy,
        F9KEY,                  region_kill,
        PF2KEY,                 Ddelline,
        PF3KEY,                 gotobol,
        PF4KEY,                 Dsearchagain,
        FCOMMAKEY,              random_forwdel,
        FMINUSKEY,              delfword,
        FDOTKEY,                basic_setmark,
        CTRL('Z'),              Dpause,
        SCROLLUPKEY,            window_mvup,
        SCROLLDNKEY,            window_mvdn,
        PAGEUPKEY,              backpage,
        PAGEDNKEY,              forwpage,
#endif

        /* Commands with a special key value    */
        0x8001,         spawn_pipe,
        0x8002,         spawn_filter,
        0x8003,         random_showcpos,
        0x8004,         ctlxlp,
#define CMD_ENDMACRO    0x8005
        CMD_ENDMACRO,   ctlxrp,
        0x8006,         random_decindent,
        0x8007,         random_incindent,
        0x8008,         window_only,
        0x8009,         removemark,
        0x800A,         spawn,         /* Run 1 command.       */
        0x800B,         window_split,
        0x800C,         usebuffer,
        0x800D,         delwind,
        0x800E,         ctlxe,
        0x800F,         random_setfillcol,
        0x8010,         killbuffer,
        0x8011,         window_next,
        0x8012,         window_prev,
        0x8013,         random_quote,
        0x8014,         buffer_next,
        0x8015,         window_enlarge,
        0x8016,         listbuffers,
        0x8017,         filename,
        0x8018,         filemodify,
        0x8019,         window_mvdn,
        0x801A,         random_deblank,
        0x801B,         window_mvup,
        0x801C,         fileread,
        0x801D,         filesave,       /* Often unreachable    */
        0x801E,         window_reposition,
        0x801F,         filevisit,
        0x8020,         filewrite,
        0x8021,         swapmark,
        0x8022,         window_shrink,

        0x8023,         delbword,
        0x8024,         random_opttab,
        0x8025,         basic_setmark,
        0x8026,         gotoeob,
        0x8027,         gotobob,
        0x8028,         region_copy,
        0x8029,         region_kill,
        0x802A,         word_back,
        0x802B,         capword,
        0x802C,         delfword,
        0x802D,         word_forw,
        0x802E,         misc_lower,
        0x802F,         queryreplacestring,
        0x8030,         replacestring,
        0x8031,         misc_upper,
        0x8032,         backpage,
        0x8033,         word_select,
        0x8034,         Dadvance,
        0x8035,         Dbackup,
        0x8036,         random_deblank,

        0x8037,         Dinsertdate,
        0x8038,         Dinsertfile,
        0x8039,         gotoline,
        0x803A,         fileunmodify,
        0x803B,         filenext,
        0x803C,         quit,
        0x803D,         normexit,
        0x803E,         Dundelline,
        0x803F,         Dsearch,
        0x8040,         Dundelword,
        0x8041,         random_undelchar,
        0x8042,         random_openline,
        0x8043,         random_kill,
        0x8044,         region_togglemode,
	0x8045,		Dcppcomment,
	0x8046,		random_hardtab,
};

/* Translation table from 2 key sequence to single value        */
short altf_tab[][2] = {
        'B',            0x8016,         /* listbuffers          */
        'D',            0x8037,         /* Dinsertdate          */
        'F',            0x8017,         /* filename             */
        'I',            0x8038,         /* Dinsertfile          */
        'M',            0x8018,         /* filemodify           */
        'N',            0x803B,         /* filenext             */
        'Q',            0x803C,         /* quit                 */
        'R',            0x801C,         /* fileread             */
        'S',            0x801D,         /* filesave             */
	'T',		0x8046,		// random_hardtab
        'U',            0x803A,         /* fileunmodify         */
        'V',            0x801F,         /* filevisit            */
        'W',            0x8020,         /* filewrite            */
        'X',            0x803D,         /* normexit             */
#if IBMPC || _WIN32
        F2KEY,          0x803E,         /* Dundelline           */
        F4KEY,          0x803F,         /* Dsearch              */
        CtrlRTKEY,      0x8040,         /* Dundelword           */
        CtrlLFKEY,      0x8040,         /* Dundelword           */
        DelKEY,         0x8041,         /* random_undelchar     */
        InsKEY,         0x8042,         /* random_openline      */
#endif
#if linux
        F2KEY,          0x803E,         /* Dundelline           */
        F4KEY,          0x803F,         /* Dsearch              */
        CtrlRTKEY,      0x8040,         /* Dundelword           */
        CtrlLFKEY,      0x8040,         /* Dundelword           */
        DelKEY,         0x8041,         /* random_undelchar     */
        InsKEY,         0x8042,         /* random_openline      */
#elif TERMCAP
        F0KEY,          0x8042,         /* random_openline      */
        F4KEY,          0x8026,         /* gotoeob              */
        F5KEY,          0x8027,         /* gotobob              */
        PF2KEY,         0x803E,         /* Dundelline           */
        PF4KEY,         0x803F,         /* Dsearch              */
        FCOMMAKEY,      0x8041,         /* random_undelchar     */
#endif
};

short esc_tab[][2] = {
        '.',            0x8025,         /* basic_setmark        */
        '>',            0x8026,         /* gotoeob              */
        '<',            0x8027,         /* gotobob              */
        '8',            0x8028,         /* region_copy          */
        '9',            0x8029,         /* region_kill          */
        'B',            0x802A,         /* word_back            */
        'C',            0x802B,         /* capword              */
        'D',            0x802C,         /* delfword             */
        'F',            0x802D,         /* word_forw            */
        'H',            0x8023,         /* delbword             */
        'I',            0x8024,         /* random_opttab        */
	'J',		0x803E,		// Dundelline
        'L',            0x802E,         /* misc_lower           */
        'N',            0x8019,         /* window_mvdn          */
        'P',            0x801B,         /* window_mvup          */
        'Q',            0x802F,         /* queryreplacestring   */
        'R',            0x8030,         /* replacestring        */
        'T',            0x8044,         /* region_togglemode    */
        'U',            0x8031,         /* misc_upper           */
        'V',            0x8032,         /* backpage             */
        'W',            0x8033,         /* word_select          */
        'X',            0x8021,         /* swapmark             */
        'Z',            0x8022,         /* window_shrink        */
#if IBMPC || _WIN32
        DNKEY,          0x8034,         /* Dadvance             */
        UPKEY,          0x8035,         /* Dbackup              */
#endif
#if linux
        DNKEY,          0x8034,         /* Dadvance             */
        UPKEY,          0x8035,         /* Dbackup              */
#elif TERMCAP
        F0KEY,          0x8036,         /* random_deblank       */
#endif
};

short ctlx_tab[][2] = {
        '@',            0x8001,		// spawn_pipe
        '#',            0x8002,		// spawn_filter
        '=',            0x8003,		// random_showcpos
        '(',            0x8004,		// ctlxlp
        ')',            0x8005,		// ctlxrp
        '[',            0x8006,		// random_decindent
        ']',            0x8007,		// random_incindent
        '.',            0x8009,		// removemark
        '!',            0x800A,		// spawn
        '1',            0x8008,		// window_only
        '2',            0x800B,		// window_split
        'B',            0x800C,		// usebuffer
        'D',            0x800D,		// delwind
        'E',            0x800E,		// ctlxe
        'F',            0x800F,		// random_setfillcol
        'K',            0x8010,		// killbuffer
        'L',            0x8039,         // gotoline
        'N',            0x8011,		// window_next
        'O',            0x801A,         // random_deblank
        'P',            0x8012,		// window_prev
        'Q',            0x8013,		// random_quote
        'T',            0x801E,         // window_reposition
        'W',            0x8014,		// buffer_next
        'Z',            0x8015,		// window_enlarge
	'/',		0x8045,		// Dcppcomment
};

struct CMDTAB
{   short    ktprefix;  /* prefix key value                     */
    short  (*kt)[2];    /* which translation table              */
    unsigned ktmax;     /* number of entries                    */
} cmdtab[] =
{
    {   CTLX,   ctlx_tab, arraysize(ctlx_tab) },
    {   META,   esc_tab,  arraysize(esc_tab)  },
    {   GOLD,   altf_tab, arraysize(altf_tab) },
};



int gargc,gargi;
char **gargv;
int c;

main(argc, argv)
char    *argv[];
{
        register int    f;
        register int    n;
        char            bname[NBUFN];

#if MOUSE
        mouse = msm_init();                     /* initialize mouse     */
#else
        mouse = FALSE;
#endif
        progname = argv[0];                     /* remember program name */
        strcpy(bname, "main");                  /* Work out the name of */
        if (argc > 1)                           /* the default buffer.  */
                makename(bname, argv[1]);
        vtinit();                               /* Displays.            */
        edinit(bname);                          /* Buffers, windows.    */
        if (argc > 1) {
                update();                       /* You have to update   */
                readin(argv[1]);                /* in case "[New file]" */
        }
        gargi = 2;
        gargc = argc;
        gargv = argv;
        lastflag = 0;                           /* Fake last flags.     */
loop:
        update();                               /* Fix up the screen    */
        c = getkey();
        if (mpresf != FALSE)            /* if there is stuff in message line */
        {   mlerase();                  /* erase it                     */
            update();
        }
        f = FALSE;
        n = 1;
        if (c == CTRL('U'))                     /* ^U, start argument   */
        {   f = TRUE;
            n = getarg();
        }
        if (kbdmip != NULL) {                   /* Save macro strokes.  */
                if (c!=CMD_ENDMACRO && kbdmip>&kbdm[NKBDM-6]) {
                        ctrlg(FALSE, 0);
                        goto loop;
                }
                if (f != FALSE) {
                        *kbdmip++ = CTRL('U');
                        *kbdmip++ = n;
                }
                *kbdmip++ = c;
        }
        execute(0, c, f, n);                       /* Do it.               */
        goto loop;
}

/******************************
 * Get and return numeric argument.
 */

int getarg()
{
    register int n;
    register int mflag;

    n = 4;                          /* with argument of 4 */
    mflag = 0;                      /* that can be discarded. */
    mlwrite("Arg: 4");
    while ((c=getkey()) >='0' && c<='9' || c==CTRL('U') || c=='-'){
        if (c == CTRL('U'))
            n = n*4;
        /*
         * If dash, and start of argument string, set arg.
         * to -1.  Otherwise, insert it.
         */
        else if (c == '-') {
            if (mflag)
                break;
            n = 0;
            mflag = -1;
        }
        /*
         * If first digit entered, replace previous argument
         * with digit and set sign.  Otherwise, append to arg.
         */
        else {
            if (!mflag) {
                n = 0;
                mflag = 1;
            }
            n = 10*n + c - '0';
        }
        mlwrite("Arg: %d", (mflag >=0) ? n : (n ? -n : -1));
    }
    /*
     * Make arguments preceded by a minus sign negative and change
     * the special argument "^U -" to an effective "^U -1".
     */
    if (mflag == -1) {
        if (n == 0)
            n++;
        n = -n;
    }
    return n;
}

/*
 * Initialize all of the buffers and windows. The buffer name is passed down
 * as an argument, because the main routine may have been told to read in a
 * file by default, and we want the buffer name to be right.
 */
edinit(bname)
char    bname[];
{
        register BUFFER *bp;
        register WINDOW *wp;

        bp = buffer_find(bname, TRUE, 0);             /* First buffer         */
        blistp = buffer_find("[List]", TRUE, BFTEMP); /* Buffer list buffer   */
        wp = (WINDOW *) calloc(sizeof(WINDOW),1); /* First window         */
        if (bp==NULL || wp==NULL || blistp==NULL)
        {       vttidy();
                exit(1);
        }
        bp->b_nwnd  = 1;                        /* Displayed.           */
        curbp  = bp;                            /* Make this current    */
        wheadp = wp;
        curwp  = wp;
        wp->w_wndp  = NULL;                     /* Initialize window    */
        wp->w_bufp  = bp;
        wp->w_linep = bp->b_linep;
        wp->w_dotp  = bp->b_linep;
        wp->w_ntrows = term.t_nrow-2;           /* -1 for mode line, -1 for minibuffer  */
        wp->w_flag  = WFMODE|WFHARD;            /* Full.                */
}
        
/*
 * This is the general command execution routine. It handles the fake binding
 * of all the keys to "self-insert". It also clears out the "thisflag" word,
 * and arranges to move it to the "lastflag", so that the next command can
 * look at it. Return the status of command.
 */
execute(prefix, c, f, n)
{
    register KEYTAB *ktp;
    register int    status;

     /* Look in key table.   */
    for (ktp = &keytab[0]; ktp < &keytab[arraysize(keytab)]; ++ktp)
    {   if (ktp->k_code == c)
        {   thisflag = 0;
            status   = (*ktp->k_fp)(f, n);
            lastflag = thisflag;
            return (status);
        }
    }

        /*
         * If a space was typed, fill column is defined, the argument is non-
         * negative, and we are now past fill column, perform word wrap.
         */
        if (c == ' ' && fillcol > 0 && n>=0 &&
            getcol(curwp->w_dotp,curwp->w_doto) > fillcol)
                word_wrap();

        if ((c>=0x20 && c<=0x7E)                /* Self inserting.      */
        ||  (c>=0xA0 && c<=0xFE)) {
                if (n <= 0) {                   /* Fenceposts.          */
                        lastflag = 0;
                        return (n<0 ? FALSE : TRUE);
                }
                thisflag = 0;                   /* For the future.      */
                status   = insertmode ? line_insert(n,c) : line_overwrite(n,c);
                lastflag = thisflag;
                return (status);
        }

        /*
         * Beep if an illegal key is typed
         */
        (*term.t_beep)();
        lastflag = 0;                           /* Fake last flags.     */
        return (FALSE);
}

/*
 * Read in a key.
 * Do the standard keyboard preprocessing. Convert the keys to the internal
 * character set.
 */
getkey()
{
        register int    c;

	ttyield();
#if MOUSE
        while (mouse && !ttkeysininput())
        {   c = mouse_command();
            if (c)
                return c;
	    ttyield();
        }
#endif
        c = (*term.t_getchar)();
        switch (c)
        {
#if MOUSE
            case MENU_BUTTON:
                c = memenu_button();
                break;
#endif
            case META:
            case GOLD:
            case CTLX:
                c = get2nd(c);
                break;
        }

        return (c);
}

/************************
 * Get second key of two key command.
 * Input:
 *      the first key value
 */

static int get2nd(flag)
int flag;
{
    register int c;
    register int i,j;

#if MOUSE
    clock_t starttime;

    starttime = clock();
    while (!ttkeysininput())
        if (clock() > starttime + CLK_TCK)
        {   switch (flag)
            {   case CTLX:
                    return (short) memenu_ctlx(1,disp_cursorrow,disp_cursorcol);
                case GOLD:
                    return (short) memenu_gold(1,disp_cursorrow,disp_cursorcol);
                case META:
                    return (short) memenu_meta(1,disp_cursorrow,disp_cursorcol);
            }
        }
#endif
    c = (*term.t_getchar)();

    /* Treat control characters and lowercase the same as upper case */
    if (c>='a' && c<='z')                   /* Force to upper       */
        c -= 0x20;
    else if (c >= CTRL('A') && c <= CTRL('Z'))
        c += 0x40;

    /* Translate to special keycode     */
    for (i = 0; 1; i++)
        if (cmdtab[i].ktprefix == flag)
            break;
    for (j = 0; 1; j++)
    {
        if (j == cmdtab[i].ktmax)
        {   c = 0;
            break;
        }
        if (cmdtab[i].kt[j][0] == c)
        {   c = cmdtab[i].kt[j][1];
            break;
        }
    }
    return c;
}

/*
 * An even better exit command.  Writes all modified files and then
 * exits.
 */
normexit(f, n)
{
        filemodify(f, n);       /* write all modified files     */
        update();               /* make the screen look nice    */
        quit(f, n);
}

/*
 * Quit command. If an argument, always quit. Otherwise confirm if a buffer
 * has been changed and not written out. Normally bound to "C-X C-C".
 */
quit(f, n)
{
        if (f != FALSE                          /* Argument forces it.  */
        || anycb() == FALSE                     /* All buffers clean.   */
        || (mlyesno("Quit [y/n]? ")))           /* User says it's OK.   */
        {   vttidy();
            exit(GOOD);
        }
        return FALSE;
}

/*
 * Begin a keyboard macro.
 * Error if not at the top level in keyboard processing. Set up variables and
 * return.
 */
ctlxlp(f, n)
{
        if (kbdmip!=NULL || kbdmop!=NULL) {
                mlwrite("Not now");
                return (FALSE);
        }
        mlwrite("[Start macro]");
        kbdmip = &kbdm[0];
        return (TRUE);
}

/*
 * End keyboard macro. Check for the same limit conditions as the above
 * routine. Set up the variables and return to the caller.
 */
ctlxrp(f, n)
{
        if (kbdmip == NULL) {
                mlwrite("Not recording");
                return (FALSE);
        }
        mlwrite("[End macro]");
        kbdmip = NULL;
        return (TRUE);
}

/*
 * If in a macro
 * 	end macro
 * Else
 *	start macro
 */

macrotoggle(f, n)
{
        if (kbdmip)
	    return ctlxrp(f, n);
	else
	    return ctlxlp(f, n);
}

/*
 * Execute a macro.
 * The command argument is the number of times to loop. Quit as soon as a
 * command gets an error. Return TRUE if all ok, else FALSE.
 */
ctlxe(f, n)
{
        register int    c;
        register int    af;
        register int    an;
        register int    s;

        if (kbdmip!=NULL || kbdmop!=NULL) {
                /* Can't execute macro if defining a macro or if        */
                /* in the middle of executing one.                      */
                mlwrite("Not now");
                return (FALSE);
        }
        if (n <= 0)
                /* Execute macro 0 or fewer (!) times   */
                return (TRUE);
        do {
                kbdmop = &kbdm[0];
                do {
                        af = FALSE;
                        an = 1;
                        if ((c = *kbdmop++) == CTRL('U')) {
                                af = TRUE;
                                an = *kbdmop++;
                                c  = *kbdmop++;
                        }
                        s = TRUE;
                } while (c!=CMD_ENDMACRO && (s=execute(0, c, af, an))==TRUE);
                kbdmop = NULL;
        } while (s==TRUE && --n);
        return (s);
}

/*
 * Abort.
 * Beep the beeper. Kill off any keyboard macro, etc., that is in progress.
 * Sometimes called as a routine, to do general aborting of stuff.
 */
ctrlg(f, n)
{
        (*term.t_beep)();
        if (kbdmip != NULL) {
                kbdm[0] = CMD_ENDMACRO;
                kbdmip  = NULL;
        }
        return (ABORT);
}

struct CONFIG config =
{
#if IBMPC || _WIN32
	//0x7400,0x0200,0x0700,0x2400,
	//0x3400,0x7F00,0x7800,0x3B00,
	//0x3400,0x0E00,0x0E00,0x3B00,
	0x3400,0x7000,0x7000,0x3B00,
        ' '/*0xAF*/,
#else
        0,0,0,0,' ',
#endif
};

/********************************
 * Save configuration.
 */

main_saveconfig(f,n)
{
#if IBMPC
    return patchexe(progname,&config,sizeof(config)) == 0;
#else
    return FALSE;
#endif
}

#if IBMPC
#include        <disp.h>
#endif

toggleinsert(f,n)
{
    insertmode ^= 1;
#if IBMPC || _WIN32
    disp_setcursortype(insertmode ? DISP_CURSORBLOCK : DISP_CURSORUL);
#endif
}

#if BSDUNIX
char *memmove(t,f,n)
register char *t,*f;
register int n;
{       char *retval = t;

        if (t <= f)
            while (n--) *t++ = *f++;
        else
            while (n--) t[n] = f[n];
        return retval;
}
#endif


