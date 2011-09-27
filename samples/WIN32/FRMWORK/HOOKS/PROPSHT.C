// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   prophst.c
//
//  PURPOSE:  Contains all helper functions required for PropertySheets.
//
//  FUNCTIONS:
//      ShowPropertySheet  - Used to display an array of property pages in 
//                           a property sheet
//      InitPropertySheet  - Initializes a property page structure
//      MsgTypeDlgInit     - To initialize the TypeDialog property sheet
//      MsgTypeDlgNotify   - To process the WM_NOTIFY message for the TypeDlg
//      MsgOutputDlgInit   - To initialize the OutputDialog property sheet
//      MsgOutputDlgCommand- Processes the WM_COMMAND message for the OutputDlg
//      MsgOutputDlgNotify - Processes the WM_NOTIFY message for the OutputDlg
//      MsgInputDlgInit    - To initialize the InputDialog property sheet
//      MsgInputDlgNotify  - Processes the WM_COMMAND message for the InputDlg
//      MsgRangeDlgInit    - To initialize the RangeDialog property sheet
//      MsgRangeDlgNotify  - Processes the WM_NOTIFY message for the RangeDlg
//      TypeDlgFunc        - Processes messages for "Type" dialog box.
//      OutputDlgFunc      - Processes messages for "Output" dialog box.
//      InputDlgFunc       - Processes messages for "Input" dialog box.
//      RangeDlgFunc       - Processes messages for "Range" dialog box.
//      InitializeTypeControls  - Initializes the controls on the property 
//                                page that affect type of messages filtered
//      InitializeOutputControls- Initializes controls that effect where
//                                the program displays its output
//      InitializeInputControls - Initializes controls that affect what windows
//                                the program monitors
//      InitializeRangeControls - Initializes the controls that affect the
//                                range of messages filtered
//      ValidateTypeControls    - Validates contents of TypeDlg controls
//      ValidateOutputControls  - Validates contents of OutputDlg controls
//      ValidateInputControls   - Validates contents of InputDlg controls
//      ValidateRangeControls   - Validates contents of RangeDlg controls
//      ApplyTypeControls       - Apply changes in TypeDlg
//      ApplyOutputControls     - Apply changes in OutputDlg
//      ApplyInputControls      - Apply changes in InputDlg
//      ApplyRangeControls      - Apply changes in RangeDlg
//      ReadType                - Read values from Type Dlg
//      ReadOuput               - Read values from Output Dlg
//      ReadInput               - Read values from Input Dlg
//      ReadRange               - Read values from Range Dlg
//      InvalidRange            - Verifies range values
//      ValidFileName           - Validates filename passed
//      WM_NotifyMsg            - Handles WM_NOTIFY message for property sheet
//      ErrorMessage            - Error-handling function
//      WarningBeep             - Beeps to draw user's attention
//
//  COMMENTS:
//
//

#include <windows.h>
#include <commctrl.h>
#include <prsht.h>


#include "globals.h"
#include "syshook.h"
#include "propsht.h"

//Pointer to global options
extern PGLOBOPTS pS;                             

//
// Global Variables for this module
//

HWND hType;
HWND hInput;
HWND hOutput;
HWND hRange;

//Prototypes for functions used within this file

LRESULT MsgTypeDlgInit     (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgTypeDlgNotify   (HWND, UINT, WPARAM, LPARAM);

LRESULT MsgInputDlgInit    (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgInputDlgNotify  (HWND, UINT, WPARAM, LPARAM);

LRESULT MsgOutputDlgInit   (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgOutputDlgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgOutputDlgNotify (HWND, UINT, WPARAM, LPARAM);

LRESULT MsgRangeDlgInit    (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgRangeDlgNotify  (HWND, UINT, WPARAM, LPARAM);

int ShowPropertySheet(LPPROPSHEETPAGE, UINT, HWND, LPSTR);
void InitPropertySheetPage(LPPROPSHEETPAGE, DLGPROC, UINT, LPSTR);

//Property page callbacks
BOOL CALLBACK TypeDlgFunc(HWND hDlg,
                          UINT uiMessage,
                          WPARAM wParam,
                          LPARAM lParam);

BOOL CALLBACK InputDlgFunc(HWND hDlg,
                           UINT uiMessage,
                           WPARAM wParam,
                           LPARAM lParam);

BOOL CALLBACK OutputDlgFunc(HWND hDlg,
                            UINT uiMessage,
                            WPARAM wParam,
                            LPARAM lParam);

BOOL CALLBACK RangeDlgFunc(HWND hDlg,
                           UINT uiMessage,
                           WPARAM wParam,
                           LPARAM lParam);


// These functions initialize the page with settings
// from the GlobalOptions structure

void InitializeTypeControls(HWND hDlg);
void InitializeInputControls(HWND hDlg);
void InitializeOutputControls(HWND hDlg);
void InitializeRangeControls(HWND hDlg);

// These functions validate the values entered into
// the controls by the user

BOOL ValidateTypeControls(HWND hDlg);
BOOL ValidateInputControls(HWND hDlg);
BOOL ValidateOutputControls(HWND hDlg);
BOOL ValidateRangeControls(HWND hDlg);

// These functions cause the selections made in the
// page to become active in the program
BOOL ApplyTypeControls(HWND hDlg);
DWORD ReadType(void);

BOOL ApplyInputControls(HWND hDlg);
BOOL ReadInput(void);

BOOL ApplyOutputControls(HWND hDlg);
void ReadOutput(void);
BOOL ApplyRangeControls(HWND hDlg);
void ReadRange(UINT *, UINT *);


//Helper functions used by this module

BOOL InvalidRange(HWND hDlg, UINT uiStartControl, UINT uiEndControl);
BOOL ValidFileName(char *pszFileName);                                          //Helper for ValidateOutputControls()
BOOL WM_NotifyMsg(UINT uiCallerID, HWND hDlg, LPNMHDR pNotifyMsgHdr);
void ErrorMessage(char *pszFunction, char *pszMessage);         
void WarningBeep(void);                                                                         //Helper for ErrorMessage()


// Type dialog message table definition.
MSD rgmsdType[] =
{
    {WM_INITDIALOG, MsgTypeDlgInit},
    {WM_NOTIFY,     MsgTypeDlgNotify}
};

MSDI msdiType =
{
    sizeof(rgmsdType) / sizeof(MSD),
    rgmsdType,
    edwpNone
};

// Input dialog message table definition.
MSD rgmsdInput[] =
{
    {WM_INITDIALOG, MsgInputDlgInit},
    {WM_NOTIFY,     MsgInputDlgNotify}
};

MSDI msdiInput =
{
    sizeof(rgmsdInput) / sizeof(MSD),
    rgmsdInput,
    edwpNone
};

// Output dialog message table definition.
MSD rgmsdOutput[] =
{
    {WM_INITDIALOG, MsgOutputDlgInit},
    {WM_COMMAND,    MsgOutputDlgCommand},
    {WM_NOTIFY,     MsgOutputDlgNotify}
};

MSDI msdiOutput =
{
    sizeof(rgmsdOutput) / sizeof(MSD),
    rgmsdOutput,
    edwpNone
};

// Range dialog message table definition.
MSD rgmsdRange[] =
{
    {WM_INITDIALOG, MsgRangeDlgInit} ,
    {WM_NOTIFY,     MsgRangeDlgNotify}
};

MSDI msdiRange =
{
    sizeof(rgmsdRange) / sizeof(MSD),
    rgmsdRange,
    edwpNone
};


//
//  FUNCTION: MsgTypeDlgInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the TypeDialog property sheet
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgTypeDlgInit(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    hType = hDlg;

    //Initialize Type Controls
    InitializeTypeControls(hDlg);

    // Return Non Zero value so that default
    // control will have the focus
    return 1L;
}

//
//  FUNCTION: MsgTypeDlgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To process the WM_NOTIFY message for the TypeDlg
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgTypeDlgNotify(HWND hDlg, 
                          UINT uMessage, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
    BOOL bReturnVal;

    bReturnVal = WM_NotifyMsg(TYPE_DLG, hDlg, (LPNMHDR)lParam);
    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG) bReturnVal);
    return bReturnVal;
}

//
//  FUNCTION: MsgInputDlgInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the InputDialog property sheet
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgInputDlgInit(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    hInput = hDlg;
    InitializeInputControls(hDlg);

    // Return Non Zero value so that default
    // control will have the focus
    return 1L;
}

//
//  FUNCTION: MsgInputDlgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To process the WM_NOTIFY message for the InputDlg
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgInputDlgNotify(HWND hDlg, 
                          UINT uMessage, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
    BOOL bReturnVal;

    bReturnVal = WM_NotifyMsg(INPUT_DLG, hDlg, (LPNMHDR)lParam);
    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG) bReturnVal);
    return bReturnVal;
 }

