/*_ osfhand.c
 * Copyright (C) 1994 by Digital Mars
 * All rights reserved.
 */


 
 #ifdef __NT__
 #include <scio.h>
 #include <scrtl.h>
 #include <errno.h>  
 #include <fcntl.h>
 #include <io.h>
 #include "semlock.h"

 /*************************************************************************
  * Given a file handle, if the handle is valid return the corresponding
  * operating system handle, return , else return -1 and set errno to EBADF.
  */
 long _get_osfhandle(int fd)
 {

   
   if ((fd < 0 || fd >= _NFILE) || (_osfhnd[fd] == NULL))
	   goto bad;
   

	return (long)_osfhnd[fd];

  bad:
   __set_errno(EBADF);  
   return -1L;
 }


/***************************************************************************
 *  Given an OS File handle, setup a corresponding C Runtime Handle 
 *  and return it, otherwise, it will set errno and return -1.
 */
 int _open_osfhandle(long osfhandle, int flags)
 {
    int errno_val;
    int fd;
    /*  Check osfhandle to be sure it is valid, if it is not set errno to EBADF
        and return -1'
     */
     if (osfhandle == -1) {
           __set_errno(EBADF);
           return -1;
     }           

    LockSemaphore(_semIO)
   /* search table for next available spot */
     for (fd = 3; 1; fd++) {
       if (fd == _NFILE) {
          __set_errno(EMFILE);
          fd = -1;
          goto done;
        }
        if (_osfhnd[fd] == 0)
	      break;
     }
     
     __fhnd_info[fd] = 0;  /* Initialize information */
     _osfhnd[fd] = (HANDLE)osfhandle;    /* Ok store The File handle */
	
	if (flags & _O_APPEND)		/* if append to file		*/
	    __fhnd_info[fd] |= FHND_APPEND;
	
	if (isatty(fd))
	   __fhnd_info[fd] |= FHND_DEVICE;
	   
	if (flags & _O_TEXT)
	   __fhnd_info[fd] |= FHND_TEXT;
	else if ((flags & _O_BINARY) == 0 && (_fmode & _O_BINARY) == 0)
	   __fhnd_info[fd] |= FHND_TEXT;
	
done:
    UnlockSemaphore(_semIO)
    return fd;
     
 }
 

#endif
