/*************************************************************************
**
**    OLE 2.0 Property Set Utilities
**
**    testsumi.cpp
**
**    This file contains a trivial test program to drive the OLE 2.0
**    Property Set utilities as implemented in SUMINFO.CPP. The
**    utilities in SUMINFO.CPP manipulate the Summary Info property set.
**
**    This sample is a simple QuickWin program.
**
**    This sample tries to read the file "\\test.dfl".
**    If this file does NOT exist, then it creates one and adds a
**    Summary Info property set.
**    If this file DOES exist, then it opens it, reads the existing
**    Summary Info property set, increments the Summary Info "REVNUMBER"
**    field, and rewrites the modified Summary Info back to the file.
**    Some interesting fields of the Summary Info before and after are
**    printed.
**
**    (c) Copyright Microsoft Corp. 1990 - 1992 All Rights Reserved
**
*************************************************************************/

#include <windows.h>
#include <ole2.h>
#include <storage.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "..\include\suminfo.h"

static int TestMakeDocFile();
static void TestReadDocFile();
static void TestPrintSummaryInfo(LPSUMINFO lpSumInf);

void main()
{
	if (FAILED(CoInitialize(NULL)))
	{
		printf("CoInitialize FAILED\n");
		return;
	};

	TestReadDocFile();

	// De-initialization for CO
	CoUninitialize();
}


/****************************************************************************
**
** TestReadDocFile()
**
** reads summary info property set from "\\test.dfl" DocFile.
**
*****************************************************************************/

static void TestReadDocFile()
{
	LPSTORAGE lpRootStg;
	LPSTREAM lpStream;
	HRESULT hrErr;
	char szFile[50]="\\test.dfl";
	char name1[50]=PropStreamName;
	LPSUMINFO lpSumInf;
	DWORD d,c;
	THUMBNAIL t;
	LPSTR lpsz;
	char szBuf[32];
	LONG lRevNum = 0;

	lpSumInf = OleStdInitSummaryInfo(0);
	hrErr = StgOpenStorage(
			szFile,
			NULL,
			STGM_READWRITE | STGM_DIRECT| STGM_SHARE_EXCLUSIVE,
			NULL,
			0,
			&lpRootStg);

	if (FAILED(hrErr))
	{
		printf("Could not open storage file %s.\n", (LPSTR)szFile);
		TestMakeDocFile();
		return;
	};
	hrErr = lpRootStg->OpenStream(
			name1,
			NULL,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
			0,
			&lpStream);

	if (FAILED(hrErr)) {
		printf("OpenStream FAILED\n");
		return;
	}
	OleStdReadSummaryInfo(lpStream, lpSumInf);

	// Print out summary info to stdout
	printf("Original Summary Info:\n");
	TestPrintSummaryInfo(lpSumInf);

	// Modify some of the summary info
	//   we will increment the PID_REVNUMBER field
	OleStdGetThumbNailProperty(lpStream, lpSumInf, &d, NULL, &t, &c, 0);
	//OleStdGetThumbNailProperty(lpStream, lpSumInf, &d, NULL, &t, &c, 1);
	lpsz = OleStdGetStringProperty(lpSumInf, PID_REVNUMBER);
	if (lpsz != NULL) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		sscanf(szBuf, "%ld", &lRevNum);
	} else {
		lRevNum = 0;
	}
	sprintf(szBuf, "%ld", ++lRevNum);
	OleStdSetStringProperty(lpSumInf, PID_REVNUMBER, szBuf);
	OleStdWriteSummaryInfo(lpStream, lpSumInf);

	// Print out modified summary info to stdout
	printf("\nModified Summary Info:\n");
	TestPrintSummaryInfo(lpSumInf);

	OleStdFreeSummaryInfo(&lpSumInf);
	lpStream->Release();

	// And release the new stream since we are finished with it
	lpRootStg->Release();
}


/***************************************************************************
**
** TestMakeDocFile()
**
** Creates a new docfile called "\\test.dfl".
**
** NOTE: This function does not correctly recover from error conditions
** in some cases.
**
****************************************************************************/

