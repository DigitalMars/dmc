/* Copyright (C) 1985-1993 by Digital Mars		*/
/* All Rights Reserved				*/
/* March 10, 1993  */

#include <dos.h>
#include <errno.h>
#include <io.h>
#include <stdlib.h>
#include <scrtl.h>
#ifdef __NT__
# include <scio.h>
#endif

unsigned _dos_read (int handle, void __far *buffer, unsigned count, unsigned *actual)
{
# ifdef __NT__
    if (ReadFile (_osfhnd[handle], buffer, count, (unsigned long *)actual, NULL))
      return 0;
    return __set_nt_errno ();
# else
    __asm
    {
      mov   bx,word ptr handle
      push  ds
      mov   INTCX,count
      lds   INTDX,buffer
      mov   ah,3fh
      int   21h
      pop   ds
      jc    error
    }
    *actual = _INTAX;
    return 0;
  error:
    return __set_dos_errno (_INTAX);
# endif
}

unsigned _dos_write (int handle, const void __far *buffer, unsigned count, unsigned *actual)
{
# if __NT__
    if (count == 0)
      if (SetEndOfFile (_osfhnd[handle]))
        return 0;
      else;
    else
      if (WriteFile (_osfhnd[handle], buffer, count, (unsigned long *)actual, NULL))
        return 0;
    return __set_nt_errno ();
# else
    __asm
    {
      mov   bx,word ptr handle
      push  ds
      lds   INTDX,buffer
      mov   INTCX,count
      mov   ah,40h
      int   21h
      pop   ds
      jc    error
    }
    *actual = _INTAX;
    return 0;
  error:
    return __set_dos_errno (_INTAX);
# endif
}

	
int _dos_creat(const char *path,unsigned attrib, int *handle)
{
	int fd;

	fd = dos_creat((char *)path,attrib);	
	if (fd == -1)
		return(_doserrno);
	else
	{
		*handle = fd;
		return(0);
	}
}

int _dos_creatnew(const char *path ,unsigned attrib, int *handle)
{
#ifdef __NT__
	int fd;
	
	fd = __nt_creat ((char *)path, attrib, 1);
	if (fd == -1)
		return _doserrno;
	*handle = fd;
	return 0;
#elif DOS386
	__asm {	
		mov	ECX,attrib		;CX = file attribute
		mov	AH,5Bh			;create new file
		mov	EDX,path			;DS:DX -> path
		int21h
		jnc	noerr 			;no error
                push    EAX
                call    __set_dos_errno
		jmp	done
noerr:		mov	*handle,EAX
		xor	EAX,EAX
done:
		}
#else
    __asm {
		mov	CX,attrib		;CX = file attribute
		mov	AH,5Bh			;create new file
  #if __SMALL__ || __MEDIUM__
		mov	DX,path			;DX -> path
		int21h
  #else
		push	DS
		lds	DX,path			;DS:DX -> path
		int21h
		pop	DS
  #endif
		jnc	noerr 			;no error
                push    AX
                call    __set_dos_errno
		jmp	done
noerr:
  #if __LARGE__ || __COMPACT__
		les	DI,handle		
		mov	ES:[DI],AX
  #else
		mov	DI,handle
		mov	[DI],AX
  #endif
		xor	AX,AX
done:
		}
#endif
}


#if __INTSIZE == 2
unsigned _dos_setblock(unsigned size,unsigned seg,unsigned *max)
{
unsigned rtn;

	rtn = dos_setblock(size,seg);
	if (errno == ENOMEM)
	{
		*max = rtn;
		return _doserrno;
	}  
	else
		return(0);
}
#endif

unsigned _dos_open(const char *filename, unsigned flags,int *handle)
{
	int fd;
	fd = dos_open(filename,flags);
	if (fd == -1)
		return _doserrno;
	else
	{
		*handle = fd;
		return(0);
	}
}
