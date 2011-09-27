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
// batchdoc.h : interface of the CBatchDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CBatchDoc : public CDocument
{
protected: // create from serialization only
	CBatchDoc();
	DECLARE_DYNCREATE(CBatchDoc)

// Attributes
public:
	AVICOMPRESSOPTIONS	AVIVideoOptions;
	AVICOMPRESSOPTIONS	AVIAudioOptions;
	CStringList FileList;
	CString TargetDir;
	CListBox ListBox;
	int iCurFile;	// which file in the list is compressing?
	int iFirstFile;	// where in list box the file list starts
	int iPercent;	// how far compressed a file is
	BOOL fCompressing;	// are we compressing?
	BOOL fStopPlease;	// please stop compressing
	
// Operations
public:
	void FixListBox();	// The text to print has changed
	void DeleteSelection();			// delete all selected files
	void CompressList();				// Compress all files in this DOC
	HRESULT CompressFile(int ipos);	// compress a file
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBatchDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBatchDoc)
	afx_msg void OnOptionsCompress();
	afx_msg void OnOptionsVideo();
	afx_msg void OnOptionsAudio();
	afx_msg void OnOptionsTargetDir();
	afx_msg void OnOptionsAddfile();
	afx_msg void OnOptionsInterleave();
	afx_msg void OnUpdateCompress(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAudio(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInterleave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTargetDir(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVideo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteSel(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDeleteSel();
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAppExit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
