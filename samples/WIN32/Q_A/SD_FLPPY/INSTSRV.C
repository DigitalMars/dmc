#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORCE_SERVICE_NAME_TO_FLOPPYLOCK (1==0)

#define PERR(api) printf("\n%s: Error %d from %s on line %d",  \
    __FILE__, GetLastError(), api, __LINE__);
#define PMSG(msg) printf("\n%s line %d: %s",  \
    __FILE__, __LINE__, msg);

#define MSG_FOR_ACCESS_DENIED "You aren't authorized to do this - please see your system Administrator"
#define MSG_1_FOR_BAD_PATH "The fully qualified path name to the .exe must be given, and"
#define MSG_2_FOR_BAD_PATH "  the drive letter must be for a fixed disk (e.g., not a net drive)"

SC_HANDLE schService;
SC_HANDLE schSCManager;

VOID DisplayHelp(VOID);

VOID InstallService(LPCTSTR serviceName, LPCTSTR serviceExe)
{
  LPCTSTR lpszBinaryPathName = serviceExe;
  LPTSTR  lpszRootPathName="?:\\";

  if ( (':' != *(lpszBinaryPathName+1)) || ('\\' != *(lpszBinaryPathName+2)) )
  { printf("\n%s",MSG_1_FOR_BAD_PATH);
    printf("\n%s\n",MSG_2_FOR_BAD_PATH);
    return;
  }

  #define DRIVE_TYPE_INDETERMINATE 0
  #define ROOT_DIR_DOESNT_EXIST    1

  *lpszRootPathName = *(lpszBinaryPathName+0) ;

  switch (  GetDriveType(lpszRootPathName)  )
  {
    case DRIVE_FIXED :
    { // OK
      break;
    }
    case  ROOT_DIR_DOESNT_EXIST :
    { printf("\n%s",MSG_1_FOR_BAD_PATH);
      printf("\n  the root directory where the .exe is specified to be must exist, and");
      printf("\n%s\n",MSG_2_FOR_BAD_PATH);
      return;
    }
    case  DRIVE_TYPE_INDETERMINATE :
    case  DRIVE_REMOVABLE          :
    case  DRIVE_REMOTE             :
    case  DRIVE_CDROM              :
    case  DRIVE_RAMDISK            :
    { printf("\n%s",MSG_1_FOR_BAD_PATH);
      printf("\n%s\n",MSG_2_FOR_BAD_PATH);
      return;
    }
    default :
    { printf("\n%s",MSG_1_FOR_BAD_PATH);
      printf("\n%s\n",MSG_2_FOR_BAD_PATH);
      return;
    }
  }

  if (INVALID_HANDLE_VALUE == CreateFile(lpszBinaryPathName,
                                         GENERIC_READ,
                                         FILE_SHARE_READ,
                                         NULL,
                                         OPEN_EXISTING,
                                         FILE_ATTRIBUTE_NORMAL,
                                         NULL))
  { printf("\n%s",MSG_1_FOR_BAD_PATH);
    printf("\n  the file must exist, and");
    printf("\n%s\n",MSG_2_FOR_BAD_PATH);
    return;
  }

  if (FORCE_SERVICE_NAME_TO_FLOPPYLOCK)
  { schService = CreateService(
        schSCManager,               // SCManager database
        serviceName,                // name of service
        serviceName,                // name to display (new parameter after october beta)
        SERVICE_ALL_ACCESS,         // desired access
        SERVICE_WIN32_OWN_PROCESS,  // service type
        SERVICE_AUTO_START,         // start type
        SERVICE_ERROR_NORMAL,       // error control type
        lpszBinaryPathName,         // service's binary
        NULL,                       // no load ordering group
        NULL,                       // no tag identifier
        NULL,                       // no dependencies
        NULL,                       // Local System account
        NULL);                      // null password
  }
  else
  { schService = CreateService(
        schSCManager,               // SCManager database
        serviceName,                // name of service
        serviceName,                // name to display (new parameter after october beta)
        SERVICE_ALL_ACCESS,         // desired access
        SERVICE_WIN32_OWN_PROCESS,  // service type
        SERVICE_AUTO_START,         // start type
        SERVICE_ERROR_NORMAL,       // error control type
        lpszBinaryPathName,         // service's binary
        NULL,                       // no load ordering group
        NULL,                       // no tag identifier
        NULL,                       // no dependencies
        ".\\Administrator",         // Admin account
        "");                        // likely incorrect password
  }

  if (NULL == schService)
  { switch (GetLastError())
    {
      case ERROR_ACCESS_DENIED :
      { printf("\n%s",MSG_FOR_ACCESS_DENIED);
        break;
      }
      case ERROR_SERVICE_EXISTS :
      { printf("\nThe %s service is already installed",serviceName);
        printf("\nRemove it first if you need to re-install a new version\n");
        break;
      }
      default :
      { PERR("CreateService");
      }
    }
    return;
  }
  else
  { if (FORCE_SERVICE_NAME_TO_FLOPPYLOCK)
    { printf("\nThe FloppyLock service was installed successfully.\n");
      printf("\nImportant! Before you can use the FloppyLock service, start Control");
      printf("\nPanel and choose the Services applet.  Then select the FloppyLock");
      printf("\nservice and press Startup.  Select System Account, or else FloppyLock");
      printf("\nwill not be able to start properly.  If you want the service to start");
      printf("\nautomatically, select Automatic.\n");
    }
    else
    { printf("\nCreateService SUCCESS\n");
      printf("\nDon't forget!!! You must now go to the Control Panel and");
      printf("\n  use the Services applet to change the account name and");
      printf("\n  password that this newly installed service will use when");
      printf("\n  it attempts to logon as a service when it starts.");
      printf("\nTo do this: use the Startup button in the Services applet,");
      printf("\n  and (for example) specify the Administrator account and");
      printf("\n  correct password.");
      printf("\nAlso, use the Services applet to ensure this newly installed");
      printf("\n  service starts automatically, since the point of this");
      printf("\n  service is to start automatically and apply the DACLs");
      printf("\n  to the floppy drives prior to a user logging on at");
      printf("\n  the keyboard\n");
    }
  }

  CloseServiceHandle(schService);
}

