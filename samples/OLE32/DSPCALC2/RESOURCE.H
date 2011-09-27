/*** 
*resource.h
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*
*Implementation Notes:
*
*****************************************************************************/

#ifdef _MAC

#define kMinSize	500	/* minimum size (in K) */
#define kPrefSize	500	/* preferred size (in K) */

#define kMinHeap	21 * 1024
#define kMinSpace	8 * 1024

#define	rMenuBar	128	/* menu bar */
#define	rAboutAlert	128	/* about alert */
#define	rUserAlert	129	/* error alert */

#define	rCalc		130

#define	mApple		128	/* Apple menu */
#define	iAbout		1

#define	mFile		129	/* File menu */
#define iClose		4
#define	iQuit		12

#define	mEdit		130	/* Edit menu */
#define	iUndo		1
#define	iCut		3
#define	iCopy		4
#define	iPaste		5
#define	iClear		6

#endif

// Note: there is code that depends on all of the digits being contiguous
// and in the following order.

// Mac Note: On the mac these IDs correspond to the control indices
// in the DITL array.

#define IDC_ZERO     1
#define IDC_ONE      2
#define IDC_TWO      3
#define IDC_THREE    4
#define IDC_FOUR     5
#define IDC_FIVE     6
#define IDC_SIX      7
#define IDC_SEVEN    8
#define IDC_EIGHT    9
#define IDC_NINE     10

// Note: there is code that depends on the operators being contiguous
// and in the following order.
//
#define IDC_PLUS     11
#define IDC_MINUS    12
#define IDC_MULT     13
#define IDC_DIV      14

#define IDC_CLEAR    15
#define IDC_EQUALS   16

#define IDC_DISPLAY  17