//
//  FUNCTION: MsgOutputDlgInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the OutputDialog property sheet
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgOutputDlgInit(HWND hDlg, 
                        UINT uMessage, 
                        WPARAM wParam, 
                        LPARAM lParam)
{
    hOutput = hDlg;
    InitializeOutputControls(hDlg);
    
    //Return Non Zero value so that
    //default control will have the focus
    return 1L;
}

//
//  FUNCTION: MsgOutputDlgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes the WM_COMMAND message for the OutputDlg
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgOutputDlgCommand(HWND hDlg, 
                            UINT uMessage, 
                            WPARAM wParam, 
                            LPARAM lParam)
{
    // We want to enable the FileName static and edit controls
    // only if the DISK checkbox is checked.
    if ((HIWORD(wParam) == BN_CLICKED) &&
        (LOWORD(wParam) == IDC_DISK))
    {
        HWND hWndFileName;
        BOOL bEnabled;
    
        hWndFileName = GetDlgItem (hDlg, IDC_FILENAME);
        bEnabled = IsWindowEnabled (hWndFileName);
    
        EnableWindow (hWndFileName, !bEnabled);
        EnableWindow (GetDlgItem (hDlg, IDC_STATIC), !bEnabled);
    
        SetFocus(GetDlgItem(hDlg, IDC_FILENAME));
        SendDlgItemMessage (hDlg, 
                            IDC_FILENAME, 
                            EM_SETSEL, 
                            (WPARAM)0,
                            (LPARAM)-1);
    }
    return 0L;
}
//
//  FUNCTION: MsgOutputDlgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To process the WM_NOTIFY message for the OutputDlg
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgOutputDlgNotify(HWND hDlg, 
                          UINT uMessage, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
    BOOL bReturnVal;


    bReturnVal = WM_NotifyMsg(OUTPUT_DLG, hDlg, (LPNMHDR)lParam);
    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG) bReturnVal);
    return bReturnVal;
}

//
//  FUNCTION: MsgRangeDlgInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the RangeDialog property sheet
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgRangeDlgInit(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    hRange = hDlg;
    InitializeRangeControls(hDlg);

    // Return Non Zero value so that default
    // control will have the focus
    return 1L;
}

//
//  FUNCTION: MsgRangeDlgNotify(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To process the WM_NOTIFY message for the RangeDlg
//
//  PARAMETERS:
//    hwnd     - The window handling the message.
//    uMessage - The message number. (unused).
//    wParam   - Message specific data (unused).
//    lParam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:

LRESULT MsgRangeDlgNotify(HWND hDlg, 
                          UINT uMessage, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
    BOOL bReturnVal;

    bReturnVal = WM_NotifyMsg(RANGE_DLG, hDlg, (LPNMHDR)lParam);
    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG) bReturnVal);
    return bReturnVal;
}

//
//  FUNCTION: OptionsPropertySheets(UINT, HWND)
//
//  PURPOSE:  Used to initalize property page structures and to
//            display the property sheet for this program.
//
//  PARAMETERS:
//    UINT     - Option to specify to initialize page structures
//               or to display property sheet
//    HWND     - Handle of window that is to own the property sheet
//
//  RETURN VALUE:
//    TRUE  - Function successful
//    FALSE - Function Failed
//
//  COMMENTS:
//    Function must first be called with option set to INITIALIZE.
//    This function should only be called once during the
//    programs execution and must be called before the SHOW
//    option is specified.
//    Call this function with the option set to SHOW to display
//    the property sheet.
//

