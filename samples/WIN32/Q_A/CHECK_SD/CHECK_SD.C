
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples.
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved.
*       This source code is only intended as a supplement to
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the
*       Microsoft samples programs.
\******************************************************************************/

/****************************************************************************\
*
* MODULE:       check_sd.c
*
*               In the Win32 .hlp file, if you click on Search, goto "Security
*                 Overview", then choose from the list of topics under
*                 Security Overview the sub-topic "Allowing Access", you'll
*                 find the comment
*
*                 Note:  It is fine to write code like this that builds
*                   security descriptors from scratch.  It is, however, a good
*                   practice for people who write code that builds or
*                   manipulates security descriptors to first write code that
*                   explores the default security descriptors that Windows NT
*                   places on objects.  For example, if Windows NT by default
*                   includes in a DACL an ACE granting the Local Logon SID
*                   certain access, it's good to know that, so that a decision
*                   not to grant any access to the Local Logon SID would be a
*                   conscious decision
*
* PURPOSE:      The comment in the .hlp file is accurate, however, for many
*                 people this task of examining the SD is easier if there is
*                 sample code to start from.  So, the purpose of this sample
*                 is to assist people by providing sample code people can
*                 start from as they examine SD(s).  This sample as is
*                 examines the SD on files, and this code can be modified to
*                 examine the SD on other objects
*
*               This sample is not a supported utility
*
* TO RUN:       Type Check_sd to check the SD on the \\.\A:  device
*
*               Type Check_sd d:\a.fil to check the SD on the d:\a.fil file.
*                 In this case d:  must be formatted NTFS, because only NTFS
*                 files have SD(s)
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

typedef enum _KINDS_OF_ACCESSMASKS_DECODED {
    FileAccessMask,
    ProcessAccessMask,
    WindowStationAccessMask,
    RegKeyAccessMask,
    ServiceAccessMask,
    DefaultDaclInAccessTokenAccessMask
    } KINDS_OF_ACCESSMASKS_DECODED, * PKINDS_OF_ACCESSMASKS_DECODED;

// These hold the well-known SIDs

PSID psidNullSid;
PSID psidWorldSid;
PSID psidLocalSid;
PSID psidCreatorOwnerSid;
PSID psidCreatorGroupSid;
PSID psidNtAuthoritySid;
PSID psidDialupSid;
PSID psidNetworkSid;
PSID psidBatchSid;
PSID psidInteractiveSid;
PSID psidLogonIdsSid; // But the X and Y values are bogus at first!!! (See below)
PSID psidServiceSid;
PSID psidLocalSystemSid;
PSID psidBuiltinDomainSid;

/****************************************************************************\
* FUNCTION PROTOTYPES
\****************************************************************************/

VOID ExamineAccessToken(HANDLE hAccessToken);
BOOL ExamineSD    (PSECURITY_DESCRIPTOR psdSD,
                   KINDS_OF_ACCESSMASKS_DECODED kamKindOfMask);
BOOL ExamineACL   (PACL paclACL,              LPTSTR lpszOldIndent,
                   KINDS_OF_ACCESSMASKS_DECODED kamKindOfMask);
VOID ExamineMask  (ACCESS_MASK amMask,        LPTSTR lpszOldIndent,
                   KINDS_OF_ACCESSMASKS_DECODED kamKindOfMask);
BOOL LookupSIDName(PSID psidSID,              LPTSTR lpszOldIndent);
BOOL SIDStringName(PSID psidSID, LPTSTR lpszSIDStringName);
BOOL SetPrivilegeInAccessToken(VOID);
VOID InitializeWellKnownSIDs(VOID);
VOID DisplayHelp(VOID);

UINT main(UINT argc, char *argv[])
{

  HANDLE               hProcess;
  HANDLE               hAccessToken;
  #define         DEFAULT_FILE_TO_CHECK "\\\\.\\A:"
  #define                   SZ_NAME_BUF MAX_PATH
  UCHAR           ucPathBuf[SZ_NAME_BUF];
  LPTSTR          lpszFullName = (LPTSTR)&ucPathBuf;
  #define                           SZ_SD_BUF 8096
  UCHAR                ucBuf       [SZ_SD_BUF] = "";
  DWORD                dwSDLength = SZ_SD_BUF;
  DWORD                dwSDLengthNeeded;
  PSECURITY_DESCRIPTOR psdSD      = (PSECURITY_DESCRIPTOR)&ucBuf;
  DWORD                dwErrorMode;

 // check if running on Windows NT, if not, display notice and terminate

    if( GetVersion() & 0x80000000 )
    {
      MessageBox( NULL,
         "This sample application can only be run on Windows NT.\n"
         "This application will now terminate.",
         "CheckSD",
         MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
      return( 1 );
    }

  /**************************************************************************\
  *
  * This sample is not inside a Win32 service, however if this code were to be
  *   moved inside a Win32 service, the following defines and code that
  *   redirects stdout will be handy, because services cannot write to the
  *   screen
  *
  * You may wish to choose a different file name for the output file if you
  *   use this mechanism - note that the "w+" will destroy an existing file!
  *
  \**************************************************************************/

  #define WE_ARE_IN_A_SERVICE_SO_REDIRECT_STDOUT (0==1)
  #define   FILE_TO_REDIRECT_STDOUT_TO "c:\\check_sd.out"

  if     (WE_ARE_IN_A_SERVICE_SO_REDIRECT_STDOUT)
  { freopen(FILE_TO_REDIRECT_STDOUT_TO,"w+",stdout);
  }


  if      (1 == argc)
  { strcpy(lpszFullName,DEFAULT_FILE_TO_CHECK);
  }
  else if (2 == argc)
  { strcpy(lpszFullName,argv[1]);
  }
  else
  { DisplayHelp();
    return(0);
  }

  /**************************************************************************\
  *
  * Set up the well-known SID(s) in global variables, and enable the privilege
  *   needed in the access token to work with SACL(s)
  *
  \**************************************************************************/

  InitializeWellKnownSIDs();

  if (!SetPrivilegeInAccessToken())
  { return(1);
  }

  /**************************************************************************\
  *
  * This sample's primary purpose is to explore Security Descriptors.
  *   However, it is all too easy to over-focus on SD(s), while losing isght
  *   of the importance of Access Tokens.  So, we will now digress briefly to
  *   examine the access token of the current process
  *
  \**************************************************************************/

  hProcess = GetCurrentProcess();
  if (!hProcess)
  { PERR("GetCurrentProcess");
    return(1);
  }

  if (!OpenProcessToken(hProcess,
                        (TOKEN_READ | TOKEN_QUERY_SOURCE),
                        &hAccessToken))
  { PERR("OpenProcessToken");
    return(1);
  }

  ExamineAccessToken(hAccessToken);

  /**************************************************************************\
  *
  * Back to examining SD(s)
  *
  \**************************************************************************/

  printf("\nChecking SD on %s",lpszFullName);

  /**************************************************************************\
  *
  * SetErrorMode so we don't get the error due to no floppy disk in the floppy
  *   drive
  *
  \**************************************************************************/

  dwErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

  if (!GetFileSecurity
        (lpszFullName,
         (SECURITY_INFORMATION)( OWNER_SECURITY_INFORMATION
                               | GROUP_SECURITY_INFORMATION
                               | DACL_SECURITY_INFORMATION
                               | SACL_SECURITY_INFORMATION),
         psdSD,
         dwSDLength,
         (LPDWORD)&dwSDLengthNeeded))
  { PERR("GetFileSecurity");
    return(1);
  }

  SetErrorMode(dwErrorMode);

  if(!ExamineSD(psdSD,FileAccessMask))
  { PERR("ExamineSD failed");
    return(1);
  }

  /**************************************************************************\
  *
  * The above code showed how to examine an SD on a file.  There are SDs on
  *   other objects that could be examined by the function ExamineSD (and the
  *   other functions it calls).  The following are one example call each of
  *   the other four api's that are used to retrieve the SD from each of the
  *   types of Win32 objects that can have an SD
  *
  * These calls will execute properly without any work on your part, however,
  *   some work on your part will be required to get the sample calls below to
  *   show the SD for the objects your program uses!  You would need to make
  *   copy of this sample in a new directory, and write the code to get a
  *   handle to the object you're interested in, so you can pass that handle
  *   to the applicable api call below
  *
  * To insert the SD checking code into your own code you would do
  *   something like
  *
  * 1) Add the global variables above in your globals
  *
  * 2) Add the function prototypes above in your globals
  *
  * 3) Add the includes and PERR/PMSG macroes above to your code
  *
  * 4) Add all the functions defined below (except DisplayHelp and except
  *   ExamineAccessToken) to your code
  *
  * 5) Add a call sequence such as that in main() above that gets an
  *   SD and passes it to Examine SD.  Or add a call sequence such as
  *   one of those that follows that gets a handle, gets the SD on the
  *   object the handle addresses, then calls ExamineSD
  *
  * 6) If you are examining a type SD not already listed in
  *   KINDS_OF_ACCESSMASKS_DECODED, then you will have to add the new type to
  *   KINDS_OF_ACCESSMASKS_DECODED, and add to ExamineMask() the necessary
  *   code to crack that type of SD's access mask bits into the defines
  *
  \**************************************************************************/

  #define I_DO_NOT_WANT_THIS_CODE_TO_CLUTTER_THIS_PROGRAM_S_OUTPUT (0==0)

  if    (!I_DO_NOT_WANT_THIS_CODE_TO_CLUTTER_THIS_PROGRAM_S_OUTPUT)
  { HANDLE    hProcess;
    HANDLE    hWindowStation;
    HKEY      hKey;
    SC_HANDLE schService;
    SC_HANDLE schSCManager;
    SECURITY_INFORMATION siSInfo =
         (SECURITY_INFORMATION)( OWNER_SECURITY_INFORMATION
                               | GROUP_SECURITY_INFORMATION
                               | DACL_SECURITY_INFORMATION
                               | SACL_SECURITY_INFORMATION);

    printf("\n\nChecking SD on current process");

    hProcess = GetCurrentProcess();
    if (!hProcess)
    { PERR("GetCurrentProcess");
      return(1);
    }

    dwSDLength = SZ_SD_BUF;

    if (!GetKernelObjectSecurity
          (hProcess,
           (SECURITY_INFORMATION)( OWNER_SECURITY_INFORMATION
                                 | GROUP_SECURITY_INFORMATION
                                 | DACL_SECURITY_INFORMATION),
           psdSD,
           dwSDLength,
           (LPDWORD)&dwSDLengthNeeded))
    { PERR("GetKernelObjectSecurity");
      return(1);
    }

    /************************************************************************\
    *
    * It is important to close all handles as soon as your code no longer
    *   needs them.  This conserves system resources.  In a sample such as
    *   this one, the practical effect is close to nil, since as soon as the
    *   sample exits (which only takes a few seconds, Windows NT destroys the
    *   process this sample was running in, which reclaims all resources
    *
    * However, in a program that creates many objects (such as threads), or
    *   where that program will be running for a long time, closing handles as
    *   soon as the program no longer needs them can save significant
    *   resources
    *
    * It is a good coding practice to make a habit of closing handles as soon
    *   as your code no longer needs the handle
    *
    \************************************************************************/

    CloseHandle(hProcess);

    if(!ExamineSD(psdSD,ProcessAccessMask))
    { PERR("ExamineSD failed");
      return(1);
    }


    printf("\n\nChecking SD on current Window-station");

    hWindowStation = GetProcessWindowStation();
    if (INVALID_HANDLE_VALUE == hWindowStation)
    { PERR("GetProcessWindowStation");
      return(1);
    }

    dwSDLength = SZ_SD_BUF;

    if (!GetUserObjectSecurity
          (hWindowStation,
           &siSInfo,
           psdSD,
           dwSDLength,
           (LPDWORD)&dwSDLengthNeeded))
    { PERR("GetUserObjectSecurity");
      return(1);
    }

    CloseHandle(hWindowStation);

    if(!ExamineSD(psdSD,WindowStationAccessMask))
    { PERR("ExamineSD failed");
      return(1);
    }


    printf("\n\nChecking SD on registry key HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet");

    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                      "SYSTEM\\CurrentControlSet",
                                      0,
                                      KEY_READ,
                                      &hKey))
    { PERR("RegOpenKeyEx");
      return(1);
    }

    dwSDLength = SZ_SD_BUF;

    if (ERROR_SUCCESS != RegGetKeySecurity
          (hKey,
           (SECURITY_INFORMATION)( OWNER_SECURITY_INFORMATION
                                 | GROUP_SECURITY_INFORMATION
                                 | DACL_SECURITY_INFORMATION),
           psdSD,
           &dwSDLength))
    { PERR("RegGetKeySecurity");
      return(1);
    }

    RegCloseKey(hKey);

    if(!ExamineSD(psdSD,RegKeyAccessMask))
    { PERR("ExamineSD failed");
      return(1);
    }


    /************************************************************************\
    *
    * Any service will do here, just be sure to pick one that is in the
    *   Service Control Manager's database, and use the service name, not the
    *   service's display name.  For example, ClipSrv is displayed as Clipbook
    *   in the Services Control Panel applet
    *
    \************************************************************************/

    printf("\n\nChecking SD on service Clipbook");

    schSCManager = OpenSCManager(
                        NULL,                 // machine (NULL == local)
                        NULL,                 // database (NULL == default)
                        SC_MANAGER_ALL_ACCESS // access required
                        );

    if (!schSCManager)
    { PERR("OpenSCManager");
      return(1);
    }

    schService = OpenService(schSCManager,"ClipSrv",SERVICE_ALL_ACCESS);
    if (!schService)
    { PERR("OpenService");
      return(1);
    }

    CloseServiceHandle(schSCManager);

    dwSDLength = SZ_SD_BUF;

    if (!QueryServiceObjectSecurity
          (schService,
           (SECURITY_INFORMATION)( OWNER_SECURITY_INFORMATION
                                 | GROUP_SECURITY_INFORMATION
                                 | DACL_SECURITY_INFORMATION),
           psdSD,
           dwSDLength,
           (LPDWORD)&dwSDLengthNeeded))
    { PERR("QueryServiceObjectSecurity");
      return(1);
    }

    CloseServiceHandle(schService);

    if(!ExamineSD(psdSD,ServiceAccessMask))
    { PERR("ExamineSD failed");
      return(1);
    }
  }

  return(0);
}

