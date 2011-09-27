/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1994 - 1995	Microsoft Corporation.	All Rights Reserved.
 *
 **************************************************************************/
// batchdoc.cpp : implementation of the CBatchDoc class
//

#define INC_OLE2
#include "stdafx.h"
#include "mainfrm.h"
#include "batch.h"
#include "batchdoc.h"
#include "destdird.h"
#include "interlea.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#if defined WIN32 && !defined UNICODE   // Chicago only

/*
 * convert a UNICODE string to 'normal'
 */
LPTSTR WINAPI aviWideToText (LPTSTR lpszOut, LPWSTR lpwIn, UINT cch)
{
   if (sizeof(TCHAR) != sizeof(WCHAR))
      WideCharToMultiByte(CP_ACP, 0, lpwIn, cch, lpszOut, cch, NULL, NULL);
   else
      lstrcpyn (lpszOut, (LPTSTR)lpwIn, cch);
   return lpszOut;
}
#else
 #define aviWideToText(lpszOut,lpwIn,cch) lstrcpyn(lpszOut,lpwIn,cch)
#endif // WIN32 on CHICAGO

/////////////////////////////////////////////////////////////////////////////
// CBatchDoc

IMPLEMENT_DYNCREATE(CBatchDoc, CDocument)

BEGIN_MESSAGE_MAP(CBatchDoc, CDocument)
	//{{AFX_MSG_MAP(CBatchDoc)
	ON_COMMAND(ID_OPTIONS_COMPRESS, OnOptionsCompress)
	ON_COMMAND(ID_OPTIONS_VIDEO, OnOptionsVideo)
	ON_COMMAND(ID_OPTIONS_AUDIO, OnOptionsAudio)
	ON_COMMAND(ID_OPTIONS_TARGETDIRECTORY, OnOptionsTargetDir)
	ON_COMMAND(ID_OPTIONS_ADDFILE, OnOptionsAddfile)
	ON_COMMAND(ID_OPTIONS_INTERLEAVE, OnOptionsInterleave)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_COMPRESS, OnUpdateCompress)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDFILE, OnUpdateAddfile)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_AUDIO, OnUpdateAudio)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_INTERLEAVE, OnUpdateInterleave)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_TARGETDIRECTORY, OnUpdateTargetDir)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_VIDEO, OnUpdateVideo)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETESELECTEDFILES, OnUpdateDeleteSel)
	ON_COMMAND(ID_OPTIONS_DELETESELECTEDFILES, OnOptionsDeleteSel)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchDoc construction/destruction

CBatchDoc::CBatchDoc()
{
	// put one-time construction code here

	_fmemset(&AVIVideoOptions, 0, sizeof(AVIVideoOptions));
	_fmemset(&AVIAudioOptions, 0, sizeof(AVIAudioOptions));
	// Default to tightly interleaved
	AVIAudioOptions.dwFlags |= AVICOMPRESSF_INTERLEAVE;
	AVIAudioOptions.dwInterleaveEvery = 1;
	fCompressing = fStopPlease = FALSE;
	
}

CBatchDoc::~CBatchDoc()
{
	LPAVICOMPRESSOPTIONS pOpt;

	// Free our Compression Options stuff
	pOpt = &AVIVideoOptions;
	AVISaveOptionsFree(1, &pOpt);
	pOpt = &AVIAudioOptions;
	AVISaveOptionsFree(1, &pOpt);
}

BOOL CBatchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
			
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBatchDoc serialization

#define COOKIE TEXT("SATY")

