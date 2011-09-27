// FtpTrCtl.cpp : implementation file
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

#include "stdafx.h"
#include "FTPTREE.h"
#include "FTPtrCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

CMyTreeCtrl::CMyTreeCtrl()
{
	// m_pFtpConnection holds a pointer to an FTP connection
	// that is maintained by CFTPTREEDlg.
	m_pFtpConnection = NULL;
}

CMyTreeCtrl::~CMyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl message handlers

void CMyTreeCtrl::OnDestroy()
{
	CImageList	*pimagelist;

	pimagelist = GetImageList(TVSIL_NORMAL);
	pimagelist->DeleteImageList();
	delete pimagelist;
}


void CMyTreeCtrl::PopulateTree(CFtpConnection* pConnection, const CString& strDir)
{
	CWaitCursor cursor;	// this will automatically display a wait cursor

	// Make sure tree is empty before (re)populating it
	DeleteAllItems();

	// If CFtpConnection is NULL, display error in tree
	if (!pConnection)
	{

		// change style -- remove TVS_LINESATROOT if it is currently set
		ModifyStyle(TVS_LINESATROOT, 0);

		TV_INSERTSTRUCT tvstruct;

		tvstruct.hParent = NULL;
		tvstruct.hInsertAfter = TVI_FIRST;
		tvstruct.item.iImage = 0;
		tvstruct.item.iSelectedImage = 0;
		tvstruct.item.pszText = _T("An FTP connection has not been established.");
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		InsertItem(&tvstruct);

		m_pFtpConnection = NULL;
	}
	else
	{
		m_pFtpConnection = pConnection;

		CString cstr = _T("/");	// look at the root directory first				

		// insert the the root into the tree struct
		TV_INSERTSTRUCT tvstruct;

		tvstruct.hParent = NULL;
		tvstruct.hInsertAfter = TVI_LAST;
		tvstruct.item.iImage = 1;
		tvstruct.item.iSelectedImage = 1;
		tvstruct.item.pszText = _T("/");
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		HTREEITEM hThisItem = InsertItem(&tvstruct);

		// the root dir needs to think it has children for Expand() to work
		// Expand() will call ExploreDir() to find the root's real children
		tvstruct.hParent = hThisItem;
		InsertItem(&tvstruct);

		Expand(GetRootItem(), TVE_EXPAND);

		// if the root has no real children, make sure TVS_LINESATROOT is not set
		if (!ItemHasChildren(GetRootItem()))
		{
			ModifyStyle(TVS_LINESATROOT, 0);
			return;
		}

		// otherwise, there are children, so set TVS_LINESATROOT
		ModifyStyle(0, TVS_LINESATROOT);

		if (!strDir)
			return;

		// expand as far as was indicated in strDir
		int nIndex;
		CString strTemp;
		CString strSrc = strDir;
		HTREEITEM hTI = GetRootItem();
		while (!strSrc.IsEmpty())
		{
			nIndex = strSrc.Find(_T("/"));
			if (nIndex < 0) 
			{
				cstr = strSrc;
				strSrc.Empty();
			}
			else if (nIndex > 0)
			{ // the "/" trailing the directory name is not included
				cstr = strSrc.Left(nIndex);	
				strTemp = strSrc.Right(strSrc.GetLength() - (nIndex+1));
				strSrc = strTemp;
			}

			if(!cstr.IsEmpty() && (nIndex != 0))
			{

				// cstr now has the name of a file or dir -- look for a child that matches it
				hTI = GetChildItem(hTI);
				while ((hTI != NULL) && (cstr != GetItemText(hTI)))
				{
					hTI = GetNextItem(hTI, TVGN_NEXT);
				} 

				// if you find it, expand the node or select the file
				if ((hTI != NULL) && (cstr == GetItemText(hTI)))
				{
					if (ItemHasChildren(hTI))
						Expand(hTI, TVE_EXPAND);
					else
					{
						SelectItem(hTI);	// focus it here, too...
						TV_ITEM* tv;
						tv->hItem = hTI;
						GetItem(tv);
						tv->state = TVIS_BOLD;
						SetItem(tv);
						strSrc.Empty();	// it's a file, so we're not gonna go any further
					}
				}

				// if it wasn't found, you've gone as far as you can, so quit...
				else strSrc.Empty();
			}
			else // nIndex == 0
			{
				strTemp = strSrc.Right(strSrc.GetLength() - 1);
				strSrc = strTemp;
			}
		} // end expand
	}
}

