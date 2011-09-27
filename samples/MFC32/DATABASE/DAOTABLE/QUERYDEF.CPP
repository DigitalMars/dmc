// QUERYDEF.CPP -- MFC DAO QueryDef specific functions
//
// contains all querydef specific functions:
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

//BOOL IsExistentQuery(CDaoDatabase *pDatabase, CString strQueryName);
//BOOL createNewQueryDef(CDaoDatabase * pDatabase,
//									CDaoQueryDef **ppQueryDef,
//									CString strQueryName);
//BOOL appendQueryDef(CDaoDatabase *pDatabase, CDaoQueryDef *pQueryDef);
//BOOL openQueryDef(CDaoDatabase * pDatabase, CDaoQueryDef **ppQueryDef,
//				  CString strQueryName);
//BOOL getQueryInfo(CDaoDatabase *pDatabase, CDaoQueryDefInfo *pQueryInfo,
//				  int index, BOOL bReportErrors);
//BOOL deleteQuery(CDaoDatabase *pDatabase, CString strQueryName);
//BOOL createSetAndSaveNewQueryDef (CDaoDatabase * pDatabase,
//									CDaoQueryDef **ppQueryDef,
//									CDaoQueryDefInfo *pQI);
//BOOL modifyQueryDef (CDaoDatabase * pDatabase,
//					 CDaoQueryDef *pQueryDef,
//					 CDaoQueryDefInfo *pQI);


#include "stdafx.h"
#include "querydef.h"

// check for duplicate query name in querydef collection
// of database object
// IN: pDatabase--pointer to database whose querydef collection we will access
// IN: strQueryName--the name of the querydef we want to check for existence
// RETURN: TRUE if the specified querydef already exists, FALSE otherwise
BOOL IsExistentQuery(CDaoDatabase *pDatabase, CString strQueryName)
{
	// if no database, then done immediately
	if (pDatabase == NULL)
		return FALSE;

	// initialize status indicator
	BOOL bDuplicateQueryName = TRUE;
	
	// see if there is a query by this name already--duplicate
	// named querys are not accepted
	CDaoQueryDefInfo queryInfo;	// only needed for the call

	// MFC exception handler macros used
	TRY
	{
		// this call will throw an exception if there is no
		// query by the specified name--test for duplication
		pDatabase->GetQueryDefInfo(strQueryName, queryInfo);
	}
	CATCH (CDaoException, e)
	{
		// if this is an 'Item not found' exception, we are
		// cleared to create the query -- else this is
		// a duplicate queryname and we got another exception
		// which is irrelevant for our purposes
		if (e->m_pErrorInfo->m_lErrorCode == 3265)
			bDuplicateQueryName = FALSE;
	}
	AND_CATCH (CMemoryException, e)
	{
		// do nothing
		;
	}
	END_CATCH

	return bDuplicateQueryName;
}

// wraps the CreateQueryDef call with an exception handler and duplicate
// name check
// IN: pDatabase--pointer to database whose querydef collection we will access
// OUT: ppQueryDef--pointer to pointer of querydef we will create and return
// RETURN: TRUE if creation succeeded, FALSE otherwise
BOOL createNewQueryDef(CDaoDatabase * pDatabase,
									CDaoQueryDef **ppQueryDef,
									CString strQueryName)
{
	// if no database, then done immediately
	if (pDatabase == NULL)
		return FALSE;

	// check for existing query with this name just to be safe
	if (IsExistentQuery(pDatabase, strQueryName))
	{
		AfxMessageBox(_T("A query by that name already exists."));

		// return FALSE since can't create duplicate query
		return FALSE;
	}

	// initialize creation failure indicator
	BOOL bCreateFailed = FALSE;

	// cleanup any existing querydef          	
	if (*ppQueryDef != NULL)                  	
		delete *ppQueryDef;                   	
	                                          
	// construct querydef         	                                          
	*ppQueryDef = new CDaoQueryDef(pDatabase);
	
	// failed to allocate so exit with failure
	if ((*ppQueryDef) == NULL)
		return FALSE;

	// no duplication, so create the querydef if possible
	TRY
	{  
		(*ppQueryDef)->Create(strQueryName);                                                
	}                                                                                       
	CATCH (CDaoException, e)
	{                                                                                       
		// construct a meaningful message                                                   
		CString strMessage = _T("Couldn't create querydef--Exception: ");                       
		strMessage += e->m_pErrorInfo->m_strDescription;                                    

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 
		bCreateFailed = TRUE;                                                               
	                                                                                        
		// delete the querydef on failure                                                   
		if (*ppQueryDef != NULL)                                                            
		{                                                                                   
			delete *ppQueryDef;                                                             
			*ppQueryDef = NULL;                                                             
		}                                                                                   
	}                                                                                       
	AND_CATCH (CMemoryException, e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to create querydef--Memory exception thrown."));               

		// indicate failure                                                                 
		bCreateFailed = TRUE;                                                               
	                                                                                        
		// delete the querydef on failure                                                   
		if (*ppQueryDef != NULL)                                                            
		{                                                                                   
			delete *ppQueryDef;                                                             
			*ppQueryDef = NULL;                                                             
		}                                                                                   
	}
	END_CATCH                                                                               

	// return TRUE if creation succeeds
	return (!bCreateFailed);
}