//
// The Load and Save code
//
// Each document can save its current settings as a .BCF file (Batch Compression
// File).  This file looks like this:
// - 4 CHAR ID
// - COMPRESSIONOPTIONS struct for all video streams
// - extra bytes for video compressor specific data if cbParms > 0
// - COMPRESSIONOPTIONS struct for all audio streams
// - extra bytes for audio compressor specific data if cbFormat > 0
// - the name of the target directory as a CString
// - the list of all files in the document as a CStringList
//
void CBatchDoc::Serialize(CArchive& ar)
{
	CString foo;
	TCHAR szCookie[10];

	if (ar.IsStoring())
	{
		// Storing code

		ar.Write(COOKIE, lstrlen(COOKIE) + 1);
		ar.Write(&AVIVideoOptions, sizeof(AVIVideoOptions));
		if (AVIVideoOptions.cbParms > 0)
			ar.Write(AVIVideoOptions.lpParms, AVIVideoOptions.cbParms);
		ar.Write(&AVIAudioOptions, sizeof(AVIAudioOptions));
		if (AVIAudioOptions.cbFormat > 0)
			ar.Write(AVIAudioOptions.lpFormat, AVIAudioOptions.cbFormat);
		ar << TargetDir;
		FileList.Serialize(ar);
		
	// LOADING code.
	} else {
		LPAVICOMPRESSOPTIONS pOpt;

		// First verify it's a valid file
		ar.Read(szCookie, lstrlen(COOKIE) + 1);
		szCookie[lstrlen(COOKIE)] = TEXT('\0');
		if (lstrcmp(szCookie, COOKIE)) {
			MessageBox(AfxGetMainWnd()->m_hWnd,
				TEXT("Invalid Batch Compressor File"),
				TEXT("File Open Error"),
				MB_ICONEXCLAMATION | MB_OK);
			return;
	   	}

		// Next free whatever is there already
		pOpt = &AVIVideoOptions;
		AVISaveOptionsFree(1, &pOpt);
		pOpt = &AVIAudioOptions;
		AVISaveOptionsFree(1, &pOpt);

		ar.Read(&AVIVideoOptions, sizeof(AVIVideoOptions));
		if (AVIVideoOptions.cbParms > 0) {
			AVIVideoOptions.lpParms = GlobalAllocPtr(GPTR, AVIVideoOptions.cbParms);
			ar.Read(AVIVideoOptions.lpParms, AVIVideoOptions.cbParms);
		}
	   	ar.Read(&AVIAudioOptions, sizeof(AVIAudioOptions));
		if (AVIAudioOptions.cbFormat > 0) {
			AVIAudioOptions.lpFormat = GlobalAllocPtr(GPTR, AVIAudioOptions.cbFormat);
			ar.Read(AVIAudioOptions.lpFormat, AVIAudioOptions.cbFormat);
		}
		ar >> TargetDir;
		FileList.Serialize(ar);
		// FixListBox(); // hasn't been created yet.  It'll happen.
	}	
}

