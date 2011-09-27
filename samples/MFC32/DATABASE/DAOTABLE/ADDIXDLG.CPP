// AddIxDlg.cpp : implementation file for dialog that lets user add
// indexes to tabledefs
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "DAOTable.h"
#include "listctrl.h"
#include "AddIxDlg.h"
#include "tabledef.h"
#include "index.h"
#include "field.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddIndexDlg dialog

// default constructor
CAddIndexDlg::CAddIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddIndexDlg::IDD, pParent)
{
	// call centralized initialization function
	initializer();
}

// constructor that will generally be used to create
// the dialog--parameters are needed to do any useful 
// database operations
//
// IN: pDatabase--pointer to the open database object
// IN: strTableName--name of the tabledef to which to add indexes
// IN: pParent--pointer to parent of this dialog
CAddIndexDlg::CAddIndexDlg(CDaoDatabase *pDatabase, 
						   CString strTableName,
						   CWnd* pParent)
	: CDialog(CAddIndexDlg::IDD, pParent)

{
	// call centralized initialization function
	initializer();

	// initiliaze and set members to incoming parameters
	m_pTableDef = NULL;
	m_pDatabase = pDatabase;
	m_strTableName = strTableName;
}

// centralized initialization function
void CAddIndexDlg::initializer()
{
	// initialize the index info struct
	indexInitializer();

	// by default, DDV functions are called in the DoDataExchange
	m_bCheckDDV = TRUE;

	// collection index starts at zero
	m_nIndexIndex = 0;
}

// initialize the index info struct members and equivalent
// class members
void CAddIndexDlg::indexInitializer()
{
	//{{AFX_DATA_INIT(CAddIndexDlg)
	//}}AFX_DATA_INIT

	// index info struct
	m_II.m_strName = _T("");
	m_II.m_bPrimary = FALSE;
	m_II.m_bIgnoreNulls = FALSE;
	m_II.m_bRequired = FALSE;
	m_II.m_bUnique = FALSE;

	// clear out the list of fields in case it is not empty--
	// this is an array of index field info structs that are
	// the fields that make up the index
	if (m_II.m_pFieldInfos != NULL)
	{
		delete [] m_II.m_pFieldInfos;
		m_II.m_pFieldInfos = NULL;
		m_II.m_nFields = 0;
	}
}


void CAddIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddIndexDlg)
	DDX_Control(pDX, IDC_UNIQUE, m_UniqueControl);
	DDX_Control(pDX, IDC_REQUIRED, m_RequiredControl);
	DDX_Control(pDX, IDC_IGNORE_NULLS, m_IgnoreNullControl);
	DDX_Control(pDX, IDC_PRIMARY, m_PrimaryControl);
	DDX_Text(pDX, IDC_TABLE_NAME, m_strTableName);
	//}}AFX_DATA_MAP

	// these have to be moved outside of the wizard block
	// since they directly map to members of the index info
	// struct
	DDX_Check(pDX, IDC_PRIMARY, m_II.m_bPrimary);
	DDX_Check(pDX, IDC_IGNORE_NULLS, m_II.m_bIgnoreNulls);
	DDX_Check(pDX, IDC_REQUIRED, m_II.m_bRequired);
	DDX_Check(pDX, IDC_UNIQUE, m_II.m_bUnique);
	
	DDX_Text(pDX, IDC_INDEX_NAME, m_II.m_strName);

	// moved outside of wizard block since DDV is used
	DDX_Control(pDX, IDC_FIELD_LIST, m_FieldListListControl);
	// conditionally check validity of input
	if (m_bCheckDDV)
	{
		// user must select at least one field for the index
		DDV_NoSel(pDX, &m_FieldListListControl);
	}
}


