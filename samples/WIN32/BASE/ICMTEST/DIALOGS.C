// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright © 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
//  PROGRAM:  dialogs.c
//
//  PURPOSE:  Processes all user input provided to the ICMTEST dialog
//            boxes.
//
//  PLATFORMS:  Chicago
//
//

#include <windows.h>    
#include <string.h>
#include <stdlib.h>

#include "icmtest.h"    
#include "dialogs.h"    
#include "print.h"
#include "resource.h"   



// ************************************************************************
//
//  Global vars external to this module
//
// ************************************************************************
extern HINSTANCE              ghInst;      
extern char                   gszICM_INIFile[MAX_CHAR];
extern BOOL                   gfExistenceCheck;
extern UINT                   guiColorSpacesUsed;
extern HCOLORSPACE            ghlcsCurrent;
extern COLORSPACECONTROLENTRY galcsControl[MAX_COLORSPACES];
extern int                    giDCType;

// ************************************************************************
//
//  Global vars local to this module
//
// ************************************************************************

// ************************************************************************
//
//   Functions private to this module
//
// ************************************************************************

void  DisplayColorSpaceInfo(HWND hDlg, LOGCOLORSPACE lcsColorSpace);
void  DisplayDwordAsHex( HWND hDlg, int iControlId, DWORD dwHexVal);
void  InitLCS(LPLOGCOLORSPACE lplcs);
BOOL  AddCSToArray(HWND hDlg, int iControlId, char szName[]);
BOOL  DeleteCSFromArray(HWND hDlg, int iControlId, LRESULT lColorSpaceData);
DWORD GetDlgItemDword(HWND hDlg, int iControlId);
void  Update_LCS(HWND hDlg, LONG lColorSpace);


LRESULT CALLBACK CmdDeviceProfile(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT MsgDeviceProfileInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDeviceProfileCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdDeviceProfileDone(HWND, UINT, WPARAM, LPARAM);

LRESULT MsgDefineColorSpaceInit(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam);

int CALLBACK  EnumICMProfilesCallback(LPCSTR lpszFilename, LPARAM lParam);


// ************************************************************************
//
// FUNCTION: dlgprcAbout(HWND, unsigned, WORD, LONG)
//
// PURPOSE:  
//    Processes messages for "About" dialog box
//
// PARAMETERS:
//    HWND      Handle to the dialog.
//    unsigned  Message identifier.
//    WPARAM
//    LPARAM
//
//  MESSAGES:
//
//  WM_INITDIALOG - initialize dialog box
//  WM_COMMAND    - Input received
//
// COMMENTS:
//
//  No initialization is needed for this particular dialog box, but TRUE
//  must be returned to Windows.
//
//  Wait for user to click on "Ok" button, then close the dialog box.
//
// ************************************************************************

BOOL FAR PASCAL dlgprcAbout(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_INITDIALOG:     
      return(TRUE);

    case WM_COMMAND:    
      if (wParam == IDOK || wParam == IDCANCEL)
      {
        EndDialog(hDlg, TRUE);  // Exits the dialog box   
        return(TRUE);
      }
      break;
  }
  return(FALSE);     // Didn't process a message   
}


// ************************************************************************
//
//  FUNCTION: DefineColorSpace
//
//  PURPOSE:
//      Processes messages for "DefineColorSpace" dialog box
//
//  PARAMETERS:
//      HWND      Handle to the dialog.
//      unsigned  Message identifier.
//      WPARAM
//      LPARAM
//
//  MESSAGES:
//
//    WM_INITDIALOG - initialize dialog box
//    WM_COMMAND    - Input received
//
//  COMMENTS:
//
//    No initialization is needed for this particular dialog box, but TRUE
//    must be returned to Windows.
//
//    Wait for user to click on "Ok" button, then close the dialog box.
//
//  RETURN VALUE:  
//      !
//
//  COMMENTS:
//      !
//
//  HISTORY
//    02/11/1994    TDM  
//      Created.
//
// ************************************************************************