//
// Something in the window changed - redraw it
//
void CBatchDoc::FixListBox()
{
	TCHAR szText[MAX_PATH];
	CString String;
	POSITION pos;
	int iPos = 0;
	int z;
	
	// Don't flicker like hell!
	ListBox.SetRedraw(FALSE);
	
	// Empty our listbox and start over
	// !!! If we felt like doing more work, we could be smarter than this.
	// This is going to flicker like hell anyway.
	ListBox.ResetContent();

	// No Video Options have been set yet
	if ((AVIVideoOptions.dwFlags & AVICOMPRESSF_VALID) == 0) {
		ListBox.AddString(TEXT("Video Compression:  Uninitialized"));
		ListBox.AddString(TEXT(""));
		iPos += 2;

	// Johnny, tell us about the Video Options!
	} else {
		HIC hic;					   
		ICINFO icinfo;
		TCHAR szDesc[120] = TEXT("ERROR\0");
		
		// First print "Video Compression:  Compact Video by Cinepak"
		hic = ICOpen(AVIVideoOptions.fccType, 
			AVIVideoOptions.fccHandler, ICMODE_DECOMPRESS);
		if (hic && AVIVideoOptions.fccHandler) {
			ICGetInfo(hic, &icinfo, sizeof(icinfo));
			aviWideToText(szDesc, icinfo.szDescription,
				sizeof(szDesc)/sizeof(szDesc[0]));
		} else if (AVIVideoOptions.fccHandler == 0)
			lstrcpy(szDesc, TEXT("No Recompression"));
		else if	(AVIVideoOptions.fccHandler == comptypeDIB)
			lstrcpy(szDesc, TEXT("Full Frames"));
		wsprintf(szText, TEXT("Video Compression:  %s"), szDesc);
		ListBox.AddString(szText);
		iPos++;

		// Now print "Data Rate: 300 K/Sec    Quality:  7500"
		if (hic && AVIVideoOptions.fccHandler) {
			if (AVIVideoOptions.dwFlags & AVICOMPRESSF_DATARATE)
				wsprintf(szDesc, TEXT("     Data Rate:  %d K/Sec    Quality:  %d"),
					(INT)(AVIVideoOptions.dwBytesPerSecond / 1024),
					(INT)(AVIVideoOptions.dwQuality));
			else
				wsprintf(szDesc, TEXT("     No Data Rate   Quality:  %d"),
					(INT)(AVIVideoOptions.dwQuality));
			ListBox.AddString(szDesc);
			iPos++;
		}

		// Now print "Key frames every 5 frames" or "No Key frames"
		if (hic && AVIVideoOptions.fccHandler) {
			if (AVIVideoOptions.dwFlags & AVICOMPRESSF_KEYFRAMES)
				wsprintf(szDesc, TEXT("     Key frames every %d frames"),
					AVIVideoOptions.dwKeyFrameEvery);
			else
				lstrcpy(szDesc, TEXT("     No Key frames"));
			ListBox.AddString(szDesc);
			iPos++;
		}
		ListBox.AddString(TEXT(""));
		iPos++;
	}
	
	// There are no Audio Compression options set 
	if ((AVIAudioOptions.dwFlags & AVICOMPRESSF_VALID) == 0) {
		ListBox.AddString(TEXT("Audio Compression:  Uninitialized"));
		ListBox.AddString(TEXT(""));
		iPos += 2;

	// Johnny, tell us what they've won!
	} else {
		ACMFORMATDETAILS acmfmt;
		ACMFORMATTAGDETAILS aftd;
		TCHAR szDesc[120] = TEXT("ERROR\0");
	

		_fmemset(&acmfmt, 0, sizeof(acmfmt));
		acmfmt.cbStruct = sizeof(acmfmt);
		acmfmt.pwfx = (LPWAVEFORMATEX)AVIAudioOptions.lpFormat;
		acmfmt.cbwfx = AVIAudioOptions.cbFormat;
        	acmfmt.dwFormatTag = acmfmt.pwfx->wFormatTag;
        	aftd.cbStruct = sizeof(aftd);
        	aftd.dwFormatTag = acmfmt.pwfx->wFormatTag;
        	aftd.fdwSupport = 0;

        	if ((acmFormatTagDetails(NULL,&aftd, 
        			ACM_FORMATTAGDETAILSF_FORMATTAG) == 0) &&
            			(acmFormatDetails(NULL, &acmfmt, 
            			ACM_FORMATDETAILSF_FORMAT) == 0)) {
        		wsprintf(szDesc, TEXT("%s %s"),
        		(LPTSTR)acmfmt.szFormat, (LPTSTR)aftd.szFormatTag);
		}
		wsprintf(szText, TEXT("Audio Compression:  %s"), szDesc);
		ListBox.AddString(szText);
		ListBox.AddString(TEXT(""));
		iPos += 2;
	}
	
	// Tell us about the interleaving
	if (AVIAudioOptions.dwFlags & AVICOMPRESSF_INTERLEAVE) {
		wsprintf(szText, TEXT("Interleave every %d frames"),
			(int)AVIAudioOptions.dwInterleaveEvery);
		ListBox.AddString(szText);
		ListBox.AddString(TEXT(""));
		iPos += 2;
	} else {
		ListBox.AddString(TEXT("Not interleaved"));
		ListBox.AddString(TEXT(""));
		iPos += 2;
	}

	// Print the Target Directory
	String = TEXT("Target Directory:  ");
	if (TargetDir == "")
		wsprintf(szText, TEXT("Target Directory:  No target directory set"));
	else
		wsprintf(szText, TEXT("Target Directory:  %s"), TargetDir);
	ListBox.AddString(szText);
	ListBox.AddString(TEXT(""));
	iPos += 2;

	// List each file.
	pos = FileList.GetHeadPosition();
	if (FileList.IsEmpty()) {
		ListBox.AddString(TEXT("     No Files Added"));
	}
	iFirstFile = iPos;	// Remember where file list starts in list box
	for (z=1; z<=FileList.GetCount(); z++) {
		String = FileList.GetNext(pos);
		wsprintf(szText, TEXT("     %s"), String.GetBuffer(0));
		ListBox.AddString(szText);
	}
	 	
	ListBox.SetRedraw(TRUE);
	ListBox.Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CBatchDoc diagnostics

#ifdef _DEBUG
void CBatchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBatchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBatchDoc commands

//
// You can't choose video options while compressing
//
void CBatchDoc::OnUpdateVideo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);
	
}