BOOL OptionsPropertySheets(UINT uiOption, HWND hOwner)
{
    
    static PROPSHEETPAGE PropertySheetPage[PROP_OPTION_PAGES];
    //static HPROPSHEETPAGE hPropertySheet[PROP_OPTION_PAGES];
    //static PROPSHEETHEADER PropertySheetHdr;

    static BOOL bInit = FALSE;      //TRUE if PropSheet initialized

    BOOL bReturnVal;

    switch (uiOption)
    {
        case INITIALIZE: 

            //Initialize the pages of the propery sheet
            InitPropertySheetPage(&PropertySheetPage[0],
                                  TypeDlgFunc, 
                                  IDD_CATEGORY, 
                                  "Type");
            InitPropertySheetPage(&PropertySheetPage[1],
                                  RangeDlgFunc, 
                                  IDD_RANGE, 
                                  "Range");
            InitPropertySheetPage(&PropertySheetPage[2],
                                  InputDlgFunc, 
                                  IDD_INPUT, 
                                  "Input");
            InitPropertySheetPage(&PropertySheetPage[3],
                                  OutputDlgFunc, 
                                  IDD_OUTPUT,
                                  "Output");
    
            bInit = TRUE;
            bReturnVal = TRUE;
            break;

        case SHOW:

            if(bInit)
            {
                ShowPropertySheet(PropertySheetPage,
                                  PROP_OPTION_PAGES,
                                  hOwner,
                                  "Hooks");
                bReturnVal = TRUE;
            }
            else
                bReturnVal = FALSE;
            break;

        default:
            bReturnVal = FALSE;
            break;
    }
    return bReturnVal;
}

//
//  FUNCTION: InitPropertySheetPage(PROPSHEETPAGE *, DLGPROC, UINT , char *)
//
//  PURPOSE:  Used to initalize a property page structures
//
//  PARAMETERS:
//    PROPSHEETPAGE *     - Pointer to the property sheet page to initialize
//    DLGPROC             - Function address of the property sheet page's
//                          dialog procedure
//    UINT                - The Resource identifier of the dialog box
//                          that contains the controls for this page
//    char *              - Pointer to a NULL terminated string that
//                          contains the title for this pages tab
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This is a helper function for OptionsPropertySheets()
//


void InitPropertySheetPage(PROPSHEETPAGE *pPage,
                           DLGPROC DlgProc,
                           UINT uiDlgID,
                           char *pszTitle)
{
    pPage->dwSize = sizeof(PROPSHEETPAGE);
    pPage->dwFlags = PSP_USETITLE; //Use the title given

    pPage->hInstance = hInst;
    pPage->pszTemplate = MAKEINTRESOURCE(uiDlgID);
    pPage->pszTitle = pszTitle;
    pPage->pfnDlgProc = DlgProc;
    pPage->lParam = 0;
//    pPage->pfnRelease = NULL;
}


//
//  FUNCTION: ShowPropertySheet(PROPSHEETPAGE *, UINT , HWND , char *)
//
//  PURPOSE:  Used to display an array of property pages in a property sheet
//
//  PARAMETERS:
//    PROPSHEETPAGE *     - Pointer to an array property sheet
//                          pages that make up the property sheet
//    UINT                - Number of entries in the array
//    HWND                - Window handle of owner of this property sheet
//    char *              - Pointer to a NULL terminated string that
//                          contains the caption for this property sheet
//
//  RETURN VALUE:
//    Returns the value from the Windows API PropertySheet()
//
//  COMMENTS:
//    This is a helper function for OptionsPropertySheets()
//

int ShowPropertySheet(PROPSHEETPAGE *pPropSheetPage,
                      UINT uiNoOfPages,
                      HWND hOwner,
                      char *pszCaption)
{

    PROPSHEETHEADER PropSheetHdr;


    PropSheetHdr.dwSize = sizeof(PROPSHEETHEADER);
    PropSheetHdr.dwFlags = PSH_PROPSHEETPAGE;
    
    //Use default intro if Caption specified
    if (pszCaption != NULL)
    {
        PropSheetHdr.dwFlags |= PSH_PROPTITLE;
        PropSheetHdr.pszCaption = pszCaption;
    }
    PropSheetHdr.hwndParent = hOwner;
    PropSheetHdr.hInstance = hInst;
    PropSheetHdr.nPages = uiNoOfPages;      //1 based
    PropSheetHdr.ppsp = pPropSheetPage;



    //Show the property
    return PropertySheet(&PropSheetHdr);
}


//
//  FUNCTION: TypeDlgFunc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Type" dialog box.
//
//  PARAMETERS:
//    hDlg     - window handle of the dialog box
//    wMessage - type of message
//    wParam   - message-specific information
//    lParam   - message-specific information
//
//  RETURN VALUE:
//    TRUE  - message handled
//    FALSE - message not handled
//    Note: WM_NOTIFY Return values specified using SetWindowLong()
//
//  COMMENTS:
//
//
BOOL CALLBACK TypeDlgFunc(HWND hDlg,
                          UINT uMessage,
                          WPARAM wParam,
                          LPARAM lParam)
{
    return DispMessage(&msdiType, hDlg, uMessage, wParam, lParam);
}

//
//  FUNCTION: InputDlgFunc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Type" dialog box.
//
//  PARAMETERS:
//    hDlg     - window handle of the dialog box
//    wMessage - type of message
//    wParam   - message-specific information
//    lParam   - message-specific information
//
//  RETURN VALUE:
//    TRUE  - message handled
//    FALSE - message not handled
//    Note: WM_NOTIFY Return values specified using SetWindowLong()
//
//  COMMENTS:
//
//
BOOL CALLBACK InputDlgFunc(HWND hDlg,
                           UINT uMessage,
                           WPARAM wParam,
                           LPARAM lParam)
{
    return DispMessage(&msdiInput, hDlg, uMessage, wParam, lParam);
}

//
//  FUNCTION: OutputDlgFunc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Type" dialog box.
//
//  PARAMETERS:
//    hDlg     - window handle of the dialog box
//    wMessage - type of message
//    wParam   - message-specific information
//    lParam   - message-specific information
//
//  RETURN VALUE:
//    TRUE  - message handled
//    FALSE - message not handled
//    Note: WM_NOTIFY Return values specified using SetWindowLong()
//
//  COMMENTS:
//
//

BOOL CALLBACK OutputDlgFunc(HWND hDlg,
                            UINT uMessage,
                            WPARAM wParam,
                            LPARAM lParam)
{
    return DispMessage(&msdiOutput, hDlg, uMessage, wParam, lParam);
}

//
//  FUNCTION: RangeDlgFunc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Type" dialog box.
//
//  PARAMETERS:
//    hDlg     - window handle of the dialog box
//    wMessage - type of message
//    wParam   - message-specific information
//    lParam   - message-specific information
//
//  RETURN VALUE:
//    TRUE  - message handled
//    FALSE - message not handled
//    Note: WM_NOTIFY Return values specified using SetWindowLong()
//
//  COMMENTS:
//
//

