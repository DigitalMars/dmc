// AddTdDlg.cpp : implementation file for dialog that allows user to 
// specify fields that constitute the tabledef's structure
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "daotable.h"
#include "tabledef.h"
#include "field.h"
#include "AddTbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddTable dialog

// default constructor
CDlgAddTable::CDlgAddTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddTable::IDD, pParent)
{
	// call centralized initialization function
	initializer();
}

// the constructor that will generally be called to create this dialog
//
// IN: pDatabase--pointer to an open database object
// IN: strTableName--name of table whose fields will be added, deleted, or viewed
// IN: pParent--pointer to parent of the dialog
//
CDlgAddTable::CDlgAddTable(CDaoDatabase *pDatabase, CString strTableName,
						   CWnd * pParent)
	: CDialog(CDlgAddTable::IDD, pParent)
{
	// the incoming database pointer that is used in creating the tabledef
	m_pDatabase = pDatabase;

	// transfer name of table (if any) to member
	m_strTableName = strTableName;

	// call centralized function to initialize dialog data and other 
	// member data
	initializer();
}

// set up the members of this class
void CDlgAddTable::initializer()
{
	// initialize the field info members (including ordinal position)
	fieldInitializer(TRUE);

	// no fields added to table yet
	m_bFirstFieldAdded = FALSE;
	// when DoDataExchange is executed, check validity of transferred data
	m_bCheckDDV = TRUE;		
	// no tabledef until it is created with the user specified name
	m_pTableDef = NULL;

	// the index into the collection of fields for the current field 
	m_nFieldIndex = 0;

	// initialize type map.  The combobox list is sequential, the internal
	// constants for DAO are not.
	m_nTypeMap[Boolean] = dbBoolean;
	m_nTypeMap[Byte] = dbByte;
	m_nTypeMap[Currency] = dbCurrency;
	m_nTypeMap[Date] = dbDate;
	m_nTypeMap[Double] = dbDouble;
	m_nTypeMap[Long] = dbLong;
	m_nTypeMap[LongBinary] = dbLongBinary;
	m_nTypeMap[Memo] = dbMemo;
	m_nTypeMap[Short] = dbInteger;	// it is really a short integer
	m_nTypeMap[Single] = dbSingle;
	m_nTypeMap[Text] = dbText;
}

// initialize the field information to a default state.  
// IN: bInitOrdPos -- The ordinal position is conditionally initialized
// to allow for it autoincrementing with each addition of a field to the
// tabledef--TRUE by default
void CDlgAddTable::fieldInitializer(BOOL bInitOrdPos /* = TRUE */)
{
	// conditionally initialize the ordinal position
	if (bInitOrdPos)
		m_FI.m_nOrdinalPosition = 0;

	// always initialize the rest
	m_FI.m_strName = _T("");
	m_FI.m_nType = -1;		// initialized for the benefit of the combox
	m_FI.m_lSize = 0;
	m_FI.m_strDefaultValue = _T("");
	m_FI.m_bRequired = FALSE;
	m_FI.m_lAttributes = dbUpdatableField;
	m_FI.m_bAllowZeroLength = TRUE;
	m_FI.m_strValidationRule = _T("");
	m_FI.m_strValidationText = _T("");
}

void CDlgAddTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddTable)
	DDX_Control(pDX, IDC_VALIDATION_TEXT, m_ValidationTextControl);
	DDX_Control(pDX, IDC_VALIDATION_RULE, m_ValidationRuleControl);
	DDX_Control(pDX, IDC_REQUIRED, m_RequireControl);
	DDX_Control(pDX, IDC_POSITION, m_OrdinalPositionControl);
	DDX_Control(pDX, IDC_NAME, m_NameControl);
	DDX_Control(pDX, IDC_THE_SIZE, m_SizeControl);
	DDX_Control(pDX, IDC_DEFAULT_VALUE, m_DefaultValueControl);
	DDX_Control(pDX, IDC_AUTO_INCR_FIELD, m_AutoIncrFieldControl);
	DDX_Control(pDX, IDC_FIXED_FIELD, m_FieldVariabilityControl);
	DDX_Text(pDX, IDC_TABLE_NAME, m_strTableName);
	//}}AFX_DATA_MAP

	// move those DDX calls that use DDV or map directly to members
	// of the field info struct, or require special processing outside
	// of the wizard block

	DDX_Control(pDX, IDC_TYPE, m_TypeControl);
	// only check validity if so instructed--avoids unnecessary warnings
	if (m_bCheckDDV)
		// user must select a type for the field
		DDV_NoSel(pDX, &m_TypeControl);
	
	// direct mapping to field info struct
	DDX_Text(pDX, IDC_POSITION, m_FI.m_nOrdinalPosition);
	DDX_Text(pDX, IDC_NAME, m_FI.m_strName);
	DDX_Text(pDX, IDC_VALIDATION_RULE, m_FI.m_strValidationRule);
	DDX_Text(pDX, IDC_VALIDATION_TEXT, m_FI.m_strValidationText);
	DDX_Text(pDX, IDC_THE_SIZE, m_FI.m_lSize);
	DDX_Text(pDX, IDC_DEFAULT_VALUE, m_FI.m_strDefaultValue);
	DDX_Check(pDX, IDC_REQUIRED, m_FI.m_bRequired);

	// manage the type control directly since we need to use the type
	// map (combobox uses sequential index, DAO types are non-sequential)
	if (pDX->m_bSaveAndValidate)
	{
		// when transferring from dialog control to member, use map the
		// easy way--just index into it
		theTypes selectedType = (theTypes) m_TypeControl.GetCurSel();
		m_FI.m_nType = m_nTypeMap[selectedType];
	}
	else
	{
		// if the type is -1, then no type selected
		if (m_FI.m_nType == -1)
		{
			// update control appropriately
			m_TypeControl.SetCurSel(m_FI.m_nType);
		}
		else
		{
			// must search the map looking for a match--
			// search for actual type to determine internally used type
			for (int i = 0; (i < MAX_TYPES) && (m_nTypeMap[i] != m_FI.m_nType); i++)
				;

			// update control appropriately
			m_TypeControl.SetCurSel(i);
		}
	}

	// do attribute specific processing -- member values are encoded in
	// the m_lAttributes of CDaoFieldInfo struct
	if (pDX->m_bSaveAndValidate)
	{
		// first do transfer
		DDX_Radio(pDX, IDC_FIXED_FIELD, m_FieldVariability);
		DDX_Check(pDX, IDC_AUTO_INCR_FIELD, m_bAutoIncrField);

		// if transferring to member from control, determine the
		// attributes value to store for field
		if (m_bAutoIncrField)
			m_FI.m_lAttributes |= dbAutoIncrField;
		else
			m_FI.m_lAttributes &= ~dbAutoIncrField;

		// for text fields only, set the variability
		if (m_FI.m_nType == dbText)
			if (m_FieldVariability == 0)
				m_FI.m_lAttributes |= dbFixedField;
			else
				m_FI.m_lAttributes |= dbVariableField;
	}
	else
	{
		// transferring from field attributes to controls
		m_bAutoIncrField = (m_FI.m_lAttributes & dbAutoIncrField) == dbAutoIncrField;

		if ((m_FI.m_lAttributes & dbFixedField) == dbFixedField) 
			m_FieldVariability = 0;
		else
			m_FieldVariability = 1;

		// now transfer the information to the controls
		DDX_Radio(pDX, IDC_FIXED_FIELD, m_FieldVariability);
		DDX_Check(pDX, IDC_AUTO_INCR_FIELD, m_bAutoIncrField);
	}
}

// check if a type selection was made--one must be made
void CDlgAddTable::DDV_NoSel(CDataExchange* pDX, CComboBox *theControl)
{
	// only process if transferring to member from control.  If
	// no selection has been made, this constitutes an error condition
	if (pDX->m_bSaveAndValidate && (CB_ERR == theControl->GetCurSel()))
	{
		AfxMessageBox(_T("You must select a type."), MB_ICONEXCLAMATION);
		pDX->m_hWndLastControl = theControl->m_hWnd;
		pDX->Fail();
	}
}