VOID RemoveService(LPCTSTR serviceName)
{
  {
    #define                                     SZ_ENUM_BUF 4096
    ENUM_SERVICE_STATUS        essServiceStatus[SZ_ENUM_BUF];
    DWORD   dwBufSize = sizeof(essServiceStatus);
    DWORD   dwBytesNeeded      = 0;
    DWORD   dwServicesReturned = 0;
    DWORD   dwResumeHandle     = 0;
    DWORD   dwI                = 0;
    BOOLEAN bFound = FALSE;

    if (!EnumServicesStatus(schSCManager,
                            SERVICE_WIN32,
                            SERVICE_ACTIVE,
                            (LPENUM_SERVICE_STATUS)&essServiceStatus,
                            dwBufSize,
                            &dwBytesNeeded,
                            &dwServicesReturned,
                            &dwResumeHandle))
    { switch (GetLastError())
      {
        case ERROR_ACCESS_DENIED :
        { printf("\n%s",MSG_FOR_ACCESS_DENIED);
          break;
        }
        default :
        { PERR("EnumServicesStatus");
        }
      }
      return;
    }

    for (dwI=0; dwI<dwServicesReturned; dwI++)
    { if(0 == _stricmp(essServiceStatus[dwI].lpServiceName,serviceName))
      { bFound = TRUE;
        break;
      }
    }

    if (bFound)
    { printf("\nThe %s service cannot be removed until it has been stopped.",serviceName);
      printf("\nTo stop the %s service, use the Stop button in the Control",serviceName);
      printf("\n  Panel Services applet\n");
      return;
    }
  }

  schService = OpenService(schSCManager,
                           serviceName,
                           SERVICE_ALL_ACCESS);
  if (NULL == schService)
  { switch (GetLastError())
    {
      case ERROR_ACCESS_DENIED :
      { printf("\n%s",MSG_FOR_ACCESS_DENIED);
        break;
      }
      case ERROR_SERVICE_DOES_NOT_EXIST :
      { printf("\nThe %s service is not installed, so cannot be removed\n",serviceName);
        break;
      }
      default :
      { PERR("OpenService");
      }
    }
    return;
  }

  if (DeleteService(schService))
  { printf("\nDeleteService SUCCESS\n");
  }
  else
  { switch (GetLastError())
    {
      case ERROR_ACCESS_DENIED :
      { printf("\n%s",MSG_FOR_ACCESS_DENIED);
        break;
      }
      default :
      { PERR("DeleteService");
      }
    }
  }
}