BOOL FAR PASCAL dlgprcDefineColorSpace(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)
{
  static  LRESULT lColorSpaceIdx;
  static  LRESULT lColorSpaceData;
  WORD            wmEvent;
  WORD            wmId;
  HCOLORSPACE     hcsTemp;
  FARPROC         lpProcAddLCS = (FARPROC)NULL;
  char            szNewLCS[MAX_FRIENDLY_NAME+1];
  LPSTR           lpszNewLCS;


#if defined (WIN32)
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
#else
        wmId = wParam;
        wmEvent = HIWORD(lParam);
#endif

  switch (message)
  {
    case WM_INITDIALOG:     // message: initialize dialog box   
      lColorSpaceData = MsgDefineColorSpaceInit(hDlg, message, wParam, lParam);
      return(TRUE);
      break;
    
    case WM_COMMAND:    // message: received a command   

      switch(wmId)
      {
        case IDC_DEFINE_CS_REFERENCE_NAME:
          switch(wmEvent)
          {
            case CBN_SELCHANGE:
              Update_LCS(hDlg, (LONG)lColorSpaceData);
              GetDlgItemText(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, gszColorSpace, MAX_CHAR);
              lColorSpaceIdx = SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)(LPCSTR)gszColorSpace);
              if (lColorSpaceIdx != CB_ERR)
              {
                lColorSpaceData = SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_GETITEMDATA, (WPARAM)lColorSpaceIdx, (LPARAM)0);
                if( lColorSpaceData != CB_ERR ) // found match   
                {
                  DisplayColorSpaceInfo( hDlg, galcsControl[lColorSpaceData].lcs);
                  if( galcsControl[lColorSpaceData].hlcs == (HCOLORSPACE)NULL)
                  {   
                      //
                      // Handle doesn't exist, so we should be able   
                      // to create, but not set or delete.
                      //
                    if(gfExistenceCheck)
                    {
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_REMOVE_LCS), TRUE);
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), TRUE);
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_SET_CURRENT), FALSE);
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), FALSE);
                    }
                  }
                  else 
                  {   // Handle exists, so we should be able to set   
                      // and delete, but not create                   
                    if(gfExistenceCheck)
                    {
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_REMOVE_LCS), FALSE);
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), FALSE);
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_SET_CURRENT), TRUE);
                      EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), TRUE);
                    }
                  }
                } 
                else 
                { 
                  //
                  // Strange error--found matching string, but no associated data   
                  //
                  ErrMsg("Combobox string has no associated data!");
                  EndDialog(hDlg, TRUE);
                }
                return(TRUE);
              }
              else  // Changed focus from combobox with unrecognized name   
              {
                ErrMsg("Couldn't get index value of current string \"%s\".", gszColorSpace);
                return(TRUE);
              }
              break;
            default:
              break;  // WM_event
          }
          break;

        case IDOK:
          Update_LCS(hDlg, (LONG)lColorSpaceData);
          EndDialog(hDlg, TRUE);  // Exits the dialog box   
          return(TRUE);
          break;

        case IDCANCEL:
          EndDialog(hDlg, TRUE);  // Exits the dialog box   
          return(TRUE);
          break;

        case IDC_DEFINE_CS_REMOVE_LCS:
          DeleteCSFromArray(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, lColorSpaceData);
          LoadCSArrayIntoDlg(hDlg,IDC_DEFINE_CS_REFERENCE_NAME);
          lColorSpaceData = FindCurrentCSInArray(hDlg, IDC_DEFINE_CS_REFERENCE_NAME);
          return(TRUE);
          break;
        
        
        case IDC_DEFINE_CS_ADD_LCS:
          Update_LCS(hDlg, (LONG)lColorSpaceData);
          lpProcAddLCS = (FARPROC)&dlgprcAddLCS;
          lpszNewLCS=(LPSTR)&szNewLCS;
          DialogBoxParam(ghInst, "DLG_CREATE_LCS", hDlg, (DLGPROC)lpProcAddLCS, (LPARAM)(LPSTR)lpszNewLCS);
          FreeProcInstance(lpProcAddLCS);
          if( lstrcmp(lpszNewLCS,(LPSTR)"") == 0)  // Operation was cancelled, don't add anything
            return(TRUE);
          lColorSpaceIdx = SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)lpszNewLCS);
          if(lColorSpaceIdx == CB_ERR)  // Didn't find one of this name already
          {
            if(AddCSToArray(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, szNewLCS))
            {
              lColorSpaceData = guiColorSpacesUsed;
              Update_LCS(hDlg, lColorSpaceData);
              SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_SELECTSTRING, (WPARAM)0, (LPARAM)(LPSTR)lpszNewLCS);
              DisplayColorSpaceInfo( hDlg, galcsControl[guiColorSpacesUsed].lcs);
              if( galcsControl[guiColorSpacesUsed].hlcs == (HCOLORSPACE)NULL)
              {   // Handle doesn't exist, so we should be able   
                  // to create, but not set or delete,            
                if(gfExistenceCheck)
                {
                  EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), TRUE);
                  EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_SET_CURRENT), FALSE);
                  EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), FALSE);
                }
              }
              else
              {   // Handle exists, so we should be able to set   
                  // and delete, but not create                   
                if(gfExistenceCheck)
                {
                  EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), FALSE);
                  EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_SET_CURRENT), TRUE);
                  EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), TRUE);
                }
              }
              guiColorSpacesUsed++;
            }
          }
          else
          {
            ErrMsg("Color Space %s already exists.  Try again.", lpszNewLCS);
          }
          return(TRUE);
          break;

        case IDC_DEFINE_CS_CREATE_CS:
              // Create a handle to the defined logical color space   
        
          if( galcsControl[lColorSpaceData].hlcs != (HCOLORSPACE)NULL)
            ErrMsg("Handle to this CS already exists");
          else
          {
            hcsTemp = CreateColorSpace((LPLOGCOLORSPACE)&galcsControl[lColorSpaceData].lcs);
            if(hcsTemp == (HCOLORSPACE)NULL)
              ErrMsg("CreateColorSpace returned NULL");
          }
          if( hcsTemp != (HCOLORSPACE)NULL)
          {
            galcsControl[lColorSpaceData].hlcs = hcsTemp;
            if (gfExistenceCheck) 
            {   // Turn off ability to create handle to same CS   
              EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), FALSE);
              EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_SET_CURRENT), TRUE);
              EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), TRUE);
            }
          }
          return(TRUE);
          break;

        case IDC_DEFINE_CS_DELETE_CS:
              // Delete a handle to the defined logical color space   
          if (DeleteColorSpace(galcsControl[lColorSpaceData].hlcs))
          {
            galcsControl[lColorSpaceData].hlcs = (HCOLORSPACE)NULL;
            if (gfExistenceCheck) 
            {   // Turn on ability to create handle for this CS   
              EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), TRUE);
              EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), FALSE);
            }
          }
          else
          {
            ErrMsg("DeleteColorSpace returned FALSE!");
          }
          return(TRUE);
          break;


        case IDC_DEFINE_CS_SET_CURRENT:
          if( galcsControl[lColorSpaceData].hlcs != (HCOLORSPACE)NULL)
          {
            ghlcsCurrent = galcsControl[lColorSpaceData].hlcs;
            EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_SET_CURRENT), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), TRUE);
          }
          else
            ErrMsg("Allowed attempt to set current CS to NULL");
          return(TRUE);
          break;
        
          
        default:
          break; // switch (wmid)
      } // case WM_COMMAND
    default:
      break;     // switch(message)
  }
  return(FALSE);     // Didn't process a message   
}



