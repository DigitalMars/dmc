// QUERYDEF.H

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

BOOL IsExistentQuery(CDaoDatabase *pDatabase, CString strQueryName);
BOOL createNewQueryDef(CDaoDatabase * pDatabase,
									CDaoQueryDef **ppQueryDef,
									CString strQueryName);
BOOL appendQueryDef(CDaoDatabase *pDatabase, CDaoQueryDef *pQueryDef);
BOOL openQueryDef(CDaoDatabase * pDatabase, CDaoQueryDef **ppQueryDef,
				  CString strQueryName);
BOOL getQueryInfo(CDaoDatabase *pDatabase, CDaoQueryDefInfo *pQueryInfo,
				  int index, BOOL bReportErrors = TRUE);
BOOL deleteQuery(CDaoDatabase *pDatabase, CString strQueryName);
BOOL createSetAndSaveNewQueryDef (CDaoDatabase * pDatabase,
									CDaoQueryDef **ppQueryDef,
									CDaoQueryDefInfo *pQI);
BOOL modifyQueryDef (CDaoDatabase * pDatabase,
					 CDaoQueryDef *pQueryDef,
					 CDaoQueryDefInfo *pQI);