/*_ w32sver.c   Modified by: Walter Bright
 * Copyright (C) 1994 by Digital Mars
 * All rights reserved.
 * Written by Walter Bright
 */

#ifdef _MBCS
#include <tchar.h>
#include <sctchar.h>
#endif
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <scrtl.h>

int GetWin32sVer()
{
   char *path=NULL;
   char verstr[128] = "";
   char intstr[8];
   char *scanpos;
   long Hi, Lo;
   unsigned path_len = 0;
   int ver;
   long version;

   version = GetVersion();
   if (((HIWORD(version) & 0x8000) == 0) || (LOBYTE(LOWORD(version)) >= 0x04))
      ver = 0x010F;
   else
   {
      path = malloc(1);  // Just in case GetSystemDirectory Does not like invalid PTRS.
      if (!path) {
         ver = 0;   // Force a startup failure
         goto end;
      }
      path_len = GetSystemDirectory(path,0);
      path = realloc(path,path_len+1+strlen("\\win32s.ini"));
      if (!path) {
        ver = 0;  // Force a startup failure.
        goto end;
      }
      GetSystemDirectory(path,path_len);
#ifdef _MBCS
      {
       char * bp;
       size_t n = _tcslen (path);
        if (n > 0) {
          bp = _tcsdec (path, path + n);
          if (*bp != _T('\\')) {
            _tcscat (path, _T("\\"));
          }
        }
      }
#else
      if (path[strlen(path)-1] != '\\')
  			strcat(path, "\\");
#endif
      strcat(path, "win32s.ini");
      GetPrivateProfileString("Win32s", "Version", "1.0.0", verstr, 128, path);
      scanpos = verstr;
      Hi = strtol(scanpos, &scanpos, 10);
      scanpos++;                                // to move beyond the '.' char;
      Lo = strtol(scanpos, &scanpos, 10);
      ver = (Hi * 256) + Lo;
   }
   
   if (path)
      free(path);
end:      
   return ver;
}

BOOL CheckWin32sVer()
{
   int ver;

   ver = GetWin32sVer();

   if (ver < 0x010F)
   {
      MessageBox(NULL, "Wrong Win32s version, or damaged installation. "
         "This application requires Win32s version 1.15 to "
         "be loaded on your system.", "Startup Error",
         MB_ICONHAND + MB_OK);
      __faterr("");
      return FALSE;
   }
   else
      return TRUE;
}