BOOL CALLBACK RangeDlgFunc(HWND hDlg,
                           UINT uMessage,
                           WPARAM wParam,
                           LPARAM lParam)
{
    return DispMessage(&msdiRange, hDlg, uMessage, wParam, lParam);
}



//
//  FUNCTION: WM_NotifyMsg(UINT, HWND, LPNMHDR)
//
//  PURPOSE:  Handles the various WM_NOTIFY messages for the property
//            sheets pages
//
//  PARAMETERS:
//    UINT     - Unique ID of the dialog procedure who is making this call
//    HWND     - Window handle of the property page
//    LPNMHDR  - Pointer to a Notify Message Header
//
//  RETURN VALUE:
//    TRUE or FALSE based on the specific message handled.
//
//  COMMENTS:
//    This is a helper function for the dialog
//    procedures for each property page.
//

BOOL WM_NotifyMsg(UINT uiCallerID, HWND hDlg, LPNMHDR pNotifyMsgHdr)
{
    BOOL bReturnVal;

    switch (pNotifyMsgHdr->code)
    {
        case PSN_SETACTIVE:
            //Page is about to become the active page
            bReturnVal = FALSE;
            SetWindowLong(hDlg, DWL_MSGRESULT, (LONG)(BOOL)bReturnVal);
            break;

        case PSN_KILLACTIVE:
            //Page is about to lose activation
            //Perform Validation at this point.
            //If a control has invalid data, return TRUE
            //to keep page from from losing activation


            switch (uiCallerID)
            {
                case TYPE_DLG:
                    bReturnVal = ValidateTypeControls(hDlg);
                    break;
                    
                case INPUT_DLG:
                    bReturnVal = ValidateInputControls(hDlg);
                    break;

                case OUTPUT_DLG:
                    bReturnVal = ValidateOutputControls(hDlg);
                    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG)(BOOL)bReturnVal);
                    return bReturnVal;
                    break;

                case RANGE_DLG:
                    bReturnVal = ValidateRangeControls(hDlg);
                    break;

                default:
                    //DEBUG START
                    ErrorMessage("Invalid Caller ID","PSN_KILLACTIVE WM_NotifyMsg");
                    //DEBUG END
                    bReturnVal = FALSE;     //Allow activation to be killed
                    break;
            }
            break;

        case PSN_APPLY:
            //The user has pressed OK or Apply Now.
            //Store changes and make them take effect

            switch (uiCallerID)
            {
                case TYPE_DLG:
                    bReturnVal = ApplyTypeControls(hDlg);
                    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG)(BOOL)FALSE);
                    return FALSE;
                    break;

                case INPUT_DLG:
                    bReturnVal = ApplyInputControls(hDlg);
                    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG)(BOOL)FALSE);
                    return FALSE;

                    break;

                case OUTPUT_DLG:
                    bReturnVal = ApplyOutputControls(hDlg);
                    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG)(BOOL)FALSE);
                    return FALSE;

                    break;

                case RANGE_DLG:
                    bReturnVal = ApplyRangeControls(hDlg);
                    SetWindowLong(hDlg, DWL_MSGRESULT, (LONG)(BOOL)FALSE);
                    return FALSE;

                    break;

                default:
                    //DEBUG START
                    ErrorMessage("Invalid Caller ID","PSN_APPLY WM_NotifyMsg");
                    //DEBUG END
                    bReturnVal = FALSE;     //Allow activation to be killed
                    break;
            }
            break;

        case PSN_RESET:
            //User has pressed CANCEL
            //No need to save changes
            bReturnVal = FALSE;
            break;
            
        case PSN_HELP:
            bReturnVal = FALSE;
            ErrorMessage("Help Not Available","Help");
            break;

        default:
            //ErrorMessage("Unkown Message Recieved","WM_NotifyMsg()");
            bReturnVal = FALSE;
            break;
    }
    return bReturnVal;
}

//
//  FUNCTION: InitializeTypeControls(HWND)
//
//  PURPOSE:  Initializes the controls on the property page that
//            effect the type of messages the program will filter
//
//  PARAMETERS:
//    HWND     - Window handle of the property page containing
//               the "Type" controls
//
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This is a helper function for the "Type" dialog procedure.
//    It is called at WM_INITDIALOG time to set the
//    controls to their initial values.
//

void InitializeTypeControls(HWND hDlg)
{
    //Load each control with it's proper value
    if (pS->dwMsgFilterOptions & MSG_UNKNOWN)
        CheckDlgButton(hDlg, IDC_OTHER, TRUE);
    else
        CheckDlgButton(hDlg, IDC_OTHER, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_MOUSE)
        CheckDlgButton(hDlg, IDC_MOUSE, TRUE);
    else
        CheckDlgButton(hDlg, IDC_MOUSE, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_KEYBOARD)
        CheckDlgButton(hDlg, IDC_KEYBOARD, TRUE);
    else
        CheckDlgButton(hDlg, IDC_KEYBOARD, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_MENU)
        CheckDlgButton(hDlg, IDC_MENU, TRUE);
    else
        CheckDlgButton(hDlg, IDC_MENU, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_NON_CLIENT)
        CheckDlgButton(hDlg, IDC_NONCLIENT, TRUE);
    else
        CheckDlgButton(hDlg, IDC_NONCLIENT, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_CLIPBOARD)
        CheckDlgButton(hDlg, IDC_CLIPBOARD, TRUE);
    else
        CheckDlgButton(hDlg, IDC_CLIPBOARD, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_SYSTEM)
        CheckDlgButton(hDlg, IDC_SYSTEM, TRUE);
    else
        CheckDlgButton(hDlg, IDC_SYSTEM, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_TIMER)
        CheckDlgButton(hDlg, IDC_TIMER, TRUE);
    else
        CheckDlgButton(hDlg, IDC_TIMER, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_DDE)
        CheckDlgButton(hDlg, IDC_DDE, TRUE);
    else
        CheckDlgButton(hDlg, IDC_DDE, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_MDI)
        CheckDlgButton(hDlg, IDC_MDI, TRUE);
    else
        CheckDlgButton(hDlg, IDC_MDI, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_WIN31_MSGS)
        CheckDlgButton(hDlg, IDC_WIN31, TRUE);
    else
        CheckDlgButton(hDlg, IDC_WIN31, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_WM_USER)
        CheckDlgButton(hDlg, IDC_USER, TRUE);
    else
        CheckDlgButton(hDlg, IDC_USER, FALSE);

    if (pS->dwMsgFilterOptions & MSG_MSG_RANGE_ONLY)
        CheckDlgButton(hDlg, IDC_RANGE, TRUE);
    else
        CheckDlgButton(hDlg, IDC_RANGE, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_WIN40_MSGS)
        CheckDlgButton(hDlg, IDC_WIN40, TRUE);
    else
        CheckDlgButton(hDlg, IDC_WIN40, FALSE);

    if (pS->dwMsgFilterOptions & MSG_WIN40_MSGS)
        CheckDlgButton(hDlg, IDC_WIN32, TRUE);
    else
        CheckDlgButton(hDlg, IDC_WIN32, FALSE);

    if (pS->dwMsgFilterOptions & MSG_LB_MSGS)
        CheckDlgButton(hDlg, IDC_LISTBOX, TRUE);
    else
        CheckDlgButton(hDlg, IDC_LISTBOX, FALSE);

    if (pS->dwMsgFilterOptions & MSG_CB_MSGS)
        CheckDlgButton(hDlg, IDC_COMBOBOX, TRUE);
    else
        CheckDlgButton(hDlg, IDC_COMBOBOX, FALSE);
        
    if (pS->dwMsgFilterOptions & MSG_EDT_MSGS)
        CheckDlgButton(hDlg, IDC_EDITCTL, TRUE);
    else
        CheckDlgButton(hDlg, IDC_EDITCTL, FALSE);

    if (pS->dwMsgFilterOptions & MSG_BTN_MSGS)
        CheckDlgButton(hDlg, IDC_BUTTONCTL, TRUE);
    else
        CheckDlgButton(hDlg, IDC_BUTTONCTL, FALSE);

    if (pS->dwMsgFilterOptions & MSG_STT_MSGS)
        CheckDlgButton(hDlg, IDC_STATICCTL, TRUE);
    else
        CheckDlgButton(hDlg, IDC_STATICCTL, FALSE);

    if (pS->dwMsgFilterOptions & MSG_UNKNOWN)
        CheckDlgButton(hDlg, IDC_UNKNOWN, TRUE);
    else
        CheckDlgButton(hDlg, IDC_UNKNOWN, FALSE);

    if (pS->dwMsgFilterOptions & MSG_CTL_MSGS)
        CheckDlgButton(hDlg, IDC_CONTROL, TRUE);
    else
        CheckDlgButton(hDlg, IDC_CONTROL, FALSE);

    return;
}