static int TestMakeDocFile()
{
	LPSTORAGE lpRootStg;
	LPSTORAGE lpStorage2;
	HRESULT hrErr;

	// flags for creating sub storages and streams.  Open in direct mode so
	// we don't have to commit each write.  Since we're opening in DIRECT
	// and WRITE modes, we must also specify SHARE_EXCLUSIVE or the call to
	// create sub storages and streams will fail.

	DWORD dwCScode = STGM_WRITE|STGM_DIRECT|STGM_SHARE_EXCLUSIVE|STGM_CREATE;

	LPSTREAM lpStream;
	char name1[] = PropStreamName;
	char data1[] = "Enterprise DocFile";

	char name2[] = "Ship Location";
	char data2[] = "Upper west side, Bronx, New York, USA, Earth, Milky Way";

	char name3[] = "Personnel Roster";

	char name4[] = "Picard";
	char data4[] = "Rank: Captain\r\nSerial # 42435553-43\r\n";

	char name5[] = "Data";
	char data5[] = "Rank: Lt. Commander\r\nSerial # 001000100010101001\r\n";

	char name6[] = "Troi";
	char data6[] = "Rank: Counselor\r\nSerial # S22434-43\r\n";

	char szFile[50]="\\test.dfl";
	OFSTRUCT of;
	DWORD cbWritten;
	LPSUMINFO lpSumInf;
	HANDLE hmf, hmfb;
	METAHEADER FAR *lp;
	LPMETAFILEPICT lpmfp;

	lpSumInf = OleStdInitSummaryInfo(0);

	OleStdSetDateProperty(lpSumInf, PID_EDITTIME, 0, 0, 0, 1, 1, 2);
	OleStdSetDateProperty(lpSumInf, PID_LASTPRINTED, 1993, 7, 3, 12, 23, 47);
	OleStdSetDateProperty(lpSumInf, PID_CREATE_DTM_RO, 1992, 6, 3, 12, 23, 47);
	OleStdSetDateProperty(lpSumInf, PID_LASTSAVE_DTM, 1993, 6, 3, 12, 23, 47);

	OleStdSetDocProperty(lpSumInf, 57, 3000, 23000);

	OleStdSetSecurityProperty(lpSumInf, fSecurityRO);

	/* This is a real HACK to get thumbnail data. if the file
	**    "\\test.wmf" exits, then it reads it and assumes it is a
	**    thumbnail whose size is 3000 by 2000. if a test.wmf file
	**    really exists, then these extent values (lpmfp->xExt,
	**    lpmfp->yExt) must be set properly.
	**
	**    NOTE: this testsumi sample does not ship with a \\test.wmf
	**    file. you must provide your own and set up the extents
	**    properly.
	*/
	if (HFILE_ERROR != OpenFile("\\test.wmf", &of, OF_EXIST) &&
		(hmf = GetMetaFile("\\test.wmf"))!=NULL) {
		lp=(METAHEADER FAR *)GlobalLock(hmf);
		// convert words to bytes
		cbWritten=lp->mtSize*sizeof(WORD)+sizeof(METAFILEPICT);
		hmfb = GlobalAlloc(GHND, sizeof(METAFILEPICT));
		lpmfp = (LPMETAFILEPICT)GlobalLock(hmfb);
		lpmfp->mm = MM_ANISOTROPIC;
		lpmfp->xExt = 3000; // REVIEW: this should match size of "test.wmf"
		lpmfp->yExt = 2000; // REVIEW: this should match size of "test.wmf"
		lpmfp->hMF = hmf;
		GlobalUnlock(hmf);
		GlobalUnlock(hmfb);
		if (lp!=NULL) {
			OleStdSetThumbNailProperty(
					NULL,
					lpSumInf,
					VT_CF_WIN,
					CF_METAFILEPICT,
					NULL,
					(THUMBNAIL)(DWORD)hmfb,
					cbWritten);
		}
	}

	OleStdSetStringProperty(lpSumInf, PID_TITLE, "Test Document");
	OleStdSetStringProperty(lpSumInf, PID_SUBJECT, "Lots of Properties");
	OleStdSetStringProperty(lpSumInf, PID_AUTHOR, "John Doe");
	OleStdSetStringProperty(lpSumInf, PID_KEYWORDS, "foobar, properties, lots");
	OleStdSetStringProperty(lpSumInf, PID_COMMENTS, "I have NO comment!");
	OleStdSetStringProperty(lpSumInf, PID_LASTAUTHOR, "Mrs. John Doe");
	OleStdSetStringProperty(lpSumInf, PID_REVNUMBER, "1");
	OleStdSetStringProperty(lpSumInf, PID_APPNAME, "Summary Info Test App");

	// Create New docfile.  Since we didn't specify STGM_FAILIFTHERE, this
	// function will wipe over any existing file.  We specify transacted
	// mode here, so we need to remember to commit our changes at the end
	// of using this root IStorage.

	hrErr = StgCreateDocfile(szFile, dwCScode, 0, &lpRootStg);

	if (FAILED(hrErr)) {
		printf("StgCreateDocfile failed, code = %ld\n", GetScode(hrErr));
		return FALSE;
	}
	else {
		printf("Created file %s.\n", (LPSTR)szFile);
	}

	// Create stream #1 with the IStorage::CreateStream() function and
	//   write data to it
	hrErr = lpRootStg->CreateStream(name1, dwCScode, 0, 0, &lpStream);
	if (FAILED(hrErr)) goto ErrorStg;
	OleStdWriteSummaryInfo(lpStream, lpSumInf);
	// And release the new stream since we are finished with it
	lpStream->Release();

	// print out the Summary Info property set that we created
	TestPrintSummaryInfo(lpSumInf);
	OleStdFreeSummaryInfo(&lpSumInf);

	// Create stream #2
	hrErr = lpRootStg->CreateStream(name2, dwCScode, 0, 0, &lpStream);
	if (FAILED(hrErr)) goto ErrorStg;
	hrErr = lpStream->Write(data2, sizeof(data2), &cbWritten);
	if (FAILED(hrErr)) goto ErrorStream;
	lpStream->Release();

	// Create child storage.  We use the IStorage::CreateStorage()
	//   function to do this.
	hrErr = lpRootStg->CreateStorage(name3, dwCScode, 0,0,&lpStorage2);
	if (FAILED(hrErr)) goto ErrorStg;

	// Create stream #1 in SECOND STORAGE
	hrErr = lpStorage2->CreateStream(name4, dwCScode, 0, 0, &lpStream);
	if (FAILED(hrErr)) goto ErrorStg;
	hrErr = lpStream->Write(data4, sizeof(data4), &cbWritten);
	if (FAILED(hrErr)) goto ErrorStream;
	lpStream->Release();

	// Create stream #2 in SECOND STORAGE
	hrErr = lpStorage2->CreateStream(name5, dwCScode, 0, 0, &lpStream);
	if (FAILED(hrErr)) goto ErrorStg;
	hrErr = lpStream->Write(data5, sizeof(data5), &cbWritten);
	if (FAILED(hrErr)) goto ErrorStream;
	lpStream->Release();

	// Create stream #3 in SECOND STORAGE
	hrErr = lpStorage2->CreateStream(name6, dwCScode, 0, 0, &lpStream);
	if (FAILED(hrErr)) goto ErrorStg;
	hrErr = lpStream->Write(data6, sizeof(data6), &cbWritten);
	if (FAILED(hrErr)) goto ErrorStream;
	lpStream->Release();

	// Now, we're completed with the second storage.  Release the pointer.
	lpStorage2->Release();

	// And finally, we're done with our root storage.  Before we release it,
	// we must commit the data since we opened the root in Transacted mode
	lpRootStg->Commit(STGC_OVERWRITE);
	lpRootStg->Release();

	return (TRUE);

ErrorStream:
	printf("Stream Error, code = %ld\n", GetScode(hrErr));
	lpStream->Release();
	lpRootStg->Release();
	return (FALSE);

ErrorStg:
	printf("Storage Error, code = %ld\n", GetScode(hrErr));
	lpRootStg->Release();
	return (FALSE);
}


