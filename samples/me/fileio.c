/*_ fileio.c   Thu Apr 28 1988 */
/*
 * The routines in this file read and write ASCII files from the disk. All of
 * the knowledge about files are here. A better message writing scheme should
 * be used.
 */
#include        <stdio.h>
#include	<errno.h>
#include        "ed.h"

#if BSDUNIX
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pwd.h>
extern int errno;
#endif

#if MSDOS || _WIN32
#include	<sys\stat.h>
#endif

#if LINUX
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pwd.h>
#include        <errno.h>
#endif

FILE    *ffp;                           /* File pointer, all functions. */

/*
 * Open a file for reading.
 */
ffropen(fn)
char    *fn;
{
	tilde_expand(fn);
	errno = 0;
        if ((ffp=fopen(fn, "r")) == NULL)
	    return (errno == ENOENT) ? FIOFNF : FIOERR;
        return (FIOSUC);
}

/*
 * Open a file for writing. Return TRUE if all is well, and FALSE on error
 * (cannot create).
 */
ffwopen(fn)
char    *fn;
{
#if     VMS
        register int    fd;

	tilde_expand(fn);
        if ((fd=creat(fn, 0666, "rfm=var", "rat=cr")) < 0
        || (ffp=fdopen(fd, "w")) == NULL) {
#else
        if ((ffp=fopen(fn, "w")) == NULL) {
#endif
                mlwrite("Cannot open file for writing");
                return (FIOERR);
        }
        return (FIOSUC);
}

/*
 * Close a file. Should look at the status in all systems.
 */
ffclose()
{
#if BSDUNIX || LINUX || MSDOS || _WIN32
        if (fclose(ffp) != FALSE) {
                mlwrite("Error closing file");
                return(FIOERR);
        }
        return(FIOSUC);
#else
        fclose(ffp);
        return (FIOSUC);
#endif
}

/*
 * Write a line to the already opened file. The "buf" points to the buffer,
 * and the "nbuf" is its length, less the free newline. Return the status.
 * Check only at the newline.
 */
ffputline(buf, nbuf)
register char *buf;
{
#if 0
        register char *buftop;

	for (buftop = &buf[nbuf]; buf < buftop; buf++)
                fputc(*buf, ffp);
#else
	fwrite(buf,sizeof(char),nbuf,ffp);
#endif

        fputc('\n', ffp);
        if (ferror(ffp)) {
                mlwrite("Write I/O error");
                return (FIOERR);
        }

        return (FIOSUC);
}

/*
 * Read a line from a file, and store the bytes in a buffer.
 * Complain about lines
 * at the end of the file that don't have a newline present. Check for I/O
 * errors too. Return status.
 * *pbuf gets a pointer to the buffer.
 * *pnbytes gets the # of chars read into the buffer.
 */
ffgetline(pbuf,pnbytes)
char **pbuf;
int *pnbytes;
{
        register int    c;
	register int i;
	static char *buf = NULL;
	static int bufmax = 0;

	i = 0;
        while ((c = fgetc(ffp)) != '\n') {
	        if (c == EOF) {
	                if (ferror(ffp)) {
	                        mlwrite("File read error");
	                        return (FIOERR);
	                }

			if (i != 0)
				break;
	                return (FIOEOF);
	        }
                if (i >= bufmax) {
			if (!buf)
				buf = malloc(80);
			else
				buf = realloc(buf,bufmax + 80);
			if (!buf)
			{	bufmax = 0;
	                        mlwrite("out of memory");
	                        return (FIOERR);
			}
			bufmax += 80;
                }
		buf[i++] = c;
        }

	// If last character is a carriage return, ignore it
	if (i && buf[i - 1] == '\r')
	    i--;			// ignore trailing <CR>'s

	*pnbytes = i;
        *pbuf = buf;
        return (FIOSUC);
}

/***************************
 * Determine if file ffp is read-only.
 */

int ffreadonly(name)
char *name;
{   struct stat buf;

    return stat(name,&buf) == 0 && (buf.st_mode & S_IWRITE) == 0;
}

/*
 * Delete a file
 */
ffunlink( n )
char *n;
{
#if BSDUNIX || LINUX || MSDOS || _WIN32
	tilde_expand( n );
	unlink( n );
#else
	a a b }}} /* error */