BEGIN_MESSAGE_MAP(CDlgAddTable, CDialog)
	//{{AFX_MSG_MAP(CDlgAddTable)
	ON_BN_CLICKED(IDC_NEXT_FIELD, OnNextField)
	ON_CBN_SELENDOK(IDC_TYPE, OnSelendokType)
	ON_BN_CLICKED(IDOK, OnDone)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PREVIOUS_FIELD, OnPreviousField)
	ON_BN_CLICKED(IDC_DELETE_FIELD, OnDeleteField)
	ON_BN_CLICKED(IDC_ADD_FIELD, OnAddField)
	ON_CBN_CLOSEUP(IDC_TYPE, OnCloseupType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddTable message handlers

BOOL CDlgAddTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// user either specified new table or existing table
	if(!IsExistentTable(m_pDatabase, m_strTableName))
	{
		// initialize size control to disabled until we know field type
		// since that is what defines size (unless variable field type)
		m_SizeControl.EnableWindow(FALSE);

		// until a field has been added, can not be "done" since a tabledef
		// must contain at least one field
		CButton *pButton = (CButton *)GetDlgItem(IDOK);
		pButton->EnableWindow(FALSE);
	}
	else
	{
		// user specified existing table, so populate
		// dialog with information from first field
		//
		// open the table rather than creating it
		if (!openTableDef(m_pDatabase, &m_pTableDef, m_strTableName))
			return TRUE;

		// get the information for the first field (no error reporting)
		if (getFieldInfo(m_pTableDef, &m_FI, m_nFieldIndex, FALSE))
		{
			// there is a first field so set controls appropriate
			setTypeDependentStatesAndExisting(m_FI.m_nType, TRUE);
		}
		else
		{	
			// no first field (user must have delete all of them),
			// so come up initialized and updatable first time
			fieldInitializer(FALSE);                                	
			setTypeDependentStatesAndExisting(m_FI.m_nType, FALSE);
		}
		
		// the first field has already been added long ago
		m_bFirstFieldAdded = TRUE;

		UpdateData(FALSE);
	}

	// set focus to name of field edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_NAME);
	pEdit->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}

// user selected to go to next existing field--prompt for acceptance
// since we lose the currently specified field if it is not added
void CDlgAddTable::OnNextField() 
{
	// get values from control -- no validity checking
	m_bCheckDDV = FALSE;	// turn validity checking off
	UpdateData(TRUE);
	m_bCheckDDV = TRUE;		// turn checking back on--don't forget this!

	// action depends on if this is an existing field or not
	// if this is a new field see if user wants to save it
	// or not--if not, then try to move to next field
	if ((m_FI.m_strName != _T("")) && (!IsExistentField(m_pTableDef, m_FI.m_strName)))
		if (IDYES != AfxMessageBox(_T("Current field will be ignored unless added.  Continue anyway?"),
								MB_YESNO))
			return;

	// can only move next if there is a item at the current index in the
	// collection--else you keep indexing into unused parts of the index.
	// So, check for existence of current indexed item and only increment
	// the index if there is an item (no error reporting)
	if (getFieldInfo(m_pTableDef, &m_FI, m_nFieldIndex, FALSE))
		// move to next field index                                          	
		m_nFieldIndex += 1;                                                  	
	                                                                     
	// get the information for the next field if possible--              	
	// last parameter indicates no failure reporting                     	
	if (getFieldInfo(m_pTableDef, &m_FI, m_nFieldIndex, FALSE))          	
	{                                                                    	
		// some settings are type dependent                              	
		setTypeDependentStatesAndExisting(m_FI.m_nType, TRUE);                            	
	}                                                                    	
	else                                                        	
	{                                                           	
		// initialize field property values for new field       	
		fieldInitializer(FALSE);                                	
	                                                            
		// set ordinal position to this index just for neatness 	
		m_FI.m_nOrdinalPosition = m_nFieldIndex;                	
	                                                            
		// some settings are type dependent                     	
		setTypeDependentStatesAndExisting(m_FI.m_nType, FALSE);                   	
	}                                                           	

	UpdateData(FALSE);

	// set focus to name of field edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_NAME);
	pEdit->SetFocus();
}