// check if a field selection was made--at least one field must be
// selected.  Selected fields contain a non-null string in their
// first sub-item (i.e. "ascending" or "descending")
void CAddIndexDlg::DDV_NoSel(CDataExchange* pDX, CListCtrl *theControl)
{
	// only process if transferring to member from control.  If
	// no selection has been made, this constitutes an error condition
	if (pDX->m_bSaveAndValidate)
	{
		// check if any items selected--initialize a counter
		int count = 0;

		// for as many items in the list view, keep checking until one is
		// found that has been selected
		for (int i = 0; (count == 0) && (i < theControl->GetItemCount()); i++)
		{
			// the contents of the subitem determines selection state
			if (theControl->GetItemText(i, 1) != _T(""))
			{
				// up the count by one to end the loop
				count += 1;
			}
		}

		// if no selection, then error!
		if (count == 0)
		{
			AfxMessageBox(_T("You must select a field."), MB_ICONEXCLAMATION);
			pDX->m_hWndLastControl = theControl->m_hWnd;
			pDX->Fail();
		}
	}
}

BEGIN_MESSAGE_MAP(CAddIndexDlg, CDialog)
	//{{AFX_MSG_MAP(CAddIndexDlg)
	ON_BN_CLICKED(IDOK, OnDone)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ADD_INDEX, OnAddIndex)
	ON_BN_CLICKED(IDC_DELETE_INDEX, OnDeleteIndex)
	ON_BN_CLICKED(IDC_NEXT_INDEX, OnNextIndex)
	ON_BN_CLICKED(IDC_PREVIOUS_INDEX, OnPreviousIndex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddIndexDlg message handlers

BOOL CAddIndexDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// do some initialization for the list control
	//
	// set style so that list view will show its selections even when the
	// control loses focus
	LONG style = ::GetWindowLong(m_FieldListListControl.m_hWnd,GWL_STYLE);
	::SetWindowLong(m_FieldListListControl.m_hWnd, GWL_STYLE,style | LVS_SHOWSELALWAYS);

	// get size of control to help set the column widths
	CRect controlRect;
	m_FieldListListControl.GetClientRect(controlRect);
	//
	// get width of control, width of potential scrollbar, width needed for sub-item
	// string
	int controlWidth = controlRect.Width();
	int scrollThumbWidth = ::GetSystemMetrics(SM_CXHTHUMB);
	int strWidth = m_FieldListListControl.GetStringWidth(_T("descending"));
	// factor the border width into the string width
	strWidth += 12 * ::GetSystemMetrics(SM_CXBORDER);
	//
	// set up columns--1st leaves room for second and scroll bar
	m_FieldListListControl.InsertColumn(1, _T("Field"), LVCFMT_LEFT,
							controlWidth - scrollThumbWidth - strWidth, 1);
	m_FieldListListControl.InsertColumn(2, _T("Sort"), LVCFMT_LEFT, strWidth + scrollThumbWidth, 2);
	
	// open the tabledef if possible
	if(openTableDef(m_pDatabase, &m_pTableDef, m_strTableName))
	{
		// now populate the list of fields list control	with fields
		populateFieldList();

		// get the information for the first index in the collection of indexes
		// last parameter specifies no error reporting--run silently since
		// there may be no indexes yet
		if(getIndexInfo(m_pTableDef, &m_II, m_nIndexIndex, FALSE))
		{
			// most properties are readonly once an index is in a collection
			disableControlsForExisting(TRUE);

			// list control should reflect the fields in the index
			setFieldListSelections();

			UpdateData(FALSE);
		}
	}
	
	// set focus to index name control unless there is some problem (unlikely)
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
	if (pEdit != NULL)
	{
		pEdit->SetFocus();
		return FALSE;
	}
	else
		return TRUE;  // return TRUE unless you set the focus to a control
}

