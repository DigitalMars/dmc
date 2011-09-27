/* Copyright (C) 1985-1993 by Digital Mars		*/
/* All Rights Reserved				*/
/* July 12, 1994  */
/* $Revision: 1.1.1.1 $ */

#include <tchar.h>
#include <sctchar.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

static fnsplit_sizes;

void _tsplitpath(const _TCHAR *path, _TCHAR *drive, _TCHAR *dir, 
                        _TCHAR *fname, _TCHAR *ext)
{
	int start = 0, end, len;
	_TCHAR *p, *fs, *bs, *extpos, *file;
	
	end = _tcslen(path);
   
/* Extract the drive if there is one */	
	if (path[1] == ':')
	{
      if (drive)
      {
		   _tcsncpy(drive, path, 2);
		   drive[2] = 0;
      }
		start = 2;
	}
   else
   {
       if (drive)
           drive[0] = 0;
   }    

/* If there is a directory specification, extract it.  It is everthing after 
   the drive and before and including the last '\' or '/' */
   fs = _tcsrchr(path, _T('/'));
   bs = _tcsrchr(path, _T('\\'));
   p = __max(fs, bs); /* This assumes that NULL == 0 */
   if (p != NULL)
   {
      len = __min((p - path + 1) - start, (fnsplit_sizes) ? MAXDIR - 1 : _MAX_DIR - 1);
      if (dir)
      {
         _tcsncpy(dir, &path[start], len) ;
         dir[len] = 0;
      }
      start = p - path + 1;
   }
   else 
   {
       if (dir)
           dir[0] = 0;
   }

/* What is left is file, we need to separate the extension from the filename */
	file = (_TCHAR *)&path[start];
	if ((extpos = _tcsrchr(file, _T('.'))) != NULL) /* we have an extension */
	{
	   len = __min(&path[end] - extpos, (fnsplit_sizes) ? MAXEXT - 1 : _MAX_EXT - 1);
      if (ext)
      {
		   _tcsncpy(ext, extpos, len);
		   ext[len] = 0;
      }
		end = start + (extpos - file - 1);
	}
   else
   {
       if (ext)
           ext[0] = 0;
   }

/* Extract the filename */
	if (end >= start) /* we have a filename */
	{
		len = __min(end - start + 1, (fnsplit_sizes) ? MAXFILE : _MAX_FNAME);
      if (fname)
      {
		   memcpy(fname, file, len * sizeof (_TCHAR));
		   fname[len] = 0;
      }
	}
   else
   {
       if (fname)
           fname[0] = 0;
   }
}

int __tfnsplit(const _TCHAR *path, _TCHAR *drive, _TCHAR *dirname,
	    _TCHAR *filename, _TCHAR *extension)
{
   int flags = 0;

   fnsplit_sizes = 1;
	_tsplitpath(path, drive, dirname, filename, extension);
   fnsplit_sizes = 0;

/* check for each componant */
   if (drive && (_tcslen(drive) != 0))
      flags |= DRIVE;
   if (dirname && (_tcslen(dirname) != 0))
      flags |= DIRECTORY;
   if (filename && (_tcslen(filename) != 0))
      flags |= FILENAME;
   if (extension && (_tcslen(extension) != 0))
      flags |= EXTENSION;

/* Check for wildcards in filename or extension */
   if (filename && (_tcschr(filename, _T('*')) != NULL))
      flags |= WILDCARDS;
   if (filename && (_tcschr(filename, _T('?')) != NULL))
      flags |= WILDCARDS;
   if (extension && (_tcschr(extension, _T('*')) != NULL))
      flags |= WILDCARDS;
   if (extension && (_tcschr(extension, _T('?')) != NULL))
      flags |= WILDCARDS;

   return flags;
}
			
	
