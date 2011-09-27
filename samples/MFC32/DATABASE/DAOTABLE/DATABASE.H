// DATABASE.H

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

void closeDatabase(CDaoDatabase **ppDatabase);
int openDatabase(CDaoDatabase **ppDatabase, CString fileName,
				  BOOL bReportNoOpen = TRUE);
BOOL createDatabase(CDaoDatabase **ppDatabase, CString fileName,
				  int dwOptions = 0);