// ************************************************************************
//
//  FUNCTION: MsgDefineColorSpaceInit
//
//  PURPOSE:
//    To perform the initialization of the Define ColorSpace dialog
//
//  PARAMETERS:
//      HWND      Handle to the dialog.
//      unsigned  Message identifier.
//      WPARAM
//      LPARAM
//
//  RETURN VALUE:  
//    Data associated with item in combobox which matches the current
//    colorspace.
//
//  COMMENTS:
//    !
//
//  HISTORY:
//    07/14/1994    TDM  
//      Created.
//
// ************************************************************************
LRESULT MsgDefineColorSpaceInit(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)
{
  LRESULT   lColorSpaceData;  //Data associated with item in combobox
  
  //
  //  Set maximum length of all controls which take user input
  // 
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_RED_X, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_RED_Y, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_RED_Z, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_GREEN_X, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_GREEN_Y, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_GREEN_Z, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_BLUE_X, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_BLUE_Y, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_BLUE_Z, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_GAMMA_RED, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_GAMMA_GREEN, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_GAMMA_BLUE, EM_LIMITTEXT, (WPARAM)10, 0L);
  SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_LIMITTEXT, (WPARAM)MAX_FRIENDLY_NAME, 0L);
  
  //
  // Obtain and display possible color spaces in combobox 
  // by reading the control array                         
  //
  LoadCSArrayIntoDlg(hDlg, IDC_DEFINE_CS_REFERENCE_NAME);
      
  //
  // Use ghlcsCurrent to find matching combobox entry 
  //
  lColorSpaceData = FindCurrentCSInArray(hDlg, IDC_DEFINE_CS_REFERENCE_NAME);
  
  //
  // Since we're getting the current colorspace, disable the 
  // "Set as Current" button
  //
  EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_SET_CURRENT), FALSE);

  return(lColorSpaceData);
}



// ************************************************************************
//
// FUNCTION: DisplayColorSpaceInfo
//
// PURPOSE:
//    To display all of the information associated with a color space.
//
// PARAMETERS:
//    HWND            Handle to dialog.
//    LOGCOLORSPACE   Logical colorspace to be displayed.
//
// RETURN VALUE:  
//    None.
//
// COMMENTS:
//    Pretty straightforward--just dump the structure into the dialog
//    box controls.  Put the round pegs in the round holes, etc.
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

void DisplayColorSpaceInfo(HWND hDlg, LOGCOLORSPACE lcsColorSpace)
{
   DisplayDwordAsHex( hDlg, IDC_DEFINE_CS_VERSION, lcsColorSpace.lcsVersion);
   DisplayDwordAsHex( hDlg, IDC_DEFINE_CS_IDENTITY, lcsColorSpace.lcsCSType);
   DisplayDwordAsHex( hDlg, IDC_DEFINE_CS_GAMUT, lcsColorSpace.lcsIntent);
   DisplayDwordAsHex( hDlg, IDC_DEFINE_CS_SIZE, lcsColorSpace.lcsSize);

   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_RED_X, lcsColorSpace.lcsEndpoints.ciexyzRed.ciexyzX);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_RED_Y, lcsColorSpace.lcsEndpoints.ciexyzRed.ciexyzY);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_RED_Z, lcsColorSpace.lcsEndpoints.ciexyzRed.ciexyzZ);

   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_GREEN_X, lcsColorSpace.lcsEndpoints.ciexyzGreen.ciexyzX);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_GREEN_Y, lcsColorSpace.lcsEndpoints.ciexyzGreen.ciexyzY);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_GREEN_Z, lcsColorSpace.lcsEndpoints.ciexyzGreen.ciexyzZ);

   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_BLUE_X, lcsColorSpace.lcsEndpoints.ciexyzBlue.ciexyzX);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_BLUE_Y, lcsColorSpace.lcsEndpoints.ciexyzBlue.ciexyzY);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_BLUE_Z, lcsColorSpace.lcsEndpoints.ciexyzBlue.ciexyzZ);

   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_GAMMA_RED, lcsColorSpace.lcsGammaRed);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_GAMMA_GREEN, lcsColorSpace.lcsGammaGreen);
   DisplayDwordAsHex(hDlg, IDC_DEFINE_CS_GAMMA_BLUE, lcsColorSpace.lcsGammaBlue);

   SetDlgItemText(hDlg, IDC_DEFINE_CS_PROFILE, lcsColorSpace.lcsFilename);
}





// ************************************************************************
//
// FUNCTION: DisplayDwordAsHex
//
// PURPOSE:
//    To display a DWORD value as a hex string preceded by '0x' and padded
//    to 8 digits.
//
// PARAMETERS:
//    HWND    Handle to the dialog.
//    int     Control identifier.
//    DWORD   Hex Value to convert
//
// RETURN VALUE:  
//
//  None.
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

void DisplayDwordAsHex( HWND hDlg, int iControlId, DWORD dwHexVal)
{
  char  szHexVal[20];

  wsprintf( szHexVal, (LPSTR)"0x%08lX", dwHexVal);
  SetDlgItemText( hDlg, iControlId, (LPSTR)szHexVal);

}




// ************************************************************************
//
// FUNCTION: LoadCSArrayIntoDlg
//
// PURPOSE:
//    To load the information from the Color Space control array into the
//    combo box of the DefineColorSpace dialog.
//
// PARAMETERS:
//    HWND  Handle to the dialog
//    int   Identifier associated witht the combobox
//
// RETURN VALUE:  
//    TRUE  if all was successful.
//    FALSE otherwise.
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

BOOL LoadCSArrayIntoDlg(HWND hDlg, int iControlId)
{
  UINT  uiCount;
  LRESULT lResult;

  // Reset contents of combobox
  lResult = SendDlgItemMessage(hDlg, iControlId, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0L);
  
  // Loop through defined color spaces until we hit max or an empty entry
  uiCount=0;
  while( (strcmp("", galcsControl[uiCount].szName) != 0) && (uiCount < MAX_COLORSPACES))
  {
    lResult = SendDlgItemMessage(hDlg, iControlId, CB_ADDSTRING, 0, (LPARAM)(LPSTR)galcsControl[uiCount].szName);
    if (lResult == CB_ERR || lResult == CB_ERRSPACE)
    {
      ErrMsg("LoadCSArrayIntoDlg:  Couldn't add %u:%s to combobox", uiCount, galcsControl[uiCount].szName);
      return(FALSE);
    }
    lResult = SendDlgItemMessage(hDlg, iControlId, CB_SETITEMDATA, (WPARAM)lResult, (LPARAM)(DWORD)uiCount);
    if (lResult == CB_ERR )
    {
      ErrMsg("LoadCSArrayIntoDlg:  Couldn't set item data %u:%s in combobox", uiCount, galcsControl[uiCount].szName);
      return(FALSE);
    }
    uiCount++;
  }
  return(TRUE);
}
    