//
//  FUNCTION: InitializeInputControls(HWND)
//
//  PURPOSE:  Initializes the controls on the property page
//            that effect what windows this program will monitor
//
//  PARAMETERS:
//    HWND     - Window handle of the property page
//               containing the "Input" controls
//
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This is a helper function for the "Input" dialog procedure. It
//    is called at WM_INITDIALOG time to set the
//    controls to their initial values.
//

void InitializeInputControls(HWND hDlg)
{
    if (pS->hWndMonitored == (HWND) 0x0000FFFF)
    {
        CheckRadioButton(hDlg, IDC_SINGLE, IDC_ALLWIN, IDC_ALLWIN);
        return;
    }    

    if(pS->fMonitorChild)
        CheckRadioButton(hDlg, IDC_SINGLE, IDC_ALLWIN, IDC_PLUSCHILD);
    else
        CheckRadioButton(hDlg, IDC_SINGLE, IDC_ALLWIN, IDC_SINGLE);
    
    return;
}

//
//  FUNCTION: InitializeOutputControls(HWND)
//
//  PURPOSE:  Initializes the controls on
//            the property page that effect where this
//            program will display its output
//
//  PARAMETERS:
//    HWND     - Window handle of the property
//               page containing the "Output" controls
//
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This is a helper function for the "Output" dialog procedure.
//    It is called at WM_INITDIALOG time to set the
//    controls to their initial values.
//

void InitializeOutputControls(HWND hDlg)
{
    // if none of the buttons are checked,
    // we default to OD_WINDOW

    if ((!pS->fOutputToWindow) &&
       (!pS->fOutputToCom) &&
       (!pS->fOutputToLogFile))
        pS->fOutputToWindow = TRUE;

    CheckDlgButton(hDlg, IDC_WINDOW, pS->fOutputToWindow);
    CheckDlgButton(hDlg, IDC_COM1,   pS->fOutputToCom);
    CheckDlgButton(hDlg, IDC_DISK,   pS->fOutputToLogFile);

    // Disable Filename edit control and static
    SetDlgItemText (hDlg, IDC_FILENAME, (LPSTR)pS->szOutputFile);
    EnableWindow (GetDlgItem (hDlg, IDC_FILENAME), pS->fOutputToLogFile);
    EnableWindow (GetDlgItem (hDlg, IDC_STATIC),   pS->fOutputToLogFile);

    return;
}

//
//  FUNCTION: InitializeRangeControls(HWND)
//
//  PURPOSE:  Initializes the controls on the property page that effect the
//            range of messages the program will filter
//
//  PARAMETERS:
//    HWND     - Window handle of the property page containing
//               the "Range" controls
//
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This is a helper function for the "Range" dialog procedure.
//    It is called at WM_INITDIALOG time to set the
//    controls to their initial values.
//

void InitializeRangeControls(HWND hDlg)
{

    SetDlgItemInt(hDlg, IDC_INCSTART, pS->uLoRange, FALSE);
    SetDlgItemInt(hDlg, IDC_INCEND, pS->uHiRange, FALSE);

    return;
}       
    
        

//
//  FUNCTION: ValidateTypeControls(HWND)
//
//  PURPOSE:  Validates the contents of each controls on the property
//            page that effects the type of messages the program will filter.
//
//  PARAMETERS:
//    HWND     - Window handle of the property page containing the
//               "Type" controls
//
//
//  RETURN VALUE:
//    TRUE if a control contains an invalid entry.
//    FALSE if all controls contain valid information.
//
//  COMMENTS:
//    This is a helper function for the WM_NOTIFY - PSN_KILLACTIVE
//    message for the "Type" dialog procedure.
//    It is called by the WM_NotifyMsg() procedure. If an invalid
//    entry is found the function displays an
//    error message to the user and sets the focus to the control
//    containing the invalid entry. The caller
//    should prevent this page from losing activation so the user
//    can correct the value.
//

BOOL ValidateTypeControls(HWND hDlg)
{
    // No validation required for this page.
    // Any combination of messages is valid
    return FALSE;
}

