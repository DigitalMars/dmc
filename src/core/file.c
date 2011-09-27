
// Copyright (c) 2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <util.h>

#if _WIN32
#include <windows.h>
#endif

#if _MSDOS
#include <fcntl.h>
#endif

/********************************************
 * Read a file.
 * Output:
 *	*pbuffer is set to a malloc'd buffer with the file contents
 *	*psize is set to file size
 * Returns:
 *	0	success
 *	!=0	error
 */

int file_read(const char *name, void **pbuffer, size_t *psize)
{
#if _MSDOS
    long length;
    size_t size;
    int numread;
    int fd;
    int result = 0;
    char *buf;

    //printf("file_read('%s')\n",name);
    length = filesize(name);
    if (length == -1L)
    {	result = errno;
	goto err1;
    }
#if __INTSIZE == 2
    if ((length + 1) & 0xFFFF0000)   /* error or file too big */
    {	result = ENOMEM;
	goto err1;
    }
#endif
    size = (size_t)length;
    buf = (char *) malloc(size + 1);
    if (!buf)
    {
	//printf("\tmalloc error, errno = %d\n",errno);
	result = ENOMEM;
	goto err1;
    }

    fd = open(name, O_RDONLY | O_BINARY);
    if (fd == -1)
    {
	//printf("\topen error, errno = %d\n",errno);
	result = errno;
	goto err;
    }

    numread = read(fd, buf, size);
    if (numread != size)
    {
	//printf("\tread error, errno = %d\n",errno);
	result = errno;
	goto err2;
    }

    if (close(fd) == -1)
    {
	//printf("\tclose error, errno = %d\n",errno);
	result = errno;
	goto err;
    }

    // Always store a 0 past end of buffer so scanner has a sentinel
    buf[size] = 0;
    goto Lret;

err2:
    close(fd);
err:
    free(buf);

err1:
    size = 0;
    buf = NULL;
Lret:
    *psize = size;
    *pbuffer = buf;
    return result;
#endif
#if linux
    off_t size;
    ssize_t numread;
    int fd;
    struct stat statbuf;
    int result = 0;
    char *buf;

    //printf("file_read('%s')\n",name);
    fd = open(name, O_RDONLY);
    if (fd == -1)
    {
	//printf("\topen error, errno = %d\n",errno);
	result = errno;
	goto err1;
    }

    //printf("\tfile opened\n");
    if (fstat(fd, &statbuf))
    {
	//printf("\tfstat error, errno = %d\n",errno);
	result = errno;
        goto err2;
    }
    size = statbuf.st_size;
    buf = (char *) malloc(size + 3);
    if (!buf)
    {
	//printf("\tmalloc error, errno = %d\n",errno);
	result = ENOMEM;
	goto err2;
    }

    numread = read(fd, buf, size);
    if (numread != size)
    {
	//printf("\tread error, errno = %d\n",errno);
	result = errno;
	goto err2;
    }

    if (close(fd) == -1)
    {
	//printf("\tclose error, errno = %d\n",errno);
	result = errno;
	goto err;
    }

    *psize = size;
    *pbuffer = buf;

    // Always store a 0 past end of buffer so scanner has a sentinel
    buf[size] = 0;
    buf[size + 1] = 0;
    buf[size + 2] = 0;
    return 0;

err2:
    close(fd);
err:
    free(buf);

err1:
    *psize = 0;
    *pbuffer = NULL;
    return result;
#endif
#if _WIN32
    DWORD size;
    DWORD numread;
    HANDLE h;
    char *buf;
    int result = EBADF;

    h = CreateFile((LPTSTR)name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,0);
    if (h == INVALID_HANDLE_VALUE)
	goto err1;

    size = GetFileSize(h,NULL);
    buf = (char *) malloc(size + 3);
    if (!buf)
    {	result = ENOMEM;
	goto err2;
    }

    if (ReadFile(h,buf,size,&numread,NULL) != TRUE)
	goto err2;

    if (numread != size)
	goto err2;

    if (!CloseHandle(h))
	goto err;

    *psize = size;
    *pbuffer = buf;

    // Always store a 0 past end of buffer so scanner has a sentinel
    buf[size] = 0;
    buf[size + 1] = 0;
    buf[size + 2] = 0;
    return 0;

err2:
    CloseHandle(h);
err:
    if (buf)
	free(buf);
err1:
    *psize = 0;
    *pbuffer = NULL;
    return result;
#endif
}

/*********************************************
 * Write a file.
 * Returns:
 *	0	success
 */

int file_write(const char *name, void *buffer, size_t size)
{
#if _MSDOS
    return 1;
#endif
#if linux
    int fd;
    ssize_t numwritten;

    fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0660);
    if (fd == -1)
	goto err;

    numwritten = write(fd, buffer, size);
    if (size != numwritten)
	goto err2;
    
    if (close(fd) == -1)
	goto err;

    return 0;

err2:
    close(fd);
err:
    return 1;
#endif
#if _WIN32
    HANDLE h;
    DWORD numwritten;

    h = CreateFileA(name,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,
	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if (h == INVALID_HANDLE_VALUE)
	goto err;

    if (WriteFile(h,buffer,size,&numwritten,NULL) != TRUE)
	goto err2;

    if (size != numwritten)
	goto err2;
    
    if (!CloseHandle(h))
	goto err;
    return 0;

err2:
    CloseHandle(h);
err:
    return 1;
#endif
}


/*********************************************
 * Append to a file.
 * Returns:
 *	0	success
 */

int file_append(const char *name, void *buffer, size_t size)
{
#if _MSDOS
    return 1;
#endif
#if linux
    // Not implemented yet
    return 1;
#endif
#if _WIN32
    HANDLE h;
    DWORD numwritten;

    h = CreateFileA(name,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,
	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if (h == INVALID_HANDLE_VALUE)
	goto err;

#if 1
    SetFilePointer(h, 0, NULL, FILE_END);
#else // INVALID_SET_FILE_POINTER doesn't seem to have a definition
    if (SetFilePointer(h, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER)
	goto err;
#endif

    if (WriteFile(h,buffer,size,&numwritten,NULL) != TRUE)
	goto err2;

    if (size != numwritten)
	goto err2;
    
    if (!CloseHandle(h))
	goto err;
    return 0;

err2:
    CloseHandle(h);
err:
    return 1;
#endif
}