// ************************************************************************
//
// FUNCTION: GetPrivateProfileDword
//
// PURPOSE:
//    To get a DWORD from an INI file.
//
// PARAMETERS:
//    LPSTR   Name of section in INI file.
//    LPSTR   Name of entry.
//    DWORD   Default value
//    LPSTR   Name of INI file.
//
// RETURN VALUE:  
//    DWORD value of requested entry.
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

DWORD GetPrivateProfileDword(LPSTR lpszSection, LPSTR lpszEntry, DWORD dwDefault, LPSTR lpszFilename)
{
  char szTemp[MAX_CHAR];
  DWORD dwKeyVal;
  
  strcpy(szTemp,"");
  GetPrivateProfileString(lpszSection, lpszEntry, "", (LPSTR)szTemp, sizeof(szTemp), gszICM_INIFile);

  if (strcmp(szTemp, "") == 0)
  {
    wsprintf(szTemp,"%lu", dwDefault);
  }
  dwKeyVal=strtoul(szTemp, NULL, 16);
  return(dwKeyVal);
}



/// ***************************************************************************  
//
// FUNCTION: InitLCS
//
// PURPOSE:
//    To initialize a logical color space to all zeroes.
//
// PARAMETERS:
//    LPLOGCOLORSPACE   Pointer to Logical Color Space to initialize.
//
// RETURN VALUE:  
//    none
//
// COMMENTS:
//    Straight-up code.  Set everything in sight to 0 or NULL.
//
// HISTORY
//  01/26/1994
//  TDM          Created.
//  10/20/1994
//  TDM          Added lcsSignature field
/// ***************************************************************************  

void InitLCS(LPLOGCOLORSPACE lplcs)
{
  lplcs->lcsSignature = (DWORD)0x50534F43;
  lplcs->lcsVersion = (DWORD)0;
  lplcs->lcsSize = (DWORD)0;
  lplcs->lcsCSType = (DWORD)0;
  lplcs->lcsIntent = (DWORD)0;
  lplcs->lcsFilename[0]=0;

  lplcs->lcsGammaRed = (DWORD)0;
  lplcs->lcsGammaGreen = (DWORD)0;
  lplcs->lcsGammaBlue = (DWORD)0;
  
  lplcs->lcsEndpoints.ciexyzRed.ciexyzX = (DWORD)0;
  lplcs->lcsEndpoints.ciexyzRed.ciexyzY = (DWORD)0;
  lplcs->lcsEndpoints.ciexyzRed.ciexyzZ = (DWORD)0;

  lplcs->lcsEndpoints.ciexyzGreen.ciexyzX = (DWORD)0;
  lplcs->lcsEndpoints.ciexyzGreen.ciexyzY = (DWORD)0;
  lplcs->lcsEndpoints.ciexyzGreen.ciexyzZ = (DWORD)0;

  lplcs->lcsEndpoints.ciexyzBlue.ciexyzX = (DWORD)0;
  lplcs->lcsEndpoints.ciexyzBlue.ciexyzY = (DWORD)0;
  lplcs->lcsEndpoints.ciexyzBlue.ciexyzZ = (DWORD)0;

}



// ************************************************************************
//
// FUNCTION: ReadCSArrayFromINI
//
// PURPOSE:
//    To initialize all members of the array of color spaces.  This en!
//    reading the INI file to get the names of all of the color spaces
//    setting the handles to NULL, and filling in the array's unused 
//    entries.
//
// PARAMETERS:
//    None.
//
// RETURN VALUE:  
//    None.
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

void ReadCSArrayFromINI()
{
  UINT  uiCount;
  char  szCSName[MAX_FRIENDLY_NAME+1];
  char  szCSIndex[MAX_FRIENDLY_NAME+1];
  DWORD dwChars;                        // Number of chars returned by 
                                        // GetPrivateProfileString

  // Position zero should have been filled at application initialization
  // time.  InitInstance should get the default colorspace from the default
  // device profile and fill in the appropriate values at that time.  This
  // function will fill in user-defined spaces starting from position 1 in
  // the array.
  
  for( uiCount = 1; uiCount < MAX_COLORSPACES ; uiCount++)
  {
    wsprintf( szCSIndex,(LPSTR)"ColorSpace%u",uiCount);
    dwChars = GetPrivateProfileString( (LPSTR)"ColorSpaces",(LPSTR)szCSIndex, 
          (LPSTR)"", (LPSTR)szCSName, sizeof(szCSName), gszICM_INIFile);
    galcsControl[uiCount].hlcs = (HCOLORSPACE)NULL;
    
    if (strlen(szCSName) > MAX_FRIENDLY_NAME)
      ErrMsg("ReadCSArrayFromINI: Friendly name too long - [%s]", szCSName);
    else
      strcpy( galcsControl[uiCount].szName, szCSName);
    if(strcmp(szCSName, "") != 0)
    {
      ReadLCSFromINI( &galcsControl[uiCount].lcs, szCSName);
      guiColorSpacesUsed = uiCount+1;
    }
  }
}



// ************************************************************************
//
// FUNCTION: AddCSToArray
//
// PURPOSE:
//    To add a newly named color space to the ColorSpace control structure.
//
// PARAMETERS:
//    char[]   Pointer to the name of the new ColorSpace
//
// RETURN VALUE:  
//    TRUE  if successful
//    FALSE otherwise
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

BOOL  AddCSToArray(HWND hDlg, int iControlId, char szName[])
{
  LRESULT lResult;
  
  if (guiColorSpacesUsed < (MAX_COLORSPACES -1))
  {
    InitLCS(&galcsControl[guiColorSpacesUsed].lcs);
    galcsControl[guiColorSpacesUsed].hlcs = (HCOLORSPACE)NULL;
    strcpy( galcsControl[guiColorSpacesUsed].szName, szName);

    lResult = SendDlgItemMessage(hDlg, iControlId, CB_ADDSTRING, 0, (LPARAM)(LPSTR)szName);
    if (lResult == CB_ERR || lResult == CB_ERRSPACE)
    {
      ErrMsg("LoadCSArrayIntoDlg:  Couldn't add %s to combobox", szName);
      return(FALSE);
    }
    lResult = SendDlgItemMessage(hDlg, iControlId, CB_SETITEMDATA, (WPARAM)lResult, (LPARAM)(DWORD)guiColorSpacesUsed);
    if (lResult == CB_ERR )
    {
      ErrMsg("LoadCSArrayIntoDlg:  Couldn't set item data %u:%s in combobox", guiColorSpacesUsed, szName);
      return(FALSE);
    }
    return(lResult != CB_ERR);
  }
  else
  {
    ErrMsg("Cannot exceed %u defined Color Spaces.  Redefine MAX_COLOR_SPACES and recompile.", guiColorSpacesUsed);
    return(FALSE);
  }
  return(TRUE);
}