// user selected a type for the field from the type combobox
void CDlgAddTable::OnSelendokType() 
{
	// need to transfer values into member variables
	// otherwize the UpdateData(FALSE) at the end will
	// wipe out all the values the user entered.
	// Do not do any validity checks
	m_bCheckDDV = FALSE;	// turn validity checking off
	UpdateData(TRUE);
	m_bCheckDDV = TRUE;		// turn checking back on--don't forget this!

	// once a type is selected, it is time to enable the size edit control
	m_SizeControl.EnableWindow();

	// now set the states that depend on the type
	setTypeDependentStatesAndExisting(m_FI.m_nType, FALSE);

	// update the controls with new info
	UpdateData(FALSE);
}

// user can scroll in dropdown--must handle this possibility.  Just pretend
// it is a selection that ended OK
void CDlgAddTable::OnCloseupType() 
{
	OnSelendokType();
}


// user is done adding/viewing/deleting fields and is ready to finalize
// the table structure
void CDlgAddTable::OnDone() 
{
	// by default, we are ready to exit the dialog
	int retCode = IDYES;

	// if user has entered a field name, then warn them they will lose
	// it if it is not explicitly added--we don't want to do an auto-add
	// since user may have entered name without meaning to add another field
	//
	// see if there is a field name specified
	CString name;
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_NAME);
	pEdit->GetWindowText(name);
	// if there is a name, then warn the user
	if (name.GetLength() != 0)
	{
		// only an issue if this is not an existing field
		if (!IsExistentField(m_pTableDef, name))
			retCode = AfxMessageBox(_T("Current field will be ignored unless added.  Continue anyway?"),
								MB_YESNO);	
	}
	// either there never was a field name specified or the user has
	// chosen not to add the field
	if (retCode == IDYES)
	{
		// done with tabledef object
		delete m_pTableDef;
		
		// end the dialog
		CDialog::EndDialog(0);
	}
}

// user cancelled tabledef creation process
void CDlgAddTable::OnClose() 
{
	// if we have already allocated a tabledef object, better
	// clean it up
	if (m_pTableDef	!= NULL)
		delete m_pTableDef;

	// let the base class cleanup the rest
	CDialog::OnClose();
}

// user selected to move to previous existing field, prompt for acceptance since
// we lose any currently specified field unless is is added to the collection
void CDlgAddTable::OnPreviousField() 
{
	// can't go previous if at 0th index
	if (m_nFieldIndex >= 1)
	{
		// get values from control -- no validity checking
		m_bCheckDDV = FALSE;	// turn validity checking off
		UpdateData(TRUE);
		m_bCheckDDV = TRUE;		// turn checking back on--don't forget this!

		// by default, move previous
		int retCode = IDYES;

		// if user has entered a field name, then warn them they will lose
		// it if it is not explicitly added--we don't want to do an auto-add
		// since user may have entered name without meaning to add another field
		//
		// if there is a name, then warn the user
		if (m_FI.m_strName != _T(""))
		{
			// only an issue if this is not an existing field
			if (!IsExistentField(m_pTableDef, m_FI.m_strName))
				retCode = AfxMessageBox(_T("Current field will be ignored unless added.  Continue anyway?"),
									MB_YESNO);	
		}

		// either there never was a field name specified or the user has
		// chosen not to add the field
		if (retCode == IDYES)
		{
			// move previous
			m_nFieldIndex -= 1;

			// get the information for the previous field (no error reporting)
			// if there is info, display appropriate to existing field
			// if no info, treat this as a new field the user can set
			if(getFieldInfo(m_pTableDef, &m_FI, m_nFieldIndex, FALSE))
			{
				// some settings are type dependent
				setTypeDependentStatesAndExisting(m_FI.m_nType, TRUE);
			}
			else
			{
				// initialize field property values for new field       	
				fieldInitializer(FALSE);                                	
																		
				// set ordinal position to this index just for neatness 	
				m_FI.m_nOrdinalPosition = m_nFieldIndex;                	
																		
				// some settings are type dependent                     	
				setTypeDependentStatesAndExisting(m_FI.m_nType, FALSE);                   	
			}

			// update the dialog controls with new values
			UpdateData(FALSE);
		}
	}
}