//
// Set the Video Options - all video streams will use these options
//
void CBatchDoc::OnOptionsVideo() 
{
	COMPVARS CompVars;
	BOOL f;

	_fmemset(&CompVars, 0, sizeof(CompVars));
	CompVars.cbSize = sizeof(CompVars);
	if (AVIVideoOptions.dwFlags & AVICOMPRESSF_VALID) {
    		CompVars.dwFlags = ICMF_COMPVARS_VALID;
    		CompVars.fccHandler = AVIVideoOptions.fccHandler;
    		CompVars.lQ = AVIVideoOptions.dwQuality;
    		CompVars.lpState = AVIVideoOptions.lpParms;
    		CompVars.cbState = AVIVideoOptions.cbParms;
    		CompVars.lKey =
    			(AVIVideoOptions.dwFlags & AVICOMPRESSF_KEYFRAMES)?
    			(AVIVideoOptions.dwKeyFrameEvery) : 0;
    		CompVars.lDataRate =
    			(AVIVideoOptions.dwFlags & AVICOMPRESSF_DATARATE) ?
    			(AVIVideoOptions.dwBytesPerSecond / 1024) : 0;
	}

	// !!! It would be nice if we had a parent!
	f = ICCompressorChoose(AfxGetMainWnd()->m_hWnd, 
		ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE,
		NULL, NULL, &CompVars, NULL);

	/* Set the options to our new values */
	if (f) {
    		AVIVideoOptions.lpParms = CompVars.lpState;
    		AVIVideoOptions.cbParms = CompVars.cbState;
    		CompVars.lpState = NULL;       // so it won't be freed
    		AVIVideoOptions.fccType = CompVars.fccType;
    		AVIVideoOptions.fccHandler = CompVars.fccHandler;
    		AVIVideoOptions.dwQuality = CompVars.lQ;
    		AVIVideoOptions.dwKeyFrameEvery = CompVars.lKey;
    		AVIVideoOptions.dwBytesPerSecond = CompVars.lDataRate * 1024;
    		if (CompVars.lKey)
        		AVIVideoOptions.dwFlags |= AVICOMPRESSF_KEYFRAMES;
    		else
        		AVIVideoOptions.dwFlags &=~AVICOMPRESSF_KEYFRAMES;
    		if (CompVars.lDataRate)
        		AVIVideoOptions.dwFlags |= AVICOMPRESSF_DATARATE;
    		else
        		AVIVideoOptions.dwFlags &=~AVICOMPRESSF_DATARATE;
        	AVIVideoOptions.dwFlags |= AVICOMPRESSF_VALID;
	}
	ICCompressorFree(&CompVars);
	FixListBox();
	UpdateAllViews(NULL);
}

//
// You can't choose audio options while compressing
//
void CBatchDoc::OnUpdateAudio(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);
}

