/*_ spawn.c   Sun Dec  4 1988 */
/*
 * The routines in this file are called to create a subjob running a command
 * interpreter. This code is a big fat nothing on CP/M-86. You lose.
 */
#include        <stdio.h>
#include        "ed.h"

#if     AMIGA
#define  NEW   1006
#endif

#if     VMS
#define EFN     0                               /* Event flag.          */

#include        <ssdef.h>                       /* Random headers.      */
#include        <stsdef.h>
#include        <descrip.h>
#include        <iodef.h>

extern  int     oldmode[];                      /* In "termio.c"        */
extern  int     newmode[];                      /* In "termio.c"        */
extern  short   iochan;                         /* In "termio.c"        */
#endif

#if     __ZTC__
#include        "capture.h"
#endif

#if     MSDOS || _WIN32
#include        <dos.h>
#endif

#if     BSDUNIX || LINUX
#include        <signal.h>
#endif

/*
 * Create a subjob with a copy of the command intrepreter in it. When the
 * command interpreter exits, mark the screen as garbage so that you do a full
 * repaint. Bound to "C-C". The message at the start in VMS puts out a newline.
 * Under some (unknown) condition, you don't get one free when DCL starts up.
 */
spawncli(f, n)
{
#if     AMIGA
        long newcli;

        newcli = Open("CON:1/1/639/199/MicroEmacs Subprocess", NEW);
        mlwrite("[Starting new CLI]");
        sgarbf = TRUE;
        Execute("", newcli, 0);
        Close(newcli);
        return(TRUE);
#endif

#if     BSDUNIX || LINUX
        register char *cp;
        char    *getenv();
#endif
#if     VMS
        movecursor(term.t_nrow-1, 0);             /* In last line.        */
        mlputs("[Starting DCL]\r\n");
        (*term.t_flush)();                      /* Ignore "ttcol".      */
        sgarbf = TRUE;
        return (sys(NULL));                     /* NULL => DCL.         */
#endif
#if     CPM
        mlwrite("Not in CP/M-86");
#endif
#if     MSDOS || _WIN32
        char *comspec,*getenv();

        movecursor(term.t_nrow-1, 0);             /* Seek to last line.   */
        (*term.t_flush)();
#if __ZTC__
        comspec = getenv("COMSPEC");
        spawnl(0,comspec,"COMMAND.COM",(char *) NULL);
#else
        sys("\\command.com", "");               /* Run CLI.             */
#endif
        sgarbf = TRUE;
        return(TRUE);
#endif
#if     BSDUNIX || LINUX
        movecursor(term.t_nrow-1, 0);             /* Seek to last line.   */
        (*term.t_flush)();
        ttclose();                              /* stty to old settings */
        if ((cp = getenv("SHELL")) != NULL && *cp != '\0')
                system(cp);
        else
                system("exec /bin/sh");
        sgarbf = TRUE;
        sleep(2);
        ttopen();
        return(TRUE);
#endif
}

/*
 * Run a one-liner in a subjob. When the command returns, wait for a single
 * character to be typed, then mark the screen as garbage so a full repaint is
 * done. Bound to "C-X !".
 */
spawn(f, n)
{
        register int    s;
        char            line[NLINE];
#if     AMIGA
        long newcli;

        newcli = Open("CON:1/1/639/199/MicroEmacs Subprocess", NEW);
        line[0] = 0;
        if ((s=mlreply("CLI command: ", line, NLINE)) != TRUE)
                return (s);
        Execute(line,0,newcli);
        Close(newcli);
        while ((*term.t_getchar)() != '\r')     /* Pause.               */
                ;
        sgarbf = TRUE;
        return(TRUE);
#endif
#if     VMS
        line[0] = 0;
        if ((s=mlreply("DCL command: ", line, NLINE)) != TRUE)
                return (s);
        (*term.t_putchar)('\n');                /* Already have '\r'    */
        (*term.t_flush)();
        s = sys(line);                          /* Run the command.     */
        mlputs("\r\n\n[End]");                  /* Pause.               */
        (*term.t_flush)();
        while ((*term.t_getchar)() != '\r')
                ;
        sgarbf = TRUE;
        return (s);
#endif
#if     CPM
        mlwrite("Not in CP/M-86");
        return (FALSE);
#endif
#if     MSDOS || _WIN32
        line[0] = 0;
        if ((s=mlreply("MS-DOS command: ", line, NLINE)) != TRUE)
                return (s);
        system(line);
        while ((*term.t_getchar)() != '\r')     /* Pause.               */
                ;
        sgarbf = TRUE;
        return (TRUE);
#endif
#if     BSDUNIX || LINUX
        line[0] = 0;
        if ((s=mlreply("! ", line, NLINE)) != TRUE)
                return (s);
        (*term.t_putchar)('\n');                /* Already have '\r'    */
        (*term.t_flush)();
        ttclose();                              /* stty to old modes    */
        system(line);
        sleep(2);
        ttopen();
        printf("[End]");                        /* Pause.               */
        (*term.t_flush)();
        while ((s = (*term.t_getchar)()) != '\r' && s != ' ')
                ;
        sgarbf = TRUE;
        return (TRUE);
#endif
}

