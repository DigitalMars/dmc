// TABLEDEF.H

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

BOOL IsExistentTable(CDaoDatabase *pDatabase, CString strTableName);
BOOL createNewTableDef(CDaoDatabase * pDatabase,
									CDaoTableDef **ppTableDef,
									CString strTableName);
BOOL appendTableDef(CDaoDatabase *pDatabase, CDaoTableDef *pTableDef);
BOOL openTableDef(CDaoDatabase * pDatabase, CDaoTableDef **ppTableDef,
				  CString strTableName);
BOOL getTableInfo(CDaoDatabase *pDatabase, CDaoTableDefInfo *pTableInfo,
				  int tableIndex, BOOL bReportErrors = TRUE);
void deleteTable(CDaoDatabase *pDatabase, CString strTableName);