//
//  FUNCTION: ValidateInputControls(HWND)
//
//  PURPOSE:  Validates the controls on the property page that effect
//            what windows this program will monitor
//
//
//  PARAMETERS:
//    HWND     - Window handle of the property page containing the
//               "Input" controls
//
//
//  RETURN VALUE:
//    TRUE if a control contains an invalid entry.
//    FALSE if all controls contain valid information.
//
//  COMMENTS:
//    This is a helper function for the WM_NOTIFY - PSN_KILLACTIVE
//    message for the "Input" dialog procedure.
//    It is called by the WM_NotifyMsg() procedure. If an invalid
//    entry is found the function displays an
//    error message to the user and sets the focus to the control
//    containing the invalid entry. The caller
//    should prevent this page from losing activation so the user
//    can correct the value.
//
BOOL ValidateInputControls(HWND hDlg)
{
    int iIncError;
    BOOL bReturnVal;
    
    //Make sure only one control is selected        
    bReturnVal = FALSE;     //Assume no error
    iIncError = 0;

    if (IsDlgButtonChecked(hDlg, IDC_SINGLE))
        iIncError++;

    if (IsDlgButtonChecked(hDlg, IDC_PLUSCHILD))
        iIncError++;

    if (IsDlgButtonChecked(hDlg, IDC_ALLWIN))
        iIncError++;
        
    if (iIncError > 1)
    {
        ErrorMessage("Only one option may be selected","Error");
        bReturnVal = TRUE;
    }
    
    if (iIncError < 1)
    {
        ErrorMessage("You must choose one option from this page","Error");
        bReturnVal = TRUE;
    }
    
    return bReturnVal;
}

//
//  FUNCTION: ValidateOutputControls(HWND)
//
//  PURPOSE:  Validate the controls on the property page that
//            effect where this program will display its output
//
//
//  PARAMETERS:
//    HWND     - Window handle of the property page containing
//               the "Output" controls
//
//
//  RETURN VALUE:
//    TRUE if a control contains an invalid entry.
//    FALSE if all controls contain valid information.
//
//  COMMENTS:
//    This is a helper function for the WM_NOTIFY - PSN_KILLACTIVE
//    message for the "Output" dialog procedure.
//    It is called by the WM_NotifyMsg() procedure. If an invalid
//    entry is found the function displays an
//    error message to the user and sets the focus to the control
//    containing the invalid entry. The caller
//    should prevent this page from losing activation so
//    the user can correct the value.
//

BOOL ValidateOutputControls(HWND hDlg)
{
    char szFileName[MAX_PATH];
    BOOL bReturnVal;
    int iCount;
        
    bReturnVal = FALSE;     //Assume no errors
    
    //If File is specified verify that a valid file name is given
    if (IsDlgButtonChecked(hDlg, IDC_DISK))
    {
        iCount = GetDlgItemText(hDlg, IDC_FILENAME, szFileName, MAX_PATH - 1);
        if (iCount < 1)
        {
            ErrorMessage("Please specify a file name.","File Option Chosen");
            SetFocus(GetDlgItem(hDlg, IDC_FILENAME));
            bReturnVal = TRUE;      // Return Error
        }
        else
        {
            if(!ValidFileName(szFileName))
            {
                ErrorMessage("File name is not valid.","File Option");
                SetFocus(GetDlgItem(hDlg, IDC_FILENAME));
                bReturnVal = TRUE;      //Error
            }
        }
    }
    
    return bReturnVal;
}




//
//  FUNCTION: ValidateRangeControls(HWND)
//
//  PURPOSE:  Validate the contents of the controls on the property
//            page that effect the range of messages the program will filter
//
//
//  PARAMETERS:
//    HWND     - Window handle of the property page containing
//               the "Range" controls
//
//
//  RETURN VALUE:
//    TRUE if a control contains an invalid entry.
//    FALSE if all controls contain valid information.
//
//  COMMENTS:
//    This is a helper function for the WM_NOTIFY - PSN_KILLACTIVE
//    message for the "Range" dialog procedure.
//    It is called by the WM_NotifyMsg() procedure. If an invalid
//    entry is found the function displays an
//    error message to the user and sets the focus to the control
//    containing the invalid entry. The caller
//    should prevent this page from losing activation so the user
//    can correct the value.
//

BOOL ValidateRangeControls(HWND hDlg)
{
    if (InvalidRange(hDlg, IDC_INCSTART, IDC_INCEND))
        return TRUE;            //Return Error
    else
        return FALSE;           //Return No Error
}

//
//  FUNCTION: ValidFileName(char *)
//
//  PURPOSE:  Verifies that the passed in file name is valid.
//
//
//  PARAMETERS:
//    char *     - Pointer to a NULL terminated string that contains
//                 the path+filename to verify
//
//
//  RETURN VALUE:
//    TRUE if file name is valid
//    FALSE if file name is invalid
//
//  COMMENTS:
//    This is a helper function for ValidateOutputControls()

BOOL ValidFileName(char *pszFileName)
{
    BOOL bReturnVal;
    HANDLE hFile;
    DWORD dwErrorVal;
    
    bReturnVal = TRUE;                   //Assume no error
    
    //See if valid file name was provided
    hFile = CreateFile(pszFileName,
                        GENERIC_READ | GENERIC_WRITE,   
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,            //No security attributes
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        0);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        //Error occured
        bReturnVal = FALSE;             //Specify return value
        dwErrorVal = GetLastError();    //See what the error was
        ErrorMessage("Unable to open file", "Warning");
    }
    else
    {
        //No error occured
        if (!CloseHandle(hFile))    //Close the file
        {
            //Function failed
            //DEBUG START
              ErrorMessage("Error","Unable to close specified file");
              dwErrorVal = GetLastError();    //Get the error
            //DEBUG END
        }
    }
        
    return bReturnVal;
}

//
//  FUNCTION: InvalidRange(HWND, UINT, UINT)
//
//  PURPOSE:  Verifies that the values of the two controls passed
//            in our in range
//
//
//  PARAMETERS:
//    HWND     - Handle of the property page that contains the controls
//    UINT     - ID of the first control
//    UINT     - ID of the second control
//
//
//  RETURN VALUE:
//    TRUE if the value in one of the controls is not in range
//    FALSE if the value of both controls is in range
//
//  COMMENTS:
//    This is a helper function for ValidateRangeControls()
//    Return TRUE if value in control is outside the range
//    of valid message numbers (error)
//    Return FALSE if value is within range (no error)


