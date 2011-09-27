/****************************************************************************\
*
*               Microsoft Developer Support
*               Copyright (c) 1992-1995, 1993 Microsoft Corporation
*
* MODULE:       sd_flppy.c
*
*               There is no way to put a DACL on the floppy drives or on the
*                 COM ports with REGEDT32, or using the Control Panel or other
*                 part of the user interface.  And there is no way to use the
*                 Win32 api to put a DACL on the floppy drives or on the COM
*                 ports that survives reboots
*
*               sd_flppy.c, however, does put DACLs on the floppy drives or on
*                 the COM ports that survive logoff/logon, that is, these
*                 DACLs are on the floppy drives or on the COM ports until the
*                 next reboot
*
*               A version of this sample program can be installed as a
*                 service, so that each time the machine boots up the DACLs
*                 are automatically re-applied
*
* PURPOSE:      Show sample code that applies DACLs to floppy drives and COM
*                 ports
*
*               There are possibly as many desired user interfaces to this
*                 sort of functionality as there are people thinking about
*                 this, so it is not a purpose of this sample (or the Win32
*                 service variation of it) to present an incredibly cool user
*                 interface to how the DACLs get applied.  A very simplistic
*                 approach is taken to the user interface.  Anyone who desires
*                 that more complicated DACLs are applied, or desires other
*                 variations in the user interface hopefully will benefit by
*                 being able to use this sample code as a starting point for
*                 their DACL-applying app
*
*               This sample is not a supported utility
*
* TO RUN:       Type sd_flppy to lock the \\.\A: and \\.\B devices
*
*               Putting sd_flppy in a Startup group or logon script could work
*                 for some people
*
\****************************************************************************/

/****************************************************************************\
*  INCLUDES, DEFINES
\****************************************************************************/
#define STRICT
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define PERR(api) printf("\n%s: Error %d from %s on line %d",  \
    __FILE__, GetLastError(), api, __LINE__);
#define PMSG(msg) printf("\n%s line %d: %s",  \
    __FILE__, __LINE__, msg);

/****************************************************************************\
* GLOBAL VARIABLES AND TYPEDEFS
\****************************************************************************/


/****************************************************************************\
* FUNCTION PROTOTYPES
\****************************************************************************/

BOOL WriteSD_ToA_File(PSECURITY_DESCRIPTOR psdAbsoluteSD, LPTSTR lpszFileName);
VOID DisplayHelp(VOID);

