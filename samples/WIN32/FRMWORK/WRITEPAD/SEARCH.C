// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: search.c
//
//  PURPOSE: Implement search and replace on an edit buffer.
//
//  FUNCTIONS:
//    FindReplace- Find and/or replace text in the current document.
//    Find       - Search for a string in the edit text.
//    Replace    - Replace the current selection.
//    ReplaceAll - To replace all of the matches found in the edit text.
//    FWholeWord - Check to see if the match is a whole word.
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#include "globals.h"            // prototypes specific to this application

BOOL Find(char *, BOOL, BOOL, BOOL, BOOL);
VOID Replace(char *);
VOID ReplaceAll(char *, char *, BOOL, BOOL);
BOOL FWholeWord(char *, char *, char *, int);

//
//  FUNCTION: FindReplace(char*,char*,BOOL,BOOL,BOOL,BOOL,BOOL,BOOL)
//
//  PURPOSE: Find and/or replace text in the current document.
//
//  PARAMETERS:
//    szFind       - The string to find.
//    szReplace    - The string used to replace.
//    frt -
//      frtFind    - Find the string szFind
//      frtReplace - Replace the current selection with szReplace
//      frtRepAll  - Replace all occurances of szFind with szReplace
//    fDown        - Search down if true, up if false.
//    fMatchCase   - Match case on a search.
//    fWholeWord   - Match whole words only.
//
//  RETURN VALUE:
//    NONE.
//
//  COMMENTS:
//
//

VOID FindReplace(char *szFind, 
                 char *szReplace,
                 FRT frt,
                 BOOL fDown, 
                 BOOL fMatchCase, 
                 BOOL fWholeWord)
{
    switch (frt)
    {
        case frtFind:
            Find(szFind, fDown, fMatchCase, fWholeWord, FALSE);
            break;
        case frtReplace:
            Replace(szReplace);
            break;
        case frtRepAll:
            ReplaceAll(szFind, szReplace, fMatchCase, fWholeWord);
    }
}


//
//  FUNCTION: Find(char *, BOOL, BOOL, BOOL, BOOL)
//
//  PURPOSE: Search for a string in the edit text.
//
//  PARAMETERS:
//    szFind    - The string to search for.
//    fDown     - Search from current position to the end of the text.
//    fMatchCase- Match the case of the search string.
//    fWholeWord- Target must have white space on both sides.
//    fNoScroll - Don't scroll the window to the selection.
//
//  RETURN VALUE:
//    TRUE - String was found.
//    FALSE - Search was not sucessful.
//
//  COMMENTS:
//
//

BOOL Find(char *szFind, 
          BOOL fDown, 
          BOOL fMatchCase, 
          BOOL fWholeWord, 
          BOOL fNoScroll)
{
    char  *szText;          // Pointer to edit text
    UINT   ichStart;        // Index of start of selection
    UINT   ichEnd;          // Index of end of selection
    UINT   cch;             // Number of charaters in edit text
    int    dch;             // Number of characters to move (+/-1)
    int    dchComp;
    char  *szTerm;          // Pointer to terminal point in search
    char  *sz;              // Pointer to current character
    UINT   cchFind;         // Length of find string
    BOOL   fFound = FALSE;  // Found flag
    int    dchOnFind;       //

    szText  = LockEditText(GetEditWindow(NULL));
    cchFind = strlen(szFind);

    SendMessage(GetEditWindow(NULL), 
                EM_GETSEL, 
                (WPARAM)&ichStart, 
                (LPARAM)&ichEnd);

    cch = strlen(szText);

    if (
        ichEnd-ichStart == cchFind &&
        !strnicmp(szText+ichStart,szFind,cchFind)
    )
        dchOnFind = 1;
    else
        dchOnFind = 0;

    if (fDown)
    {
        dch = 1;
        dchComp = 0;
        sz = szText + ichStart + dchOnFind;
        szTerm = szText + cch;
    }
    else
    {
        dch = -1;
        dchComp = 1;
        sz = szText + ichStart - dchOnFind;
        szTerm = szText;
    }

    for (; sz + dchComp != szTerm; sz += dch)
    {
        if (
            tolower(*sz) == tolower(*szFind) &&
            (fMatchCase ?
                !strncmp(sz,szFind,cchFind) :
                !strnicmp(sz,szFind,cchFind)
            ) &&
            (!fWholeWord || FWholeWord(sz, szText, szTerm, cchFind)))
        {
            SendMessage(GetEditWindow(NULL),
                        EM_SETSEL, 
                        sz-szText, 
                        sz-szText+cchFind);
            if (!fNoScroll)
                SendMessage(GetEditWindow(NULL), EM_SCROLLCARET, 0, 0L);
            fFound = TRUE;
            break;
        }
    }

    if (!fNoScroll && !fFound)
        MessageBox(NULL, szFind, "String not found", MB_OK);

    return fFound;
}