// wraps the OpenQueryDef call with an exception handler
// IN: pDatabase--pointer to database whose querydef collection we will access
// OUT: ppQueryDef--pointer to pointer to querydef we will open and return
// IN: strQueryName--the name of the querydef we want to check for existence
// RETURN: TRUE if the open succeeded, FALSE otherwise
BOOL openQueryDef(CDaoDatabase * pDatabase, CDaoQueryDef **ppQueryDef,
				  CString strQueryName)
{
	// if no database, then done immediately
	if (pDatabase == NULL)
		return FALSE;

	// initialize creation failure indicator
	BOOL bOpenFailed = FALSE;

	// cleanup any existing querydef          	
	if (*ppQueryDef != NULL)                  	
		delete *ppQueryDef;                   	
	                                          
	// construct querydef         	                                          	
	*ppQueryDef = new CDaoQueryDef(pDatabase);	                                          	
	
	// failed to allocate so exit with failure
	if ((*ppQueryDef) == NULL)
		return FALSE;

	// open the querydef if possible
	TRY
	{                                                                                       	
		(*ppQueryDef)->Open(strQueryName);                                                		
	}                                                                                       	
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message                                                   	
		CString strMessage = _T("Couldn't open querydef--Exception: ");                          	
		strMessage += e->m_pErrorInfo->m_strDescription;                                       	

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 	
		bOpenFailed = TRUE;                                                               		
	                                                                                        
		// delete the querydef on failure                                                       
		if (*ppQueryDef != NULL)                                                            	
		{                                                                                   	
			delete *ppQueryDef;                                                             	
			*ppQueryDef = NULL;                                                             	
		}                                                                                   	
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to open querydef--Memory exception thrown."));                                       	

		// indicate failure                                                                 	
		bOpenFailed = TRUE;                                                               		
	                                                                                        
		// delete the querydef on failure                                                       
		if (*ppQueryDef != NULL)                                                            	
		{                                                                                   	
			delete *ppQueryDef;                                                             	
			*ppQueryDef = NULL;                                                             	
		}                                                                                   	
	}
	END_CATCH                                                                               	

	// return TRUE if open succeeds
	return (!bOpenFailed);
}