/*
 * Pipe a one line command into a window
 * Bound to ^X @
 */
spawn_pipe(f, n)
{
    register int    s;  /* return status from CLI */
    register WINDOW *wp;        /* pointer to new window */
    register BUFFER *bp;        /* pointer to buffer to zot */
    char *p;
    static char bname[] = "[DOS]";

#if     AMIGA
    static char filnam[] = "ram:command";
    long newcli;
#else
    static char filnam[] = "DOS.TMP";
#endif
    static char line[NLINE + 2 + sizeof(filnam)]; /* command line sent to shell */

#if     VMS
    mlwrite("Not availible under VMS");
    goto fail;
#endif
#if     CPM
    mlwrite("Not availible under CP/M-86");
    goto fail;
#endif

    /* get the command to pipe in */
    if ((s = mlreply("DOS:", line, NLINE)) != TRUE)
        return s;

#if __ZTC__ && !__OS2__
    {   unsigned count;
        char *buffer;

        movecursor(term.t_nrow - 2, 0);
        if (capture_init(0) == FALSE)
            goto fail;
        system(line);
        sgarbf = TRUE;
        buffer = capture_term(&count);

        if (buffer == NULL ||
            (bp = buffer_find(bname,TRUE,BFTEMP)) == NULL ||
            buffer_clear(bp) == FALSE || /* blow away text in buffer    */
            !(bp == curbp || window_split(FALSE,1) && buffer_switch(bp))
           )
        {   free(buffer);
            goto fail;
        }
        for (p = buffer; count--; p++)
        {
            switch (*p)
            {   case '\n':
                    random_newline(FALSE,1);
                    break;
                case '\r':
                    break;
                default:
                    line_insert(1,*p);
                    break;
            }
        }
        free(buffer);
        return TRUE;
    }
#else
    /* get rid of the command output buffer if it exists */
    if ((bp=buffer_find(bname, FALSE, BFTEMP)) != FALSE) /* if buffer exists */
    {   
        /* If buffer is displayed, try to move it off screen            */
        /* (can't remove an on-screen buffer)                           */
        if (bp->b_nwnd)                 /* if buffer is displayed       */
        {   if (bp == curbp)            /* if it's the current window   */
                window_next(FALSE,1);   /* make another window current  */
            window_only(FALSE, 1);

            if (buffer_remove(bp) != TRUE)
                goto fail;
        }
    }

    /* split the current window to make room for the command output */
    if (window_split(FALSE, 1) == FALSE)
        goto fail;

    p = line + strlen(line);

#if     AMIGA
    newcli = Open("CON:1/1/639/199/MicroEmacs Subprocess", NEW);
    strcat(line, " >");
    strcat(line, filnam);
    Execute(line,0,newcli);
    Close(newcli);
    sgarbf = TRUE;
#endif
#if     MSDOS || _WIN32 || __OS2__
    strcat(line," >");
    strcat(line,filnam);
    movecursor(term.t_nrow - 2, 0);
    system(line);
    sgarbf = TRUE;
    {   FILE *fp;

        if ((fp = fopen(filnam, "r")) == NULL)
            return FALSE;
        else
            fclose(fp);
    }
#endif
#if     BSDUNIX || LINUX
    ttputc('\n');                /* Already have '\r'    */
    ttflush();
    ttclose();                              /* stty to old modes    */
    strcat(line,">");
    strcat(line,filnam);
    system(line);
    ttopen();
    ttflush();
    sgarbf = TRUE;
#endif
    *p = 0;                             /* dump trailing garbage        */

    /* and read the stuff in */
    if (file_readin(filnam) == FALSE)
        return(FALSE);

    /* and get rid of the temporary file */
    unlink(filnam);
    return(TRUE);
#endif

fail:
    return FALSE;
}

/*
 * filter a buffer through an external DOS program
 * Bound to ^X #
 */