// user selected to move to the next index in the collection
void CAddIndexDlg::OnNextIndex() 
{
	// get values from control -- no validity checking
	m_bCheckDDV = FALSE;	// turn validity checking off
	UpdateData(TRUE);
	m_bCheckDDV = TRUE;		// turn checking back on--don't forget this!

	// action depends on if this is an existing index or not
	// if this is a new index (based on name) see if user wants to save it
	// or not--if not, then try to move to next index
	if ((m_II.m_strName != _T("")) && (!IsExistentIndex(m_pTableDef, m_II.m_strName)))
		if (IDYES != AfxMessageBox(_T("Current index will be ignored unless added.  Continue anyway?"),
								MB_YESNO))
			return;

	// can only move next if there is a item at the current index in the
	// collection--else you keep indexing into unused parts of the index.
	// So, check for existence of current indexed item and only increment
	// the index if there is an item (no error reporting)
	if (getIndexInfo(m_pTableDef, &m_II, m_nIndexIndex, FALSE))
		// move to next field index                                          	
		m_nIndexIndex += 1;                                                  	
	                                                                     
	// get the information for the next index if possible--              	
	// last parameter indicates no failure reporting                     	
	if (getIndexInfo(m_pTableDef, &m_II, m_nIndexIndex, FALSE))          	
	{                                                                    	
		// list control should be cleared and then set to new values for this index
		setFieldListSelections(FALSE);

		// most properties are readonly once a field is in a collection  	
		disableControlsForExisting(TRUE);                                	
	}                                                                    	
	else                                                        	
	{                                                           	
		// initialize field property values for new field       	
		indexInitializer();
		
		// list box should be cleared
		setFieldListSelections(TRUE);
	                                                            		                                                        	
		// new field have read/write properties                 	
		disableControlsForExisting(FALSE);                      	
	}                                                           	

	UpdateData(FALSE);

	// set focus to name of field edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
	pEdit->SetFocus();	
}

// user selected to more to the previous index in the collection
void CAddIndexDlg::OnPreviousIndex() 
{
	// can't go previous if at 0th index
	if (m_nIndexIndex >= 1)
	{
		// by default, we will move previous
		int retCode = IDYES;

		// if user has entered an index name, then warn them they will lose
		// it if it is not explicitly added--we don't want to do an auto-add
		// since user may have entered name without meaning to add another index
		//
		// see if there is a index name specified
		CString name;
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
		pEdit->GetWindowText(name);
		// if there is a name, then warn the user
		if (name.GetLength() != 0)
		{
			// only an issue if this is not an existing index
			if (!IsExistentIndex(m_pTableDef, name))
				retCode = AfxMessageBox(_T("Current index will be ignored unless added.  Continue anyway?"),
									MB_YESNO);	
		}

		// either there never was a field name specified or the user has
		// chosen not to add the field
		if (retCode == IDYES)
		{
			// move previous
			m_nIndexIndex -= 1;

			// check for this item by index, no error reporting
			// if found, display info, if not treat as new index
			if (getIndexInfo(m_pTableDef, &m_II, m_nIndexIndex, FALSE))          	
			{                                                                    	
				// list ctrl should be cleared and then set to new values for this index
				setFieldListSelections(FALSE);

				// most properties are readonly once a field is in a collection  	
				disableControlsForExisting(TRUE);                                	
			}                                                                    	
			else                                                        	
			{                                                           	
				// initialize field property values for new field       	
				indexInitializer();
				
				// list box should be cleared
				setFieldListSelections(TRUE);
	                                                            		                                                        			
				// new field have read/write properties                 	
				disableControlsForExisting(FALSE);                      	
			}                                                           	

			// update the dialog controls with new values
			UpdateData(FALSE);
		}
	}
	
	// set focus to name of field edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
	pEdit->SetFocus();	
}

// user selected the "done" button--no more indexes to add/view.  The only
// risk is losing an index that is under construction--check for that
void CAddIndexDlg::OnDone() 
{
	// set default to "ready to exit dialog"
	int retCode = IDYES;

	// if user has entered an index name, then warn them they will lose
	// it if it is not explicitly added--we don't want to do an auto-add
	// since user may have entered name without meaning to add another index
	//
	// see if there is a index name specified
	CString name;
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
	pEdit->GetWindowText(name);
	// if there is a name, then warn the user
	if (name.GetLength() != 0)
	{
		// only an issue if this is not an existing index
		if (!IsExistentIndex(m_pTableDef, name))
			retCode = AfxMessageBox(_T("Current index will be ignored unless added.  Continue anyway?"),
								MB_YESNO);	
	}

	// either there never was a index name specified or the user has
	// chosen not to add the index
	if (retCode == IDYES)
	{
		// done with tabledef object
		delete m_pTableDef;
		
		// end the dialog
		CDialog::EndDialog(0);
	}
}

