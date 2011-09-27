/* Copyright (C) 1985-1993 by Digital Mars		*/
/* All Rights Reserved				*/
/* January 5, 1993  */

#include <bios.h>
#if __INTSIZE != 4
int biosdisk(int cmd, int drive, int head, int track, int sector,
				 int nsects, void *buffer)
{
	struct diskinfo_t dinfo;
	
	dinfo.drive = drive;
	dinfo.head = head;
	dinfo.track = track;
	dinfo.sector = sector;
	dinfo.nsectors = nsects;
	dinfo.buffer = buffer;

	return(_bios_disk(cmd,&dinfo));
}
#endif

long biostime(int cmd, long newtime)
{
	   _bios_timeofday(cmd,&newtime);
 		return(newtime);
}	