spawn_filter(f, n)
{
        register int    s;      /* return status from CLI */
        register BUFFER *bp;    /* pointer to buffer to zot */
        static char filterline[] = " <fltinp >fltout";
        static char line[NLINE + sizeof(filterline) - 1]; /* command line to send to shell */
        char tmpnam[NFILEN];    /* place to store real file name */
        char *p;
        static char bname1[] = "fltinp";

#if     AMIGA
        static char filnam1[] = "ram:fltinp";
        static char filnam2[] = "ram:fltout";
        long newcli;
#else
        static char filnam1[] = "fltinp";
        static char filnam2[] = "fltout";
#endif

        if (curbp->b_flag & BFRDONLY)   /* if buffer is read-only       */
            return FALSE;               /* fail                         */

#if     VMS
        mlwrite("Not availible under VMS");
        return(FALSE);
#endif
#if     CPM
        mlwrite("Not availible under CP/M-86");
        return(FALSE);
#endif

        /* get the filter name and its args */
        if ((s=mlreply("Filter:", line, NLINE)) != TRUE)
                return(s);

        /* setup the proper file names */
        bp = curbp;
        strcpy(tmpnam, bp->b_fname);    /* save the original name */
        strcpy(bp->b_fname, bname1);    /* set it to our new one */

        /* write it out, checking for errors */
        if (writeout(filnam1) != TRUE) {
                mlwrite("[Cannot write filter file]");
                strcpy(bp->b_fname, tmpnam);
                return(FALSE);
        }

        p = line + strlen(line);
#if     AMIGA
        newcli = Open("CON:1/1/639/199/MicroEmacs Subprocess", NEW);
        strcat(line, " <ram:fltinp >ram:fltout");
        Execute(line,0,newcli);
        s = TRUE;
        Close(newcli);
        sgarbf = TRUE;
#endif
#if     MSDOS || _WIN32
        strcat(line," <fltinp >fltout");
        movecursor(term.t_nrow - 2, 0);
        system(line);
        sgarbf = TRUE;
        s = TRUE;
#endif
#if     BSDUNIX || LINUX
        ttputc('\n');                /* Already have '\r'    */
        ttflush();
        ttclose();                              /* stty to old modes    */
        strcat(line,filterline);
        system(line);
        ttopen();
        ttflush();
        sgarbf = TRUE;
        s = TRUE;
#endif
        *p = 0;                 /* chop off appended stuff      */

        /* on failure, escape gracefully */
        if (s != TRUE || ((s = readin(filnam2)) == FALSE)) {
                mlwrite("[Execution failed]");
                strcpy(bp->b_fname, tmpnam);
                goto ret;
        }

        /* reset file name */
        strcpy(bp->b_fname, tmpnam);    /* restore name */
        bp->b_flag |= BFCHG;            /* flag it as changed */
        s = TRUE;

ret:
        /* and get rid of the temporary file */
        unlink(filnam1);
        unlink(filnam2);
        return s;
}


/*
 * Run a command. The "cmd" is a pointer to a command string, or NULL if you
 * want to run a copy of DCL in the subjob (this is how the standard routine
 * LIB$SPAWN works. You have to do wierd stuff with the terminal on the way in
 * and the way out, because DCL does not want the channel to be in raw mode.
 */

#if     VMS
static sys(cmd)
register char   *cmd;
{
        struct  dsc$descriptor  cdsc;
        struct  dsc$descriptor  *cdscp;
        long    status;
        long    substatus;
        long    iosb[2];

        status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
                          oldmode, sizeof(oldmode), 0, 0, 0, 0);
        if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
                return (FALSE);
        cdscp = NULL;                           /* Assume DCL.          */
        if (cmd != NULL) {                      /* Build descriptor.    */
                cdsc.dsc$a_pointer = cmd;
                cdsc.dsc$w_length  = strlen(cmd);
                cdsc.dsc$b_dtype   = DSC$K_DTYPE_T;
                cdsc.dsc$b_class   = DSC$K_CLASS_S;
                cdscp = &cdsc;
        }
        status = LIB$SPAWN(cdscp, 0, 0, 0, 0, 0, &substatus, 0, 0, 0);
        if (status != SS$_NORMAL)
                substatus = status;
        status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
                          newmode, sizeof(newmode), 0, 0, 0, 0);
        if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
                return (FALSE);
        if ((substatus&STS$M_SUCCESS) == 0)     /* Command failed.      */
                return (FALSE);
        return (TRUE);
}
#endif

/*
 * This routine, once again by Bob McNamara, is a C translation of the "system"
 * routine in the MWC-86 run time library. It differs from the "system" routine
 * in that it does not unconditionally append the string ".exe" to the end of
 * the command name. We needed to do this because we want to be able to spawn
 * off "command.com". We really do not understand what it does, but if you don't
 * do it exactly "malloc" starts doing very very strange things.
 */
#if     MSDOS || _WIN32
static sys(cmd, tail)
char    *cmd;
char    *tail;
{
#if MWC_86
        register unsigned n;
        extern   char     *__end;

        n = __end + 15;
        n >>= 4;
        n = ((n + dsreg() + 16) & 0xFFF0) + 16;
        return(execall(cmd, tail, n));
#endif

#if LATTICE
        return forklp(cmd, tail, NULL);
#endif
}
#endif