void CMyTreeCtrl::ExploreDir(const CString& strDir, HTREEITEM hParent)
{
	TV_INSERTSTRUCT tvstruct;
	CString strSearchDir;

	// remove any children of this directory
	HTREEITEM hIT = GetChildItem(hParent);
	HTREEITEM hTemp;

	while (hIT != NULL)
	{
		hTemp = GetNextSiblingItem(hIT);
		DeleteItem(hIT);
		hIT = hTemp;
	}

	// prepare to find all the files in the specified directory
	if (hParent != GetRootItem())
	{
		strSearchDir = strDir + _T("/*");
	}
	else
	{
		strSearchDir = _T("/*");
	}


	CFtpFileFind ftpFind(m_pFtpConnection);

	BOOL bContinue = ftpFind.FindFile(strSearchDir);
	if (!bContinue)
	{
		// the directory is empty; just close up and return.
		ftpFind.Close();
		return;
	}

	BOOL bDir = FALSE;
	HTREEITEM hThisItem;
	CString strFileName;

	while (bContinue) // set up and insert a tvstruct for each item in the directory
	{
		// FindNextFile muxt be called before info can be gleaned from ftpFind
		bContinue = ftpFind.FindNextFile();
		strFileName = ftpFind.GetFileName();

		if (ftpFind.IsDirectory())
		{
			// found a directory.  MUST find out if it has a child, and if so

			bDir = TRUE;

			tvstruct.item.iImage = 1;
			tvstruct.item.iSelectedImage = 1;
		}
		else
		{

			tvstruct.item.iImage = 2;
			tvstruct.item.iSelectedImage = 2;
		}

		tvstruct.hParent = hParent;
		tvstruct.hInsertAfter = TVI_LAST;
		tvstruct.item.pszText = (LPTSTR)(LPCTSTR)strFileName;	// GetFileName() returns a CString
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;

		hThisItem = InsertItem(&tvstruct);

	} 

	ftpFind.Close();

	// If any directories were found, and if they are not empty, a dummy child 
	// must be created so they can be expanded later.  This check is done AFTER 
	// all of strDir has been explored because only one CFtpFileFind object can 
	// be open for any given FTP session.
	if (bDir)	
	{	
		CFtpFileFind* pTempFtpFind = NULL;
		CString strFullSearchPath;
		int nImage, nSelectedImage;

		hThisItem = GetChildItem(hParent);

		while (hThisItem != NULL)
		{
			GetItemImage(hThisItem, nImage, nSelectedImage);
			if (nImage == 1) // this item is a directory
			{
				strFileName = GetItemText(hThisItem);
				// build the full path to the current directory
				strFullSearchPath = strDir + strFileName + _T("/*");	
				pTempFtpFind = new CFtpFileFind(m_pFtpConnection);
				if (pTempFtpFind->FindFile(strFullSearchPath))
				{
					tvstruct.hParent = hThisItem;
					tvstruct.hInsertAfter = TVI_LAST;
					tvstruct.item.iImage = 1;
					tvstruct.item.iSelectedImage = 1;
					tvstruct.item.pszText = _T("1");;
					tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
					InsertItem(&tvstruct);
				}
				pTempFtpFind->Close();
				delete pTempFtpFind;	
			}
			hThisItem = GetNextSiblingItem(hThisItem);
		}
	}
	
	return;

}

void CMyTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	// find out what item is getting expanded, and send that to Expand(hItem, TVE_EXPAND)
	if (pNMTreeView->hdr.code == TVN_ITEMEXPANDING)
	{
		HTREEITEM hIT = pNMTreeView->itemNew.hItem;
		CString cstr, strPath;

		// build up the path to htreeitem
		strPath = GetItemText(hIT);

		while (hIT != GetRootItem())
		{
			hIT = GetParentItem(hIT);

			if (hIT == GetRootItem())
				cstr.Format(_T("/%s"), (LPCTSTR)strPath);
			else
				cstr.Format(_T("%s/%s"), (LPCTSTR)GetItemText(hIT), (LPCTSTR)strPath);

			strPath = cstr;
		}

		// use that dir to call ExploreDir
		ExploreDir(strPath, pNMTreeView->itemNew.hItem);
	}
	*pResult = 0;
}