// append the specified query def to the collection of querydefs in the 
// database object
//
// IN: pDatabase--pointer to database whose querydef collection we will access
// IN: pQueryDef--pointer to querydef we will append to the collection
// RETURN: TRUE if querydef appended, FALSE otherwise
BOOL appendQueryDef(CDaoDatabase *pDatabase, CDaoQueryDef *pQueryDef)
{
	// if no database, then done immediately
	if (pDatabase == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	// append the querydef to the collection                         	
	// catch exceptions using C++ style exceptions                   	
	TRY                                                              	
	{                                                                	
		pQueryDef->Append();                                         	
	}                                                                	
	CATCH (CDaoException, e)                                         	
	{                                                                	
		// construct informative message                             	
		CString strMessage = _T("Couldn't append QueryDef--Exception: ");	
		strMessage += e->m_pErrorInfo->m_strDescription;             	
	                                                                 
		// output status                                             	
		AfxMessageBox(strMessage);                                   	
	
		// failure 
		bSuccess = FALSE;
	}                                                                	
	AND_CATCH (CMemoryException, e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to append querydef--Memory exception thrown."));                                       	

		// failure 
		bSuccess = FALSE;
	}
	END_CATCH

	// return status
	return bSuccess;
}

// fill a querydef info struct--handle exceptions
// IN: pDatabase--pointer to database whose querydef collection we will access
// IN/OUT: pQueryInfo--pointer to querydef info struct we want to fill in 
// IN: index--index into the collection of querydefs for the item we want
//	   pass a negative value if you want to use the name in pQueryInfo
//	   to look up the querydef
// IN: bReportErrors--if TRUE, report any errors that occur, else silent
//	   TRUE by default
// RETURN: TRUE if information obtained, FALSE otherwise
BOOL getQueryInfo(CDaoDatabase *pDatabase, CDaoQueryDefInfo *pQueryInfo,
				  int index, BOOL bReportErrors /*= TRUE*/)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	TRY
	{                                                                                       	
		// try to get info on the querydef either by index or by name
		if (index < 0)
			pDatabase->GetQueryDefInfo(pQueryInfo->m_strName, *pQueryInfo,
								AFX_DAO_ALL_INFO );
		else
			pDatabase->GetQueryDefInfo(index, *pQueryInfo,
								AFX_DAO_ALL_INFO );

	}                                                                                       	
	CATCH (CDaoException, e)
	{                                                                                       	
		// construct a meaningful message if requested
		if (bReportErrors)
		{
			CString strMessage = _T("Couldn't get information on QueryDef--Exception: ");                  
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
			AfxMessageBox(_T("Failed to get info on QueryDef--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	// return status
	return bSuccess;
}

// wrap query deletion with exception handlers
// check for duplicate query name in query collection
// of tabledef object
// IN: pDatabase--pointer to database whose querydef collection we will access
// IN: strQueryName--the name of the querydef we want to delete
// RETURN: TRUE if the querydef was deleted, FALSE otherwise
BOOL deleteQuery(CDaoDatabase *pDatabase, CString strQueryName)
{
	// if the database is non-existent, then the answer is obvious
	if (pDatabase == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	// MFC exception handler macros used
	TRY
	{
		pDatabase->DeleteQueryDef(strQueryName);
	}
	CATCH (CDaoException, e)
	{                                                                                       	
		CString strMessage = _T("Couldn't delete the query--Exception: ");                  
		strMessage += e->m_pErrorInfo->m_strDescription;                                       	

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 	
		bSuccess = FALSE;                                                               		
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		AfxMessageBox(_T("Failed to delete the query--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	return bSuccess;
}

// create a querydef and set its properties, append it to
// the collection
// IN: pDatabase--pointer to database whose querydef collection we will access
// IN/OUT: ppQueryDef--pointer to pointer of querydef we will process
// IN: pQI--pointer to information needed to create the querydef
// RETURN: TRUE if all processing succeeds, FALSE otherwise
BOOL createSetAndSaveNewQueryDef (CDaoDatabase * pDatabase,
									CDaoQueryDef **ppQueryDef,
									CDaoQueryDefInfo *pQI)
{
	// if no database, then done immediately
	if (pDatabase == NULL)
		return FALSE;

	// initialize failure indicator
	BOOL bFailure = TRUE;

	// call our create function and continue if it succeeds
	if (createNewQueryDef(pDatabase, ppQueryDef, pQI->m_strName))
	{
		// can only specify some properties on create, so specify the rest now
		if (modifyQueryDef (pDatabase, *ppQueryDef, pQI))
		{	
			// now store the query def permanently
			if (appendQueryDef(pDatabase, *ppQueryDef))
			{
				// if you get here, you've succeeded,
				bFailure = FALSE;
			}
		}
	}

	// return TRUE if everything succeeds
	return (!bFailure);
}

// set the properties of the query def after it has been appended
//
// IN: pDatabase--pointer to database whose querydef collection we will access
// IN: pQueryDef--pointer to  querydef we will modify
// IN: pQI--pointer to information needed to modifiy the querydef
// RETURN: TRUE is modification succeeds, FALSE otherwise
BOOL modifyQueryDef (CDaoDatabase * pDatabase,
					 CDaoQueryDef *pQueryDef,
					 CDaoQueryDefInfo *pQI)
{
	// if no database, then done immediately
	if (pDatabase == NULL)
		return FALSE;

	// initialize success indicator
	BOOL bSuccess = TRUE;

	// MFC exception handler macros used
	TRY
	{
		pQueryDef->SetSQL(pQI->m_strSQL);
		pQueryDef->SetName(pQI->m_strName);
	}
	CATCH (CDaoException, e)
	{                                                                                       	
		CString strMessage = _T("Couldn't modify the query--Exception: ");                  
		strMessage += e->m_pErrorInfo->m_strDescription;                                       	

		AfxMessageBox(strMessage);
	                                                                                        
		// indicate failure                                                                 	
		bSuccess = FALSE;                                                               		
	}                                                                                       	
	AND_CATCH (CMemoryException, e)
	{
		AfxMessageBox(_T("Failed to modify the query--Memory exception thrown."));                                       	

		// indicate failure                                                                 
		bSuccess = FALSE;                                                               	        
	}
	END_CATCH                                                                               	

	return (bSuccess);
}