// user selected to exit the dialog--don't do any
// checking for loss of newly specified indexes
void CAddIndexDlg::OnClose() 
{
	// close the tabledef if it is open
	if (m_pTableDef->IsOpen())
		m_pTableDef->Close();

	// clean up
	delete m_pTableDef;
	
	CDialog::OnClose();
}

// user wants to add the specified index to the collection
void CAddIndexDlg::OnAddIndex() 
{
	// get values from control -- don't continue if failure
	if (!UpdateData(TRUE))
		return;

	// don't do anything if this is an existing index (except
	// say so)
	if (!IsExistentIndex(m_pTableDef, m_II.m_strName))
	{
		// must create the fields that are in the index
		createFieldArray(&(m_II.m_pFieldInfos), &(m_II.m_nFields));

		// try to create the index with error checking--may fail if a
		// duplicate named index already exists. Note: creating an index
		// also appends it to the tabledef's index collection
		if (!createNewIndex(m_pTableDef, &m_II))
			return;

		// clean out all properties	
		indexInitializer();

		// list constrol should be cleared
		setFieldListSelections(TRUE);

		// performs visible clearing of controls that are initialized
		UpdateData(FALSE);

		// move to next field index                                          	
		m_nIndexIndex += 1;                                                  	
	}
	else
	{
		AfxMessageBox(_T("Can't add index--it already exists in the TableDef."));
	}

	// set focus to name of index edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
	pEdit->SetFocus();	
}

// user selected to delete the current index--prompt for acceptance
void CAddIndexDlg::OnDeleteIndex() 
{
	// get values from control -- don't continue if failure
	if (!UpdateData(TRUE))
		return;

	// can only delete existing indexes
	if (IsExistentIndex(m_pTableDef, m_II.m_strName))
	{
		// is user sure?
		if (IDYES == AfxMessageBox(_T("Delete current index?"), MB_YESNO))
		{
			// only react if field is deleted!
			if (deleteIndex(m_pTableDef, m_II.m_strName))
			{
				// index into collection is unchanged, so
				// get the information for this index if there is one
				// (no error reporting)
				if (getIndexInfo(m_pTableDef, &m_II, m_nIndexIndex, FALSE))
				{
					// list control should be cleared and then set to reflect new index
					setFieldListSelections(FALSE);

					// disable user selections
					disableControlsForExisting(TRUE);
				}
				else
				{
					// there is no index in collection following the
					// deletion at this collection index, so 
					// set the index info to initial state
					indexInitializer();

					// list control should be cleared
					setFieldListSelections(TRUE);

					// enable user selections
					disableControlsForExisting(FALSE);
				}

				// update the dialog controls to erase deleted field
				UpdateData(FALSE);
			}
		}
	}

	// set focus to name of field edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
	pEdit->SetFocus();	
}


// since most index properties become read-only once the index 
// is added to a collection, manage the controls on the dialog
// appropriate to whether this is an existing index or not
void CAddIndexDlg::disableControlsForExisting(BOOL bDisable/* = TRUE*/)
{
	m_UniqueControl.EnableWindow(!bDisable);
	m_RequiredControl.EnableWindow(!bDisable);
	m_IgnoreNullControl.EnableWindow(!bDisable);
	m_PrimaryControl.EnableWindow(!bDisable);

	// set the name edit to read-only
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_INDEX_NAME);
	pEdit->SetReadOnly(bDisable);
}

