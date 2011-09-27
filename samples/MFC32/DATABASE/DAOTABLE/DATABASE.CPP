// DATABASE.CPP -- MFC DAO Database specific functions

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
// contains the functions that deal directly with DAODatabase
// creation, opening and closing:
//
// void closeDatabase(CDaoDatabase **ppDatabase);
//
// int openDatabase(CDaoDatabase **ppDatabase, CString fileName,
//				  BOOL bReportNoOpen = TRUE);
//
// BOOL createDatabase(CDaoDatabase **ppDatabase, CString fileName,
//				  int dwOptions = 0);

#include "stdafx.h"
#include "database.h"

// close the database and destruct it
// IN/OUT:: ppDatabase--pointer to pointer to database to close and
// destruct
void closeDatabase(CDaoDatabase **ppDatabase)
{
	// only process if the database object exists
	if (*ppDatabase != NULL)
	{
		if ((*ppDatabase)->IsOpen())
			(*ppDatabase)->Close();
		
		// closing doesn't delete the object
		delete *ppDatabase;
		*ppDatabase = NULL;
	}
}

// open the database from a file (close if necessary)
// IN/OUT: ppDatabase--pointer to pointer to database object
// IN: fileName--name of .mdb file to open
// IN: bReportNoOpen--TRUE by default, if true, if open fails
//     because the specified MDB file doesn't exist, report that
//	   fact to the user.  Note: other errors are reported always
// RETURN: 1 if success, 0 if non-fatal failure, -1 if fatal failure
int openDatabase(CDaoDatabase **ppDatabase, CString fileName,
				  BOOL bReportNoOpen /* = TRUE */)
{
	// initialize success indicator
	int nReturnCode = 1;

	// close and delete if necessary
	if (*ppDatabase != NULL)
	{
		if ((*ppDatabase)->IsOpen())
			closeDatabase(ppDatabase);
		delete *ppDatabase;
	}

	// construct new database
	*ppDatabase = new CDaoDatabase;

	// failed to allocate
	if (ppDatabase == NULL)
		return -1; // fatal error

	// now open the database object with error checking
	try
	{
		(*ppDatabase)->Open(fileName);
	}
	catch (CDaoException *e)
	{
		// special case--couldn't find the file, so it may be because
		// user specified a new file to open
		if (e->m_pErrorInfo->m_lErrorCode == 3024)
		{
			if (bReportNoOpen)
			{
				// create a message to display
				CString message = _T("Couldn't open database--Exception: ");    	
				message += e->m_pErrorInfo->m_strDescription;                 	
																			  
				// output status                                              	
				AfxMessageBox(message);                                       	
			}

			// indicate failure but not fatal
			nReturnCode = 0;
		}
		else // other type of DAO exception--always report
		{
			// create a message to display
			CString message = _T("Couldn't open database--Exception: ");    	
			message += e->m_pErrorInfo->m_strDescription;                 	
																			  
			// output status                                              	
			AfxMessageBox(message);                                       	

			// indicate fatal error
			nReturnCode = -1;
		}
		
		// not rethrowing, so delete exception                        	
		e->Delete();

		delete *ppDatabase;
		*ppDatabase = NULL;
	}
	catch (CMemoryException *e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to open database--Memory exception thrown."));                                       	

		// not rethrowing, so delete exception                        	
		e->Delete();

		delete *ppDatabase;
		*ppDatabase = NULL;

		// indicate fatal error
		nReturnCode = -1;
	}

	return nReturnCode;
}

// create the database file (close any open database)
// IN/OUT: ppDatabase--pointer to pointer to database object
// IN: fileName--name of .mdb file to open
// IN: dwOptions--info like version and encryption settings
//	   0 by default
// RETURN: TRUE if success, FALSE if failure
BOOL createDatabase(CDaoDatabase **ppDatabase, CString fileName,
				  int dwOptions /* = 0 */)
{
	// initialize success indicator
	BOOL bSuccess = TRUE;

	// close and delete if necessary
	if (*ppDatabase != NULL)
	{
		if ((*ppDatabase)->IsOpen())
			closeDatabase(ppDatabase);
		delete *ppDatabase;
	}

	// construct new database
	*ppDatabase = new CDaoDatabase;

	// failed to allocate
	if (ppDatabase == NULL)
		return FALSE; // error

	// now create the database object with error checking
	try
	{
		// default language specified
		(*ppDatabase)->Create(fileName, dbLangGeneral, dwOptions);
	}
	catch (CDaoException *e)
	{
		// create a message to display                               	
		CString message = _T("Couldn't create database--Exception: ");   	 	
		message += e->m_pErrorInfo->m_strDescription;                	 	
																	 	 
		// output status                                             	 	
		AfxMessageBox(message);                                      	                        

		// not rethrowing, so delete exception                        	
		e->Delete();

		delete *ppDatabase;
		*ppDatabase = NULL;

		// failure
		bSuccess = FALSE;
	}
	catch (CMemoryException *e)
	{
		// output status                                              	
		AfxMessageBox(_T("Failed to create database--Memory exception thrown."));  

		// not rethrowing, so delete exception                        	
		e->Delete();

		delete *ppDatabase;
		*ppDatabase = NULL;

		// failure
		bSuccess = FALSE;
	}

	return bSuccess;
}