// user has selected to delete the current field, prompt for acceptance
void CDlgAddTable::OnDeleteField() 
{
	// get values from control -- don't continue if failure
	if (!UpdateData(TRUE))
		return;

	// can only delete existing fields
	if (IsExistentField(m_pTableDef, m_FI.m_strName))
	{
		// is user sure?
		if (IDYES == AfxMessageBox(_T("Delete current field?"), MB_YESNO))
		{
			// only react if field is deleted!
			if (deleteField(m_pTableDef, m_FI.m_strName))
			{
				// index into collection is unchanged, so
				// get the information for this field if there is one
				// (no error reporting)
				if (getFieldInfo(m_pTableDef, &m_FI, m_nFieldIndex, FALSE))
				{
					// there is a field, so set type depenedent properties
					// and existing field properties of controls
					setTypeDependentStatesAndExisting(m_FI.m_nType, TRUE);
				}
				else
				{
					// there is no field in collection following the
					// deletion at this index, so 
					// set the field info to initial state
					fieldInitializer(FALSE);

					// clear read-only status of control
					setTypeDependentStatesAndExisting(m_FI.m_nType, FALSE);                   	
				}

				// update the dialog controls to erase deleted field
				UpdateData(FALSE);
			}
		}
	}
}

// control availability depends on two things--the type of the field and
// whether this is an existing field or new.  Some controls only make
// sense for certain types (like autoincrement for longs only).  Existing
// fields have primarily read-only properties.  This function takes both
// into account when setting control states
//
// Note: since disabled is acceptable for existing field controls, you will
//		 not see the bExisting taken into consideration if we are disabling
//		 a control based on type
//
// IN: theType--the type of the current field (long, text, etc.)
// IN: bExistingField--TRUE if this is an existing field, else false
//
void CDlgAddTable::setTypeDependentStatesAndExisting(int theType, BOOL bExistingField)
{
	// only longs can be auto increment
	if (theType == dbLong)
		m_AutoIncrFieldControl.EnableWindow(TRUE && (!bExistingField));
	else
		m_AutoIncrFieldControl.EnableWindow(FALSE);

	// except for text, setting variability of
	// field size is not an option
	if (theType == dbText)
	{
		// the state of controls depends on if this is an existing field or new
		CButton *pButton = (CButton *)GetDlgItem(IDC_FIXED_FIELD);
		pButton->EnableWindow(!bExistingField);
		pButton = (CButton *)GetDlgItem(IDC_VARIABLE_FIELD);
		pButton->EnableWindow(!bExistingField);
	}
	else
	{
		// the controls are disabled for all but non-existing text fields 
		CButton *pButton = (CButton *)GetDlgItem(IDC_FIXED_FIELD);
		pButton->EnableWindow(FALSE);
		pButton = (CButton *)GetDlgItem(IDC_VARIABLE_FIELD);
		pButton->EnableWindow(FALSE);
	}

	// by default, the default value depends only on field existence
	m_DefaultValueControl.EnableWindow(TRUE && (!bExistingField));

	// by default the validation depends only on field existence
	m_ValidationTextControl.SetReadOnly(bExistingField);
	m_ValidationRuleControl.SetReadOnly(bExistingField);

	// In general, the selected type determines the following:
	//   * the size of the field (in bytes)
	//   * whether the size is user modifiable or not
	//   * what the setting for field variability is by default
	switch(theType)
	{
		case dbBoolean:
		case dbByte:	m_SizeControl.SetReadOnly();
						m_FI.m_lSize = 1;
						// set to fixed field type
						m_FI.m_lAttributes &= ~dbVariableField;
						m_FI.m_lAttributes |= dbFixedField;
						break;
		case dbInteger:	m_SizeControl.SetReadOnly();
						m_FI.m_lSize = 2;
						// set to fixed field type
						m_FI.m_lAttributes &= ~dbVariableField;
						m_FI.m_lAttributes |= dbFixedField;
						break;
		case dbLong:
		case dbSingle:	m_SizeControl.SetReadOnly();
						m_FI.m_lSize = 4;
						// set to fixed field type
						m_FI.m_lAttributes &= ~dbVariableField;
						m_FI.m_lAttributes |= dbFixedField;
						break;
		case dbCurrency:
		case dbDate:
		case dbDouble:	m_SizeControl.SetReadOnly();
						m_FI.m_lSize = 8;
						// set to fixed field type
						m_FI.m_lAttributes &= ~dbVariableField;
						m_FI.m_lAttributes |= dbFixedField;
						break;
		case dbText:	m_SizeControl.SetReadOnly(FALSE | bExistingField);
						// set to variable field type
						m_FI.m_lAttributes &= ~dbFixedField;
						m_FI.m_lAttributes |= dbVariableField;
						break;
		case dbLongBinary:
						m_SizeControl.SetReadOnly(FALSE | bExistingField);
						// set to variable field type
						m_FI.m_lAttributes &= ~dbFixedField;
						m_FI.m_lAttributes |= dbVariableField;
						break;
		case dbMemo:	m_SizeControl.SetReadOnly(FALSE | bExistingField);
						// set to variable field type
						m_FI.m_lAttributes &= ~dbFixedField;
						m_FI.m_lAttributes |= dbVariableField;
						break;
	} // end of switch

	// now handle setting availability for controls that are not type
	// dependent
	m_NameControl.SetReadOnly(bExistingField);
	m_OrdinalPositionControl.SetReadOnly(bExistingField);
	m_TypeControl.EnableWindow(!bExistingField);
	m_RequireControl.EnableWindow(!bExistingField);

	UpdateData(FALSE);
}