// Simple routine to print out some interesting fields of a Summary Info
static void TestPrintSummaryInfo(LPSUMINFO lpSumInf)
{
	LPSTR lpsz;
	char  szBuf[128];
	lpsz = OleStdGetStringProperty(lpSumInf, PID_TITLE);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("TITLE: %s\n", (LPSTR)szBuf);
	}
	lpsz = OleStdGetStringProperty(lpSumInf, PID_SUBJECT);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("SUBJECT: %s\n", (LPSTR)szBuf);
	}
	lpsz = OleStdGetStringProperty(lpSumInf, PID_AUTHOR);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("AUTHOR: %s\n", (LPSTR)szBuf);
	}
	lpsz = OleStdGetStringProperty(lpSumInf, PID_KEYWORDS);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("KEYWORDS: %s\n", (LPSTR)szBuf);
	}
	lpsz = OleStdGetStringProperty(lpSumInf, PID_COMMENTS);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("COMMENTS: %s\n", (LPSTR)szBuf);
	}
	lpsz = OleStdGetStringProperty(lpSumInf, PID_LASTAUTHOR);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("LASTAUHOR: %\n", (LPSTR)szBuf);
	}
	lpsz = OleStdGetStringProperty(lpSumInf, PID_REVNUMBER);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("REVNUMBER: %s\n", (LPSTR)szBuf);
	}
	lpsz = OleStdGetStringProperty(lpSumInf, PID_APPNAME);
	if (lpsz) {
		_fstrncpy(szBuf, lpsz, sizeof(szBuf));
		printf("APPNAME: %s\n", (LPSTR)szBuf);
	}
}