// ************************************************************************
//
// FUNCTION: DeleteCSFromArray
//
// PURPOSE:
//    To delete a color space from the control structure.
//
// PARAMETERS:
//    char[]  pointer to name of Color Space to delete!
//    UINT    index to item to delete
//
// RETURN VALUE:  
//    None
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

BOOL DeleteCSFromArray(HWND hDlg, int iControlId, LRESULT lColorSpaceData)
{
  LRESULT lResult;
  
  if (galcsControl[lColorSpaceData].hlcs != (HCOLORSPACE)NULL)
    return(FALSE);
  strcpy(galcsControl[lColorSpaceData].szName, galcsControl[guiColorSpacesUsed].szName);
  galcsControl[lColorSpaceData].hlcs=galcsControl[guiColorSpacesUsed].hlcs;
  galcsControl[lColorSpaceData].lcs=galcsControl[guiColorSpacesUsed].lcs;
  guiColorSpacesUsed--;
  lResult = SendDlgItemMessage(hDlg, iControlId, CB_SETITEMDATA, (UINT)lColorSpaceData, (LPARAM)(DWORD) lColorSpaceData);
  if (lResult == CB_ERR)
  {
    ErrMsg("DeleteCSFromArray:  Couldn't set item data");
    return(FALSE);
  }
  return(TRUE);
}


// ************************************************************************
//
// FUNCTION: ReadLCSFromINI
//
// PURPOSE:
//    To read the values associated with a logical color space from the!
//    ICMTEST.INI file.
//
// PARAMETERS:
//    LPLOGCOLORSPACE Color Space to assign values.
//    PSTR            Name of Color Space.
//
// RETURN VALUE:  
//    !
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

void ReadLCSFromINI(LPLOGCOLORSPACE lplcs, char szName[])
{

  GetPrivateProfileString( (LPSTR)szName,(LPSTR)"FileName", (LPSTR)"",
     (LPSTR)lplcs->lcsFilename, sizeof(lplcs->lcsFilename), gszICM_INIFile);
  lplcs->lcsSignature = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"Signature", (DWORD)0x50534F43, gszICM_INIFile);
  lplcs->lcsVersion = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"Version", (DWORD)0x0400, gszICM_INIFile);
  lplcs->lcsSize = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"Size", sizeof(LOGCOLORSPACE), gszICM_INIFile);
  lplcs->lcsCSType = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"Identity", LCS_CALIBRATED_RGB, gszICM_INIFile);
  lplcs->lcsIntent = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"GamutMatch", 0x0001, gszICM_INIFile);

  lplcs->lcsGammaRed = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"GammaRed", 0x001, gszICM_INIFile);
  lplcs->lcsGammaGreen = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"GammaGreen", 0x001, gszICM_INIFile);
  lplcs->lcsGammaBlue = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"GammaBlue", 0x001, gszICM_INIFile);
  
  lplcs->lcsEndpoints.ciexyzRed.ciexyzX = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"RedX", 0x0000, gszICM_INIFile);
  lplcs->lcsEndpoints.ciexyzRed.ciexyzY = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"RedY", 0x0000, gszICM_INIFile);
  lplcs->lcsEndpoints.ciexyzRed.ciexyzZ = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"RedZ", 0x0000, gszICM_INIFile);

  lplcs->lcsEndpoints.ciexyzGreen.ciexyzX = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"GreenX", 0x0000, gszICM_INIFile);
  lplcs->lcsEndpoints.ciexyzGreen.ciexyzY = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"GreenY", 0x0000, gszICM_INIFile);
  lplcs->lcsEndpoints.ciexyzGreen.ciexyzZ = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"GreenZ", 0x0000, gszICM_INIFile);

  lplcs->lcsEndpoints.ciexyzBlue.ciexyzX = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"BlueX", 0x0000, gszICM_INIFile);
  lplcs->lcsEndpoints.ciexyzBlue.ciexyzY = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"BlueY", 0x0000, gszICM_INIFile);
  lplcs->lcsEndpoints.ciexyzBlue.ciexyzZ = GetPrivateProfileDword((LPSTR)szName, (LPSTR)"BlueZ", 0x0000, gszICM_INIFile);

}


// ************************************************************************
//
// FUNCTION: WriteLCSToINI
//
// PURPOSE:
//    To write the contents of a logical color space to the ICMTEST.INI
//    file.
//
// PARAMETERS:
//    LPSTR         Name of section in ICMTEST.INI
//    LOGCOLORSPACE Color space which will be written.
//
// RETURN VALUE:  
//    none
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

void WriteLCSToINI(LPSTR lpszSection, LOGCOLORSPACE lcs)
{
  char  szEntryVal[MAX_CHAR];
  int   iSize;

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsSignature);
  WritePrivateProfileString( lpszSection, (LPSTR)"Signature", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsVersion);
  WritePrivateProfileString( lpszSection, (LPSTR)"Version", szEntryVal, gszICM_INIFile);

  strcpy(szEntryVal, lcs.lcsFilename);
  WritePrivateProfileString( lpszSection, (LPSTR)"FileName", szEntryVal, gszICM_INIFile);

  iSize = sizeof(LOGCOLORSPACE);
  wsprintf(szEntryVal, "0x%08lX", (DWORD)iSize);
  WritePrivateProfileString( lpszSection, (LPSTR)"Size", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsCSType);
  WritePrivateProfileString( lpszSection, (LPSTR)"Identity", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsIntent);
  WritePrivateProfileString( lpszSection, (LPSTR)"GamutMatch", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsGammaRed);
  WritePrivateProfileString( lpszSection, (LPSTR)"GammaRed", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsGammaGreen);
  WritePrivateProfileString( lpszSection, (LPSTR)"GammaGreen", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsGammaBlue);
  WritePrivateProfileString( lpszSection, (LPSTR)"GammaBlue", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzRed.ciexyzX);
  WritePrivateProfileString( lpszSection, (LPSTR)"RedX", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzRed.ciexyzY);
  WritePrivateProfileString( lpszSection, (LPSTR)"RedY", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzRed.ciexyzZ);
  WritePrivateProfileString( lpszSection, (LPSTR)"RedZ", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzGreen.ciexyzX);
  WritePrivateProfileString( lpszSection, (LPSTR)"GreenX", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzGreen.ciexyzY);
  WritePrivateProfileString( lpszSection, (LPSTR)"GreenY", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzGreen.ciexyzZ);
  WritePrivateProfileString( lpszSection, (LPSTR)"GreenZ", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzBlue.ciexyzX);
  WritePrivateProfileString( lpszSection, (LPSTR)"BlueX", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzBlue.ciexyzY);
  WritePrivateProfileString( lpszSection, (LPSTR)"BlueY", szEntryVal, gszICM_INIFile);

  wsprintf(szEntryVal, "0x%08lX", lcs.lcsEndpoints.ciexyzBlue.ciexyzZ);
  WritePrivateProfileString( lpszSection, (LPSTR)"BlueZ", szEntryVal, gszICM_INIFile);
}  



