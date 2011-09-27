// TABLEDEF.CPP -- MFC DAO TableDef specific functions
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

// contains:
//
//BOOL IsExistentTable(CDaoDatabase *pDatabase, CString strTableName);
//BOOL createNewTableDef(CDaoDatabase * pDatabase,
//									CDaoTableDef **ppTableDef,
//									CString strTableName);
//BOOL appendTableDef(CDaoDatabase *pDatabase, CDaoTableDef *pTableDef);
//BOOL openTableDef(CDaoDatabase * pDatabase, CDaoTableDef **ppTableDef,
//				  CString strTableName);
//BOOL getTableInfo(CDaoDatabase *pDatabase, CDaoTableDefInfo *pTableInfo,
//				  int tableIndex, BOOL bReportErrors);
//void deleteTable(CDaoDatabase *pDatabase, CString strTableName);


#include "stdafx.h"
#include "tabledef.h"

// check for duplicate table name in tabledef collection
// of database object
// IN: pDatabase--pointer to database object whose tabledef collection we will
//     access
// IN: strTableName--name of the table whose existence we want to check
// RETURN: TRUE if the table already exists, FALSE otherwise 
BOOL IsExistentTable(CDaoDatabase *pDatabase, CString strTableName)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return FALSE;

	// initialize status indicator
	BOOL bDuplicateTableName = TRUE;
	
	// see if there is a table by this name already--duplicate
	// named tables are not accepted
	CDaoTableDefInfo tableInfo;	// only needed for the call

	// MFC exception handler macros used
	TRY
	{
		// this call will throw an exception if there is no
		// table by the specified name--test for duplication
		pDatabase->GetTableDefInfo(strTableName, tableInfo);
	}
	CATCH (CDaoException, e)
	{
		// if this is an 'Item not found' exception, we are
		// cleared to create the table -- else this is
		// a duplicate tablename and we got another exception
		// which is irrelevant for our purposes
		if (e->m_pErrorInfo->m_lErrorCode == 3265)
			bDuplicateTableName = FALSE;
	}
	AND_CATCH (CMemoryException, e)
	{
		// do nothing
		;
	}
	END_CATCH

	return bDuplicateTableName;
}

// wraps the CreateTableDef call with an exception handler and duplicate
// name check
// IN: pDatabase--pointer to database object whose tabledef collection we will
//     access
// OUT: ppTableDef--pointer to pointer to tabledef we are creating
// IN: strTableName--name of the table we want to create
// RETURN: TRUE if creation succeeds, FALSE otherwise
BOOL createNewTableDef(CDaoDatabase * pDatabase,
									CDaoTableDef **ppTableDef,
									CString strTableName)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return FALSE;

	// check for existing table with this name just to be safe
	if (IsExistentTable(pDatabase, strTableName))
	{
		AfxMessageBox(_T("A table by that name already exists."));

		// return FALSE since can't create duplicate table
		return FALSE;
	}

	// initialize creation failure indicator
	BOOL bCreateFailed = FALSE;
	
	// construct tabledef                     	                              	
	*ppTableDef = new CDaoTableDef(pDatabase);
	
	// failed to allocate so exit
	if ((*ppTableDef) == NULL)
		return FALSE;

	// no duplication, so create the tabledef if possible
	TRY
	{                                                                                       	
		(*ppTableDef)->Create(strTableName);                                                	
	}                                                                                       	
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message                                                   	
		CString strMessage = _T("Couldn't create tabledef--Exception: ");                          	
		strMessage += e->m_pErrorInfo->m_strDescription;                                       	

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 	
		bCreateFailed = TRUE;                                                               	
	                                                                                        
		// delete the tabledef on failure                                                       
		if (*ppTableDef != NULL)                                                            	
		{                                                                                   	
			delete *ppTableDef;                                                             	
			*ppTableDef = NULL;                                                             	
		}                                                                                   	
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to create tabledef--Memory exception thrown."));                   

		// indicate failure                                                                 	
		bCreateFailed = TRUE;                                                               	
	                                                                                        
		// delete the tabledef on failure                                                       
		if (*ppTableDef != NULL)                                                            	
		{                                                                                   	
			delete *ppTableDef;                                                             	
			*ppTableDef = NULL;                                                             	
		}                                                                                   	
	}
	END_CATCH                                                                               	

	// return TRUE if creation succeeds
	return (!bCreateFailed);
}

