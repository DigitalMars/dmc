
	This is a part of the Microsoft Foundation Classes C++ library.
	Copyright (C) 1992-1995 Microsoft Corporation
	All rights reserved.

	This source code is only intended as a supplement to the
	Microsoft Foundation Classes Reference and related
	electronic documentation provided with the library.
	See these sources for detailed information regarding the
	Microsoft Foundation Classes product.


-------------------------------------------------------
CHKBOOK Sample Microsoft Foundation Classes Application
-------------------------------------------------------

The CHKBOOK sample application primarily serves to illustrate the
following Microsoft Foundation Classes features and programming
techniques:

  1. Transaction-based file i/o, where the document (file) is written
	 to on a per transaction basis rather than on a load/save basis.

  2. Multiple views of a document, where (a) each view is of a different
	 CView-derived class, and (b) each view is in a separate MDI child
	 window.

  3. Using the CScrollView class, which supports viewing of a document
	 in a scrolling window.

  4. Using the CFormView class, which supports viewing of a document
	 using a form with controls laid out in a dialog template resource.

  5. Implementing page breaks during printing.

  6. Writing custom data exchange and validation (DDX/V) functions.

  7. Using a private INI file to automatically open a document
	 when the application is started.

  8. Using Registry entries to define a specific icon for a data file, and to
	 allow running the application by double-clicking on the data file.

The CHKBOOK sample illustrates the above using the following user interface:

  * The CHKBOOK sample is a single document application in
	the sense that it only allows only one checkbook file to be open at a
	time.  But CHKBOOK uses MDI features of MFC to provide multiple views
	of the document via views in separate MDI child windows.  To enforce
	that only one document is open at a time, we simply remove the File
	New and File Open commands from the menus and toolbars of the MDI
	children, which represent an already opened document.

  * The two views of the document are a check view and a book view.  The
	check view, implemented in class CCheckView, provides a form for
	entering or viewing the data for a single check.  The book view,
	implemented in CBookView, provides a scrolling window showing a
	summary of each check.

  * The CHKBOOK application supports the concept of current selection.
	The user can change the current selection in the book view by using
	up and down arrow keys, and by clicking a check with the mouse.
	The second check view always shows the current selection in the
	book view.

  * The check amount field in the check form view validates and converts
	a textual representation such as "19.98" to an internal DWORD
	representation.  The use of DWORD facilitates money arithmetic
	better than, for example, floating point.  CHKBOOK happens to not
	do money arithmetic, but it easily could, for example, add up all
	the check amounts and display it on a bottom line.

  * The check view shows an English text presentation of the check amount,
	such as "Nineteen and 98/100ths Dollars" as the user types along in the
	numeric field.

The CHKBOOK sample has several simplifcations in order to keep the code
focused on the above MFC features and techniques the sample is designed
to illustrate.  For example:

  * CHKBOOK supports only checks and no other money transactions such
	as deposits.  CHKBOOK does not support the deletion (voiding) of
	checks.  The check data is written using fixed length records rather
	than using a more space-efficient variable length records.  These
	simplications allow the CChkBookDoc class to access individual
	check records according to the following very simple algorithm.
	The location of the check is equal the length of the file header,
	plus the <n> times the fixed length of a check record, where <n>
	is the number of sequentially issued checks before this one.
	The reusable class CFixedLenRecDoc implements the
	behavior of a fixed-length record document, separately from the
	the details about the particular check document, which is implemented
	in the a CChkBookDoc class derived from CFixedLenRecDoc.  Similarly,
	the reusable class CRowView implements the behavior of a scroll view
	that rows of fixed heights.  The class CCheckView, derived from
	CRowView, implements the details about the row-based scroll view
	that are particular to the check book application.

  * CHKBOOK does not fully support resource-based localization to
	other languages.  In particular, the check amount field assumes
	dollar and cents, and the written-out check amount ("Nineteen
	and 98/100ths Dollars") is displayed in English.

  * CHKBOOK supports a limited number of checks, equal to the 32K
	devided by the pixel height of text on the display or printer.

  * CHKBOOK.REG must be edited to include the full path to CHKBOOK.EXE 
	wherever it appears. Once that is done, double clicking on the 
	CHKBOOK.REG icon merges the entries into the registration database.
