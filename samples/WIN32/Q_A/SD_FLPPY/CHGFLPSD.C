////////////////////////////////////////////////////////
//
//  Client.c --
//
//      This program is a command line oriented
//      demonstration of the FloppyLocker service
//      sample, aka floplock.exe
//
//      Copyright 1992-1995, Microsoft Corp.  All Rights Reserved

/****************************************************************************\
*  INCLUDES, DEFINES, TYPEDEFS
\****************************************************************************/
#define STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERR(api) printf("\n%s: Error %d from %s on line %d",  \
    __FILE__, GetLastError(), api, __LINE__);
#define PMSG(msg) printf("\n%s line %d: %s",  \
    __FILE__, __LINE__, msg);

/****************************************************************************\
* GLOBAL VARIABLES
\****************************************************************************/

#define              SZ_NAME_BUF 30
UCHAR   ucMchNameBuf[SZ_NAME_BUF] = "";
LPTSTR  lpszMchName = (LPTSTR)&ucMchNameBuf;
UCHAR   ucOperation;

/****************************************************************************\
* FUNCTION PROTOTYPES
\****************************************************************************/

BOOL CrackArgs(UINT argc, char *argv[]);
VOID DisplayHelp(VOID);

int main(int argc, char *argv[])
{
  char    inbuf[180];
  char    outbuf[180];
  DWORD   bytesRead;
  BOOL    ret;
  #define               SZ_PIPE_NAME_BUF 50
  UCHAR   ucPipeNameBuf[SZ_PIPE_NAME_BUF] = "";
  LPTSTR  lpszPipeName = (LPTSTR)&ucPipeNameBuf;

  // check if Win32s, if so, display notice and terminate
      if( GetVersion() & 0x80000000 )
      {
        MessageBox( NULL,
           "This application cannot run on Windows 3.1 or Windows 95.\n"
           "This application will now terminate.",
           "SD_FLPPY",
           MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
      }

  if (!CrackArgs(argc,argv))
    exit(1);

  strcpy(lpszPipeName,lpszMchName);
  strcat(lpszPipeName,"\\pipe\\sd_flppy");

  outbuf[0] = ucOperation;
  outbuf[1] = '\0';

  ret = CallNamedPipe(lpszPipeName,
                      outbuf,
                      sizeof(outbuf),
                      inbuf,
                      sizeof(inbuf),
                      &bytesRead,
                      NMPWAIT_WAIT_FOREVER);

  if (!ret)
  { if (ERROR_ACCESS_DENIED == GetLastError())
    { printf("\nAccess denied\n");
    }
    else if (ERROR_BAD_NETPATH == GetLastError())
    { printf("\nMachine %s not found\n",lpszMchName);
    }
    else
    { PERR("CallNamedPipe");
    }
    exit(1);
  }

  printf("\n%s %s\n",lpszMchName,inbuf);
}

/****************************************************************************\
*
* FUNCTION: CrackArgs
*
\****************************************************************************/

BOOL CrackArgs(UINT argc, char *argv[])
{
  char *p;

  /**************************************************************************\
  *
  * There must be two arguments
  *
  \**************************************************************************/

  if (argc != 3)
  { DisplayHelp();
    return(FALSE);
  }

  p=argv[1];

  /**************************************************************************\
  *
  * The machine name argument must be 3 to (SZ_NAME_BUF-3) chars long
  *
  \**************************************************************************/

  if ((strlen(p) < 3) || (strlen(p) > (SZ_NAME_BUF-3) ))
  { DisplayHelp();
    return(FALSE);
  }

  /**************************************************************************\
  *
  * The first two chars in the machine name argument must be \
  *
  \**************************************************************************/

  if ('\\' != *p)
  { DisplayHelp();
    return(FALSE);
  }
  if ('\\' != *(p+1))
  { DisplayHelp();
    return(FALSE);
  }

  /**************************************************************************\
  *
  * Set up the machine name
  *
  \**************************************************************************/

  strcpy(lpszMchName,"\\\\");
  strcat(lpszMchName,(p+2));


  p=argv[2];

  /**************************************************************************\
  *
  * The switch argument must be 2 chars long
  *
  \**************************************************************************/

  if (strlen(p) != 2)
  { DisplayHelp();
    return(FALSE);
  }

  /**************************************************************************\
  *
  * The first char in the switch argument must be /
  *
  \**************************************************************************/

  if ('/' != *p)
  { DisplayHelp();
    return(FALSE);
  }

  /**************************************************************************\
  *
  * Chars 2 must be U or L or Q
  *
  \**************************************************************************/


  switch (*(p+1))
  { case 'u':
    case 'U':
      ucOperation = 'U';
      break;
    case 'l':
    case 'L':
      ucOperation = 'L';
      break;
    case 'q':
    case 'Q':
      ucOperation = 'Q';
      break;
    default :
      DisplayHelp();
      return(FALSE);
  }

  return(TRUE);
}

/****************************************************************************\
*
* FUNCTION: DisplayHelp
*
\****************************************************************************/

VOID DisplayHelp(VOID)
{
  printf("\nusage: chgflpsd \\\\machinenam /switch");
  printf("\n                  Values for /switch");
  printf("\n                        /u      Unlock");
  printf("\n                        /l      Lock");
  printf("\n                        /q      Query\n");
  printf("\nFor example to manage DACLs on machine \\\\flip5's floppies");
  printf("\n       chgflpsd \\\\flip5 /u      Gives access to everyone");
  printf("\n       chgflpsd \\\\flip5 /l      Gives access to only local admins");
  printf("\n       chgflpsd \\\\flip5 /q      Query current access on \\\\flip5\n");

  return;
}