BOOL InvalidRange(HWND hDlg, UINT uiStartControl, UINT uiEndControl)
{
    BOOL bReturnVal;
    char szStart[6];
    char szEnd[6];
    int iStartCount;
    int iEndCount;
    UINT uiRange;
    BOOL bNoError;
    char szErrorMsg[75];
    
    bReturnVal = FALSE;     //Assume no error
    
    iStartCount = GetDlgItemText(hDlg, uiStartControl, szStart, 6);
    iEndCount = GetDlgItemText(hDlg, uiEndControl, szEnd, 6);
    
    //If either of them have a value in them then both must be validated
    if ( (iStartCount > 0) || (iEndCount > 0) )
    {
        //Check starting value
        uiRange = GetDlgItemInt(hDlg, uiStartControl, &bNoError, FALSE);
        if (bNoError)
        {
            if ( (uiRange < MIN_MESSAGEID) || (uiRange > MAX_MESSAGEID) )
            {
                SetFocus(GetDlgItem(hDlg, uiStartControl));
                bReturnVal = TRUE;      //Error
            }
        }
        else
        {
            SetFocus(GetDlgItem(hDlg, uiStartControl));
            bReturnVal = TRUE;      //Error
        }

        //Check end value
        uiRange = GetDlgItemInt(hDlg, uiEndControl, &bNoError, FALSE);
        if (bNoError)
        {
            if ( (uiRange < MIN_MESSAGEID) || (uiRange > MAX_MESSAGEID) )
            {
                SetFocus(GetDlgItem(hDlg, uiEndControl));
                bReturnVal = TRUE;      //Error
            }
        }
        else
        {
            SetFocus(GetDlgItem(hDlg, uiEndControl));
            bReturnVal = TRUE;      //Error
        }
        
    }

    //If error occured display error message
    if (bReturnVal)
    {       
        wsprintf(szErrorMsg,
        "Value must be in the range %d - %d",MIN_MESSAGEID,MAX_MESSAGEID);
        ErrorMessage(szErrorMsg,"Out Of Range");
    }
            
    return bReturnVal;
}

//
//  FUNCTION: ApplyTypeControls(HWND)
//
//  PURPOSE:  Store the changes requested by the user for the
//            "Type" of messages filtered in GlobalOptions
//
//
//  PARAMETERS:
//    HWND     - Handle of the property page that contains the controls
//
//  RETURN VALUE:
//    TRUE
//
//  COMMENTS:
//

BOOL ApplyTypeControls(HWND hDlg)
{



    DWORD dwMsgFilterOptions;
    UINT uLoValue;
    UINT uHiValue;
    BOOL bMonitorChild;
    
    dwMsgFilterOptions = ReadType();    // Read the message types
                                        // we want to process
    ReadRange(&uLoValue, &uHiValue);    // Read the range specified
    bMonitorChild = ReadInput();        // Read the source for the messages


    // Call DLL to make changes active
    ChangeFilterOptions(dwMsgFilterOptions,
                        uLoValue,
                        uHiValue,
                        bMonitorChild);
        
    return TRUE;
    
}

//
//  FUNCTION: ReadType(void)
//
//  PURPOSE:  Read all the values from the Type page
//
//
//  PARAMETERS:
//    non
//
//  RETURN VALUE:
//    DWORD that contains the flags
//
//  COMMENTS:
//    This function assumes the page has already been validated and
//    therefore does no validate the page
//

DWORD ReadType(void)
{

    DWORD dwMsgFilterOptions;
    
    dwMsgFilterOptions = 0;
    
    //Turn on those flags whose box is checked
    if (IsDlgButtonChecked(hType, IDC_OTHER))
        dwMsgFilterOptions |= MSG_UNKNOWN;

    if (IsDlgButtonChecked(hType, IDC_MOUSE))
        dwMsgFilterOptions |= MSG_MOUSE;

    if (IsDlgButtonChecked(hType, IDC_KEYBOARD))
        dwMsgFilterOptions |= MSG_KEYBOARD;

    if (IsDlgButtonChecked(hType, IDC_MENU))
        dwMsgFilterOptions |= MSG_MENU;

    if (IsDlgButtonChecked(hType, IDC_NONCLIENT))
        dwMsgFilterOptions |= MSG_NON_CLIENT;

    if (IsDlgButtonChecked(hType, IDC_CLIPBOARD))
        dwMsgFilterOptions |= MSG_CLIPBOARD;

    if (IsDlgButtonChecked(hType, IDC_SYSTEM))
        dwMsgFilterOptions |= MSG_SYSTEM;

    if (IsDlgButtonChecked(hType, IDC_TIMER))
        dwMsgFilterOptions |= MSG_TIMER;

    if (IsDlgButtonChecked(hType, IDC_DDE))
        dwMsgFilterOptions |= MSG_DDE;

    if (IsDlgButtonChecked(hType, IDC_MDI))
        dwMsgFilterOptions |= MSG_MDI;

    if (IsDlgButtonChecked(hType, IDC_WIN31))
        dwMsgFilterOptions |= MSG_WIN31_MSGS;

    if (IsDlgButtonChecked(hType, IDC_USER))
        dwMsgFilterOptions |= MSG_WM_USER;

    if (IsDlgButtonChecked(hType, IDC_RANGE))
        dwMsgFilterOptions |= MSG_MSG_RANGE_ONLY;
        
    if (IsDlgButtonChecked(hType, IDC_WIN40))
        dwMsgFilterOptions |= MSG_WIN40_MSGS;

    if (IsDlgButtonChecked(hType, IDC_WIN32))
        dwMsgFilterOptions |= MSG_WIN40_MSGS;

    if (IsDlgButtonChecked(hType, IDC_LISTBOX))
        dwMsgFilterOptions |= MSG_LB_MSGS;

    if (IsDlgButtonChecked(hType, IDC_COMBOBOX))
        dwMsgFilterOptions |= MSG_CB_MSGS;
        
    if (IsDlgButtonChecked(hType, IDC_EDITCTL))
        dwMsgFilterOptions |= MSG_EDT_MSGS;

    if (IsDlgButtonChecked(hType, IDC_BUTTONCTL))
        dwMsgFilterOptions |= MSG_BTN_MSGS;
        
    if (IsDlgButtonChecked(hType, IDC_STATICCTL))
        dwMsgFilterOptions |= MSG_STT_MSGS;
        
    if (IsDlgButtonChecked(hType, IDC_UNKNOWN))
        dwMsgFilterOptions |= MSG_UNKNOWN;
        
    if (IsDlgButtonChecked(hType, IDC_CONTROL))
        dwMsgFilterOptions |= MSG_CTL_MSGS;
        
    if (IsDlgButtonChecked(hType, IDC_STANDARD))
        dwMsgFilterOptions |= MSG_STANDARD_MSGS;
        
    return dwMsgFilterOptions;
}