int main(int argc, char *argv[])
{
  #define           SZ_NAME_BUF  270  // 256 is max, add a little
  UCHAR   ucNameBuf[SZ_NAME_BUF] = "FloppyLock";
  LPTSTR  lpszServName = (LPTSTR)&ucNameBuf;

  UCHAR   ucExeNBuf[SZ_NAME_BUF] = "";
  LPTSTR  lpszExeName  = (LPTSTR)&ucExeNBuf;

  BOOL    bRemovingService = FALSE;
  char *p;

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

  if ( (argc != 2) && (argc != 3) )
  { DisplayHelp();
    exit(1);
  }

  p=argv[1];
  if (    ('/' == *p)
       || ('-' == *p) )
  { DisplayHelp();
    exit(1);
  }

  if (FORCE_SERVICE_NAME_TO_FLOPPYLOCK)
  { if (argc != 2)
    { DisplayHelp();
      exit(1);
    }
    else
    { strcpy(lpszExeName ,argv[1]);

      bRemovingService = (!stricmp(argv[1], "remove"));
    }
  }
  else
  { if (argc != 3)
    { DisplayHelp();
      exit(1);
    }
    else
    { if (strlen(argv[1]) > 256)
      { printf("\nThe service name cannot be longer than 256 characters\n");
        exit(1);
      }
      strcpy(lpszServName,argv[1]);
      strcpy(lpszExeName ,argv[2]);

      bRemovingService = (!stricmp(argv[2], "remove"));
    }
  }

  schSCManager = OpenSCManager(
                      NULL,                   // machine (NULL == local)
                      NULL,                   // database (NULL == default)
                      SC_MANAGER_ALL_ACCESS); // access required
  if (NULL == schSCManager)
  { switch (GetLastError())
    {
      case ERROR_ACCESS_DENIED :
      { printf("\n%s",MSG_FOR_ACCESS_DENIED);
        break;
      }
      default :
      { PERR("OpenSCManager");
      }
    }
    return (0);
  }

  if (bRemovingService)
  { RemoveService(lpszServName);
  }
  else
  { InstallService(lpszServName,lpszExeName);
  }

  CloseServiceHandle(schSCManager);
}

VOID DisplayHelp(VOID)
{
  if (FORCE_SERVICE_NAME_TO_FLOPPYLOCK)
  { printf("\nInstalls or removes the FloppyLock service.  When the FloppyLock");
    printf("\nservice is started, only members of the Administrators group can use");
    printf("\nthe floppy drives on the computer.\n");
    printf("\nTo install the FloppyLock service, type INSTSRV <path>\n");
    printf("\n    path    Absolute path to the FloppyLock service, floplock.exe.  You must");
    printf("\n            use a fully qualified path and the drive letter must be for a");
    printf("\n            fixed, local drive.  For example, INSTSRV c:\\reskit\\floplock.exe.\n");
    printf("\nTo remove the FloppyLock service, type INSTSRV remove\n");
  }
  else
  { printf("\nUsage: instsrv <service name> <exe location>");
    printf("\n           to install a service, or:");
    printf("\n       instsrv <service name> remove");
    printf("\n           to remove a service\n");
    printf("\nIn the case of this service, a more specific example");
    printf("\n       instsrv FloppyLocker c:\\q_a\\samples\\sd_flppy\\floplock.exe");
    printf("\n           (note fully-qualified path name to .exe)");
    printf("\n       instsrv FloppyLocker remove\n");
  }
  return;
}
