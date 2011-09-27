
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************************************************************
*
*    PROGRAM: DRIVES.C
*
*    PURPOSE: Determines all drives in the system, both local and remote,
*             and their file system type
*
*    FUNCTIONS:
*
*    CheckRM() - verifies that a removeable media drive contains a disk
*    GVI() - gets the volume information for a drive and prints out the
*            file system type
*    UpdateBuffer() - updates string pointer to the next string in an array
*            of strings
*
*    COMMENTS:
*
****************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "drives.h"

#define BUFSIZE 2048

CHAR chBuffer[BUFSIZE];

/****************************************************************************
*
*    FUNCTION: CheckRM(LPTSTR)
*
*    PURPOSE: Verifies that a removeable media drive contains a disk
*
*    COMMENTS:
*
*        This function is called each time a drive type is determined to be
*        removeable (DRIVE_REMOVEABLE).  An attempt is made to open a
*        file in the root directory of the drive.  If the attempt succeeds,
*        then media is present in the drive and subsequent calls to the
*        drive can be safely made.  If the call fails, then there is no media
*        present in the drive and no attempts should be made to access this
*        drive.  The Error Mode is temporarily set to 1 to allow failures
*        to immediately return to the calling program.  This eliminates
*        unwanted dialog boxes that prompt for disks to be placed in the
*        drive.
*
*    INPUT: lpszDriveName - removeable media drive name (ex - "a:")
*
*    OUTPUT: Returns TRUE if media present
*                    FALSE if media is not present
*
****************************************************************************/

BOOL CheckRM(LPTSTR lpszDriveName)
{
  int      iRC;
  LPTSTR   lpszFileName="         ";
  OFSTRUCT ofstruct;
  WORD     wStyle=OF_EXIST;

  SetErrorMode(1);
  strcpy(lpszFileName,lpszDriveName);
  strcat(lpszFileName,".");
  iRC=OpenFile(lpszFileName,&ofstruct,wStyle);
  SetErrorMode(0);
  if (iRC==-1)
    return (FALSE);
  _lclose(iRC);
  return (TRUE);
}

/****************************************************************************
*
*    FUNCTION: GVI(LPTSTR, LPTSTR, DWORD)
*
*    PURPOSE: gets the Volume Information for the specified drive and prints
*             the file system type
*
*    COMMENTS:
*        This function makes a call to GetVolumeInformation.  The file system
*        type is returned in the szSystemName buffer.  The file system type
*        is printed to the screen.
*
*    INPUT: lpszDriveName - pointer to the drive name (ex - "a:")
*           lpszSystemName - pointer to the system name buffer.  This will
*                            be filled in by the GetVolumeInformation call
*                            (e.g. FAT, HPFS)
*           nSystemNameSize - size of the lpszSystemName buffer
*
*    OUTPUT: None
*
****************************************************************************/

VOID GVI(LPTSTR lpszDriveName,LPTSTR lpszSystemName, DWORD nSystemNameSize)
{
  BOOL     bRC;

  lstrcpy( lpszSystemName, "Unknown Media Type" );
  bRC=GetVolumeInformation(lpszDriveName,
                           NULL,
                           0,
                           NULL,
                           NULL,
                           NULL,
                           lpszSystemName,
                           nSystemNameSize);
  printf("File system = %s",lpszSystemName);
}

/****************************************************************************
*
*    FUNCTION: UpdateBuffer(LPTSTR)
*
*    PURPOSE: updates string pointer to the next string in an array
*             of strings
*
*    COMMENTS:
*
*    INPUT: lpszString - pointer to an array of zero terminated strings.
*                        The final string is terminated with a double null.
*
*    OUTPUT: modified pointer that points to the next string in the array
*
****************************************************************************/

LPTSTR UpdateBuffer(LPTSTR lpszString)
{
  return (lpszString+strlen(lpszString)+1);
}

/****************************************************************************
*
*    FUNCTION: main()
*
*    PURPOSE: the main C routine
*
*    COMMENTS:
*        The number of available drives is first determined by a call to
*        GetLogicalDrives.  This provides a bit mask of all logical drives.
*        Next, a call is made to GetLogicalDriveStrings to get the valid
*        drive strings for the system.
*
*        For each logical drive, a call is made to GetDriveType to determine
*        the drive type.  If the logical drive is not in the bit mask that
*        was created with the GetLogicalDrives call, then this drive is
*        bypassed.
*
*        GetVolumeInformation is used to determine the file system
*        for the logical drive.  This information is returned in the
*        lpszFileSystemNameBuffer variable.  If the drive type is
*        remote, a check must be made to see if the drive contains a
*        disk. This is accomplished by opening '.' in the root
*        directory of the remote drive.  The error level is temporarily
*        changed from 0 to 1, to allow any OpenFileErrors to immediately
*        return to the calling function, thus eliminating any unwanted
*        dialog boxes.  If the remote drive contains a disk, then it is
*        safe to proceed with the GetVolumeInformation call.
*
*    INPUT: None
*
*    OUTPUT: None
*
****************************************************************************/

VOID main(VOID)
{
  DWORD     dwDriveMask;
  DWORD     dwRC;
  DWORD     dwTemp;
  DWORD     cbDriveStringBufferSize=BUFSIZE;
  LPTSTR    lpszDriveStringBuffer=chBuffer;
  LPTSTR    lpszRootPathName="?:\\";
  LPTSTR    lpszFileSystemNameBuffer="                              ";
  DWORD     nFileSystemNameSize=sizeof("                              ");

  // check if running on Windows NT, if not, display notice and terminate
  if( GetVersion() & 0x80000000 )
  {
     MessageBox( NULL,
       "This sample application can only be run on Windows NT.\n"
       "This application will now terminate.",
       "Drives",
       MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
       return;
  }

  dwDriveMask=GetLogicalDrives();
  dwRC=GetLogicalDriveStrings(cbDriveStringBufferSize,lpszDriveStringBuffer);
  for (*lpszRootPathName='a';*lpszRootPathName<='z';(*lpszRootPathName)++)
    {
    dwTemp=dwDriveMask & 1;
    dwDriveMask >>= 1;
    if (dwTemp)
      {
      dwRC=GetDriveType(lpszRootPathName);
      printf("The drive type for %s is ",lpszDriveStringBuffer);
      lpszDriveStringBuffer=UpdateBuffer(lpszDriveStringBuffer);
      switch (dwRC)
        {
        case DRIVE_REMOVEABLE :
            printf("removeable. ");
            if (CheckRM(lpszRootPathName))
              GVI(lpszRootPathName,lpszFileSystemNameBuffer,nFileSystemNameSize);
            else
              printf("File system = INDETERMINATE");
            break;
        case DRIVE_FIXED      :
            printf("fixed.      ");
            GVI(lpszRootPathName,lpszFileSystemNameBuffer,nFileSystemNameSize);
            break;
        case DRIVE_REMOTE     :
            printf("remote.     ");
            GVI(lpszRootPathName,lpszFileSystemNameBuffer,nFileSystemNameSize);
            break;
        case DRIVE_CDROM      :
            printf("CD-ROM.     ");
            GVI(lpszRootPathName,lpszFileSystemNameBuffer,nFileSystemNameSize);
            break;
        case DRIVE_RAMDISK    :
            printf("RAM disk.   ");
            GVI(lpszRootPathName,lpszFileSystemNameBuffer,nFileSystemNameSize);
            break;
        default               :
            break;
        }
      printf("\n");
      }
    }
}