//
// Set the Audio Options - all audio streams will use these options
//
void CBatchDoc::OnOptionsAudio() 
{
	ACMFORMATCHOOSE acf;
	LONG l;
	MMRESULT f;

	AVIAudioOptions.fccType = streamtypeAUDIO;
	_fmemset(&acf, 0, sizeof(acf));
	acf.cbStruct = sizeof(acf);
	// Default to the current settings (if any)
	acf.fdwStyle = (AVIAudioOptions.dwFlags & AVICOMPRESSF_VALID) ?
		ACMFORMATCHOOSE_STYLEF_INITTOWFXSTRUCT : 0;
	acf.hwndOwner = AfxGetMainWnd()->m_hWnd;
	acmMetrics(NULL, ACM_METRIC_MAX_SIZE_FORMAT, (LPVOID)&l);
	if (AVIAudioOptions.cbFormat == 0) {
		AVIAudioOptions.lpFormat = GlobalAllocPtr(GPTR, l);
		AVIAudioOptions.cbFormat = l;
	}
	acf.pwfx = (LPWAVEFORMATEX)AVIAudioOptions.lpFormat;
	acf.cbwfx = AVIAudioOptions.cbFormat;
	if (acf.pwfx == NULL)	// error: Can't do this
		return;
	f = acmFormatChoose(&acf);
	if (f == MMSYSERR_NOERROR)
		AVIAudioOptions.dwFlags |= AVICOMPRESSF_VALID;

	FixListBox();
	UpdateAllViews(NULL);
}

//
// Can't choose a target directory while compressing
//
void CBatchDoc::OnUpdateTargetDir(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);	
}

//
// Choose a target directory for these compressed files
//
void CBatchDoc::OnOptionsTargetDir()
{
	CDestDirDlg DirDlg(AfxGetMainWnd());
	DirDlg.m_DestDir = TargetDir;
	if (DirDlg.DoModal() == IDOK) {
		TargetDir = DirDlg.m_DestDir;
		if (TargetDir.GetAt(TargetDir.GetLength() - 1) != TEXT('\\'))
			TargetDir += TEXT("\\"); 
		FixListBox();	 
	}
}

//
// Can't add files while compressing
//
void CBatchDoc::OnUpdateAddfile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);	
}

//
// Add a file to the list
//
void CBatchDoc::OnOptionsAddfile() 
{
	CFileDialog FileDlg(TRUE, NULL, NULL,
		 OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		 TEXT("AVI Files|*.avi|All Files|*.*||"),
		 AfxGetMainWnd());
	CString fileName;

	if (FileDlg.DoModal() == IDOK) {
		fileName = FileDlg.GetPathName();
		FileList.AddTail(fileName);
		FixListBox();
	}	
}				  

//
// Can't change the interleaving while compressing
//
void CBatchDoc::OnUpdateInterleave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (!fCompressing);	
}

//
// Change the interleaving
//
void CBatchDoc::OnOptionsInterleave() 
{
	CInterleaveDlg InterleaveDlg(AfxGetMainWnd());
	InterleaveDlg.m_InterleaveEvery = AVIAudioOptions.dwInterleaveEvery;
	if (InterleaveDlg.DoModal() == IDOK) {
		AVIAudioOptions.dwFlags |= AVICOMPRESSF_INTERLEAVE;
		AVIAudioOptions.dwInterleaveEvery = InterleaveDlg.m_InterleaveEvery;
		FixListBox();
	}
}

#define SLASH(c)     ((c) == '/' || (c) == '\\')

/**************************************************************************
 FileName  - return a pointer to the filename part of szPath
             with no preceding path.
***************************************************************************/
LPTSTR FileName(LPCTSTR lszPath)
{
    LPCTSTR   lszCur;

    for (lszCur = lszPath + lstrlen(lszPath); lszCur > lszPath && !SLASH(*lszCur
) && *lszCur != ':';)
        lszCur = AnsiPrev(lszPath, lszCur);
    if (lszCur == lszPath)
        return (LPTSTR)lszCur;
    else
        return (LPTSTR)(lszCur + 1);
}

/****************************************************************************
 CreateDestFile - Take the path part of szPath, append the
             filename part of szName, and return it as szResult.
****************************************************************************/
static void CreateDestFile(LPTSTR szResult, LPCTSTR szPath, LPCTSTR szName)
{
#if 0
    lstrcpy(szResult, szPath);
    lstrcpy(FileName(szResult), FileName(szName));
#else
    lstrcpy(szResult, szPath);
    lstrcpy(FileName(szResult), FileName(szName));
#endif
}								  

