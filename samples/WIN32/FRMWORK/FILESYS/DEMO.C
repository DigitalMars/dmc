// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995 Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:    Demo.c
//
//  PURPOSE:   Displays the "DEMOTABCTRL" dialog box
//
//  FUNCTIONS:
//    DemoDlgProc      - Processes messages for "DemoTabCtrl" dialog box.
//    MsgDemoInit      - To initialize the about box with version info 
//                       from resources.
//    MsgDemoCommand   - Process WM_COMMAND message sent to the about box.
//    CmdDemoDone      - Free the about box and related data.
//    CmdDemoTabCtrl   - Command messages for the tab control. 
//                       Specifically the TCN_SELCHANGE message.
//    CreateTabControl - Creates the tab control window. 
//    InitTabControl   - Initializes the tab control with tabs
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>
#include "globals.h"            // prototypes specific to this application
#include "demo.h"


// Global variables


// Local function prototypes
LRESULT MsgDemoCommand      (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDemoInit         (HWND, UINT, WPARAM, LPARAM);
LRESULT CmdDemoDone         (HWND, WORD, WORD, HWND);
LRESULT CmdDemoLogDrives    (HWND, WORD, WORD, HWND);
void    ResetDemoDialog		(HWND);


// Demo dialog message table definition.
MSD rgmsdDemo[] =
{
    {WM_COMMAND,     MsgDemoCommand},
    {WM_INITDIALOG,  MsgDemoInit   }
};

MSDI msdiDemo =
{
    sizeof(rgmsdDemo) / sizeof(MSD),
    rgmsdDemo,
    edwpNone
};

// Demo dialog command table definition.
CMD rgcmdDemo[] =
{
    {IDOK,           CmdDemoDone},
    {IDCANCEL,       CmdDemoDone},
    {IDM_LOGDRIVES,  CmdDemoLogDrives}
};

CMDI cmdiDemo =
{
    sizeof(rgcmdDemo) / sizeof(CMD),
    rgcmdDemo,
    edwpNone
};


//
//  FUNCTION: DemoDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Demo" dialog box.
//
//  PARAMETERS:
//    hdlg     - window handle of the dialog box
//    wMessage - type of message
//    wparam   - message-specific information
//    lparam   - message-specific information
//
//  RETURN VALUE:
//    TRUE  - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//
//     Display the File System information inside the dialog box. 
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK DemoDlgProc(HWND   hdlg,
                             UINT   uMessage,
                             WPARAM wparam,
                             LPARAM lparam)
{

    return DispMessage(&msdiDemo, hdlg, uMessage, wparam, lparam);

}


//
//  FUNCTION: MsgDemoCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the Demo box.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses this DipsCommand function defined in wndproc.c combined
//    with the cmdiDemo structure defined in this file to handle
//    the command messages for the Demo dialog box.
//

LRESULT MsgDemoCommand (HWND   hwnd,
                        UINT   uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{

    return DispCommand(&cmdiDemo, hwnd, wparam, lparam);

}


//
//  FUNCTION: MsgDemoInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Demo box with version info from resources.
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//   
//   
//

LRESULT MsgDemoInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    // Obtain all logical drives on the system.
    GetDrivesOnSystem(hdlg, GetDlgItem(hdlg, IDM_LOGDRIVES));

    // Get Volume information. 
//    GetVolumeInfo(hdlg);

    return TRUE;
}


//
//  FUNCTION: CmdDemoDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the Demo box and related data.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session. Also calls Shell_Terminate
//    to uninitialze the shell library. This API is not yet documented. 
//    Also, destroys all the demo dialogs by calling DestroyWindow().
//

LRESULT CmdDemoDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{   
    EndDialog(hdlg, TRUE);
    return TRUE;
}




//
//  FUNCTION: CmdDemoLogDrives(HWND, WORD, HWND)
//
//  PURPOSE: Process the drive change information
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//

LRESULT CmdDemoLogDrives(HWND hdlg, 
                         WORD wCommand, 
                         WORD wNotify, 
                         HWND hwndCtrl)
{
    char  szBuffer[100];
    
    if(wNotify == CBN_SELCHANGE)
    {
        ComboBox_GetText(hwndCtrl, szBuffer, sizeof(szBuffer));
        GetLogDriveInfo(hdlg, szBuffer);    
        GetDiskInfo(hdlg, szBuffer);    
        GetVolumeInfo(hdlg, szBuffer);
    }
    return 0;
}

//
//  FUNCTION: GetDrivesOnSystem(HWND, hWndCtl)
//
//  PURPOSE: Find all the drives on the system
//
//  PARAMETERS:
//    hwnd - The window handle of the dialog.
//    
//    
//
//  RETURN VALUE:
//      Always returns TRUE.
//
//  COMMENTS:
//      This function fills the drives combobox with all the
//      drives (physical and logical) connected to the system.
//
//

LRESULT GetDrivesOnSystem(HWND hdlg, HWND hWndCtl)
{
    char   szLogBuffer[100];
    HANDLE hHeap;
    LPSTR  lpstr, lpSave;
    static DWORD  bytesForDriveStrings;
    static int    nNumDrives, nDriveNum;
    
    // calling GetLogicalDrives() with the second parameter
    // set to NULL, it will return the number of bytes required
    // to allocate for the buffer that will actually hold the 
    // strings. 

/*******************************************************************
THIS API IS EITHER BROKEN IN CHICAGO OR NOT IMPLEMENENTED 
IT RETURNS 0 AS THE RETURN VALUE AND ALSO, IT DOES NOT RETURN
THE NAMES OF THE LOGICAL AND PHYSICAL DRIVES...
********************************************************************/

    bytesForDriveStrings = GetLogicalDriveStrings(0, 
                           (LPSTR)NULL) * sizeof(char);
    if(!bytesForDriveStrings)
    {
        MessageBox(NULL, 
                   "GetLogicalDrives Function Failed",
                   "ERROR",
                   MB_OK);
        return 0;
    }
    else
    {
    // GetLogicalDriveStrings() returned a positive value, so 
    // allocate a buffer to hold the actual strings. 
        hHeap = GetProcessHeap();
        lpstr = (LPSTR) HeapAlloc(hHeap, 
                                  HEAP_ZERO_MEMORY,
                                  bytesForDriveStrings);
		lpSave = lpstr;		// save the pointer so we can free it later
    }

    // Get the strings into our buffer..this time..
    GetLogicalDriveStrings((DWORD)HeapSize(hHeap, 0, lpstr),
                            lpstr);

    while(*lpstr != 0)
    {
        ComboBox_AddString(hWndCtl, lpstr);
        nNumDrives++;                                    
        lpstr = lpstr + lstrlen(lpstr) + 1;
    }

    // We are done with Heap Allocation, so free it. 

    HeapFree(hHeap, 0, lpSave);

    // Get the first fixed drive and get drive information about
    // it, so that we don't try to read volume information from a 
    // drive that does not have a disk. 
    for(nDriveNum = 0; nDriveNum < nNumDrives; nDriveNum++)
    {
        ComboBox_GetLBText(hWndCtl, nDriveNum, szLogBuffer);
        if(GetDriveType(szLogBuffer) == DRIVE_FIXED)
            break;
    }

    if(nDriveNum == nNumDrives)
    {
        // there are no fixed drives...just use the first drive.
        nDriveNum = 0;
        ComboBox_GetLBText(hWndCtl, nDriveNum, szLogBuffer);
    }

    // Select the First fixed drive, or select the first 
    // available drive. 
    ComboBox_SetCurSel(hWndCtl, nDriveNum);

    // Fill the demo dialog, with the firt batch of information...
    GetLogDriveInfo(hdlg, szLogBuffer);    
    GetDiskInfo(hdlg, szLogBuffer);    
    GetVolumeInfo(hdlg, szLogBuffer);

    return 0;

}


//
//  FUNCTION: GetLogDriveInfo(HWND, LPSTR)
//
//  PURPOSE: Fills the statics with drive information
//
//  PARAMETERS:
//    hwnd - The window handle of the dialog.
//    LPSTR - The String name of the drive in question
//    
//
//  RETURN VALUE:
//      Always returns TRUE.
//
//  COMMENTS:
//      This function fills the static controls with the 
//      various information about the drive. 
//
//

LRESULT GetLogDriveInfo(HWND hdlg, LPSTR lpstr)
{           
    int    driveType;
    char   szBuffer[100];


    //obtain the drive type information..
    driveType = GetDriveType(lpstr) ;

    switch(driveType)
    {

            case 0:
                lstrcpy((LPSTR)szBuffer, "DRIVE TYPE") ;
                lstrcat((LPSTR)szBuffer, "\t") ;
                lstrcat((LPSTR)szBuffer, "UNDETERMINABLE") ;
                SetDlgItemText(hdlg, IDM_DRIVETYPE, szBuffer);
            break ;

            case 1:
                lstrcpy((LPSTR)szBuffer, "DRIVE TYPE") ;
                lstrcat((LPSTR)szBuffer, "\t") ;
                lstrcat((LPSTR)szBuffer, "PATH INVALID") ;
                SetDlgItemText(hdlg, IDM_DRIVETYPE, szBuffer);
            break ;

            case DRIVE_FIXED:
                lstrcpy((LPSTR)szBuffer, "DRIVE TYPE") ;
                lstrcat((LPSTR)szBuffer, "\t") ;
                lstrcat((LPSTR)szBuffer, "FIXED DRIVE") ;
                SetDlgItemText(hdlg, IDM_DRIVETYPE, szBuffer);
            break ;

            case DRIVE_REMOVABLE:
                lstrcpy((LPSTR)szBuffer, "DRIVE TYPE") ;
                lstrcat((LPSTR)szBuffer, "\t") ;
                lstrcat((LPSTR)szBuffer, "REMOVABLE") ;
                SetDlgItemText(hdlg, IDM_DRIVETYPE, szBuffer);
            break ;


            default:
            break;

    } //end of switch..


    return 0;


}


//
//  FUNCTION: GetVolumeInfo(HWND, LPSTR)
//
//  PURPOSE: Fills the statics with drive information
//
//  PARAMETERS:
//    hwnd - The window handle of the dialog.
//    LPSTR - The String name of the drive in question
//    
//
//  RETURN VALUE:
//      Always returns TRUE.
//
//  COMMENTS:
//      This function fills the static controls with the 
//      various information about the drive. 
//
//

LRESULT  GetVolumeInfo(HWND  hdlg, LPSTR lpstr) 
{
    // char    szRootPathName[50]; 
    char    szFileSystemName[50],  szVolumeName[50];
    char    szBuffer[50]; 


    
    // DWORD   cbVolumeName, cbFileSystemName;
    DWORD   VolumeSerNum, MaxCompLen, FileSysFlags ;
    BOOL    retVal ;

      
    retVal = GetVolumeInformation(lpstr,
                                  (LPSTR)szVolumeName, 
                                  50, 
                                  &VolumeSerNum, 
                                  &MaxCompLen,
                                  &FileSysFlags, 
                                  (LPSTR)szFileSystemName, 
                                  50);
		  
    if(!retVal) 
    {
        MessageBox(NULL,
                   "GetVolumeInformation Failed:\r\nPlease Check your Drive Letter:", 
                   "ERROR",   
                    MB_OK | MB_ICONSTOP);

		ResetDemoDialog(hdlg);
		return 0;
    }


    // Fill all the static controls with the available information....
    // First fill in the volume name
    lstrcpy((LPSTR)szBuffer, "Volume Name:") ;
    lstrcat((LPSTR)szBuffer, "\t") ;
    lstrcat((LPSTR)szBuffer, (LPSTR)szVolumeName) ;
    SetDlgItemText(hdlg, IDM_VOLNAME, szBuffer);

    // fill in the serial number
    wsprintf((LPSTR)szBuffer, "Serial Number:\t%d",  VolumeSerNum) ;
    SetDlgItemText(hdlg, IDM_SERIALNUM, szBuffer);
    
    // fill in the component length information
    wsprintf((LPSTR)szBuffer, "Component Len:\t%d", MaxCompLen) ;
    SetDlgItemText(hdlg, IDM_COMPLENGTH, szBuffer);

    // fill in the File system name
    wsprintf((LPSTR)szBuffer, "File System:\t%s", (LPSTR)szFileSystemName);
    SetDlgItemText(hdlg, IDM_FILESYSTEM, szBuffer);
		  			
    // fill in the flie system Flags
    if(FileSysFlags & FS_CASE_IS_PRESERVED)
    {
        wsprintf((LPSTR)szBuffer, 
                 "System Flag:\t%s",
                 (LPSTR)"CASE PRESERVED");
        SetDlgItemText(hdlg, IDM_SYSFLAGS, szBuffer);
    }
			
    if(FileSysFlags & FS_CASE_SENSITIVE)
    {
        wsprintf((LPSTR)szBuffer, 
                 "System Flag:\t%s", 
                 (LPSTR)"CASE SENSITIVE");
        SetDlgItemText(hdlg, IDM_SYSFLAGS, szBuffer);
    }
	
    if(FileSysFlags & FS_UNICODE_STORED_ON_DISK)
    {
        wsprintf((LPSTR)szBuffer, 
                 "System Flag:\t%s", 
                 (LPSTR)"UNICODE STORED");
        SetDlgItemText(hdlg, IDM_SYSFLAGS, szBuffer);

    }
		 
    return 0;					
}


//
//  FUNCTION: GetDiskInfo(HWND, LPSTR)
//
//  PURPOSE: Fills the statics with disk information
//
//  PARAMETERS:
//    hwnd - The window handle of the dialog.
//    LPSTR - The String name of the disk in question
//    
//
//  RETURN VALUE:
//      Always returns TRUE.
//
//  COMMENTS:
//      This function fills the static controls with the 
//      various information about the disk. 
//
//


LRESULT GetDiskInfo(HWND hdlg, LPSTR lpstr)
{
    char    szBuffer[100] ;
    DWORD 	dwSectorsPerCluster, dwTotalClusters ;
    DWORD   dwBytesPerSector, 	dwFreeClusters ;
		
   if(GetDiskFreeSpace(lpstr,
                       &dwSectorsPerCluster, 
                       &dwBytesPerSector, 
                       &dwFreeClusters,
                       &dwTotalClusters))
    {
        wsprintf((LPSTR)szBuffer, 
                 "Sectors/Cluster:\t%d", 
                 dwSectorsPerCluster) ;
        SetDlgItemText(hdlg, IDM_SECTORSINCLUSTER, szBuffer);

        wsprintf((LPSTR)szBuffer, 
                 "Bytes/Sector:\t%d", 
                 dwBytesPerSector) ;
        SetDlgItemText(hdlg, IDM_BYTESINSECTOR, szBuffer);

        wsprintf((LPSTR)szBuffer, 
                 "Free Clusters:\t%d", 
                 dwFreeClusters) ;
        SetDlgItemText(hdlg, IDM_FREECLUSTERS, szBuffer);


        wsprintf((LPSTR)szBuffer, "Clusters:\t\t%d", dwTotalClusters) ;
        SetDlgItemText(hdlg, IDM_CLUSTERS, szBuffer);

    }
 
    return 0;
}


void ResetDemoDialog(HWND hdlg)
{

	char szBuffer[50];

	    
    lstrcpy((LPSTR)szBuffer, "Volume Name:");
	SetDlgItemText(hdlg, IDM_VOLNAME, szBuffer);

    // fill in the serial number
    lstrcpy((LPSTR)szBuffer, "Serial Number:");
    SetDlgItemText(hdlg, IDM_SERIALNUM, szBuffer);
    
    // fill in the component length information
    lstrcpy((LPSTR)szBuffer, "Component Len:") ;
    SetDlgItemText(hdlg, IDM_COMPLENGTH, szBuffer);

    // fill in the File system name
    lstrcpy((LPSTR)szBuffer, "File System:\t");
    SetDlgItemText(hdlg, IDM_FILESYSTEM, szBuffer);

	// fill in the File system name		  			
    lstrcpy((LPSTR)szBuffer, "System Flag:");
    SetDlgItemText(hdlg, IDM_SYSFLAGS, szBuffer);

	// fill in the File system name		  			
    lstrcpy((LPSTR)szBuffer, "Sectors/Cluster:");
    SetDlgItemText(hdlg, IDM_SECTORSINCLUSTER, szBuffer);

	// fill in the File system name		  			
    lstrcpy((LPSTR)szBuffer, "Byte/Setor:");
    SetDlgItemText(hdlg, IDM_BYTESINSECTOR, szBuffer);
	
	// fill in the File system name		  			
    lstrcpy((LPSTR)szBuffer, "Free Clusters:");
    SetDlgItemText(hdlg, IDM_FREECLUSTERS, szBuffer);

	// fill in the File system name		  			
    lstrcpy((LPSTR)szBuffer, "Clusters:");
    SetDlgItemText(hdlg, IDM_CLUSTERS, szBuffer);

	
				
}