// ************************************************************************
//
// FUNCTION: WriteLCSArraytoINI
//
// PURPOSE:
//    To write the contents of the LCS control array into the INI file.
//
// PARAMETERS:
//    None.
//
// RETURN VALUE:  
//    None.
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

void WriteLCSArrayToINI()
{
  UINT  uiCount;
  char  szCSIndex[MAX_FRIENDLY_NAME+1];
  char  szControlSection[]="ColorSpaces";

  strcpy( galcsControl[0].szName, "Default" );
  galcsControl[0].hlcs = (HCOLORSPACE)NULL;

  for( uiCount = 0; uiCount < guiColorSpacesUsed; uiCount++)
  {
    wsprintf( szCSIndex,(LPSTR)"ColorSpace%u",uiCount);
    WritePrivateProfileString((LPSTR)szControlSection, (LPSTR)szCSIndex, 
        (LPSTR)galcsControl[uiCount].szName, gszICM_INIFile);
    
    WriteLCSToINI((LPSTR)galcsControl[uiCount].szName, galcsControl[uiCount].lcs);
  }
  for( uiCount=guiColorSpacesUsed; uiCount < MAX_COLORSPACES; uiCount++)
  {
    wsprintf( szCSIndex,(LPSTR)"ColorSpace%u",uiCount);
    WritePrivateProfileString((LPSTR)szControlSection, (LPSTR)szCSIndex, 
        (LPSTR)"", gszICM_INIFile);
  }
}

// ************************************************************************
//
// FUNCTION: FindCurrentCSInArray
//
// PURPOSE:
//    To match the current HLOGCOLORSPACE to the associated entry
//    in the control structure.
//
// PARAMETERS:
//    HWND    Handle to the dialog.
//    int     Identifier of control.
//
// RETURN VALUE:  
//    LRESULT Value of data item associated with current HLOGCOLORSPACE.
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//    TDM       Created.
//  07/29/1994
//    TDM       Modified to return ghlcsDefault if ghlcsCurrent == NULL.  
//  
//
// ************************************************************************

LRESULT FindCurrentCSInArray(HWND hDlg, int iControlId)
{
  UINT    uiCount;
  LRESULT lColorSpaceIdx=0;
  LRESULT lColorSpaceData=0;

  if (ghlcsCurrent != NULL)  // ghlcsCurrent has been set.  Find the ownwer.
  {
    uiCount = 0;
    while ((uiCount < MAX_COLORSPACES) && (galcsControl[uiCount].hlcs != ghlcsCurrent))
      uiCount++;
    if (uiCount == MAX_COLORSPACES)
      uiCount = 0;
  }
  else
  {
    uiCount = 0;  // No current hlcs, so use default 
  }

  lColorSpaceIdx = SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_FINDSTRINGEXACT, 
      (WPARAM)-1, (LPARAM)(LPSTR)galcsControl[uiCount].szName);
  if (lColorSpaceIdx == CB_ERR)
    ErrMsg("FindCurrentCSInArray:  Could not find current color space");
  else
  {
    lColorSpaceData = SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_GETITEMDATA, (WPARAM)lColorSpaceIdx, (LPARAM)0);
    if (lColorSpaceData == CB_ERR)
      ErrMsg("dlgprcDefineColorSpace:  Invalid data associated with combobox index");
    else
    {
        // Set text, disable remove button and handle operations    
        // because CS is in use                                     
      SendDlgItemMessage(hDlg, IDC_DEFINE_CS_REFERENCE_NAME, CB_SELECTSTRING, (WPARAM)0, (LPARAM)(LPSTR)galcsControl[lColorSpaceData].szName);
      DisplayColorSpaceInfo( hDlg, galcsControl[lColorSpaceData].lcs );
      if(gfExistenceCheck) //Turn these off if enforcing existence checking
      {
        EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_REMOVE_LCS), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_CREATE_CS), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_DEFINE_CS_DELETE_CS), FALSE);
      }
    }
  }
  return(lColorSpaceData);
}



// ************************************************************************
//
// FUNCTION: dlgprcAddLCS
//
// PURPOSE:
//    Dialog procedure for adding logical color space name to control
//    structure.
//
// PARAMETERS:
//    HWND      Handle to the dialog.
//    unsigned  Message identifier.
//    WPARAM
//    LPARAM
//
// RETURN VALUE:  
//    LRESULT Boolean indicating if message was processed.
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994    TDM
//    Created.
//  08/16/1994    TDM
//    Set string to "" if CANCEL was chosen.
//
// ************************************************************************