/**************************************************************************
 FixName  -  Given a filespec followed by a space and other text,
             copy just the filespec over to the new string.
***************************************************************************/
static void FixName(LPTSTR lszFixed, LPCTSTR lszName)
{
	while (*lszName	!= ' ' && *lszName != '\0')
		*lszFixed++ = *lszName++;
	*lszFixed = '\0';
}

//
// AVISaveV callback where we print our progress as a percentage after the
// filename.
//
HRESULT SaveCallback(int iProgress)
{
	CBatchDoc *lpDoc;
	CString File;
	TCHAR szFile[MAX_PATH + 20];
	TCHAR szText[20];
	int iPos;
	POSITION pos;
	
	// Use ThreadLocalStorage to get the pDoc of this thread so we know
	// which window we are currently in.
	lpDoc = (CBatchDoc *)TlsGetValue(gdwTlsCookie);
	iPos = lpDoc->iCurFile;
	pos = lpDoc->FileList.FindIndex(iPos);
	
	// We've been asked to stop compressing
	if (lpDoc->fStopPlease) {
		lpDoc->fStopPlease = FALSE;
		return AVIERR_USERABORT;	// tells AVISaveV to fail	
	}

	// We're further along than last time.  Show our progress by 
	// changing "ski.avi" to "ski.avi <15%>"
	if (iProgress > lpDoc->iPercent) {
		File = lpDoc->FileList.GetAt(pos);
		FixName(szFile, File.GetBuffer(0));
		wsprintf(szText, TEXT("  <%d%%>"), iProgress);
		lstrcat(szFile, szText);
		File = szFile;
		lpDoc->FileList.SetAt(pos, File);
		// Add indent
		lstrcpy(szFile, TEXT("     "));
		lstrcat(szFile, File.GetBuffer(0));
		// Try to be clever... don't update the whole list box.
		// !!! Nice try but no cigar
		lpDoc->ListBox.SetRedraw(FALSE);
		lpDoc->ListBox.DeleteString(lpDoc->iFirstFile + iPos);
		lpDoc->ListBox.InsertString(lpDoc->iFirstFile + iPos, szFile);
		lpDoc->ListBox.SetRedraw(TRUE);
		lpDoc->ListBox.Invalidate(FALSE);
	}
	lpDoc->iPercent = iProgress;

	return NOERROR;
}

#define MAXSTREAMS 25
	
//
// Compress the file at a given position in this window's list.
//
HRESULT CBatchDoc::CompressFile(int iPos)
{
	POSITION pos = FileList.FindIndex(iPos);
	PAVIFILE pfile;
	PAVISTREAM pavi[MAXSTREAMS];
	LPAVICOMPRESSOPTIONS pOpt[MAXSTREAMS];
	int y;
	AVISTREAMINFOW avis;
	HRESULT hr;
	TCHAR szDest[MAX_PATH];
	TCHAR szFile[MAX_PATH];
	CString File = FileList.GetAt(pos);
	
	FixName(szFile, File.GetBuffer(0));
	hr =AVIFileOpen(&pfile, szFile, 0, 0);
	if (hr == AVIERR_OK && pfile != NULL) {
		
		// Get all the streams in this file and a compression options
		// for each
		for (y = 0; y < MAXSTREAMS; y++) {
			hr = pfile->GetStream(&pavi[y], 0, y);
			if (hr != AVIERR_OK)
				break;
			pavi[y]->Info(&avis, sizeof(avis));
			if (avis.fccType == streamtypeVIDEO)
				pOpt[y] = &AVIVideoOptions;
			else if (avis.fccType == streamtypeAUDIO)
				pOpt[y] = &AVIAudioOptions;
			else
				; // ???
		}		
		
		// Create the new filename, and save it
		CreateDestFile(szDest, TargetDir, szFile);
		iPercent = -1;
		iCurFile = iPos; // So Callback knows which file is compressing
		TlsSetValue(gdwTlsCookie, (LPVOID)this);
		hr = AVISaveV(szDest, NULL, (AVISAVECALLBACK)&SaveCallback,
			y, pavi, pOpt);
		
		// Release the streams and file
		for (--y; y>=0; y--) {
			pavi[y]->Release();
		}
		pfile->Release();
	}
	return hr;
}		