/****************************************************************************\
*
* FUNCTION: ExamineSD
*
\****************************************************************************/

BOOL ExamineSD    (PSECURITY_DESCRIPTOR psdSD,
                   KINDS_OF_ACCESSMASKS_DECODED kamKindOfMask)
{

  PACL                        paclDACL;
  PACL                        paclSACL;
  BOOL                        bHasDACL        = FALSE;
  BOOL                        bHasSACL        = FALSE;
  BOOL                        bDaclDefaulted  = FALSE;
  BOOL                        bSaclDefaulted  = FALSE;
  BOOL                        bOwnerDefaulted = FALSE;
  BOOL                        bGroupDefaulted = FALSE;
  PSID                        psidOwner;
  PSID                        psidGroup;
  SECURITY_DESCRIPTOR_CONTROL sdcSDControl;
  DWORD                       dwSDRevision;
  DWORD                       dwSDLength;

  if (!IsValidSecurityDescriptor(psdSD))
  { PERR("IsValidSecurityDescriptor");
    return(FALSE);
  }

  dwSDLength = GetSecurityDescriptorLength(psdSD);

  if (!GetSecurityDescriptorDacl(psdSD,
                                 (LPBOOL)&bHasDACL,
                                 (PACL *)&paclDACL,
                                 (LPBOOL)&bDaclDefaulted))
  { PERR("GetSecurityDescriptorDacl");
    return(FALSE);
  }

  if (!GetSecurityDescriptorSacl(psdSD,
                                 (LPBOOL)&bHasSACL,
                                 (PACL *)&paclSACL,
                                 (LPBOOL)&bSaclDefaulted))
  { PERR("GetSecurityDescriptorSacl");
    return(FALSE);
  }

  if (!GetSecurityDescriptorOwner(psdSD,
                                  (PSID *)&psidOwner,
                                  (LPBOOL)&bOwnerDefaulted))
  { PERR("GetSecurityDescriptorOwner");
    return(FALSE);
  }

  if (!GetSecurityDescriptorGroup(psdSD,
                                  (PSID *)&psidGroup,
                                  (LPBOOL)&bGroupDefaulted))
  { PERR("GetSecurityDescriptorGroup");
    return(FALSE);
  }

  if (!GetSecurityDescriptorControl(psdSD,
                                    (PSECURITY_DESCRIPTOR_CONTROL)&sdcSDControl,
                                    (LPDWORD)&dwSDRevision))
  { PERR("GetSecurityDescriptorControl");
    return(FALSE);
  }

  printf("\nSD is valid.  SD is %d bytes long.  SD revision is %d == ",dwSDLength,dwSDRevision);

  switch (dwSDRevision)
  {
    case      SECURITY_DESCRIPTOR_REVISION1 :
    { printf("SECURITY_DESCRIPTOR_REVISION1");
      break;
    }
    default :
    { printf("! SD Revision is an IMPOSSIBLE SD revision!!! Perhaps a new revision was added...");
      return(FALSE);
    }
  }

  if (SE_SELF_RELATIVE & sdcSDControl)
    printf("\nSD is in self-relative format (all SDs returned by the system are)");

  if (NULL == psidOwner)
  { printf("\nSD's Owner is NULL, so SE_OWNER_DEFAULTED is ignored");
  }
  else
  { printf("\nSD's Owner is Not NULL");
    if (bOwnerDefaulted )
    { printf("\nSD's Owner-Defaulted flag is TRUE");
    }
    else
    { printf("\nSD's Owner-Defaulted flag is FALSE");
    }
    if(!LookupSIDName(psidOwner,""))
    { PERR("LookupSIDName failed");
    }
  }

  /**************************************************************************\
  *
  * The other use for psidGroup is for Macintosh client support
  *
  \**************************************************************************/

  if (NULL == psidGroup)
  { printf("\nSD's Group is NULL, so SE_GROUP_DEFAULTED is ignored");
    printf("\nSD's Group being NULL is typical, GROUP in SD(s) is mainly for POSIX compliance");
  }
  else
  { if (bGroupDefaulted)
    { printf("\nSD's Group-Defaulted flag is TRUE");
    }
    else
    { printf("\nSD's Group-Defaulted flag is FALSE");
    }
    if(!LookupSIDName(psidGroup,""))
    { PERR("LookupSIDName failed");
    }
  }

  if   (SE_DACL_PRESENT & sdcSDControl)
  { printf("\nSD's DACL is Present");
    if (bDaclDefaulted)
    { printf("\nSD's DACL-Defaulted flag is TRUE");
    }
    else
    { printf("\nSD's DACL-Defaulted flag is FALSE");
    }

    if (NULL == paclDACL)
    { printf("\nSD has a NULL DACL explicitly specified      (allows all access to Everyone)");
      printf("\n    This does not apply to this SD, but for comparison,");
      printf("\n    a non-NULL DACL pointer to a 0-length ACL allows  no access to   anyone");
    }
    else if(!ExamineACL(paclDACL,"",kamKindOfMask))
    {  PERR("ExamineACL failed");
    }
  }
  else
  { printf("\nSD's DACL is Not Present, so SE_DACL_DEFAULTED is ignored");
    printf("\nSD has no DACL at all (allows all access to Everyone)");
  }

  if   (SE_SACL_PRESENT & sdcSDControl)
  { printf("\nSD's SACL is Present");
    if (bSaclDefaulted)
    { printf("\nSD's SACL-Defaulted flag is TRUE");
    }
    else
    { printf("\nSD's SACL-Defaulted flag is FALSE");
    }

    if (NULL == paclSACL)
    { printf("\nSD has a NULL SACL explicitly specified");
    }
    else if(!ExamineACL(paclSACL,"",kamKindOfMask))
    {  PERR("ExamineACL failed");
    }
  }
  else
  { printf("\nSD's SACL is Not Present, so SE_SACL_DEFAULTED is ignored");
    printf("\nSD has no SACL at all (or we did not request to see it)");
  }
}

/****************************************************************************\
*
* FUNCTION: ExamineACL
*
\****************************************************************************/