LRESULT FAR PASCAL dlgprcAddLCS(HWND hDlg, unsigned message, WPARAM wParam, LONG lParam)
{
  static LPSTR   lpszText;

  switch (message)
  {
    case WM_INITDIALOG:     // message: initialize dialog box   
      lpszText = (LPSTR)lParam;
      SendDlgItemMessage(hDlg, IDC_CREATE_LCS_NAME, EM_LIMITTEXT, (WPARAM)MAX_FRIENDLY_NAME, 0L);
      return(TRUE);
      break;

    case WM_COMMAND:    // message: received a command   
      if (wParam == IDOK )
      {
        GetDlgItemText(hDlg, IDC_CREATE_LCS_NAME, lpszText, MAX_FRIENDLY_NAME);
        EndDialog(hDlg, TRUE);  // Exits the dialog box   
        return(TRUE);
      }
      if (wParam == IDCANCEL)
      {
        strcpy(lpszText,"");  // Make sure string is initialized to empty string before 
                              // returning to calling function
        EndDialog(hDlg, TRUE);  // Exits the dialog box   
        return(TRUE);
      }
      break;
  }
  return(FALSE);     // Didn't process a message   
}  


// ************************************************************************
//
// FUNCTION: Update_LCS
//
// PURPOSE:
//    To update the data items in the current logical color space with
//    the values in the dialog box.
//
// PARAMETERS:
//    HWND    Handle to the dialog
//    LONG    Data value associated with current logical color space
//
// RETURN VALUE:  
//    none
//
// COMMENTS:
//    !
//
// HISTORY
//  02/11/1994
//  TDM          Created.
//
// ************************************************************************

void Update_LCS(HWND hDlg, LONG lColorSpaceData)
{
  galcsControl[lColorSpaceData].lcs.lcsSignature=(DWORD)0x50534F43;
  galcsControl[lColorSpaceData].lcs.lcsVersion=GetDlgItemDword(hDlg, IDC_DEFINE_CS_VERSION);
  galcsControl[lColorSpaceData].lcs.lcsSize=GetDlgItemDword(hDlg, IDC_DEFINE_CS_SIZE);
  galcsControl[lColorSpaceData].lcs.lcsCSType=GetDlgItemDword(hDlg, IDC_DEFINE_CS_IDENTITY);
  galcsControl[lColorSpaceData].lcs.lcsIntent=GetDlgItemDword(hDlg, IDC_DEFINE_CS_GAMUT);

  galcsControl[lColorSpaceData].lcs.lcsGammaRed=GetDlgItemDword(hDlg, IDC_DEFINE_CS_GAMMA_RED);
  galcsControl[lColorSpaceData].lcs.lcsGammaGreen=GetDlgItemDword(hDlg, IDC_DEFINE_CS_GAMMA_GREEN);
  galcsControl[lColorSpaceData].lcs.lcsGammaBlue=GetDlgItemDword(hDlg, IDC_DEFINE_CS_GAMMA_BLUE);

  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzRed.ciexyzX=GetDlgItemDword(hDlg, IDC_DEFINE_CS_RED_X);
  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzRed.ciexyzY=GetDlgItemDword(hDlg, IDC_DEFINE_CS_RED_Y);
  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzRed.ciexyzZ=GetDlgItemDword(hDlg, IDC_DEFINE_CS_RED_Z);

  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzGreen.ciexyzX=GetDlgItemDword(hDlg, IDC_DEFINE_CS_GREEN_X);
  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzGreen.ciexyzY=GetDlgItemDword(hDlg, IDC_DEFINE_CS_GREEN_Y);
  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzGreen.ciexyzZ=GetDlgItemDword(hDlg, IDC_DEFINE_CS_GREEN_Z);

  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzBlue.ciexyzX=GetDlgItemDword(hDlg, IDC_DEFINE_CS_BLUE_X);
  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzBlue.ciexyzY=GetDlgItemDword(hDlg, IDC_DEFINE_CS_BLUE_Y);
  galcsControl[lColorSpaceData].lcs.lcsEndpoints.ciexyzBlue.ciexyzZ=GetDlgItemDword(hDlg, IDC_DEFINE_CS_BLUE_Z);
                                     
}



// ************************************************************************
//
// FUNCTION: GetDlgItemDword
//
// PURPOSE:
//    To get a DWORD from a dialog item and return it
//
// PARAMETERS:
//    HWND    Handle to the dialog
//    int     Identifier of control from which to retrieve text.
//
// RETURN VALUE:  
//    DWORD value of the item specified
//
// COMMENTS:
//    !
//
// HISTORY
//  02/10/1994
//  TDM          Created.
//
// ************************************************************************

DWORD GetDlgItemDword(HWND hDlg, int iControlId)
{
  DWORD  dwKeyVal;
  UINT   uiSize;
  char   szKeyVal[MAX_CHAR];

  uiSize = GetDlgItemText(hDlg, iControlId, (LPSTR)&szKeyVal, MAX_CHAR);
  dwKeyVal=strtoul(szKeyVal, NULL, 16);
  return(dwKeyVal);
}





// ************************************************************************
//
//  FUNCTION: SetDeviceProfile_DlgProc
//
//  PURPOSE:
//    To allow the user to select from the available color spaces.
//
//  PARAMETERS:
//    !
//
//  RETURN VALUE:
//    !
//
//  COMMENTS:
//    !
//
//  HISTORY:
//    07/06/1994    TDM  
//      Created.
//
// ************************************************************************

LRESULT CALLBACK SetDeviceProfile_DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_INITDIALOG:     // message: initialize dialog box   
      return(MsgDeviceProfileInit(hDlg, message, wParam, lParam));

    case WM_COMMAND:    // message: received a command   
      return(CmdDeviceProfile(hDlg, message, wParam, lParam));
      break;
  }
  return(FALSE);     // Didn't process a message   
}

// ************************************************************************
//
//  FUNCTION: CmdDeviceProfile
//
//  PURPOSE:
//    To allow the user to select from the available color spaces.
//
//  PARAMETERS:
//    !
//
//  RETURN VALUE:
//    !
//
//  COMMENTS:
//    !
//
//  HISTORY:
//    07/06/1994    TDM  
//      Created.
//
// ************************************************************************

LRESULT CALLBACK CmdDeviceProfile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (wParam)
  {
    case IDOK:
    case IDCANCEL:    
      //
      // If IDOK, we'll set the profile
      // If IDCANCEL, no activity will occur
      //
      CmdDeviceProfileDone(hDlg, message, wParam, lParam);  
      break;    
  }
  return(FALSE);     // Didn't process a message   
}

