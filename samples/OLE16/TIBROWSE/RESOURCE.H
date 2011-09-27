/***
*resource.h
*
*  Copyright (C) 1992-93, Microsoft Corporation.  All Rights Reserved.
*
*Purpose:
*
*Implementation Notes:
*
*****************************************************************************/

#ifdef _MAC
# define IDC_BASE 0

# define kMinSize       500     /* minimum size (in K) */
# define kPrefSize      500     /* preferred size (in K) */
# define kMinHeap       21 * 1024
# define kMinSpace      8 * 1024

# define rMenuBar       128     /* menu bar */
# define rAboutAlert    128     /* about alert */
# define rUserAlert     129     /* error alert */
# define rDlg           130

# define mApple         128     /* Apple menu */
# define iAbout         1

# define mFile          129     /* File menu */
# define iOpen          2
# define iClose         4
# define iQuit          12

# define mEdit          130     /* Edit menu */
# define iUndo          1
# define iCut           3
# define iCopy          4
# define iPaste         5
# define iClear         6

#else
# define IDC_BASE 1000
#endif


#define IDC_TYPELIST    (IDC_BASE+1)
#define IDC_MEMBERLIST  (IDC_BASE+2)
#define IDC_PARAMLIST   (IDC_BASE+3)
#define IDC_TYPEKIND    (IDC_BASE+5)
#define IDC_VERSION     (IDC_BASE+7)
#define IDC_GUID        (IDC_BASE+9)
#define IDC_HELPSTRING  (IDC_BASE+11)
#define IDC_HELPCONTEXT (IDC_BASE+13)

#define IDC_STATIC      -1
