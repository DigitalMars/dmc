/* Copyright (C) 1985-1993 by Digital Mars		*/
/* All Rights Reserved				*/
/* February 9, 1993  */

#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <scrtl.h>
#include <errno.h>

int creatnew(const char *path, int mode)
{
int fd;
long size;
unsigned temp;

	size = filesize(path);
	if (size != -1L)
		return(-1);
	else
	{
		fd = open(path,O_CREAT);

		if (fd != -1)
		{
			temp = mode;
			dos_setfileattr(path,temp);
			return(fd);
		}
		else
			return(-1);
	}
}	

int creattemp(char *path, int attrib)
{

int fd;
long size;
unsigned temp;
char temp_path[8];

  strcpy (temp_path, "XXXXXXX");
	if (mktemp(temp_path) == NULL)
	{
	  __set_errno (ENOENT);
	  return -1;
	}
	strcat(path,temp_path);

	if (path)
	{
		size = filesize(path);
		if (size != -1L)
			return(-1);
		else
		{
			fd = open(path,O_CREAT);

			if (fd != -1)
			{
				temp = attrib;
				dos_setfileattr(path,temp);
				return(fd);
			}
			else
		 		return(-1);
		}
	}
}