// A thread is created for each document window.  This function compresses all
// files in the window in series.  Different windows will compress in parallel.
UINT CompressThread(void *pParams)
{
 	CBatchDoc *pDoc = (CBatchDoc *) pParams;

	HRESULT hr;
	int z, iPos;
	POSITION pos, posT;
	CString String;
	TCHAR szFixed[MAX_PATH];

	pDoc->fStopPlease = FALSE;	// just in case it's stuck
	pDoc->fCompressing = TRUE;

	AVIFileInit();

	// Walk through all the filenames we have to compress
	pos = pDoc->FileList.GetHeadPosition();
	iPos = 0;	// index of pos
	for (z=1; z<=pDoc->FileList.GetCount(); z++) {
		posT = pos;
		String = pDoc->FileList.GetNext(pos);
		iPos++;
		FixName(szFixed, String.GetBuffer(0));
		
		// Do the dirty work
		hr = pDoc->CompressFile(iPos - 1);

		// Mark this entry as either <COMPRESSED> or <ERROR> or <ABORTED>
		String = szFixed;
		if (hr == AVIERR_OK)
			String += TEXT("  <COMPRESSED>");
		else if (hr == AVIERR_USERABORT)
			String += TEXT("  <ABORTED>");	
		else 
			String += TEXT("  <ERROR!>");	
		pDoc->FileList.SetAt(posT, String);
		pDoc->FixListBox();
	}

	AVIFileExit();

	pDoc->fCompressing = FALSE;

	return 0;
}

#define THREADED	// Use threads.  Otherwise, it will compress everything
			// in series.

//
// Can't compress if we're already compressing
//
void CBatchDoc::OnUpdateCompress(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);
}

//
// Compress every file in this document
//
void CBatchDoc::CompressList() 
{
	if (FileList.IsEmpty() || fCompressing)
		return;
	
#ifdef THREADED
	AfxBeginThread(CompressThread, (void *) this);
#else
	CompressThread(this);
#endif
}				   

//
// Compress every file in this document
//
void CBatchDoc::OnOptionsCompress() 
{
	CompressList();
}				   

//
// Have we selected something we could delete?
//
void CBatchDoc::OnUpdateDeleteSel(CCmdUI* pCmdUI) 
{
	BOOL f;
	int aSel[1];

	// A file is selected if there is a selection and it's in the area of
	// file names, and it's a non-empty list.
	f = (ListBox.GetSelCount() && ListBox.GetSelItems(1, aSel)
		&& aSel[0] >= iFirstFile && !FileList.IsEmpty());
	pCmdUI->Enable(f);	
}

#define MAXSEL 256	// largest selection we can handle

//
// Delete all of the selected filenames from the list box
//
void CBatchDoc::DeleteSelection() 
{
	int cSel, i, j, k = -1;
	int aSel[MAXSEL];
	POSITION pos;
		
	cSel = min(ListBox.GetSelCount(), MAXSEL);
	if (cSel && !FileList.IsEmpty()) {
		ListBox.GetSelItems(cSel, aSel);
		for (i = 0; i < cSel; i++) {
			// The first bunch of items in the list box aren't files 
			j = aSel[i] - iFirstFile;
			if (j >= 0) {
				if (k == -1)
					k = i;
				pos = FileList.FindIndex(j - i + k);
				FileList.RemoveAt(pos);
			}
		}
	}
	if (k >= 0)
		FixListBox();
}

//
// Delete all of the selected filenames from the list box
//
void CBatchDoc::OnOptionsDeleteSel() 
{
	DeleteSelection();
}

void CBatchDoc::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);
}

void CBatchDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);
}

void CBatchDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!fCompressing);
}
