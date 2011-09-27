// INDEX.CPP MFC DAO Index specific functions
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

//
// contains index specific functions:
//BOOL IsExistentIndex(CDaoTableDef *pTableDef, CString strIndexName);
//BOOL createNewIndex(CDaoTableDef *pTableDef, CDaoIndexInfo *pIndexInfo);
//BOOL getIndexInfo(CDaoTableDef *pTableDef, CDaoIndexInfo *pIndexInfo,
//				  int IndexIndex, BOOL bReportErrors = TRUE);
//BOOL deleteIndex(CDaoTableDef *pTableDef, CString strIndexName);

#include "stdafx.h"
#include "index.h"

// check for duplicate name in collection
// of tabledef object
// IN: pTableDef--pointer to tabledef object whose index collection we access
// IN: strIndexName--name of index to check for existence
// RETURN: TRUE if the index exists in the collection, FALSE otherwise
BOOL IsExistentIndex(CDaoTableDef *pTableDef, CString strIndexName)
{
	// if the tabledef is non-existent, then the answer is obvious
	if (pTableDef == NULL)
		return FALSE;

	// initialize status indicator
	BOOL bDuplicateIndexName = TRUE;
	
	// see if there is a Index by this name already--duplicate
	// named Indexs are not accepted
	CDaoIndexInfo IndexInfo;	// only needed for the call

	// MFC exception handler macros used
	TRY
	{
		// this call will throw an exception if there is no
		// Index by the specified name--test for duplication
		pTableDef->GetIndexInfo(strIndexName, IndexInfo);
	}
	CATCH (CDaoException, e)
	{
		// if this is an 'Item not found' exception, we are
		// cleared to create the Index -- else this is
		// a duplicate Index name and we got another exception
		// which is irrelevant for our purposes
		if (e->m_pErrorInfo->m_lErrorCode == 3265)
			bDuplicateIndexName = FALSE;
	}
	AND_CATCH (CMemoryException, e)
	{
		// do nothing
		;
	}
	END_CATCH

	return bDuplicateIndexName;
}

// wraps the CreateIndex DAO call in an exception handler
// IN: pTableDef--pointer to tabledef object whose index collection we access
// IN: pIndexInfo--information on index to be created
// RETURN: TRUE if new index created, FALSE otherwise
BOOL createNewIndex(CDaoTableDef *pTableDef, CDaoIndexInfo *pIndexInfo)
{
	// if the tabledef is non-existent, then the answer is obvious
	if (pTableDef == NULL)
		return FALSE;

	// check for existing Index with this name just to be safe
	if (IsExistentIndex(pTableDef, pIndexInfo->m_strName))
	{
		AfxMessageBox(_T("A Index by that name already exists."));

		// return FALSE since can't create duplicate Index
		return FALSE;
	}

	// initialize failure indicators
	BOOL bCreateFailed = FALSE;

	// create a Index with the specified properties                 	
	// it is automatically appended to Index collection             	
	// of the tabledef                                              	
	TRY                                                             	
	{                                                               	
		pTableDef->CreateIndex(*pIndexInfo);                      	
	}                                                               	
	CATCH (CDaoException, e)                                        	
	{                                                               	
		// construct a meaningful message                           	
		CString message = _T("Couldn't create Index--Exception: ");     	
		message += e->m_pErrorInfo->m_strDescription;
		
		AfxMessageBox(message);
	                                                                
		// indicate failure                                         	
		bCreateFailed = TRUE;                                       	
	}                                                               	
	AND_CATCH (CMemoryException, e)
	{
		AfxMessageBox(_T("Failed to create Index--Memory exception thrown."));                                       	

		// indicate failure                                         	
		bCreateFailed = TRUE;                                       	
	}
	END_CATCH                                                       	
	
	// return TRUE if creation succeeds
	return (!bCreateFailed);
}

// fill a Indexinfo struct--handle exceptions
// IN: pTableDef--pointer to tabledef object whose index collection we access
// OUT: pIndexInfo--information on index that is specifieed
// IN: IndexIndex--the index into the index collection for the item we want
// IN: bReportErrors--if TRUE, report any errors that occur, else silent
//	   TRUE by default
// RETURN: TRUE if information obtained, FALSE otherwise
BOOL getIndexInfo(CDaoTableDef *pTableDef, CDaoIndexInfo *pIndexInfo,
				  int IndexIndex, BOOL bReportErrors /*= TRUE*/)
{
	// if the tabledef is non-existent, then the answer is obvious
	if (pTableDef == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	TRY
	{                                                                                       	
		// try to get info on the Index                                                   		
		pTableDef->GetIndexInfo(IndexIndex, *pIndexInfo, AFX_DAO_ALL_INFO );
	}                                                                                       	
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message if requested
		if (bReportErrors)
		{
			CString strMessage = _T("Couldn't get information on Index--Exception: ");                  
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
			AfxMessageBox(_T("Failed to get info on Index--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	// return status
	return bSuccess;
}

// wrap Index deletion with exception handlers
// IN: pTableDef--pointer to tabledef object whose index collection we access
// IN: strIndexName--name of index to delete
// RETURN: TRUE if deletion suceeded, FALSE otherwise
BOOL deleteIndex(CDaoTableDef *pTableDef, CString strIndexName)
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
		// Index by the specified name--test for duplication
		pTableDef->DeleteIndex(strIndexName);
	}
	CATCH (CDaoException, e)
	{                                                                                       	
		CString strMessage = _T("Couldn't delete the Index--Exception: ");                  
		strMessage += e->m_pErrorInfo->m_strDescription;                                       	

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 	
		bSuccess = FALSE;                                                               		
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		AfxMessageBox(_T("Failed to delete the Index--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	return bSuccess;
}