//
//  FUNCTION: ApplyInputControls(HWND)
//
//  PURPOSE:  Store the changes requested by the user for the
//            what windows should be hooked
//
//
//  PARAMETERS:
//    HWND     - Handle of the property page that contains the controls
//
//  RETURN VALUE:
//    TRUE
//
//  COMMENTS:
//
BOOL ApplyInputControls(HWND hDlg)
{
    
    DWORD dwMsgFilterOptions;
    UINT uLoValue;
    UINT uHiValue;
    BOOL bMonitorChild;
    
    dwMsgFilterOptions = ReadType();    // Read the message types
                                        // we want to process
    ReadRange(&uLoValue, &uHiValue);    // Read the range specified
    bMonitorChild = ReadInput();        // Read the source for the messages


    //Call DLL to make changes active
    ChangeFilterOptions(dwMsgFilterOptions,
                        uLoValue,
                        uHiValue,
                        bMonitorChild); 

    return TRUE;
}

//
//  FUNCTION: BOOL ReadInput(void)
//
//  PURPOSE:  Read all the values from the Input page
//
//
//  PARAMETERS:
//    non
//
//  RETURN VALUE:
//    BOOL - TRUE if we are to monitor child windows
//    FALSE - If we are to monitor a single window
//
//  COMMENTS:
//    This function assumes the page has already been
//    validated and therefore does no validate the page
//

BOOL ReadInput(void)
{
    BOOL bMonitorChild;
    
    if (IsDlgButtonChecked(hInput, IDC_SINGLE))
    {
        bMonitorChild = FALSE;
        pS->fMonitorChild = FALSE;
    }
    else
    {
        bMonitorChild = TRUE;
        pS->fMonitorChild = TRUE;
    }

    if (IsDlgButtonChecked(hInput, IDC_ALLWIN))
        pS->hWndMonitored = (HWND) 0x0000FFFF;


    return bMonitorChild;
}        

    
//
//  FUNCTION: ApplyOutputControls(HWND)
//
//  PURPOSE:  Store the changes requested by the user for the
//           "Type" of messages filtered in GlobalOptions
//
//
//  PARAMETERS:
//    HWND     - Handle of the property page that contains the controls
//
//  RETURN VALUE:
//    TRUE
//
//  COMMENTS:
//

BOOL ApplyOutputControls(HWND hDlg)
{

    ReadOutput();
    return TRUE;
}

//
//  FUNCTION: UINT ReadOutput(void)
//
//  PURPOSE:  Read all the values from the output page
//
//
//  PARAMETERS:
//    non
//
//  RETURN VALUE:
//    UINT - Flags specifying where to show the messages being captured
//
//
//
//  COMMENTS:
//    This function assumes the page has already been validated and therefore does no validate the page
//
void ReadOutput(void)
{
    pS->fOutputToWindow = (IsDlgButtonChecked(hOutput, IDC_WINDOW));
    pS->fOutputToCom    = (IsDlgButtonChecked(hOutput, IDC_COM1));
        
    if (IsDlgButtonChecked(hOutput, IDC_DISK))
    {
        char szFileName[MAX_PATH];

        pS->fOutputToLogFile = TRUE;
        GetDlgItemText(hOutput, IDC_FILENAME, szFileName, MAX_PATH - 1);
        lstrcpy (pS->szOutputFile, szFileName);
    }
    else
        pS->fOutputToLogFile = FALSE;
}
    



//
//  FUNCTION: ApplyRangeControls(HWND)
//
//  PURPOSE:  Store the changes requested by the user for
//            the range of messages that will be filtered
//
//
//  PARAMETERS:
//    HWND     - Handle of the property page that contains the controls
//
//  RETURN VALUE:
//    TRUE
//
//  COMMENTS:
//

BOOL ApplyRangeControls(HWND hDlg)
{
    DWORD dwMsgFilterOptions;
    BOOL bMonitorChild;
    UINT uLoRange, uHiRange;
    
    dwMsgFilterOptions = ReadType();    // Read the message
                                        // types we want to process
    ReadRange(&uLoRange, &uHiRange);    // Read the range specified
    bMonitorChild = ReadInput();        // Read the source for the messages

    // Call DLL to make changes active
    ChangeFilterOptions(dwMsgFilterOptions,
                        uLoRange,
                        uHiRange,
                        bMonitorChild); 

    return TRUE;
}

//
//  FUNCTION: void ReadRange(UINT *, UINT *)
//
//  PURPOSE:  Read all the values from the Range page
//
//
//  PARAMETERS:
//    UINT * - Address to place the Starting range value
//    UINT * - Address to place the Ending range value
//
//  RETURN VALUE:
//    DWORD that contains the flags
//
//  COMMENTS:
//    This function assumes the page has already
//    been validated and therefore does no validate the page
//

void ReadRange(UINT *uLoValue, UINT *uHiValue)
{

    char szBuffer[6];
    BOOL bNoError;
    
    //If their is a range value specified, get that value
    if(GetDlgItemText(hRange, IDC_INCSTART, szBuffer, 6))
    {
        *uLoValue = GetDlgItemInt(hRange, IDC_INCSTART, &bNoError, FALSE);
        *uHiValue = GetDlgItemInt(hRange, IDC_INCEND, &bNoError, FALSE);
    }
    else
    {
        *uLoValue = 0;
        *uHiValue = 0;
    }
    return;
}


//
//  FUNCTION: ErrorMessage(char *, char *)
//
//  PURPOSE:  Sound a beep, and display a error message message box
//            to the user explaining the problem
//
//
//  PARAMETERS:
//    char *     - Pointer to a NULL terminated character string that
//                 contains the title for the message box.
//    char *     - Pointer to a NULL terminated character string that
//                 contains the text of the message box.
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    Helper function available to the program to display messages
//    to the user
//

void ErrorMessage(char *pszFunction, char *pszMessage)
{
    WarningBeep();
    MessageBox(NULL,pszFunction,pszMessage, MB_ICONEXCLAMATION);
}

//
//  FUNCTION: WarningBeep(void)
//
//  PURPOSE:  Make a sound to draw the users attention to the screen.
//
//
//  PARAMETERS:
//    None
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    Helper function available to the program to make a audible sound.
//


void WarningBeep(void)
{
    MessageBeep(MB_ICONEXCLAMATION);
    return;
}
