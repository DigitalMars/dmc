// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   rtf.c
//
//  PURPOSE:  Provide an interface to the RichEdit control
//
//  FUNCTIONS:
//    RTF_ChangeCharAttribute -  To set the selected text to 
//                               have the supplied attributes.
//    RTF_GetCharAttribute -     To determine if the selected 
//                               text has the requested attributes
//    RTF_ChangeSizeAttribute -  Change the point size of the selected text
//    RTF_ChangeFont -           Change the attributes of the selected text 
//                               with the Common Font Dialog.
//    SaveCallback -             Callback routine for the 
//                               EM_STREAMOUT Message from 
//                               the RichEdit control.
//    RTF_Save -                 Save the contents of the Richedit 
//                               control to a file       
//    OpenCallback -             Callback routine for the EM_STREAMIN 
//                               Message from the RichEdit control.
//    RTF_Open -                 Load the contents of a file ito the 
//                               Richedit control
//    RTF_GetCurrentFont -       Get the facename of the selected text's font
//    RTF_SetCurrentFont -       Set the facename of the selected text's font
//    RTF_ChangeMargins -        Set the margins of the current paragraph.
//    RTF_ShowMargins -          Get the margins of the current paragraph 
//                               and reflect them on the ruler.
//    SetToolbarButtonToReflectCharacterAttribute - Show the status of an 
//                                                  attribute on a toolbar button
//    RTF_ShowCharAttributes -   Show the status of the bold, itailc, 
//                               and underline attributes on the toolbar
//
//  COMMENTS:
//

 
#include <windows.h>     // required for all Windows applications
#include <windowsx.h>
#include <malloc.h>
#include <commctrl.h>
#include <richedit.h>
#include "globals.h"     // prototypes specific to this application
#include "resource.h"
#include "rtf.h"         // prototypes specific to this application
#include "ruler.h"
#include "toolbar.h"

//
//  FUNCTION: RTF_ChangeCharAttribute(HWND, DWORD, DWORD)
//
//  PURPOSE: To set the selected text to have the supplied
//           attributes.
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    dwMask - Mask of which attributes to change
//    dwEffects - State of each attribute
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//
//