//
//  FUNCTION: Replace(char *)
//
//  PURPOSE: Replace the current selection.
//
//  PARAMETERS:
//    szReplace - The string to replace with.
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//
//

VOID Replace(char *szReplace)
{

    SendMessage(GetEditWindow(NULL), 
                EM_REPLACESEL, 
                0, 
                (LPARAM) (LPSTR) szReplace);
}

//
//  FUNCTION: ReplaceAll(char *, char *, BOOL, BOOL)
//
//  PURPOSE: To replace all of the matches found in the edit text.
//
//  PARAMETERS:
//    szFind     - The string to replace.
//    szReplace  - The string to replace with.
//    fMatchCase - Replace only exact matches.
//    fWholeWord - Replace only whole word matches.
//
//  RETURN VALUE:
//
//  COMMENTS:
//
//

VOID ReplaceAll(char *szFind, 
                char *szReplace, 
                BOOL fMatchCase, 
                BOOL fWholeWord)
{
    UINT ichStart;
    UINT ichEnd;

    SendMessage(GetEditWindow(NULL), 
                EM_GETSEL, 
                (WPARAM)&ichStart, 
                (LPARAM)&ichEnd);

    SendMessage(GetEditWindow(NULL), EM_SETSEL, (WPARAM)0, (LPARAM)0);

    while (Find(szFind, TRUE, fMatchCase, fWholeWord, TRUE))
    {
       Replace(szReplace);
    }

    SendMessage(GetEditWindow(NULL),
                EM_SETSEL, 
                (WPARAM)ichStart, 
                (LPARAM)ichEnd);
}


//
//  FUNCTION: FWholeWord(char *, char*, char *, int)
//
//  PURPOSE: Check to see if the match is a whole word.
//
//  PARAMETERS:
//    sz     - A pointer to the found string.
//    szStart- A pointer to the beginning of the buffer.
//    szEnd  - A pointer to the end of the buffer.
//    cch    - The lenght of the found string.
//
//  RETURN VALUE:
//    TRUE  - Whole word.
//    FALSE - No a whole word.
//
//  COMMENTS:
//

BOOL FWholeWord(char *sz, char *szStart, char *szEnd, int cch)
{
    // Is the start of the found text preceded by whitespace?
    if (
        sz != szStart &&
        *(sz-1) != ' ' &&
        *(sz-1) != '\t' &&
        *(sz-1) != ',' &&
        *(sz-1) != '.' &&
        *(sz-1) != '\r'
    ) {
        return FALSE;
    }

    // Is the end of the found text followed by whitespace?
    if (
        sz + cch != szEnd &&
        *(sz+cch) != ' ' &&
        *(sz+cch) != '\t' &&
        *(sz+cch) != ',' &&
        *(sz+cch) != '.' &&
        *(sz+cch) != '\r'
    ) {
        return FALSE;
    }

    return TRUE;
}
