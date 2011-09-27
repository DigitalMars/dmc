/*
 *  filesys.h -- interface to file system
 *
 * $Id: filesys.h,v 1.1.1.1 1997/01/02 19:18:03 smarx Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010	FAX: (503) 757-6650
 *
 * Copyright (C) 1991,  1993, 1994.
 * This software is subject to copyright protection under the laws of 
 * the United States and other countries.
 * ALL RIGHTS RESERVED
 *
 ***************************************************************************
 *
 * $Log: filesys.h,v $
 * Revision 1.1.1.1  1997/01/02 19:18:03  smarx
 * cafe12
 *
 * 
 *    Rev 1.5   08 May 1995 18:16:24   Andrew
 * Fixed CR/LF problems that were giving Aruba Fits
 * 
 *    Rev 1.4   15 Dec 1994 13:42:10   Welden
 * o Changes to support change of third filebuf (& fstream) parameter from
 *   attribute bits to file sharing bits.
 * 
 *    Rev 1.3   05 Oct 1994 13:35:00   anderson
 * Added constants O_binary and O_text.
 * 
 *    Rev 1.2   24 Sep 1994 14:23:46   BUSHNELL
 * Changed #if SOLARIS to #if 1 for Open Mode bug fixes
 * 
 *    Rev 1.1   06 Jun 1994  8:52:50   bushnell
 * Changed LF to CR/LF to view files in ARUBA/Brief
 * 
 *    Rev 1.0   20 Apr 1994 17:38:32   thompson                   
 * Initial revision.
 * Revision 1.2  1994/04/14  00:48:52  vriezen
 * Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
 *
 *
 * 
 */



/*
 * The following declarations will need to be adjusted for the
 * operating system in use.
 *
 * We assume a Unix model.  If yours is very different, additional
 * interface functions will be needed, or some rewriting of the
 * implementation which uses these functions and data.
 */


// Assume the following OS routines return this value on error,
// and that this value is NOT ZERO.
// Assume they also set errno when needed.
const int OS_err = -1;	

// open file, return fd, open mode 'how', protection 'prot'
extern "C" int	open(const char *name, int how, int prot);
extern "C" int  sopen (const char *name, int how, int prot, ...);
// 'how' bits, these values for SunOS and SOLARIS
const int O_rdonly = 0x000;	// read only
const int O_wronly = 0x001;	// write only
const int O_rdwr   = 0x002;	// read and write
const int O_append = 0x008;	// append at each write
#if 1	//SOLARIS
const int O_create = 0x100;	// create file if it does not exist
const int O_trunc  = 0x200;	// truncate file length to 0 if exists
const int O_excl   = 0x400;	// error if create and file exists
const int O_text   = 0x4000;  // file is translated
const int O_binary = 0x8000;  // file is not translated
#else
const int O_create = 0x200;	// create file if it does not exist
const int O_trunc  = 0x400;	// truncate file length to 0 if exists
const int O_excl   = 0x800;	// error if create and file exists
#endif

// close file, return 0 on success
extern "C" int	close(int fd);

// read 'count' chars into 'buf', return number read
extern "C" int	read(int fd, void* buf, int count);

// write 'count' chars from 'buf', return number written
extern "C" int	write(int fd, void* buf, int count);

// seek to 'offset' relative to 'whence',
// return new file position, long(OS_err) on error (set errno)
extern "C" long	lseek(int fd, long offset, int whence);
// values for 'whence':
const int L_set = 0;	// from beginning
const int L_cur = 1;	// from current pos
const int L_end = 2;	// from end


// natural size for a file buffer, plus 4 for putback
const int B_size = 1028;

// default protection bits
const int P_default = 0;


// fd numbers for standard in, out, error
const int F_stdin = 0;
const int F_stdout = 1;
const int F_stderr = 2;