void RTF_ChangeCharAttribute(HWND hRTFWnd, DWORD dwMask, DWORD dwEffects)
{
    CHARFORMAT cf; // This is defined in RICHEDIT.H

    cf.cbSize = sizeof(cf);

    // Get the current status of the text into the CHARFORMAT structure,
    // use the selected text. wParam is TRUE to indicate the
    // selected textt, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    // Set the mask to tell the RTF Edit control to pay attention to
    // the appropriate bit of the dwEffects field.
    cf.dwMask      = dwMask;
    // Twiddle de bits, captain!
    cf.dwEffects  ^= dwEffects;

    // Set the new underline status to the selected text
    SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

//
//  FUNCTION: RTF_GetCharAttribute(HWND, DWORD, DWORD)
//
//  PURPOSE: To determine if the selected text has the requested
//           attributes
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    dwMask - Mask of which attributes to check
//    dwEffects - State of each attribute
//
//  RETURN VALUE:
//    One of three constants:
//
//      CHAR_ATTRIB_YES - All of the selected text has the attribute
//      CHAR_ATTRIB_NO  - None of the selected text has the attribute
//      CHAR_ATTRIB_WAFFLE - Some, but not all of the selected text 
//                           has the attribute
//
//  COMMENTS:
//
//

int RTF_GetCharAttribute(HWND hRTFWnd, DWORD dwMask, DWORD dwEffects)
{
    CHARFORMAT cf; // This is defined in RICHEDIT.H

    cf.cbSize = sizeof(cf);

    // Get the current status of the text into the CHARFORMAT structure,
    // use the selected text. wParam is TRUE to indicate the
    // selected textt, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    if (cf.dwMask & dwMask)
    {
        if (cf.dwEffects & dwEffects)
            return CHAR_ATTRIB_YES;
        else
            return CHAR_ATTRIB_NO;
    }
    else
    {
        return CHAR_ATTRIB_WAFFLE;
    }
}

//
//  FUNCTION: RTF_ChangeSizeAttribute(HWND, int)
//
//  PURPOSE: Change the point size of the selected text
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    iPointChange - Number of points, positive or neagtive to
//                   change text
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
//

void RTF_ChangeSizeAttribute(HWND hRTFWnd, int iPointChange)
{
    CHARFORMAT cf; // This is defined in RICHEDIT.H

    cf.cbSize = sizeof(cf);

    // Get the current status of the text into the CHARFORMAT structure,
    // use the selected text. wParam is TRUE to indicate the
    // selected textt, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    // Set the mask to tell the RTF Edit control to pay attention to
    // the size bit of the dwEffects field.
    cf.dwMask      = CFM_SIZE;
    // Increase or decrease the font size, keeping it between 6 and 128 points
    if ( 
         ((cf.yHeight + 20*iPointChange) <= (128*20))
         &&
         ((cf.yHeight + 20*iPointChange) >= (6*20))
       )
        cf.yHeight += 20*iPointChange;

    // Set the new underline status to the selected text
    SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

//
//  FUNCTION: RTF_ChangeFont(HWND hWnd, HWND hRTFWnd)
//
//  PURPOSE: Change the attributes of the selected text with the
//           Common Font Dialog.
//
//  PARAMETERS:
//    hWnd - Handle to a frame suitable as a parent to the Common
//           font dialog.
//    hRTFWnd - Handle to RichEdit control
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
//

void RTF_ChangeFont(HWND hWnd, HWND hRTFWnd)
{
    CHARFORMAT cf;                // This is defined in RICHEDIT.H
    CHOOSEFONT ChooseFontStruct;  // Common Dialog fun
    LOGFONT    lf;                // Log font information
    HDC        hDC;

    cf.cbSize = sizeof(cf);

    // Get the font formatting status into the CHARFORMAT strucuture,
    // use the selected text. wParam is TRUE to indicate the
    // selected text, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    // Fill in the font info for the font common fialog

    memset(&ChooseFontStruct, 0, sizeof(CHOOSEFONT));
    memset(&lf, 0, sizeof(LOGFONT));

    hDC = GetDC(hWnd); // Need a screen DC, use the parent's DC

    // The RTF Edit control measures in twips. Each point is
    // 20 twips.

    lf.lfHeight = cf.yHeight/-20;

    // Set up the rest of the logfont structure according to the 
    // information retrieved from the EM_GETCHARFORMAT message

    if (cf.dwEffects & CFE_BOLD)
        lf.lfWeight = FW_BOLD;
    else
        lf.lfWeight = FW_NORMAL;
    lf.lfItalic = (BOOL)(cf.dwEffects & CFE_ITALIC);
    lf.lfUnderline = (BOOL)(cf.dwEffects & CFE_UNDERLINE);
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = cf.bPitchAndFamily;
    lstrcpy(lf.lfFaceName, cf.szFaceName);

    // Fire up the common dialog.

    ChooseFontStruct.lStructSize = sizeof(ChooseFontStruct);
    ChooseFontStruct.hwndOwner = hWnd;
    ChooseFontStruct.hDC = hDC;
    ChooseFontStruct.lpLogFont = &lf;
    ChooseFontStruct.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
    ChooseFontStruct.rgbColors = RGB(0,0,0);
    ChooseFontStruct.nFontType = SCREEN_FONTTYPE;

    if (ChooseFont(&ChooseFontStruct))
    {
        // Set the mask to tell the RTF Edit control to pay attention to
        // the font formatting bits.
        cf.dwMask = CFM_BOLD | CFM_FACE | CFM_ITALIC | 
                    CFM_OFFSET | CFM_SIZE | CFM_UNDERLINE;

        //  Undo the equation from above.

        cf.yHeight = lf.lfHeight * -20;

        // Fill in the rest of the characater formatting
         
        cf.dwEffects = 0;
        if (FW_BOLD == lf.lfWeight)
            cf.dwEffects |= CFE_BOLD;
        if (lf.lfItalic)
            cf.dwEffects |= CFE_ITALIC;

        if (lf.lfUnderline)
            cf.dwEffects |= CFE_UNDERLINE;

        cf.bPitchAndFamily = lf.lfPitchAndFamily;

        lstrcpy(cf.szFaceName, lf.lfFaceName);

        // Set the new formatting to the selected text
        SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    // Don't forget this!
    ReleaseDC(hWnd, hDC);
}

//
//  FUNCTION: SaveCallback(DWORD, LPBYTE, LONG, LONG *)
//
//  PURPOSE: Callback routine for the EM_STREAMOUT Message from
//           the RichEdit control.
//
//  PARAMETERS:
//    dwCookie - App supplied value (a file handle for this example)
//    pbBuff - Pointer to data to write
//    cb - size of data to write
//    pcb - Number of bytes written to the file
//
//  RETURN VALUE:
//    Always 0
//
//  COMMENTS:
//
//

DWORD CALLBACK SaveCallback(DWORD    dwCookie, // App supplied value
                            LPBYTE   pbBuff,   // Pointer to data
                            LONG     cb,       // size of data
                            LONG     *pcb)     // Not used in streamout
{
    // Write the data to the disk for our example. dwCookie is used as a file
    // handle for our example.
    cb = _lwrite((HFILE)dwCookie, pbBuff, cb);
    *pcb = cb; // Indicate the number of bytes written to the file
    return 0;
}

//
//  FUNCTION: RTF_Save(LPSTR, HWND)
//
//  PURPOSE: Save the contents of the Richedit control to a file
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    szFileName - Name of file to save to
//
//  RETURN VALUE:
//    TRUE if successful, FALSE otherwise
//
//  COMMENTS:
//    This function utilizes the SaveCallback function above
//

BOOL RTF_Save(LPSTR szFileName, HWND hRTFWnd)
{
    HFILE         hFile; // File handle
    OFSTRUCT      of;    // open file strucuture
    EDITSTREAM    es;    // The EDITSTREAM structure

    // Call the file save as dialog only if the szFileName is empty,
    // otherwise szFileName already has a valid filename in it.

    if (*szFileName)
    {
        // Open the file, erasing previous contents
        hFile = OpenFile(szFileName, &of, OF_CREATE);

        // Set up the EDITSTREAM structure
        es.dwCookie    = (DWORD)hFile;   // our file handle
        es.dwError     = 0;              // No error
        es.pfnCallback = SaveCallback;   // Use above callback

        // Engage!
        SendMessage(hRTFWnd, EM_STREAMOUT, SF_RTF, (LPARAM)&es);

        // Close the file
        _lclose ( hFile );

        return TRUE;
    }

    return FALSE;
}

//
//  FUNCTION: OpenCallback(DWORD, LPBYTE, LONG, LONG *)
//
//  PURPOSE: Callback routine for the EM_STREAMIN Message from
//           the RichEdit control.
//
//  PARAMETERS:
//    dwCookie - App supplied value (a file handle for this example)
//    pbBuff - Pointer to data to read from disk
//    cb - size of data to read
//    pcb - Number of bytes read from the file
//
//  RETURN VALUE:
//    Always 0
//
//  COMMENTS:
//
//

DWORD CALLBACK OpenCallback(DWORD    dwCookie,
                            LPBYTE   pbBuff,
                            LONG     cb,
                            LONG     *pcb)
{
  // Read as much data as allowed in the cb variable
  *pcb = _lread((HFILE)dwCookie, pbBuff, cb);
  if (*pcb <= 0) // EOF?
      *pcb = 0;    // Indicate the read is complete
  return 0;
}

//
//  FUNCTION: RTF_Open(LPSTR, HWND)
//
//  PURPOSE: Load the contents of a file ito the Richedit control
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    szFileName - Name of file to save to
//
//  RETURN VALUE:
//    TRUE if successful, FALSE otherwise
//
//  COMMENTS:
//    This function utilizes the OpenCallback function above
//

BOOL RTF_Open ( LPSTR szFileName, HWND hRTFWnd )
{
    HFILE         hFile;  // File handle
    OFSTRUCT      of;     // open file strucuture
    EDITSTREAM    es;     // The EDITSTREAM structure

    // Get a filename to load from

    if (*szFileName)
    {
        // Open the file for reading
        hFile = OpenFile(szFileName, &of, OF_READ);

        if (hFile)
        {
            es.dwCookie    = (DWORD)hFile; // our file handle
            es.dwError     = 0;            // No error
            es.pfnCallback = OpenCallback; // Use above callback

            // Make it so!
            SendMessage(hRTFWnd, EM_STREAMIN, SF_RTF, (LPARAM)&es);

            // Clear the dirty bit
            SendMessage(hRTFWnd, EM_SETMODIFY, 0, 0L);

            // Close the file
            _lclose(hFile);
        }

        // Update the toolbar
        RTF_ShowCharAttributes();

        return TRUE;
    }

    return FALSE;
}

//
//  FUNCTION: RTF_GetCurrentFont(HWND, LPSTR)
//
//  PURPOSE: Get the facename of the selected text's font
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    szFont - String to contain facename
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    The string is a null string if the selection has more than
//    one facename of font
//

void RTF_GetCurrentFont ( HWND hRTFWnd, LPSTR szFont )
{
    CHARFORMAT cf;                // This is defined in RICHEDIT.H

    cf.cbSize = sizeof(cf);
        
    // Get the font formatting status into the CHARFORMAT strucuture,
    // use the selected text. wParam is TRUE to indicate the
    // selected text, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    if (cf.dwMask & CFM_FACE)
        lstrcpy(szFont, cf.szFaceName);
    else
        *szFont = 0;

}

//
//  FUNCTION: RTF_SetCurrentFont(HWND, LPSTR)
//
//  PURPOSE: Set the facename of the selected text's font
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    szFont - String that contains facename
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void RTF_SetCurrentFont ( HWND hRTFWnd, LPSTR szFont )
{
    CHARFORMAT cf;                // This is defined in RICHEDIT.H

    cf.cbSize = sizeof(cf);

    // Get the font formatting status into the CHARFORMAT strucuture,
    // use the selected text. wParam is TRUE to indicate the
    // selected text, FALSE for the first character in the
    // RTF Edit control.
    SendMessage(hRTFWnd, EM_GETCHARFORMAT, TRUE, (LPARAM)&cf);

    cf.dwMask = CFM_FACE;

    lstrcpy(cf.szFaceName, szFont);

    SendMessage(hRTFWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

//
//  FUNCTION: RTF_ChangeMargins(HWND, UINT, UINT, UINT)
//
//  PURPOSE: Set the margins of the current paragraph.
//
//  PARAMETERS:
//    hRTFWnd - Handle to RichEdit control
//    dxStartIndent - Indent in twips of first line's left margin
//    dxRightIndent - Indent in twips from right edge of printable area
//                    of right margin.
//    dxOffset - Distance in twips of second, third, etc line's left
//               margin
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This function uses the RM_GETTABS (RULER.H) message to set the
//    tabs.
//

void RTF_ChangeMargins(HWND hwndRuler,
                       UINT dxStartIndent,
                       UINT dxRightIndent,
                       UINT dxOffset)
{
    PARAFORMAT pf;

    pf.cbSize = sizeof(pf);
    pf.dwMask = PFM_STARTINDENT | PFM_RIGHTINDENT | PFM_OFFSET | PFM_TABSTOPS;
    pf.dxStartIndent = dxStartIndent;
    pf.dxRightIndent = dxRightIndent;
    pf.dxOffset = dxOffset;

    pf.cTabCount = (short)SendMessage(hwndRuler, 
                                      RM_GETTABS, 
                                      (WPARAM)25,
                                      (LPARAM)(pf.rgxTabs));

    SendMessage(GetEditWindow(NULL), EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

//
//  FUNCTION: RTF_ShowMargins(HWND)
//
//  PURPOSE: Get the margins of the current paragraph and reflect them
//           on the ruler.
//
//  PARAMETERS:
//    hwndRuler - Handle to Ruler control (RULER.H)
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void RTF_ShowMargins(HWND hwndRuler)
{
     PARAFORMAT pf;
     
     pf.cbSize = sizeof(pf);
     SendMessage (GetEditWindow(NULL), EM_GETPARAFORMAT, 0, (LPARAM)&pf);

     if (
          (pf.dwMask & (PFM_STARTINDENT|PFM_RIGHTINDENT|PFM_OFFSET)) ==
          (PFM_STARTINDENT|PFM_RIGHTINDENT|PFM_OFFSET)
        )
     {
         int iMargins[5];
         iMargins[0] = pf.dxStartIndent + pf.dxOffset;
         iMargins[1] = pf.dxRightIndent;
         iMargins[2] = pf.dxStartIndent;
         iMargins[3] = 0;
         iMargins[4] = 7*1440;
         SendMessage(hwndRuler, RM_SETMARGINS, 0, (LPARAM)iMargins);
     }
     SendMessage (hwndRuler, 
                  RM_SETTABS, 
                  (WPARAM)(pf.cTabCount), 
                  (LPARAM)(pf.rgxTabs));

}

//
//  FUNCTION: SetToolbarButtonToReflectCharacterAttribute(UINT,DWORD,DWORD)
//
//  PURPOSE: Show the status of an attribute on a toolbar button
//
//  PARAMETERS:
//    uID - ID of the toolbar button
//    dwMask - Attribute mask to check for
//    dwEffects - Attribute to check for
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void SetToolbarButtonToReflectCharacterAttribute(UINT uID, 
                                                 DWORD dwMask, 
                                                 DWORD dwEffects)
{
    int iState = RTF_GetCharAttribute(GetEditWindow(NULL), 
                                      dwMask, 
                                      dwEffects);
    switch (iState)
    {
        case CHAR_ATTRIB_YES:    

            UpdateButton (uID, TBSTATE_ENABLED | TBSTATE_CHECKED); 
            break;

        case CHAR_ATTRIB_NO :    

            UpdateButton (uID, TBSTATE_ENABLED); 
            break;

        case CHAR_ATTRIB_WAFFLE: 

            UpdateButton (uID, TBSTATE_ENABLED | TBSTATE_INDETERMINATE); 
            break;
    }
}

//
//  FUNCTION: RTF_ShowCharAttributes()
//
//  PURPOSE: Show the status of the bold, itailc, and underline
//           attributes on the toolbar
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void RTF_ShowCharAttributes(void)
{
  SetToolbarButtonToReflectCharacterAttribute(IDM_EDITBOLD, 
                                              CFM_BOLD, 
                                              CFE_BOLD);
  SetToolbarButtonToReflectCharacterAttribute(IDM_EDITITALIC, 
                                              CFM_ITALIC, 
                                              CFE_ITALIC);
  SetToolbarButtonToReflectCharacterAttribute(IDM_EDITUNDERLINE, 
                                              CFM_UNDERLINE, 
                                              CFE_UNDERLINE);
}