UINT main(UINT argc, char *argv[])
{

  #define                               SZ_SD_BUF   100
  #define                               SZ_SID_BUF   75
  #define                               SZ_ACL_BUF  150

  UCHAR                ucAbsSDBuf      [SZ_SD_BUF]  = "";
  UCHAR                ucSIDBuf        [SZ_SID_BUF] = "";
  UCHAR                ucACLBuf        [SZ_ACL_BUF] = "";

  DWORD                dwSID          = SZ_SID_BUF;
  DWORD                dwDACL         = SZ_ACL_BUF;

  PSECURITY_DESCRIPTOR psdAbsoluteSD      = (PSECURITY_DESCRIPTOR)&ucAbsSDBuf;
  PSID                 psidAdministrators = (PSID)&ucSIDBuf;
  PACL                 pNewDACL           = (PACL)&ucACLBuf;


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

  /**************************************************************************\
  *
  * Display help if any parameters passed in
  *
  \**************************************************************************/

  if (argc != 1)
  { DisplayHelp();
    return(1);
  }

  /**************************************************************************\
  *
  * Get SID of local Administrators
  *
  \**************************************************************************/

  {
    #define                               SZ_DOMAIN_BUF 40
    #define                               SZ_PSNU_BUF    8
    UCHAR                ucDomainBuf     [SZ_DOMAIN_BUF] = "";
    UCHAR                ucPSNUBuf       [SZ_PSNU_BUF]   = "";

    DWORD                dwDomainName   = SZ_DOMAIN_BUF;

    LPSTR                lpszDomain     = (LPSTR)&ucDomainBuf;
    PSID_NAME_USE        psnuType       = (PSID_NAME_USE)&ucPSNUBuf;

    if(!LookupAccountName((LPSTR)NULL, /* local name */
                          "Administrators",
                          psidAdministrators,
                          &dwSID,
                          lpszDomain,
                          &dwDomainName,
                          psnuType))
    { PERR("LookupAccountName");
      return(1);
    }

    if (*psnuType != SidTypeAlias)
    { PMSG("LookupAccountName returned the wrong SID type");
      return(1);
    }
  }

  /**************************************************************************\
  *
  * Initialize new DACL
  *
  \**************************************************************************/

  if (!InitializeAcl(pNewDACL,
                     dwDACL,
                     ACL_REVISION2))
  { PERR("InitializeAcl");
    return(1);
  }

  /**************************************************************************\
  *
  * Allow All access to the floppy for local Administrators only
  *
  \**************************************************************************/

  if (!AddAccessAllowedAce(pNewDACL,
                           ACL_REVISION2,
                           FILE_ALL_ACCESS,
                           psidAdministrators))
  { PERR("AddAccessAllowedAce");
    return(1);
  }

  /**************************************************************************\
  *
  * Build SD in absolute format
  *
  \**************************************************************************/

  if (!InitializeSecurityDescriptor(psdAbsoluteSD,
                                    SECURITY_DESCRIPTOR_REVISION))
  { PERR("InitializeSecurityDescriptor");
    return(1);
  }

  /**************************************************************************\
  *
  * Set DACL into SD
  *
  \**************************************************************************/

  if (!SetSecurityDescriptorDacl(psdAbsoluteSD,
                                 TRUE,      // fDaclPresent flag
                                 pNewDACL,
                                 FALSE))    // not a default DACL
  { PERR("SetSecurityDescriptorDacl");
    return(1);
  }

  /**************************************************************************\
  *
  * Check to see that SD is valid before attempting to write it to the file
  *
  \**************************************************************************/

  if (!IsValidSecurityDescriptor(psdAbsoluteSD))
  { PERR("IsValidSecurityDescriptor");
    return(1);
  }

  /**************************************************************************\
  *
  * Write SD to file system - first for A: then B:
  *
  \**************************************************************************/

  if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\A:"))
  { return(1);
  }

  if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\B:"))
  { return(1);
  }

  /**************************************************************************\
  *
  * Works for CDROM drives as well - commented out as this samples is floppy
  *   only
  *
  \**************************************************************************/
/*
  if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\E:"))
  { return(1);
  }
*/
  /**************************************************************************\
  *
  * Works for COM ports as well - commented out as this samples is floppy only
  *
  \**************************************************************************/
/*
  if (!WriteSD_ToA_File(psdAbsoluteSD,"COM1:"))
  { return(1);
  }
*/
  return(0);
}

/****************************************************************************\
*
* FUNCTION: WriteSD_ToA_File
*
\****************************************************************************/

BOOL WriteSD_ToA_File(PSECURITY_DESCRIPTOR psdAbsoluteSD, LPTSTR lpszFileName)
{
  DWORD dwErrorMode;
  BOOL  bStatus;

  /**************************************************************************\
  *
  * SetErrorMode so we don't get the error due to no floppy disk in the floppy
  *   drive
  *
  \**************************************************************************/

  dwErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

  /**************************************************************************\
  *
  * Write SD to file system
  *
  \**************************************************************************/

  bStatus = SetFileSecurity(lpszFileName,
                            (SECURITY_INFORMATION)(DACL_SECURITY_INFORMATION),
                            psdAbsoluteSD);

  /**************************************************************************\
  *
  * SetErrorMode back to its previous value
  *
  \**************************************************************************/

  SetErrorMode(dwErrorMode);

  if (!bStatus)
  { if (ERROR_FILE_NOT_FOUND == GetLastError())
    { printf("\nAttempted to lock %s, but it was not found",lpszFileName);
    }
    else
    { PERR("SetFileSecurity");
      return(FALSE);
    }
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
  printf("\nTo run type SD_FLPPY and no (0) parameters.  Syntax:");
  printf("\n  SD_FLPPY");
  printf("\n          ");
  printf("\nExamples:");
  printf("\n  SD_FLPPY");
  printf("\n            Puts a DACL on A: and B: so that local Administrators");
  printf("\n              have all access and no one else has any access.");
  printf("\n            Since domain Administrators are by default members of");
  printf("\n              local Administrators, this will in many cases give");
  printf("\n              the desired result, but this utility is only one");
  printf("\n              example of the many possible interfaces that may be");
  printf("\n              desired.  Full source to this program is available,");
  printf("\n              so people may write different interfaces.");
  printf("\n            This .exe could be run from a logon script, or from");
  printf("\n              the Startup group.  An alternative approach is to");
  printf("\n              use the other version of this program.  The other");
  printf("\n              version is packaged as a service that applies the");
  printf("\n              DACLs when the machine boots up\n");
}