// populate the list control with the fields in the field collection of
// the current tabledef
void CAddIndexDlg::populateFieldList()
{
	// struct for getting field information
	CDaoFieldInfo fieldInfo;
	// loop controls and index into the field collection
	BOOL bContinue = TRUE;
	int itemIndex = 0;

	// until we run out of items in the field collection, keep adding items
	for (int i = 0; bContinue; i++)
	{
		// try to get info on the ith field -- no error reporting as specified by last
		// parameter being FALSE
		if (getFieldInfo(m_pTableDef, &fieldInfo, i, FALSE))
		{
			// if this is an indexable field type, then display it
			if ((fieldInfo.m_nType != dbLongBinary) &&
				(fieldInfo.m_nType != dbMemo))
			{
				// insert the name into the list view item, clear the subitem text
				m_FieldListListControl.InsertItem(itemIndex, fieldInfo.m_strName);
				m_FieldListListControl.SetItemText(itemIndex, 1, _T(""));

				// move to next index into the collection
				itemIndex += 1;
			}
		}
		else	// once we fail to get info on an item in the collection, stop the loop
			bContinue = FALSE;
	}
}

// for every item selected in the list control, create an index field info
// struct with the pertinent info and add it to the list of such
// structures
void CAddIndexDlg::createFieldArray(CDaoIndexFieldInfo **ppFields, short *pnFields)
{
	// get maximum count of items in the list control
	int limit = m_FieldListListControl.GetItemCount();

	// if array isn't initialized, delete it, then allocate a new one
	if ((*ppFields) != NULL)
		delete [] (*ppFields);
	
	// allocate at maximum size although we probably won't use all
	// just for simplicity's sake!
	(*ppFields) = new CDaoIndexFieldInfo[limit];

	// keep track of how many items selected and loop through all items
	(*pnFields) = 0;
	for (int i = 0; i < limit; i++)
	{
		// if selected, get the name of the field (selection indicated by
		// non-empty subitem string)
		if (m_FieldListListControl.GetItemText(i, 1) != _T(""))
		{
			// put the name into the array
			(*ppFields)[(*pnFields)].m_strName = m_FieldListListControl.GetItemText(i, 0);
			
			// put the sort order into the array as a boolean
			(*ppFields)[(*pnFields)].m_bDescending = 
						(m_FieldListListControl.GetItemText(i, 1) == _T("descending"));

			// up the count by one
			(*pnFields) += 1;
		}
	}
}

// make the list control reflect which fields make up the current index
//
// IN: bJustClear-by default, clear AND make selection in the list control, but
//     you can specify to just clear the control of all selections
//
void CAddIndexDlg::setFieldListSelections(BOOL bJustClear /* = FALSE*/)
{
	// get maximum number of items in list
	int numStrings = m_FieldListListControl.GetItemCount();
	int selection;

	// reset any current selections
	for (int i = 0; i < numStrings; i++)
	{
		m_FieldListListControl.SetItemState(i, 0, LVIS_SELECTED);
		m_FieldListListControl.SetItemText(i, 1, _T(""));
	}

	// this function can be called to simply clear the list control
	// of selections or you can reflect the current index selections too
	if (!bJustClear)
	{
		// select the appropriate items--set up the finder struct
		LV_FINDINFO lvfi;
		lvfi.flags = LVFI_STRING;	// searching for strings
		CString strSort;			// used to create string to indicate sort direction

		// for all items in the index info struct, find and set the items
		for (int i = 0; i < m_II.m_nFields; i++)
		{
			// find the item in the already populated list control--start from the top
			lvfi.psz = m_II.m_pFieldInfos[i].m_strName;
			selection = m_FieldListListControl.FindItem(&lvfi, -1);

			// select the item
			m_FieldListListControl.SetItemState(selection, LVIS_SELECTED, LVIS_SELECTED);

			// set the sort order subitem text appropriately
			strSort = m_II.m_pFieldInfos[i].m_bDescending ? _T("descending") : _T("ascending");
			m_FieldListListControl.SetItemText(selection, 1, strSort);
		}

		UpdateData(FALSE);
	}
}


