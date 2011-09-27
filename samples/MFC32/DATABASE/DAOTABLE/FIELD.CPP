// FIELD.CPP MFC DAO Field specific functions

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// all the field specific functions:
//
//BOOL IsExistentField(CDaoTableDef *pTableDef, CString strFieldName);
//BOOL createNewField(CDaoTableDef *pTableDef, CDaoFieldInfo *pFieldInfo);
//BOOL getFieldInfo(CDaoTableDef *pTableDef, CDaoFieldInfo *pFieldInfo,
//				  int fieldIndex, BOOL bReportErrors = TRUE);
//BOOL deleteField(CDaoTableDef *pTableDef, CString strFieldName);


#include "stdafx.h"
#include "field.h"


// check for duplicate field name in field collection
// of tabledef object
// IN: pTableDef--the tabledef whose fields collection we access
// IN: strFieldName--the name of the field to check	for existence
// RETURN: TRUE if field already exists in collection, FALSE otherwise
BOOL IsExistentField(CDaoTableDef *pTableDef, CString strFieldName)
{
	// if the tabledef is non-existent, then the answer is obvious
	if (pTableDef == NULL)
		return FALSE;

	// initialize status indicator
	BOOL bDuplicateFieldName = TRUE;
	
	// see if there is a field by this name already--duplicate
	// named fields are not accepted
	CDaoFieldInfo fieldInfo;	// only needed for the call

	// MFC exception handler macros used
	TRY
	{
		// this call will throw an exception if there is no
		// field by the specified name--test for duplication
		pTableDef->GetFieldInfo(strFieldName, fieldInfo);
	}
	CATCH (CDaoException, e)
	{
		// if this is an 'Item not found' exception, we are
		// cleared to create the field -- else this is
		// a duplicate field name and we got another exception
		// which is irrelevant for our purposes
		if (e->m_pErrorInfo->m_lErrorCode == 3265)
			bDuplicateFieldName = FALSE;
	}
	AND_CATCH (CMemoryException, e)
	{
		// do nothing--no need to process errors since this is
		// just a duplicate checker
	}
	END_CATCH

	return bDuplicateFieldName;
}

// wraps the CreateField DAO call in an exception handler
// IN: pTableDef--the tabledef whose fields collection we access
// IN: pFieldInfo--information used to create field
// RETURN: TRUE if creation succeeds, FALSE if it fails
BOOL createNewField(CDaoTableDef *pTableDef, CDaoFieldInfo *pFieldInfo)
{
	// if the tabledef is non-existent, then the answer is obvious
	if (pTableDef == NULL)
		return FALSE;

	// check for existing field with this name just to be safe
	if (IsExistentField(pTableDef, pFieldInfo->m_strName))
	{
		AfxMessageBox(_T("A field by that name already exists."));

		// return FALSE since can't create duplicate field
		return FALSE;
	}

	// initialize failure indicators
	BOOL bCreateFailed = FALSE;

	// create a field with the specified properties                 	
	// it is automatically appended to field collection             	
	// of the tabledef                                              	
	TRY                                                             	
	{                                                               	
		pTableDef->CreateField(*pFieldInfo);                      	
	}                                                               	
	CATCH (CDaoException, e)                                        	
	{                                                               	
		// construct a meaningful message                           	
		CString message = _T("Couldn't create field--Exception: ");     	
		message += e->m_pErrorInfo->m_strDescription;
		
		// display message
		AfxMessageBox(message);
	                                                                
		// indicate failure                                         	
		bCreateFailed = TRUE;                                       	
	}                                                               	
	AND_CATCH (CMemoryException, e)
	{
		AfxMessageBox(_T("Failed to create field--Memory exception thrown."));                                       	

		// indicate failure                                         	
		bCreateFailed = TRUE;                                       	
	}
	END_CATCH                                                       	
	
	// return TRUE if creation succeeds
	return (!bCreateFailed);
}

// fill a fieldinfo struct with the field's properties--handle exceptions
// IN: pTableDef--the tabledef whose fields collection we access
// OUT: pFieldInfo--information we get from the field in the collection
// IN: fieldIndex--the index into the collection for the item we want
// IN: bReportErrors--if TRUE, then report any problems, else silent
//	   TRUE by default
// RETURN: TRUE if information was obtained, FALSE indicates pFieldInfo 
//		   does not contain informaton that was requested
BOOL getFieldInfo(CDaoTableDef *pTableDef, CDaoFieldInfo *pFieldInfo,
				  int fieldIndex, BOOL bReportErrors /* = TRUE */)
{
	// if the tabledef is non-existent, then the answer is obvious
	if (pTableDef == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	TRY
	{                                                                                       	
		// try to get info on the field                                                   		
		pTableDef->GetFieldInfo(fieldIndex, *pFieldInfo,
								AFX_DAO_ALL_INFO );
	}                                                                                       	
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message if request
		if (bReportErrors)
		{
			CString strMessage = _T("Couldn't get information on field--Exception: ");                  
			strMessage += e->m_pErrorInfo->m_strDescription;                                       	

			AfxMessageBox(strMessage);
		}
	                                                                                        
		// indicate failure                                                                 	
		bSuccess = FALSE;                                                               		
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		// output status if requested
		if (bReportErrors)
			AfxMessageBox(_T("Failed to get info on field--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	// return status
	return bSuccess;
}

// wrap field deletion with exception handlers
// IN: pTableDef--the tabledef whose fields collection we access
// IN: strFieldName--name of field to delete
// RETURN: TRUE if deletion succeeded, FALSE otherwise
BOOL deleteField(CDaoTableDef *pTableDef, CString strFieldName)
{
	// if the tabledef is non-existent, then the answer is obvious
	if (pTableDef == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	// MFC exception handler macros used
	TRY
	{
		// this call will throw an exception if there is no
		// field by the specified name--test for duplication
		pTableDef->DeleteField(strFieldName);
	}
	CATCH (CDaoException, e)
	{                                                                                       	
		CString strMessage = _T("Couldn't delete the field--Exception: ");                  
		strMessage += e->m_pErrorInfo->m_strDescription;                                       	

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 	
		bSuccess = FALSE;                                                               		
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		AfxMessageBox(_T("Failed to delete the field--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	return bSuccess;
}