// wraps the OpenTableDef call with an exception handler
// IN: pDatabase--pointer to database object whose tabledef collection we will
//     access
// OUT: ppTableDef--pointer to pointer to tabledef we are opening
// IN: strTableName--name of the table we want to open
// RETURN: TRUE if open succeeds, FALSE otherwise 
BOOL openTableDef(CDaoDatabase * pDatabase, CDaoTableDef **ppTableDef,
				  CString strTableName)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return FALSE;

	// initialize creation failure indicator
	BOOL bOpenFailed = FALSE;
	
	// construct tabledef         	                                          	
	*ppTableDef = new CDaoTableDef(pDatabase);	                                          	

	// failed to allocate so exit
	if ((*ppTableDef) == NULL)
		return FALSE;

	// open the tabledef if possible
	TRY
	{                                                                                       	
		(*ppTableDef)->Open(strTableName);                                                		
	}                                                                                       	
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message                                                   	
		CString strMessage = _T("Couldn't open tabledef--Exception: ");                          	
		strMessage += e->m_pErrorInfo->m_strDescription;                                       	

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 	
		bOpenFailed = TRUE;                                                               		
	                                                                                        
		// delete the tabledef on failure                                                       
		if (*ppTableDef != NULL)                                                            	
		{                                                                                   	
			delete *ppTableDef;                                                             	
			*ppTableDef = NULL;                                                             	
		}                                                                                   	
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to open tabledef--Memory exception thrown."));                                       	

		// indicate failure                                                                 	
		bOpenFailed = TRUE;                                                               		
	                                                                                        
		// delete the tabledef on failure                                                       
		if (*ppTableDef != NULL)                                                            	
		{                                                                                   	
			delete *ppTableDef;                                                             	
			*ppTableDef = NULL;                                                             	
		}                                                                                   	
	}
	END_CATCH                                                                               	

	// return TRUE if open succeeds
	return (!bOpenFailed);
}

// append the tabledef to the database collection 
// IN: pDatabase--pointer to database object whose tabledef collection we will
//     access
// IN: pTableDef--pointer tabledef we are appending to the collection
// RETURN: TRUE if append succeeds, FALSE otherwise 
BOOL appendTableDef(CDaoDatabase *pDatabase, CDaoTableDef *pTableDef)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	// append the tabledef to the collection                         	
	TRY                                                              	
	{                                                                	
		pTableDef->Append();                                         	
	}                                                                	
	CATCH (CDaoException, e)
	{                                                                	
		// construct informative message                             	
		CString strMessage = _T("Couldn't append TableDef--Exception: ");	
		strMessage += e->m_pErrorInfo->m_strDescription;             	
	                                                                 
		// output status                                             	
		AfxMessageBox(strMessage);                                   	
	
		// failure 
		bSuccess = FALSE;
	}                                                                	
	AND_CATCH (CMemoryException, e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to append tabledef--Memory exception thrown."));                                       	

		// failure 
		bSuccess = FALSE;
	}
	END_CATCH                                                                               	

	// return status
	return bSuccess;
}

// fill a tableinfo struct--handle exceptions
// IN: pDatabase--pointer to database object whose tabledef collection we will
//     access
// OUT: pTableInfo--pointer to structure to hold the tabledef info
// IN: tableIndex--index into the collection indicating which item we want
// IN: bReportErrors--TRUE by default, if TRUE, report all errors, else silent
// RETURN: TRUE if information obtained, FALSE otherwise 
BOOL getTableInfo(CDaoDatabase *pDatabase, CDaoTableDefInfo *pTableInfo,
				  int tableIndex, BOOL bReportErrors /*= TRUE*/)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	TRY
	{                                                                                       	
		// try to get info on the table                                                   		
		pDatabase->GetTableDefInfo(tableIndex, *pTableInfo, AFX_DAO_ALL_INFO );
	}                                                                                       	
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message if request
		if (bReportErrors)
		{
			CString strMessage = _T("Couldn't get information on table--Exception: ");                  
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
			AfxMessageBox(_T("Failed to get info on table--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	// return status
	return bSuccess;
}

// detete the specified table from the collection of tables
// IN: pDatabase--pointer to database object whose tabledef collection we will
//     access
// IN: strTableName--name of the table we want to delete
void deleteTable(CDaoDatabase *pDatabase, CString strTableName)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return;

	// see if there is a table by this name -- if not, can't proceed
	if (!IsExistentTable(pDatabase, strTableName))
		return;

	// MFC exception handler macros used
	TRY
	{
		pDatabase->DeleteTableDef(strTableName);
	}
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message if request
		CString strMessage = _T("Couldn't delete the table--Exception: ");	                  
		strMessage += e->m_pErrorInfo->m_strDescription;                          	             	
		                                                                          
		AfxMessageBox(strMessage);                                                	
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		// output status if requested
		AfxMessageBox(_T("Failed to delete the table--Memory exception thrown."));                                       	
	}
	END_CATCH                                                                               	

	return;
}