// user selected to add a new field to the collection
// NOTE: here is also where the tabledef is created and
//       appended to the tabledef collection.  The tabledef
//		 is created before the first field is created and appended
//		 after which it is appended to the tabledef collection
void CDlgAddTable::OnAddField() 
{
	// get values from control -- don't continue if failure
	if (!UpdateData(TRUE))
		return;

	// don't do anything if this is an existing field (except
	// say so)
	if (!IsExistentField(m_pTableDef, m_FI.m_strName))
	{
		// if no fields added yet, then this is first so create the tabledef
		if (!m_bFirstFieldAdded)
		{
			// try to create the tabledef, if failure, stop processing
			if (!createNewTableDef(m_pDatabase, &m_pTableDef, m_strTableName))
				return;
		}

		// try to create the field with error checking--may fail if a
		// duplicate named field already exists. Note: creating a field
		// also appends it to the tabledef's field collection
		if (!createNewField(m_pTableDef, &m_FI))
		{
			// if this is the first field, then failure means we delete the
			// table we just created
			if (!m_bFirstFieldAdded)
				deleteTable(m_pDatabase, m_strTableName);
			// done
			return;
		}

		// field created successfully, so continue doing required
		// initialization
		if (!m_bFirstFieldAdded)
		{
			// append the tabldef to the database collection of tabledefs
			// this must be done after at least the first first field 
			// has been added
			if (!appendTableDef(m_pDatabase, m_pTableDef))
			{
				// if append fails, then we should delete the first field
				// since it is probably the culprit and we should allow
				// user to set new properties
				deleteField(m_pTableDef, m_FI.m_strName);

				// delete the table too if since this is the first field
				// that failed--else we will have a created table that is
				// not appended
				deleteTable(m_pDatabase, m_strTableName);

				// done
				return;
			}
			else
				// now first field has been added successfully
				m_bFirstFieldAdded = TRUE;

			// now that first field added, can possbily be "done"
			CButton *pButton = (CButton *)GetDlgItem(IDOK);
			pButton->EnableWindow(TRUE);
		}

		// clean out all fields except ordinal position to prepare
		// user to enter next field's properties
		fieldInitializer(FALSE);

		// manage the ordinal position -- autoincrement
		// Note: it is OK for fields to have the same ordinal position--
		// see the DAO docs for this property.
		// also increment the index into the collection
		m_FI.m_nOrdinalPosition += 1;
		m_nFieldIndex += 1;

		// performs visible clearing of controls that are initialized
		UpdateData(FALSE);
	}
	else
	{
		AfxMessageBox(_T("Can't add field--it already exists in the TableDef."));
	}

	// set focus to name of field edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_NAME);
	pEdit->SetFocus();
}


