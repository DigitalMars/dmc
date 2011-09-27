// tmlimage.cpp
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLImage member functions
// Notes:
//   An array of objects of this type is stored in the document, one
//    to represent each image found in the TML source file.
//   Uses DIB API from MFC 2.5 example program 'diblook' to read
//     image files (see dibapi2.c).

#include "stdafx.h"
#include "dibapi.h"

#include "tmlimage.h"

////
//// Constructor
////  Initializes class data
////
CTMLImage::CTMLImage () :
        strFilename(""),
        sizImageSize(50,50),
        pPalette(NULL),
        hDIB(NULL)
{
}

////
//// Destructor
////  Removes image data from memory
////
CTMLImage::~CTMLImage ()
{
  UnRead();
}

////
//// Read
////  Reads image from disk into memory
////  Arguments:
////    path is the drive and directory where the file is located
////  Returns:
////    TRUE if error, FALSE if okay
////
BOOL
CTMLImage::Read (const char *path = "")
{
  // construct temporary buffer for combined path and filename
  char *pathname = new char[strlen(path)+strFilename.GetLength()+1];
  strcpy (pathname, path);
  strcat (pathname, strFilename);

  // initialize return value
  BOOL retval = FALSE;

  // open and read file
  CFile file;
  CFileException fe;
  if (!file.Open(pathname, CFile::modeRead | CFile::shareDenyWrite, &fe)) {
    retval = TRUE;
  }
  else { // read file using DIB API function
    TRY
    {
      hDIB = ::ReadDIBFile(file);
    }
    CATCH (CFileException, eLoad)
    {
      file.Abort();
      retval = TRUE;
    }
    END_CATCH
  }

  if (retval)     // failure - store NULL pointer
    hDIB = NULL;
  else            // success - initialize
    InitDIBData();

  // cleanup and return
  delete pathname;
  return retval;
}

////
//// UnRead
////  Removes image from memory, deletes palette
////
void
CTMLImage::UnRead()
{
	if (hDIB)	{
		::GlobalFree((HGLOBAL) hDIB);
		hDIB = NULL;
	}
	if (pPalette)	{
		delete pPalette;
		pPalette = NULL;
	}
	sizImageSize = CSize(50,50);
}

////
//// Draw
////  Calls DIB API to draw image
////  Arguments:
////    pDC is device context for drawing
////    destrect is destination rectangle in pDC
////
void
CTMLImage::Draw(CDC *pDC, CRect destrect)
{
  if (hDIB) {
    CRect srcrect(CPoint(0,0),sizImageSize);
    ::PaintDIB(pDC->m_hDC, &destrect, hDIB, &srcrect, pPalette);
  }
  // if there is no bitmap in memory, draws a blank rectangle
  else
    pDC->Rectangle(destrect);
}

////
//// InitDIBData
////  Set image size and palette
////
void
CTMLImage::InitDIBData()
{
  if (hDIB == NULL)
    return;

  // set image size
  LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
  sizImageSize = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
  ::GlobalUnlock((HGLOBAL) hDIB);

  // create palette object
  if ((pPalette = new CPalette) == NULL) {
    ::GlobalFree((HGLOBAL) hDIB);     // low memory
    hDIB = NULL;
    return;
  }
  // create palette
  if (::CreateDIBPalette(hDIB, pPalette) == NULL) {
    delete pPalette;        // image may have no palette
    pPalette = NULL;
    return;
  }
}

