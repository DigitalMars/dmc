 /*_ chsize.c	22 May 1990   Modified by: Bob Stout */
/* DOS chsize() is Copyright (C) 1990 by Robert B. Stout dba MicroFirm	*/
/* All Rights Reserved							*/
/* Written by Bob Stout							*/
/* Windows support added by G. Eric Engstrom Dec 1990			*/
/* Remainder copyright(C) 1990 by Walter Bright				*/
/* All Rights reserved							*/

#include <stdio.h>
#include <io.h>
#include <sys\stat.h>
#include <errno.h>
#include <dos.h>
#include <scrtl.h>

#ifndef __DOS_H_
unsigned	__CLIB dos_getfileattr(const char *, unsigned *);
#endif

#define LPTR M_I86CM || M_I86LM || M_I86VM


#ifdef __OS2__
#include    <bsedos.h>
#endif

/*							    */
/*  chsize						    */
/*							    */
/*  Truncate or extend a file opened under DOS		    */
/*							    */
/*  Arguments: 1 - file handle				    */
/*	      2 - Position for truncation		    */
/*							    */
/*  Returns: 0 if successful, else -1			    */
/*							    */

int __CLIB chsize(int fd, long posn)
{
#ifdef __OS2__
		register unsigned short usRes;
		if (usRes = DosNewSize( fd, posn )) {
	   	 __set_errno (usRes);
		    return(-1);
		}
		else
		    return(0);
#else
    unsigned actual;
    
		if (fflush( &(_iob[fd] )))
	    return(-1);
    if (_dos_seek (fd, posn, 0) == -1)
      return -1;
    if (_dos_write (fd, NULL, 0, &actual) != 0) /*writing 0 sets EOF!*/
      return -1;
	  return 0;
#endif
}
/*	#include <sys\stat.h>					*/
/*	#include <io.h>						*/
/*	int chmod(char *filename, int mode)			*/
/* Change file access mode to that specified by mode.		*/
/* Mode is one of the following values:				*/
/*	S_IREAD			Set file to read-only.		*/
/*	S_IWRITE		Set file to write-only.		*/
/*	S_IREAD | S_IWRITE	Set file to read or write.	*/
/* S_IREAD is ignored under DOS.				*/
/* Return Value							*/
/*	0 means chmod was successful. -1 means it is not,	*/
/*	and errno is set.					*/
/* See Also							*/
/*	access, fstat, creat, stat, open			*/
int __CLIB chmod(const char *filename,int mode)
{
#ifdef __OS2__
		register unsigned short usRes;
		usRes = DosGetFileMode(filename);
		usRes &= ~FA_RDONLY;
		mode |= usRes;
		if (usRes = DosSetFileMode( filename, mode, 0L )) {
	  		__set_errno (usRes);
	  		return(-1);
		else
		   return(0);
#else
#if __INTSIZE == 4
		unsigned att;
		dos_getfileattr(filename,&att);

	 __asm {
		mov	EDX,filename
		mov 	EBX,att
		and	EBX,0xfffe				; and off read_only	
		mov	CL,char ptr mode
		shl	CL,1						;put S_IWRITE bit into Carry bit
		sbb	ECX,ECX
		inc	ECX						;turn into read-only bit in bit 0
		or		ECX,EBX
		mov	EAX,4301h				;set file attribute
		int21h
		jnc	short C2					;no error
		movzx	EAX,AX					;clear high word of EAX
      push    EAX
		call    __set_dos_errno
		mov     EAX,-1
		jmp     C3
C2:	        mov	EAX,0
C3:
    }
#else
	__asm {
#if LPTR
		push	DS
		lds	DX,filename				;get filename
#else 									;LPTR
		mov	DX,filename
#endif 									;LPTR
		mov	AX,4300h
		int21h
		jc	C4
		and	CL,0xfe					; take read only flag off
		mov	BX,CX
		mov	CL,byte ptr mode
		shl	CL,1						;put S_IWRITE bit into Carry bit
		sbb	CX,CX
		inc	CX							;turn into read-only bit in bit 0
		or		CX,BX
		mov	AX,4301h					;set file attribute
		int21h
		jnc	C2							;no error
		
C4:     
#if LPTR
        pop     DS                      ; Restore Original DS prior to __set_dos_errno!!
#endif
        push    AX
		call    __set_dos_errno
		mov     AX,-1
		jmp     C3
C2:   
#if LPTR
        pop    DS     ; Restore DS prior to returning.
#endif
        mov	AX,0
C3:
	}
#endif
#endif
}