BOOL ExamineACL   (PACL paclACL,              LPTSTR lpszOldIndent,
                   KINDS_OF_ACCESSMASKS_DECODED kamKindOfMask)
{
  #define                          SZ_INDENT_BUF 80
  UCHAR                ucIndentBuf[SZ_INDENT_BUF] = "";
  ACL_SIZE_INFORMATION                      asiAclSize;
  ACL_REVISION_INFORMATION                  ariAclRevision;
  DWORD                dwBufLength;
  DWORD                dwAcl_i;
  ACCESS_ALLOWED_ACE   *paaAllowedAce;

  strcpy(ucIndentBuf,lpszOldIndent);
  strcat(ucIndentBuf,"  ");

  if (!IsValidAcl(paclACL))
  { PERR("IsValidAcl");
    return(FALSE);
  }

  dwBufLength = sizeof(asiAclSize);

  if (!GetAclInformation(paclACL,
                         (LPVOID)&asiAclSize,
                         (DWORD)dwBufLength,
                         (ACL_INFORMATION_CLASS)AclSizeInformation))
  { PERR("GetAclInformation");
    return(FALSE);
  }

  dwBufLength = sizeof(ariAclRevision);

  if (!GetAclInformation(paclACL,
                         (LPVOID)&ariAclRevision,
                         (DWORD)dwBufLength,
                         (ACL_INFORMATION_CLASS)AclRevisionInformation))
  { PERR("GetAclInformation");
    return(FALSE);
  }

  printf("\n%sACL has %d ACE(s), %d bytes used, %d bytes free",ucIndentBuf,
     asiAclSize.AceCount,
     asiAclSize.AclBytesInUse,
     asiAclSize.AclBytesFree);

  printf("\n%sACL revision is %d == ",ucIndentBuf,ariAclRevision.AclRevision);

  switch (ariAclRevision.AclRevision)
  {
    case      ACL_REVISION1 :
    { printf("ACL_REVISION1");
      break;
    }
    case      ACL_REVISION2 :
    { printf("ACL_REVISION2");
      break;
    }
    default :
    { printf("\n%sACL Revision is an IMPOSSIBLE ACL revision!!! Perhaps a new revision was added...",ucIndentBuf);
      return(FALSE);
    }
  }

  for (dwAcl_i = 0; dwAcl_i < asiAclSize.AceCount;  dwAcl_i++)
  {
    if (!GetAce(paclACL,
                dwAcl_i,
                (LPVOID *)&paaAllowedAce))
    { PERR("GetAce");
      return(FALSE);
    }

    printf("\n%sACE %d size %d",ucIndentBuf,dwAcl_i,paaAllowedAce->Header.AceSize);

    { DWORD dwAceFlags = paaAllowedAce->Header.AceFlags;

      printf("\n%sACE %d flags 0x%.2x",ucIndentBuf,dwAcl_i,dwAceFlags);

      if (dwAceFlags)
      {
        DWORD dwExtraBits;
        UCHAR ucIndentBitsBuf[SZ_INDENT_BUF] = "";

        strcpy(ucIndentBitsBuf,ucIndentBuf);
        strcat(ucIndentBitsBuf,"            ");

        if ((dwAceFlags  & OBJECT_INHERIT_ACE        ) == OBJECT_INHERIT_ACE        )
        { printf("\n%s0x01 OBJECT_INHERIT_ACE        ",ucIndentBitsBuf);
        }
        if ((dwAceFlags  & CONTAINER_INHERIT_ACE     ) == CONTAINER_INHERIT_ACE     )
        { printf("\n%s0x02 CONTAINER_INHERIT_ACE     ",ucIndentBitsBuf);
        }
        if ((dwAceFlags  & NO_PROPAGATE_INHERIT_ACE  ) == NO_PROPAGATE_INHERIT_ACE  )
        { printf("\n%s0x04 NO_PROPAGATE_INHERIT_ACE  ",ucIndentBitsBuf);
        }
        if ((dwAceFlags  & INHERIT_ONLY_ACE          ) == INHERIT_ONLY_ACE          )
        { printf("\n%s0x08 INHERIT_ONLY_ACE          ",ucIndentBitsBuf);
        }
        if ((dwAceFlags  & VALID_INHERIT_FLAGS       ) == VALID_INHERIT_FLAGS       )
        { printf("\n%s0x0F VALID_INHERIT_FLAGS       ",ucIndentBitsBuf);
        }
        if ((dwAceFlags  & SUCCESSFUL_ACCESS_ACE_FLAG) == SUCCESSFUL_ACCESS_ACE_FLAG)
        { printf("\n%s0x40 SUCCESSFUL_ACCESS_ACE_FLAG",ucIndentBitsBuf);
        }
        if ((dwAceFlags  & FAILED_ACCESS_ACE_FLAG    ) == FAILED_ACCESS_ACE_FLAG    )
        { printf("\n%s0x80 FAILED_ACCESS_ACE_FLAG    ",ucIndentBitsBuf);
        }

        dwExtraBits = dwAceFlags & ( ~( OBJECT_INHERIT_ACE
                                      | CONTAINER_INHERIT_ACE
                                      | NO_PROPAGATE_INHERIT_ACE
                                      | INHERIT_ONLY_ACE
                                      | VALID_INHERIT_FLAGS
                                      | SUCCESSFUL_ACCESS_ACE_FLAG
                                      | FAILED_ACCESS_ACE_FLAG) );
        if (dwExtraBits)
        { printf("\n%sExtra AceFlag bits     == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
        }
      }
    }

    switch (paaAllowedAce->Header.AceType)
    {
      case                       ACCESS_ALLOWED_ACE_TYPE :
      { printf("\n%sACE %d is an ACCESS_ALLOWED_ACE_TYPE",ucIndentBuf,dwAcl_i);
        break;
      }
      case                       ACCESS_DENIED_ACE_TYPE :
      { printf("\n%sACE %d is an ACCESS_DENIED_ACE_TYPE",ucIndentBuf,dwAcl_i);
        break;
      }
      case                       SYSTEM_AUDIT_ACE_TYPE :
      { printf("\n%sACE %d is a  SYSTEM_AUDIT_ACE_TYPE",ucIndentBuf,dwAcl_i);
        break;
      }
      case                       SYSTEM_ALARM_ACE_TYPE :
      { printf("\n%sACE %d is a  SYSTEM_ALARM_ACE_TYPE",ucIndentBuf,dwAcl_i);
        break;
      }
      default :
      { printf("\n%sACE %d is an IMPOSSIBLE ACE_TYPE!!! Run debugger, examine value!",ucIndentBuf,dwAcl_i);
        return(FALSE);
      }
    }

    printf("\n%sACE %d mask               == 0x%.8x",ucIndentBuf,dwAcl_i,paaAllowedAce->Mask);

    ExamineMask(paaAllowedAce->Mask,ucIndentBuf,kamKindOfMask);

    if(!LookupSIDName((PSID)&(paaAllowedAce->SidStart),ucIndentBuf))
    { PERR("LookupSIDName failed");
    }
  }
}

/****************************************************************************\
*
* FUNCTION: ExamineMask
*
\****************************************************************************/

VOID ExamineMask  (ACCESS_MASK amMask,        LPTSTR lpszOldIndent,
                   KINDS_OF_ACCESSMASKS_DECODED kamKindOfMask)
{
  #define STANDARD_RIGHTS_ALL_THE_BITS 0x00FF0000L
  #define GENERIC_RIGHTS_ALL_THE_BITS  0xF0000000L
  UCHAR ucIndentBuf[SZ_INDENT_BUF]     = "";
  UCHAR ucIndentBitsBuf[SZ_INDENT_BUF] = "";
  DWORD dwGenericBits;
  DWORD dwStandardBits;
  DWORD dwSpecificBits;
  DWORD dwAccessSystemSecurityBit;
  DWORD dwExtraBits;

  strcpy(ucIndentBuf,    lpszOldIndent);
  strcat(ucIndentBuf,    "  ");
  strcpy(ucIndentBitsBuf,lpszOldIndent);
  strcat(ucIndentBitsBuf,"                            ");

  dwStandardBits            = (amMask & STANDARD_RIGHTS_ALL_THE_BITS);
  dwSpecificBits            = (amMask & SPECIFIC_RIGHTS_ALL         );
  dwAccessSystemSecurityBit = (amMask & ACCESS_SYSTEM_SECURITY      );
  dwGenericBits             = (amMask & GENERIC_RIGHTS_ALL_THE_BITS );

  /**************************************************************************\
  *
  * Print then decode the standard rights bits
  *
  \**************************************************************************/

  printf("\n%sStandard Rights        == 0x%.8x",ucIndentBuf,dwStandardBits);

  if (dwStandardBits)
  {
    if ((dwStandardBits    & DELETE                  ) == DELETE                  )
    { printf("\n%s0x00010000 DELETE                  ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & READ_CONTROL            ) == READ_CONTROL            )
    { printf("\n%s0x00020000 READ_CONTROL            ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & STANDARD_RIGHTS_READ    ) == STANDARD_RIGHTS_READ    )
    { printf("\n%s0x00020000 STANDARD_RIGHTS_READ    ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & STANDARD_RIGHTS_WRITE   ) == STANDARD_RIGHTS_WRITE   )
    { printf("\n%s0x00020000 STANDARD_RIGHTS_WRITE   ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & STANDARD_RIGHTS_EXECUTE ) == STANDARD_RIGHTS_EXECUTE )
    { printf("\n%s0x00020000 STANDARD_RIGHTS_EXECUTE ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & WRITE_DAC               ) == WRITE_DAC               )
    { printf("\n%s0x00040000 WRITE_DAC               ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & WRITE_OWNER             ) == WRITE_OWNER             )
    { printf("\n%s0x00080000 WRITE_OWNER             ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & SYNCHRONIZE             ) == SYNCHRONIZE             )
    { printf("\n%s0x00100000 SYNCHRONIZE             ",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & STANDARD_RIGHTS_REQUIRED) == STANDARD_RIGHTS_REQUIRED)
    { printf("\n%s0x000F0000 STANDARD_RIGHTS_REQUIRED",ucIndentBitsBuf);
    }
    if ((dwStandardBits    & STANDARD_RIGHTS_ALL     ) == STANDARD_RIGHTS_ALL     )
    { printf("\n%s0x001F0000 STANDARD_RIGHTS_ALL     ",ucIndentBitsBuf);
    }

    dwExtraBits = dwStandardBits & ( ~( DELETE
                                      | READ_CONTROL
                                      | STANDARD_RIGHTS_READ
                                      | STANDARD_RIGHTS_WRITE
                                      | STANDARD_RIGHTS_EXECUTE
                                      | WRITE_DAC
                                      | WRITE_OWNER
                                      | SYNCHRONIZE
                                      | STANDARD_RIGHTS_REQUIRED
                                      | STANDARD_RIGHTS_ALL) );
    if (dwExtraBits)
    { printf("\n%sExtra standard bits    == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
    }
  }

  /**************************************************************************\
  *
  * Print then decode the specific rights bits
  *
  \**************************************************************************/

  printf("\n%sSpecific Rights        == 0x%.8x",ucIndentBuf,dwSpecificBits);

  if (dwSpecificBits)
  {
    if      (FileAccessMask          == kamKindOfMask)
    {
      if ((dwSpecificBits    & FILE_READ_DATA           ) == FILE_READ_DATA           )
      { printf("\n%s0x00000001 FILE_READ_DATA            (file & pipe)     ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_LIST_DIRECTORY      ) == FILE_LIST_DIRECTORY      )
      { printf("\n%s0x00000001 FILE_LIST_DIRECTORY       (directory)       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_WRITE_DATA          ) == FILE_WRITE_DATA          )
      { printf("\n%s0x00000002 FILE_WRITE_DATA           (file & pipe)     ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_ADD_FILE            ) == FILE_ADD_FILE            )
      { printf("\n%s0x00000002 FILE_ADD_FILE             (directory)       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_APPEND_DATA         ) == FILE_APPEND_DATA         )
      { printf("\n%s0x00000004 FILE_APPEND_DATA          (file)            ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_ADD_SUBDIRECTORY    ) == FILE_ADD_SUBDIRECTORY    )
      { printf("\n%s0x00000004 FILE_ADD_SUBDIRECTORY     (directory)       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_CREATE_PIPE_INSTANCE) == FILE_CREATE_PIPE_INSTANCE)
      { printf("\n%s0x00000004 FILE_CREATE_PIPE_INSTANCE (named pipe)      ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_READ_EA             ) == FILE_READ_EA             )
      { printf("\n%s0x00000008 FILE_READ_EA              (file & directory)",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_WRITE_EA            ) == FILE_WRITE_EA            )
      { printf("\n%s0x00000010 FILE_WRITE_EA             (file & directory)",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_EXECUTE             ) == FILE_EXECUTE             )
      { printf("\n%s0x00000020 FILE_EXECUTE              (file)            ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_TRAVERSE            ) == FILE_TRAVERSE            )
      { printf("\n%s0x00000020 FILE_TRAVERSE             (directory)       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_DELETE_CHILD        ) == FILE_DELETE_CHILD        )
      { printf("\n%s0x00000040 FILE_DELETE_CHILD         (directory)       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_READ_ATTRIBUTES     ) == FILE_READ_ATTRIBUTES     )
      { printf("\n%s0x00000080 FILE_READ_ATTRIBUTES      (all)             ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & FILE_WRITE_ATTRIBUTES    ) == FILE_WRITE_ATTRIBUTES    )
      { printf("\n%s0x00000100 FILE_WRITE_ATTRIBUTES     (all)             ",ucIndentBitsBuf);
      }

      if (((dwStandardBits | dwSpecificBits   )
           & FILE_ALL_ACCESS     ) == FILE_ALL_ACCESS     )
      { printf("\n%s0x001F01FF FILE_ALL_ACCESS      == (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF)",ucIndentBitsBuf);
      }
      if (((dwStandardBits | dwSpecificBits   )
           & FILE_GENERIC_READ   ) == FILE_GENERIC_READ   )
      { printf("\n%s0x00120089 FILE_GENERIC_READ    == (STANDARD_RIGHTS_READ | FILE_READ_DATA | FILE_READ_ATTRIBUTES | FILE_READ_EA | SYNCHRONIZE)",ucIndentBitsBuf);
      }
      if (((dwStandardBits | dwSpecificBits   )
           & FILE_GENERIC_WRITE  ) == FILE_GENERIC_WRITE  )
      { printf("\n%s0x00120116 FILE_GENERIC_WRITE   == (STANDARD_RIGHTS_WRITE | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA | FILE_APPEND_DATA | SYNCHRONIZE)",ucIndentBitsBuf);
      }
      if (((dwStandardBits | dwSpecificBits   )
           & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE)
      { printf("\n%s0x001200A0 FILE_GENERIC_EXECUTE == (STANDARD_RIGHTS_EXECUTE | FILE_READ_ATTRIBUTES | FILE_EXECUTE | SYNCHRONIZE)",ucIndentBitsBuf);
      }

      dwExtraBits = dwSpecificBits & ( ~( FILE_READ_DATA
                                        | FILE_LIST_DIRECTORY
                                        | FILE_WRITE_DATA
                                        | FILE_ADD_FILE
                                        | FILE_APPEND_DATA
                                        | FILE_ADD_SUBDIRECTORY
                                        | FILE_CREATE_PIPE_INSTANCE
                                        | FILE_READ_EA
                                        | FILE_WRITE_EA
                                        | FILE_EXECUTE
                                        | FILE_TRAVERSE
                                        | FILE_DELETE_CHILD
                                        | FILE_READ_ATTRIBUTES
                                        | FILE_WRITE_ATTRIBUTES
                                        | (FILE_ALL_ACCESS      & SPECIFIC_RIGHTS_ALL)
                                        | (FILE_GENERIC_READ    & SPECIFIC_RIGHTS_ALL)
                                        | (FILE_GENERIC_WRITE   & SPECIFIC_RIGHTS_ALL)
                                        | (FILE_GENERIC_EXECUTE & SPECIFIC_RIGHTS_ALL) ) );
      if (dwExtraBits)
      { printf("\n%sExtra specific bits    == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
      }
    }
    else if (ProcessAccessMask       == kamKindOfMask)
    {
      if ((dwSpecificBits    & PROCESS_TERMINATE        ) == PROCESS_TERMINATE        )
      { printf("\n%s0x00000001 PROCESS_TERMINATE        ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_CREATE_THREAD    ) == PROCESS_CREATE_THREAD    )
      { printf("\n%s0x00000002 PROCESS_CREATE_THREAD    ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_VM_OPERATION     ) == PROCESS_VM_OPERATION     )
      { printf("\n%s0x00000008 PROCESS_VM_OPERATION     ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_VM_READ          ) == PROCESS_VM_READ          )
      { printf("\n%s0x00000010 PROCESS_VM_READ          ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_VM_WRITE         ) == PROCESS_VM_WRITE         )
      { printf("\n%s0x00000020 PROCESS_VM_WRITE         ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_DUP_HANDLE       ) == PROCESS_DUP_HANDLE       )
      { printf("\n%s0x00000040 PROCESS_DUP_HANDLE       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_CREATE_PROCESS   ) == PROCESS_CREATE_PROCESS   )
      { printf("\n%s0x00000080 PROCESS_CREATE_PROCESS   ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_SET_INFORMATION  ) == PROCESS_SET_INFORMATION  )
      { printf("\n%s0x00000200 PROCESS_SET_INFORMATION  ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & PROCESS_QUERY_INFORMATION) == PROCESS_QUERY_INFORMATION)
      { printf("\n%s0x00000400 PROCESS_QUERY_INFORMATION",ucIndentBitsBuf);
      }

      if (((dwStandardBits | dwSpecificBits )
           & PROCESS_ALL_ACCESS) == PROCESS_ALL_ACCESS)
      { printf("\n%s0x001F0FFF PROCESS_ALL_ACCESS == (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x00000FFF) ",ucIndentBitsBuf);
      }

      dwExtraBits = dwSpecificBits & ( ~( PROCESS_TERMINATE
                                        | PROCESS_CREATE_THREAD
                                        | PROCESS_VM_OPERATION
                                        | PROCESS_VM_READ
                                        | PROCESS_VM_WRITE
                                        | PROCESS_DUP_HANDLE
                                        | PROCESS_CREATE_PROCESS
                                        | PROCESS_SET_INFORMATION
                                        | PROCESS_QUERY_INFORMATION
                                        | (PROCESS_ALL_ACCESS & SPECIFIC_RIGHTS_ALL) ) );
      if (dwExtraBits)
      { printf("\n%sExtra specific bits    == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
      }
    }
    else if (WindowStationAccessMask == kamKindOfMask)
    {
      if ((dwSpecificBits    & WINSTA_ENUMDESKTOPS     ) == WINSTA_ENUMDESKTOPS     )
      { printf("\n%s0x00000001 WINSTA_ENUMDESKTOPS     ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_READATTRIBUTES   ) == WINSTA_READATTRIBUTES   )
      { printf("\n%s0x00000002 WINSTA_READATTRIBUTES   ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_ACCESSCLIPBOARD  ) == WINSTA_ACCESSCLIPBOARD  )
      { printf("\n%s0x00000004 WINSTA_ACCESSCLIPBOARD  ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_CREATEDESKTOP    ) == WINSTA_CREATEDESKTOP    )
      { printf("\n%s0x00000008 WINSTA_CREATEDESKTOP    ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_WRITEATTRIBUTES  ) == WINSTA_WRITEATTRIBUTES  )
      { printf("\n%s0x00000010 WINSTA_WRITEATTRIBUTES  ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_ACCESSGLOBALATOMS) == WINSTA_ACCESSGLOBALATOMS)
      { printf("\n%s0x00000020 WINSTA_ACCESSGLOBALATOMS",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_EXITWINDOWS      ) == WINSTA_EXITWINDOWS      )
      { printf("\n%s0x00000040 WINSTA_EXITWINDOWS      ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_ENUMERATE        ) == WINSTA_ENUMERATE        )
      { printf("\n%s0x00000100 WINSTA_ENUMERATE        ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & WINSTA_READSCREEN       ) == WINSTA_READSCREEN       )
      { printf("\n%s0x00000200 WINSTA_READSCREEN       ",ucIndentBitsBuf);
      }

      dwExtraBits = dwSpecificBits & ( ~( WINSTA_ENUMDESKTOPS
                                        | WINSTA_READATTRIBUTES
                                        | WINSTA_ACCESSCLIPBOARD
                                        | WINSTA_CREATEDESKTOP
                                        | WINSTA_WRITEATTRIBUTES
                                        | WINSTA_ACCESSGLOBALATOMS
                                        | WINSTA_EXITWINDOWS
                                        | WINSTA_ENUMERATE
                                        | WINSTA_READSCREEN) );
      if (dwExtraBits)
      { printf("\n%sExtra specific bits    == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
      }
    }
    else if (RegKeyAccessMask        == kamKindOfMask)
    {
      if ((dwSpecificBits    & KEY_QUERY_VALUE       ) == KEY_QUERY_VALUE       )
      { printf("\n%s0x00000001 KEY_QUERY_VALUE       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & KEY_SET_VALUE         ) == KEY_SET_VALUE         )
      { printf("\n%s0x00000002 KEY_SET_VALUE         ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & KEY_CREATE_SUB_KEY    ) == KEY_CREATE_SUB_KEY    )
      { printf("\n%s0x00000004 KEY_CREATE_SUB_KEY    ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & KEY_ENUMERATE_SUB_KEYS) == KEY_ENUMERATE_SUB_KEYS)
      { printf("\n%s0x00000008 KEY_ENUMERATE_SUB_KEYS",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & KEY_NOTIFY            ) == KEY_NOTIFY            )
      { printf("\n%s0x00000010 KEY_NOTIFY            ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & KEY_CREATE_LINK       ) == KEY_CREATE_LINK       )
      { printf("\n%s0x00000020 KEY_CREATE_LINK       ",ucIndentBitsBuf);
      }

      if (((dwStandardBits | dwSpecificBits   )
           & KEY_READ      ) == KEY_READ      )
      { printf("\n%s0x00020019 KEY_READ       == ((STANDARD_RIGHTS_READ | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY) & (~SYNCHRONIZE))",ucIndentBitsBuf);
      }
      if (((dwStandardBits | dwSpecificBits   )
           & KEY_WRITE     ) == KEY_WRITE     )
      { printf("\n%s0x00020006 KEY_WRITE      == ((STANDARD_RIGHTS_WRITE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY) & (~SYNCHRONIZE))",ucIndentBitsBuf);
      }
      if (((dwStandardBits | dwSpecificBits   )
           & KEY_EXECUTE   ) == KEY_EXECUTE   )
      { printf("\n%s0x00020019 KEY_EXECUTE    == ((KEY_READ) & (~SYNCHRONIZE))",ucIndentBitsBuf);
      }
      if (((dwStandardBits | dwSpecificBits   )
           & KEY_ALL_ACCESS) == KEY_ALL_ACCESS)
      { printf("\n%s0x000F003F KEY_ALL_ACCESS == ((STANDARD_RIGHTS_ALL | KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY | KEY_CREATE_LINK) & (~SYNCHRONIZE))",ucIndentBitsBuf);
      }

      dwExtraBits = dwSpecificBits & ( ~( KEY_QUERY_VALUE
                                        | KEY_SET_VALUE
                                        | KEY_CREATE_SUB_KEY
                                        | KEY_ENUMERATE_SUB_KEYS
                                        | KEY_NOTIFY
                                        | KEY_CREATE_LINK
                                        | (KEY_READ       & SPECIFIC_RIGHTS_ALL)
                                        | (KEY_WRITE      & SPECIFIC_RIGHTS_ALL)
                                        | (KEY_EXECUTE    & SPECIFIC_RIGHTS_ALL)
                                        | (KEY_ALL_ACCESS & SPECIFIC_RIGHTS_ALL) ) );
      if (dwExtraBits)
      { printf("\n%sExtra specific bits    == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
      }
    }
    else if (ServiceAccessMask       == kamKindOfMask)
    {
      if ((dwSpecificBits    & SERVICE_QUERY_CONFIG        ) == SERVICE_QUERY_CONFIG        )
      { printf("\n%s0x00000001 SERVICE_QUERY_CONFIG        ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_CHANGE_CONFIG       ) == SERVICE_CHANGE_CONFIG       )
      { printf("\n%s0x00000002 SERVICE_CHANGE_CONFIG       ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_QUERY_STATUS        ) == SERVICE_QUERY_STATUS        )
      { printf("\n%s0x00000004 SERVICE_QUERY_STATUS        ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_ENUMERATE_DEPENDENTS) == SERVICE_ENUMERATE_DEPENDENTS)
      { printf("\n%s0x00000008 SERVICE_ENUMERATE_DEPENDENTS",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_START               ) == SERVICE_START               )
      { printf("\n%s0x00000010 SERVICE_START               ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_STOP                ) == SERVICE_STOP                )
      { printf("\n%s0x00000020 SERVICE_STOP                ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_PAUSE_CONTINUE      ) == SERVICE_PAUSE_CONTINUE      )
      { printf("\n%s0x00000040 SERVICE_PAUSE_CONTINUE      ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_INTERROGATE         ) == SERVICE_INTERROGATE         )
      { printf("\n%s0x00000080 SERVICE_INTERROGATE         ",ucIndentBitsBuf);
      }
      if ((dwSpecificBits    & SERVICE_USER_DEFINED_CONTROL) == SERVICE_USER_DEFINED_CONTROL)
      { printf("\n%s0x00000100 SERVICE_USER_DEFINED_CONTROL",ucIndentBitsBuf,ucIndentBitsBuf);
      }

      if (((dwStandardBits | dwSpecificBits )
           & SERVICE_ALL_ACCESS) == SERVICE_ALL_ACCESS)
      { printf("\n%s0x000F01FF SERVICE_ALL_ACCESS == (STANDARD_RIGHTS_REQUIRED | SERVICE_QUERY_CONFIG | SERVICE_CHANGE_CONFIG | ", ucIndentBitsBuf );
        printf("%s", "SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS | SERVICE_START | SERVICE_STOP | SERVICE_PAUSE_CONTINUE | SERVICE_INTERROGATE | SERVICE_USER_DEFINED_CONTROL)" );
      }

      dwExtraBits = dwSpecificBits & ( ~( SERVICE_QUERY_CONFIG
                                        | SERVICE_CHANGE_CONFIG
                                        | SERVICE_QUERY_STATUS
                                        | SERVICE_ENUMERATE_DEPENDENTS
                                        | SERVICE_START
                                        | SERVICE_STOP
                                        | SERVICE_PAUSE_CONTINUE
                                        | SERVICE_INTERROGATE
                                        | SERVICE_USER_DEFINED_CONTROL
                                        | (SERVICE_ALL_ACCESS & SPECIFIC_RIGHTS_ALL) ) );
      if (dwExtraBits)
      { printf("\n%sExtra specific bits    == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
      }
    }
    else if (DefaultDaclInAccessTokenAccessMask == kamKindOfMask)
    {
      printf("\n%sSpecific bits in default Dacl(s) in token not broken down into defines",ucIndentBitsBuf);
    }
    else
    { printf("\n%sYou will need to write some code (such as that directly",ucIndentBuf);
      printf("\n%s  above the code that wrote out this message) to decode",ucIndentBuf);
      printf("\n%s  this kind of access mask",ucIndentBuf);
    }
  }

  /**************************************************************************\
  *
  * Print then decode the ACCESS_SYSTEM_SECURITY bit
  *
  \**************************************************************************/

  printf("\n%sAccess System Security == 0x%.8x",ucIndentBuf,dwAccessSystemSecurityBit);

  /**************************************************************************\
  *
  * Print then decode the generic rights bits, which will rarely be on
  *
  * Generic bits are nearly always mapped by Windows NT before it tries to do
  *   anything with them.  You can ignore the fact that generic bits are
  *   special in any way, although it helps to keep track of what the mappings
  *   are so that you don't have any surprises
  *
  * The only time the generic bits are not mapped immediately is if they are
  *   placed in an inheritable ACE in an ACL, or in an ACL that will be
  *   assigned by default (such as the default DACL in an access token).  In
  *   that case they're mapped when the child object is created (or when the
  *   default DACL is used at object creation time)
  *
  \**************************************************************************/

  printf("\n%sGeneric  Rights        == 0x%.8x",ucIndentBuf,dwGenericBits);

  if (dwGenericBits)
  {
    if ((dwGenericBits     & GENERIC_READ   ) == GENERIC_READ   )
    { printf("\n%s0x80000000 GENERIC_READ   ",ucIndentBitsBuf);
    }
    if ((dwGenericBits     & GENERIC_WRITE  ) == GENERIC_WRITE  )
    { printf("\n%s0x40000000 GENERIC_WRITE  ",ucIndentBitsBuf);
    }
    if ((dwGenericBits     & GENERIC_EXECUTE) == GENERIC_EXECUTE)
    { printf("\n%s0x20000000 GENERIC_EXECUTE",ucIndentBitsBuf);
    }
    if ((dwGenericBits     & GENERIC_ALL    ) == GENERIC_ALL    )
    { printf("\n%s0x10000000 GENERIC_ALL    ",ucIndentBitsBuf);
    }

    dwExtraBits = dwGenericBits & ( ~( GENERIC_READ
                                     | GENERIC_WRITE
                                     | GENERIC_EXECUTE
                                     | GENERIC_ALL) );
    if (dwExtraBits)
    { printf("\n%sExtra generic bits     == 0x%.8x <-This is a problem, should be all 0s",ucIndentBuf,dwExtraBits);
    }
  }
}

/****************************************************************************\
*
* FUNCTION: LookupSIDName
*
\****************************************************************************/

BOOL LookupSIDName(PSID psidSID, LPTSTR lpszOldIndent)
{
  UCHAR        ucIndentBuf    [SZ_INDENT_BUF]    = "";
  #define                      SZ_ACCT_NAME_BUF  60
  UCHAR        ucNameBuf      [SZ_ACCT_NAME_BUF] = "";
  DWORD        dwNameLength  = SZ_ACCT_NAME_BUF;
  #define                      SZ_DMN_NAME_BUF   60
  UCHAR        ucDomainNmBuf  [SZ_DMN_NAME_BUF]  = "";
  DWORD        dwDNameLength = SZ_DMN_NAME_BUF;
  #define                      SZ_SID_STRING_BUF 150
  UCHAR        ucSIDStringBuf [SZ_SID_STRING_BUF] = "";
  SID_NAME_USE peAcctNameUse = SidTypeInvalid;
  DWORD        dwLookupStatus;
  BOOL         bGotBadLookupThatIsNotLocalLogonSID;

  strcpy(ucIndentBuf,lpszOldIndent);
  strcat(ucIndentBuf,"  ");

  if (!IsValidSid(psidSID))
  { PERR("IsValidSid");
    return(FALSE);
  }

  if (!SIDStringName(psidSID,ucSIDStringBuf))
  { PERR("SIDStringName");
    return(FALSE);
  }

  if (!LookupAccountSid(
         (LPTSTR)"",         // Look on local machine
         psidSID,
         (LPTSTR)&ucNameBuf,
         (LPDWORD)&dwNameLength,
         (LPTSTR)&ucDomainNmBuf,
         (LPDWORD)&dwDNameLength,
         (PSID_NAME_USE)&peAcctNameUse))
  {
    dwLookupStatus = GetLastError();

    /************************************************************************\
    *
    * Got a bad Lookup, so check is SID the Local Logon SID?
    *
    * The problem is that LookupAccountSid api will find all the well-known
    *   SIDs except the Local Logon SID.  The last two sub-authorities are
    *   always different, so to check to see if the SID we're looking at is
    *   the Local Logon SID, we take the psidLogonIdsSid variable we built at
    *   initialization time, and blast into it's last two sub-authorities the
    *   last two sub-authorities that we have.  Then compare for EqualSid
    *
    \************************************************************************/

    // Must have same number of sub authorities

    bGotBadLookupThatIsNotLocalLogonSID = FALSE;  // Assume the best :)

    if ( ( *(GetSidSubAuthorityCount(psidLogonIdsSid))) !=
         ( *(GetSidSubAuthorityCount(psidSID        )))    )
    { // Not same number of sub-authorities, so can't be a match
      bGotBadLookupThatIsNotLocalLogonSID = TRUE;
    }
    else
    {
      // Force the last two sub-authorities to match
      *(GetSidSubAuthority( psidLogonIdsSid, 1 )) =
      *(GetSidSubAuthority( psidSID        , 1 ));
      *(GetSidSubAuthority( psidLogonIdsSid, 2 )) =
      *(GetSidSubAuthority( psidSID        , 2 ));

      /**********************************************************************\
      *
      * EqualPrefixSid could be used instead if we want to blast in all but
      *   the last sub-authority.  For demonstration purposes, as long as we
      *   did one of the two previous assignment statements, we may as well to
      *   the other and use EqualSID
      *
      \**********************************************************************/

      if (EqualSid(psidSID,psidLogonIdsSid))
      { printf("\n%sSID is the Local Logon SID   %s",ucIndentBuf,ucSIDStringBuf);
      }
      else
      { bGotBadLookupThatIsNotLocalLogonSID = TRUE;
      }
    }
    if (bGotBadLookupThatIsNotLocalLogonSID)
    {
      /**********************************************************************\
      *
      * ERROR_NONE_MAPPED means account unknown.  RegEdt32.exe will show
      *   1332-error-type accounts as Account Unknown, so we will also
      *
      \**********************************************************************/

      if (ERROR_NONE_MAPPED == dwLookupStatus)
      { printf("\n%sSID domain == %s, Name == %s    (Account Unknown)  %s",ucIndentBuf,ucDomainNmBuf,ucNameBuf,ucSIDStringBuf);
      }
      else
      { SetLastError(dwLookupStatus);
        PERR("LookupAccountSid");
        return(FALSE);
      }
    }
  }
  else
  { // Got good Lookup, so SID Is NOT the Local Logon SID
    printf("\n%sSID domain == %s, Name == %s   %s",ucIndentBuf,ucDomainNmBuf,ucNameBuf,ucSIDStringBuf);

    /************************************************************************\
    *
    * For demonstration purposes see which well-known SID it might be
    * For demonstration purposes do a silly search demonstrating
    *   no two well-known SIDs are equal
    *
    \************************************************************************/

    if (EqualSid(psidSID,psidNullSid))
    { printf("\n%sSID is the Null SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidWorldSid))
    { printf("\n%sSID is the World SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidLocalSid))
    { printf("\n%sSID is the Local SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidCreatorOwnerSid))
    { printf("\n%sSID is the CreatorOwner SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidCreatorGroupSid))
    { printf("\n%sSID is the CreatorGroup SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidNtAuthoritySid))
    { printf("\n%sSID is the NtAuthority SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidDialupSid))
    { printf("\n%sSID is the DialUp SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidNetworkSid))
    { printf("\n%sSID is the Network SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidBatchSid))
    { printf("\n%sSID is the Batch SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidInteractiveSid))
    { printf("\n%sSID is the Interactive SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidServiceSid))
    { printf("\n%sSID is the Service SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidLocalSystemSid))
    { printf("\n%sSID is the LocalSystem SID",ucIndentBuf);
    }
    if (EqualSid(psidSID,psidBuiltinDomainSid))
    { printf("\n%sSID is the Builtin Domain SID",ucIndentBuf);
    }
  }

  switch (peAcctNameUse)
  { case                      SidTypeUser           :
      printf("\n%sSID type is SidTypeUser"          ,ucIndentBuf);
      break;
    case                      SidTypeGroup          :
      printf("\n%sSID type is SidTypeGroup"         ,ucIndentBuf);
      break;
    case                      SidTypeDomain         :
      printf("\n%sSID type is SidTypeDomain"        ,ucIndentBuf);
      break;
    case                      SidTypeAlias          :
      printf("\n%sSID type is SidTypeAlias"         ,ucIndentBuf);
      break;
    case                      SidTypeWellKnownGroup :
      printf("\n%sSID type is SidTypeWellKnownGroup",ucIndentBuf);
      break;
    case                      SidTypeDeletedAccount :
      printf("\n%sSID type is SidTypeDeletedAccount",ucIndentBuf);
      break;
    case                      SidTypeInvalid        :
      printf("\n%sSID type is SidTypeInvalid"       ,ucIndentBuf);
      break;
    case                      SidTypeUnknown        :
      printf("\n%sSID type is SidTypeUnknown"       ,ucIndentBuf);
      break;
    default                   :
      printf("\n%sSID type is IMPOSSIBLE!!!!  Run debugger, see value!",ucIndentBuf);
      break;
  }
}

/****************************************************************************\
*
* FUNCTION: SIDStringName
*
\****************************************************************************/

BOOL SIDStringName(PSID psidSID, LPTSTR lpszSIDStringName)
{
  /**************************************************************************\
  *
  * Unfortunately there is no api to return the SID Revision, and the number
  *   of bytes in the Identifier Authority must be expressed as a define
  *   (since the == operator won't operate on structures so mempcy has to be
  *   used for the identifier authority compares)
  *
  \**************************************************************************/

  DWORD dwNumSubAuthorities;
  DWORD dwLen;
  DWORD dwSubAuthorityI;
  #define BytesInIdentifierAuthority  6
  SID_IDENTIFIER_AUTHORITY siaSidAuthority;
  SID_IDENTIFIER_AUTHORITY siaNullSidAuthority    = SECURITY_NULL_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaWorldSidAuthority   = SECURITY_WORLD_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaLocalSidAuthority   = SECURITY_LOCAL_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaCreatorSidAuthority = SECURITY_CREATOR_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaNtAuthority         = SECURITY_NT_AUTHORITY;

  dwLen = sprintf(lpszSIDStringName,"S-%d-",SID_REVISION);

  if (SID_REVISION != ((PISID)psidSID)->Revision)
  { dwLen += sprintf(lpszSIDStringName+dwLen,"bad_revision==%d",((PISID)psidSID)->Revision);
  }

  siaSidAuthority = *(GetSidIdentifierAuthority(psidSID));

  if      (0==memcmp(&siaSidAuthority,&siaNullSidAuthority   ,BytesInIdentifierAuthority))
  { dwLen += sprintf(lpszSIDStringName+dwLen,"0");
  }
  else if (0==memcmp(&siaSidAuthority,&siaWorldSidAuthority  ,BytesInIdentifierAuthority))
  { dwLen += sprintf(lpszSIDStringName+dwLen,"1");
  }
  else if (0==memcmp(&siaSidAuthority,&siaLocalSidAuthority  ,BytesInIdentifierAuthority))
  { dwLen += sprintf(lpszSIDStringName+dwLen,"2");
  }
  else if (0==memcmp(&siaSidAuthority,&siaCreatorSidAuthority,BytesInIdentifierAuthority))
  { dwLen += sprintf(lpszSIDStringName+dwLen,"3");
  }
  else if (0==memcmp(&siaSidAuthority,&siaNtAuthority        ,BytesInIdentifierAuthority))
  { dwLen += sprintf(lpszSIDStringName+dwLen,"5");
  }
  else
  { dwLen += sprintf(lpszSIDStringName+dwLen,"UnknownAuthority!");
  }

  dwNumSubAuthorities = (DWORD)( *(GetSidSubAuthorityCount(psidSID)) );

  for (dwSubAuthorityI=0; dwSubAuthorityI<dwNumSubAuthorities; dwSubAuthorityI++)
  { dwLen += sprintf(lpszSIDStringName+dwLen,"-%d",*(GetSidSubAuthority(psidSID,dwSubAuthorityI)));
  }

  return(TRUE);
}

/****************************************************************************\
*
* FUNCTION: ExamineAccessToken
*
\****************************************************************************/

VOID ExamineAccessToken(HANDLE hAccessToken)
{ TOKEN_INFORMATION_CLASS ticInfoClass;
  #define                               SZ_TOK_INFO_BUF  2000
  UCHAR                   ucTokInfoBuf [SZ_TOK_INFO_BUF] = "";
  DWORD                   dwTokInfoBufSz;
  PTOKEN_USER             ptuTokenUser         = (PTOKEN_USER         )&ucTokInfoBuf;
  PTOKEN_GROUPS           ptgTokenGroups       = (PTOKEN_GROUPS       )&ucTokInfoBuf;
  PTOKEN_PRIVILEGES       ptpTokenPrivileges   = (PTOKEN_PRIVILEGES   )&ucTokInfoBuf;
  PTOKEN_OWNER            ptoTokenOwner        = (PTOKEN_OWNER        )&ucTokInfoBuf;
  PTOKEN_PRIMARY_GROUP    ptgTokenPrimaryGroup = (PTOKEN_PRIMARY_GROUP)&ucTokInfoBuf;
  PTOKEN_DEFAULT_DACL     ptdTokenDefaultDacl  = (PTOKEN_DEFAULT_DACL )&ucTokInfoBuf;
  PTOKEN_SOURCE           ptsTokenSource       = (PTOKEN_SOURCE       )&ucTokInfoBuf;
  PTOKEN_TYPE             pttTokenType         = (PTOKEN_TYPE         )&ucTokInfoBuf;
  PSECURITY_IMPERSONATION_LEVEL psilSecurityImpersonationLevel = (PSECURITY_IMPERSONATION_LEVEL)&ucTokInfoBuf;
  PTOKEN_STATISTICS       ptsTokenStatistics   = (PTOKEN_STATISTICS   )&ucTokInfoBuf;
  DWORD                   dwGroupI;
  DWORD                   dwPrivilegeI;
  #define                               SZ_PRIV_INFO_BUF  250
  UCHAR                   ucPrivInfoBuf[SZ_PRIV_INFO_BUF] = "";
  DWORD                   dwPrivInfoBufSz;
  DWORD                   dwExtraBits;
  UCHAR                   ucIndentBitsBuf[SZ_INDENT_BUF]  = "";

  strcpy(ucIndentBitsBuf,"");
  strcat(ucIndentBitsBuf,"                                  ");


  if    (!I_DO_NOT_WANT_THIS_CODE_TO_CLUTTER_THIS_PROGRAM_S_OUTPUT)
  {
    ticInfoClass   = TokenUser;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    printf("\nToken's User SID");
    printf("\n    (this is a SID that is used to compare to SIDs in DACL(s) and SACL(s)");

    if(!LookupSIDName( (*ptuTokenUser).User.Sid,""))
    { PERR("LookupSIDName failed");
    }

    printf("\nToken's User SID Attributes == 0x%.8x",(*ptuTokenUser).User.Attributes);
    printf("\n    These should always be 0 - see \\mstools\\h\\winnt.h right after");
    printf("\n      the defines such as SE_GROUP_LOGON_ID - there are no user");
    printf("\n      attributes yet defined");



    ticInfoClass   = TokenGroups;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    printf("\nToken groups (%d)",(*ptgTokenGroups).GroupCount);
    printf("\n    (these SID(s) also are used to compare to SIDs in DACL(s) and SACL(s)");

    for (dwGroupI=0; dwGroupI<(*ptgTokenGroups).GroupCount; dwGroupI++)
    {
      DWORD dwAttributeBits = (*ptgTokenGroups).Groups[dwGroupI].Attributes;
      printf("\n  Token group (%d)",dwGroupI);

      if(!LookupSIDName( (*ptgTokenGroups).Groups[dwGroupI].Sid,"  "))
      { PERR("LookupSIDName failed");
      }
      printf("\n  Token's group (%d) attributes == 0x%.8x",dwGroupI,dwAttributeBits);

      if (dwAttributeBits)
      {
        if ((dwAttributeBits   & SE_GROUP_MANDATORY         ) == SE_GROUP_MANDATORY         )
        { printf("\n%s0x00000001 SE_GROUP_MANDATORY         ",ucIndentBitsBuf);
        }
        if ((dwAttributeBits   & SE_GROUP_ENABLED_BY_DEFAULT) == SE_GROUP_ENABLED_BY_DEFAULT)
        { printf("\n%s0x00000002 SE_GROUP_ENABLED_BY_DEFAULT",ucIndentBitsBuf);
        }
        if ((dwAttributeBits   & SE_GROUP_ENABLED           ) == SE_GROUP_ENABLED           )
        { printf("\n%s0x00000004 SE_GROUP_ENABLED           ",ucIndentBitsBuf);
        }
        if ((dwAttributeBits   & SE_GROUP_OWNER             ) == SE_GROUP_OWNER             )
        { printf("\n%s0x00000008 SE_GROUP_OWNER             ",ucIndentBitsBuf);
        }
        if ((dwAttributeBits   & SE_GROUP_LOGON_ID          ) == SE_GROUP_LOGON_ID          )
        { printf("\n%s0xC0000000 SE_GROUP_LOGON_ID          ",ucIndentBitsBuf);
        }

        dwExtraBits = dwAttributeBits & ( ~( SE_GROUP_MANDATORY
                                           | SE_GROUP_ENABLED_BY_DEFAULT
                                           | SE_GROUP_ENABLED
                                           | SE_GROUP_OWNER
                                           | SE_GROUP_LOGON_ID) );
        if (0 != dwExtraBits)
        { printf("\n          Extra attribute bits == 0x%.8x <-This is a problem, should be all 0s",dwExtraBits);
        }
      }
    }



    ticInfoClass   = TokenPrivileges;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    printf("\nToken privileges (%d)",(*ptpTokenPrivileges).PrivilegeCount);
    printf("\n  NOTE: Most token privileges are not enabled by default.");
    printf("\n    For example the privilege to reboot or logoff is not.");
    printf("\n    0x00000000 for attributes implies the privilege is not enabled.");
    printf("\n    Use care when enabling privileges.  Enable only those needed,");
    printf("\n      and leave them enabled only for as long as they are needed.");

    for (dwPrivilegeI=0; dwPrivilegeI<(*ptpTokenPrivileges).PrivilegeCount; dwPrivilegeI++)
    {
      LUID  luidTokenLuid   = (*ptpTokenPrivileges).Privileges[dwPrivilegeI].Luid;
      DWORD dwAttributeBits = (*ptpTokenPrivileges).Privileges[dwPrivilegeI].Attributes;

      dwPrivInfoBufSz = SZ_PRIV_INFO_BUF;

      if (!LookupPrivilegeName(NULL,
                               (PLUID)&luidTokenLuid,
                               (LPTSTR)ucPrivInfoBuf,
                               (LPDWORD)&dwPrivInfoBufSz))
      { PERR("LookUpPrivilegeName");
        return;
      }

      printf("\n  Token's privilege (%.2d) name       == %s",dwPrivilegeI,ucPrivInfoBuf);

      printf("\n  Token's privilege (%.2d) attributes == 0x%.8x",dwPrivilegeI,dwAttributeBits);

      if (dwAttributeBits)
      {
        if ((dwAttributeBits   & SE_PRIVILEGE_ENABLED_BY_DEFAULT) == SE_PRIVILEGE_ENABLED_BY_DEFAULT)
        { printf("\n%s     0x00000001 SE_PRIVILEGE_ENABLED_BY_DEFAULT",ucIndentBitsBuf);
        }
        if ((dwAttributeBits   & SE_PRIVILEGE_ENABLED           ) == SE_PRIVILEGE_ENABLED           )
        { printf("\n%s     0x00000002 SE_PRIVILEGE_ENABLED           ",ucIndentBitsBuf);
        }
        if ((dwAttributeBits   & SE_PRIVILEGE_USED_FOR_ACCESS   ) == SE_PRIVILEGE_USED_FOR_ACCESS   )
        { printf("\n%s     0x80000000 SE_PRIVILEGE_USED_FOR_ACCESS   ",ucIndentBitsBuf);
        }

        dwExtraBits = dwAttributeBits & ( ~( SE_PRIVILEGE_ENABLED_BY_DEFAULT
                                           | SE_PRIVILEGE_ENABLED
                                           | SE_PRIVILEGE_USED_FOR_ACCESS ) );
        if (0 != dwExtraBits)
        { printf("\n               Extra attribute bits == 0x%.8x <-This is a problem, should be all 0s",dwExtraBits);
        }
      }
    }



    ticInfoClass   = TokenOwner;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    printf("\nToken's default-owner-SID for created objects");
    printf("\n    (this is NOT a SID that is used to compare to SIDs in DACL(s) and SACL(s)");

    if(!LookupSIDName((*ptoTokenOwner).Owner,""))
    { PERR("LookupSIDName failed");
    }



    ticInfoClass   = TokenPrimaryGroup;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    printf("\nToken's Primary Group SID");
    printf("\n    (Current uses are Posix and Macintosh client support)");

    if(!LookupSIDName((*ptgTokenPrimaryGroup).PrimaryGroup,""))
    { PERR("LookupSIDName failed");
    }



    ticInfoClass   = TokenDefaultDacl;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    if (NULL         == (*ptdTokenDefaultDacl).DefaultDacl)
    { printf("\nToken has a NULL Default DACL explicitly specified (allows all access to");
      printf("\n    Everyone, only on objects that are created where the object's Dacl is");
      printf("\n    assigned by default from this default Dacl in this access token)");
    }
    else
    { printf("\nToken's default-DACL for created objects");
      if(!ExamineACL((*ptdTokenDefaultDacl).DefaultDacl,"",DefaultDaclInAccessTokenAccessMask))
      { PERR("ExamineACL failed");
      }
    }



    ticInfoClass   = TokenSource;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    printf("\nToken's Source");
    printf("\n  Source Name        == %.8s",(*ptsTokenSource).SourceName);
    printf("\n  Source Identifier  == 0x%.8x%.8x",
       (*ptsTokenSource).SourceIdentifier.HighPart,
       (*ptsTokenSource).SourceIdentifier.LowPart);



    ticInfoClass   = TokenType;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    switch (*pttTokenType)
    { case  TokenPrimary       :
        printf("\nToken's Type is TokenPrimary");
        break;
      case  TokenImpersonation :
        printf("\nToken's Type is TokenImpersonation");
        printf("\n    Hence the token's TokenImpersonationLevel can be examined");

        ticInfoClass   = TokenImpersonationLevel;
        dwTokInfoBufSz = SZ_TOK_INFO_BUF;

        if (!GetTokenInformation(hAccessToken,
                                 ticInfoClass,
                                 ucTokInfoBuf,
                                 (DWORD)SZ_TOK_INFO_BUF,
                                 &dwTokInfoBufSz))
        { PERR("GetTokenInformation");
          return;
        }

        switch (*psilSecurityImpersonationLevel)
        { case                     SecurityAnonymous      :
            printf("\n  Token is a SecurityAnonymous impersonation token");
            break;
          case                     SecurityIdentification :
            printf("\n  Token is a SecurityIdentification impersonation token");
            break;
          case                     SecurityImpersonation  :
            printf("\n  Token is a SecurityImpersonation impersonation token");
            break;
          case                     SecurityDelegation     :
            printf("\n  Token is a SecurityDelegation impersonation token");
            break;
          default                   :
            printf("\n  Token is an ILLEGAL KIND OF impersonation token!!! == 0x%.8x",*psilSecurityImpersonationLevel);
            break;
        }

      default                  :
        printf("\nToken's Type is ILLEGAL!!! == 0x%.8x",*pttTokenType);
        break;
    }



    ticInfoClass   = TokenStatistics;
    dwTokInfoBufSz = SZ_TOK_INFO_BUF;

    if (!GetTokenInformation(hAccessToken,
                             ticInfoClass,
                             ucTokInfoBuf,
                             (DWORD)SZ_TOK_INFO_BUF,
                             &dwTokInfoBufSz))
    { PERR("GetTokenInformation");
      return;
    }

    printf("\nToken's Statistics");
    printf("\n  TokenId            == 0x%.8x%.8x",
      (*ptsTokenStatistics).TokenId.HighPart,
      (*ptsTokenStatistics).TokenId.LowPart);
    printf("\n  AuthenticationId   == 0x%.8x%.8x",
      (*ptsTokenStatistics).AuthenticationId.HighPart,
      (*ptsTokenStatistics).AuthenticationId.LowPart);
    printf("\n  ExpirationTime     == (not supported in this release of Windows NT)");
    printf("\n  TokenType          == See token type above");
    printf("\n  ImpersonationLevel == See impersonation level above (only if TokenType is not TokenPrimary)");
    printf("\n  DynamicCharged     == %ld",(*ptsTokenStatistics).DynamicCharged    );
    printf("\n  DynamicAvailable   == %ld",(*ptsTokenStatistics).DynamicAvailable  );
    printf("\n  GroupCount         == %d",(*ptsTokenStatistics).GroupCount        );
    printf("\n  PrivilegeCount     == %d",(*ptsTokenStatistics).PrivilegeCount    );
    printf("\n  ModifiedId         == 0x%.8x%.8x",
      (*ptsTokenStatistics).ModifiedId.HighPart,
      (*ptsTokenStatistics).ModifiedId.LowPart);



    printf("\n\n");

  }
}

/****************************************************************************\
*
* FUNCTION: SetPrivilegeInAccessToken
*
\****************************************************************************/

BOOL SetPrivilegeInAccessToken(VOID)
{
  HANDLE           hProcess;
  HANDLE           hAccessToken;
  LUID             luidPrivilegeLUID;
  TOKEN_PRIVILEGES tpTokenPrivilege;

  hProcess = GetCurrentProcess();
  if (!hProcess)
  { PERR("GetCurrentProcess");
    return(FALSE);
  }

  if (!OpenProcessToken(hProcess,
                        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                        &hAccessToken))
  { PERR("OpenProcessToken");
    return(FALSE);
  }

  /**************************************************************************\
  *
  * Get LUID of SeSecurityPrivilege privilege
  *
  \**************************************************************************/

  if (!LookupPrivilegeValue(NULL,
                            "SeSecurityPrivilege",
                            &luidPrivilegeLUID))
  { PERR("LookupPrivilegeValue");
    printf("\nThe above error means you need to log on as an Administrator");
    return(FALSE);
  }

  /**************************************************************************\
  *
  * Enable the SeSecurityPrivilege privilege using the LUID just
  *   obtained
  *
  \**************************************************************************/

  tpTokenPrivilege.PrivilegeCount = 1;
  tpTokenPrivilege.Privileges[0].Luid = luidPrivilegeLUID;
  tpTokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

  AdjustTokenPrivileges (hAccessToken,
                         FALSE,  // Do not disable all
                         &tpTokenPrivilege,
                         sizeof(TOKEN_PRIVILEGES),
                         NULL,   // Ignore previous info
                         NULL);  // Ignore previous info

  if ( GetLastError() != NO_ERROR )
  { PERR("AdjustTokenPrivileges");
    return(FALSE);
  }

  return(TRUE);
}

/****************************************************************************\
*
* FUNCTION: InitializeWellKnownSIDs
*
\****************************************************************************/

VOID InitializeWellKnownSIDs(VOID)
{
  DWORD dwSidWith0SubAuthorities;
  DWORD dwSidWith1SubAuthority;
  DWORD dwSidWith2SubAuthorities;
  DWORD dwSidWith3SubAuthorities;
  DWORD dwSidWith4SubAuthorities;

  SID_IDENTIFIER_AUTHORITY siaNullSidAuthority    = SECURITY_NULL_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaWorldSidAuthority   = SECURITY_WORLD_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaLocalSidAuthority   = SECURITY_LOCAL_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaCreatorSidAuthority = SECURITY_CREATOR_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY siaNtAuthority         = SECURITY_NT_AUTHORITY;

  //  These SID sizes need to be allocated

  dwSidWith0SubAuthorities = GetSidLengthRequired( 0 );
  dwSidWith1SubAuthority   = GetSidLengthRequired( 1 );
  dwSidWith2SubAuthorities = GetSidLengthRequired( 2 );
  dwSidWith3SubAuthorities = GetSidLengthRequired( 3 );
  dwSidWith4SubAuthorities = GetSidLengthRequired( 4 );

  //  Allocate and initialize the universal SIDs

  psidNullSid         = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidWorldSid        = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidLocalSid        = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidCreatorOwnerSid = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidCreatorGroupSid = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);

  InitializeSid( psidNullSid,         &siaNullSidAuthority,    1 );
  InitializeSid( psidWorldSid,        &siaWorldSidAuthority,   1 );
  InitializeSid( psidLocalSid,        &siaLocalSidAuthority,   1 );
  InitializeSid( psidCreatorOwnerSid, &siaCreatorSidAuthority, 1 );
  InitializeSid( psidCreatorGroupSid, &siaCreatorSidAuthority, 1 );

  *(GetSidSubAuthority( psidNullSid,         0 )) = SECURITY_NULL_RID;
  *(GetSidSubAuthority( psidWorldSid,        0 )) = SECURITY_WORLD_RID;
  *(GetSidSubAuthority( psidLocalSid,        0 )) = SECURITY_LOCAL_RID;
  *(GetSidSubAuthority( psidCreatorOwnerSid, 0 )) = SECURITY_CREATOR_OWNER_RID;
  *(GetSidSubAuthority( psidCreatorGroupSid, 0 )) = SECURITY_CREATOR_GROUP_RID;

  // Allocate and initialize the NT defined SIDs

  psidNtAuthoritySid   = (PSID)LocalAlloc(LPTR,dwSidWith0SubAuthorities);
  psidDialupSid        = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidNetworkSid       = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidBatchSid         = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidInteractiveSid   = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidLogonIdsSid      = (PSID)LocalAlloc(LPTR,dwSidWith3SubAuthorities);
  psidServiceSid       = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidLocalSystemSid   = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);
  psidBuiltinDomainSid = (PSID)LocalAlloc(LPTR,dwSidWith1SubAuthority);

  InitializeSid( psidNtAuthoritySid,   &siaNtAuthority, 0 );
  InitializeSid( psidDialupSid,        &siaNtAuthority, 1 );
  InitializeSid( psidNetworkSid,       &siaNtAuthority, 1 );
  InitializeSid( psidBatchSid,         &siaNtAuthority, 1 );
  InitializeSid( psidInteractiveSid,   &siaNtAuthority, 1 );
  InitializeSid( psidLogonIdsSid,      &siaNtAuthority, 3 );
  InitializeSid( psidServiceSid,       &siaNtAuthority, 1 );
  InitializeSid( psidLocalSystemSid,   &siaNtAuthority, 1 );
  InitializeSid( psidBuiltinDomainSid, &siaNtAuthority, 1 );

  *(GetSidSubAuthority( psidDialupSid,        0 )) = SECURITY_DIALUP_RID;
  *(GetSidSubAuthority( psidNetworkSid,       0 )) = SECURITY_NETWORK_RID;
  *(GetSidSubAuthority( psidBatchSid,         0 )) = SECURITY_BATCH_RID;
  *(GetSidSubAuthority( psidInteractiveSid,   0 )) = SECURITY_INTERACTIVE_RID;
  *(GetSidSubAuthority( psidLogonIdsSid,      0 )) = SECURITY_LOGON_IDS_RID;
  *(GetSidSubAuthority( psidLogonIdsSid,      1 )) = 0; // Bogus!
  *(GetSidSubAuthority( psidLogonIdsSid,      2 )) = 0; // Also bogus!
  *(GetSidSubAuthority( psidServiceSid,       0 )) = SECURITY_SERVICE_RID;
  *(GetSidSubAuthority( psidLocalSystemSid,   0 )) = SECURITY_LOCAL_SYSTEM_RID;
  *(GetSidSubAuthority( psidBuiltinDomainSid, 0 )) = SECURITY_BUILTIN_DOMAIN_RID;
}

/****************************************************************************\
*
* FUNCTION: DisplayHelp
*
\****************************************************************************/

VOID DisplayHelp(VOID)
{
  printf("\nTo run type CHECK_SD and 0 or 1 parameters.  Syntax:");
  printf("\n  CHECK_SD");
  printf("\n      or");
  printf("\n  CHECK_SD filename");
  printf("\n           filename is the name of the file that is passed");
  printf("\n             to GetFileSecurity() to fetch the SD to examine");
  printf("\nExamples:");
  printf("\n  CHECK_SD            Checks the SD on A: (this is the default)");
  printf("\n  CHECK_SD \\\\.\\A:     Checks the SD on A:");
  printf("\n  CHECK_SD d:\\a.fil   Checks the SD on d:\a.fil");
  printf("\n  CHECK_SD A:         Checks the SD on the A: root, but that");
  printf("\n                        is not where the DACL is that controls");
  printf("\n                        access to the floppy, so don't do this");
}