//
//  FUNCTION: MsgDeviceProfileInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the about box with version info from resources.
//
//  PARAMETERS:
//    hWnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wParam - Message specific data (unused).
//    lParam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses the version apis to retrieve version information for
//    each of the static text boxes in the about box.
//

LRESULT MsgDeviceProfileInit(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
  ICMENUMPROC fpEnumICMProfilesCallback;    // Enumeration proc for device profiles
  HDC     hDC;                    // DC associated with the HWND
  char    szItemText[256];
  LONG    lTextLen, lItemCount;
  BOOL    bSetText;
  int     iProfilesEnumerated;    // Number of profiles enumerated
  
  DebugOut("ICMDLG.C : MsgDeviceProfileInit\r\n");
  //
  // Init vars
  //
  hDC = NULL;
  
  //
  // Initialize listbox contents
  //
  SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, EM_LIMITTEXT, (WPARAM)32, 0L);
  SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0L);

  //
  // Get enum proc address
  //
  fpEnumICMProfilesCallback = (ICMENUMPROC)&EnumICMProfilesCallback;
  if (!fpEnumICMProfilesCallback)
  {
    return(FALSE);
  }
  DebugOut("ICMDLG.C : MsgDeviceProfileInit got fn handle\r\n");
  if(giDCType == SCREEN_DC)
  {
    hDC = GetDC(hDlg);
    SetWindowText(hDlg, "SetColorProfile - Screen");
  }
  else // PRINTER
  {
    hDC = GetPrinterDC();
    SetWindowText(hDlg, "SetColorProfile - Printer");
  }
  if(!hDC)
  {
    return(FALSE);
  }
  //
  //Enumerate profiles and cram them into the listbox
  //
  iProfilesEnumerated = EnumICMProfiles(hDC, fpEnumICMProfilesCallback, (LPARAM)hDlg);
  ReleaseDC(hDlg, hDC);

  //
  // If ICM is enabled for this DC, use GetColorProfile to id current profile 
  //
  lItemCount = SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
  if(lItemCount > 0)
  {
    //
    // WORKITEM:
    //    -Call GetICMProfile with parameters to get string into szItemText.
    //    -Set DlgItemText to szItemText
    //
    lTextLen = SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, LB_GETTEXT, (WPARAM)0, (LPARAM)(LPSTR)szItemText);
    if (lTextLen != LB_ERR)
    {
      bSetText = SetDlgItemText(hDlg,IDC_AVAIL_PROFILES, (LPSTR)szItemText);
    }
  }
  else
  {
    bSetText = SetDlgItemText(hDlg,IDC_AVAIL_PROFILES, "Didn't get any profiles");
  }
  if (!bSetText)
  {
    ErrMsg("MsgDeviceProfileInit:  SetDlgItemText FAILED");
  }
  return(TRUE);
}



int CALLBACK  EnumICMProfilesCallback(LPCSTR lpszFilename, LPARAM lParam)
{
  HWND  hDlg;                           // Handle to dlg, passed in as LPARAM
  LRESULT lProfileIdx;                  // Index of item in dlg box
  LRESULT lProfileData;                 // Data associated with item number lProfileIdx
  static UINT    uiNumProfiles = 0;     // Number of profiles enumerated

  // 
  // Get the handle to the dialog from the LPARAM
  //
  hDlg = (HWND)lParam;
  
  //
  // Add each enumerated item to the listbox
  //
  lProfileIdx = SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, CB_ADDSTRING, 0, (LPARAM)(LPSTR)lpszFilename);
  if (lProfileIdx == CB_ERR || lProfileIdx == CB_ERRSPACE)
  {
    ErrMsg("EnumICMProfilesCallback:  Couldn't add %s to combobox", lpszFilename);
    return(FALSE);
  }
  lProfileData = SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, CB_SETITEMDATA, (WPARAM)lProfileIdx, (LPARAM)(DWORD)uiNumProfiles);
  if (lProfileData == CB_ERR )
  {
    ErrMsg("EnumICMProfilesCallback:  Couldn't set item data %u:%s in combobox", uiNumProfiles, lpszFilename);
    return(FALSE);
  }
  uiNumProfiles++;
  return(TRUE);  // keep enumerating if we made it this far  
}





//
//  FUNCTION: CmdDeviceProfileDone(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Free the SetColorProfile dialog  and related data.
//
//  PARAMETERS:
//    hWnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify   - Notification number (unused)
//    hWndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdDeviceProfileDone(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lDeviceProfileIdx;
    LRESULT lDeviceProfileData;
    UINT    uiNumChars;
    BOOL    bSetProfile;
    char    szDlgItem[MAX_COLORSPACE_NAME+1];

  if (wParam == IDOK) // Set the new device profile
  {
    //
    //Get the string in the edit control
    //
    uiNumChars = GetDlgItemText(hDlg, IDC_AVAIL_PROFILES, szDlgItem, MAX_COLORSPACE_NAME);

    //
    //Get the index of the string.  If OK, get data associated with index.
    //
    lDeviceProfileIdx = SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, CB_FINDSTRINGEXACT,
      (WPARAM)-1, (LPARAM)(LPSTR)szDlgItem);
    if (lDeviceProfileIdx == CB_ERR)
      ErrMsg("CmdDeviceProfileDone:  Could not find current device profile");
    else
    {
      lDeviceProfileData = SendDlgItemMessage(hDlg, IDC_AVAIL_PROFILES, CB_GETITEMDATA, (WPARAM)lDeviceProfileIdx, (LPARAM)0);
      if (lDeviceProfileData == CB_ERR)
        ErrMsg("CmdDeviceProfileDone:  Invalid data associated with combobox index");
    }
    if(lDeviceProfileData != CB_ERR)  // update the current profile
    {
      HDC     hDC;

      hDC = GetDC(hDlg);
      if(hDC)
      {
#ifdef DW
        bSetProfile = TRUE;
#else
//	  bSetProfile = SetColorProfile(hDC, szDlgItem);
#endif
        if(!bSetProfile)

        {
          ErrMsg("CmdDeviceProfileDone:  SetColorProfile(%08X, %s) FAILED", hDC, szDlgItem);
        }
        ReleaseDC(hDlg, hDC);
      }
    }
  }
  EndDialog(hDlg, TRUE);          // Exit the dialog
  return(TRUE);
}