#endif
}

/*
 * Rename a file
 */
ffrename( from, to )
char *from, *to;
{
#if BSDUNIX || LINUX
	struct stat buf;
	tilde_expand( from );
	tilde_expand( to );
	if( stat( from, &buf ) != -1
	 && !(buf.st_uid == getuid() && (buf.st_mode & 0200))
	 && !(buf.st_gid == getgid() && (buf.st_mode & 0020))
	 && !(                          (buf.st_mode & 0002)) )
	{
		mlwrite("Cannot open file for writing.");
		/* Note the above message is a lie, but because this	*/
		/* routine is only called by the backup file creation	*/
		/* code, the message will look right to the user.	*/
		return( FIOERR );
	}
#else
	tilde_expand( from );
	tilde_expand( to );
#endif
#if BSDUNIX || LINUX || MSDOS || _WIN32
	rename( from, to );
	return( FIOSUC );
#else
	a a b }}}	/* error */
#endif
}
/*********************
 * Rename a file (by copying it).
 * Returns:
 *	0	success
 *	-1	failure
 */

#if hpux
int rename(from,to)
char *from,*to;
{	int result,system();
	char *command;

	/*assert(from && to);*/
	command = malloc(strlen(from) + strlen(to) + 4 + 1);
	if (!command)
		return -1;
	strcpy(command,"mv ");
	strcat(command,from);
	strcat(command," ");
	strcat(command,to);
	result = system(command);	/* mv from to			*/
	free(command);
	return (result < 0) ? -1 : 0;
}
#endif

/****************************
 * Expand any ~name into the home directory
 * of the user.  On the PC it just replaces 
 * ~name with \name (hack-hack).  On the
 * HP it does??
 */

tilde_expand( str )
char *str;
{
#if BSDUNIX || LINUX
	struct passwd *passwdPtr;
	int i;
	char str2[NFILEN];

	strcpy( str2, str );
	if( *str == '~' )
	{
		for(i=1; str[i] && str[i] != '/'; i++);
		str[i+(str[i]=='\0')] = '\0';
		if(
			( (i != 1)	/* case: ~name/.... */
			 && ((passwdPtr = getpwnam(str+1)) == 0
			  || strlen(str2)+strlen(passwdPtr->pw_dir)+1>=NFILEN)
			)
		  ||	( (i == 1)	/* case: ~/.... */
			 && ((passwdPtr = getpwuid(getuid())) == 0
			  || strlen(str2)+strlen(passwdPtr->pw_dir)+1>=NFILEN)
			)
		)
		{	strcpy(str,str2);
			return;
		}
		strcpy(str,passwdPtr->pw_dir);
		strcat(str,str2+i);
	}
#endif
#if MSDOS || _WIN32
	if( *str == '~' ) *str = '\\';
#endif
}

/*
 * Change the protection on a file <subject> to atch that on file <image>
 */
ffchmod( subject, image )
char *subject,*image;
{
#if BSDUNIX || LINUX
	struct stat buf;
	tilde_expand( subject );
	tilde_expand( image );
	if( stat( image, &buf ) == -1 )
		return( FIOSUC );
		/* Note that this won't work in all cases, but because	*/
		/* this is only called from the backup file creator, it	*/
		/* will work.  UGLY!!					*/
	if( chmod( subject, buf.st_mode ) == -1 )
	{
		mlwrite("Cannot open file for writing.");
		/* Note the above message is a lie, but because this	*/
		/* routine is only called by the backup file creation	*/
		/* code, the message will look right to the user.	*/
		return( FIOERR );
	}
#endif
	return( FIOSUC );
}

