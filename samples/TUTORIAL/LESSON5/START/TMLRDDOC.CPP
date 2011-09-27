// tmlrddoc.cpp
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLReadDoc message map and member functions
//

#include "stdafx.h"
#include "dibapi.h"
#include <dir.h>  // for splitting document file path name

#include "TMLRead.h"
#include "tmlword.h"
#include "tmllink.h"
#include "tmlimage.h"
#include "tmlparse.h"
#include "ptrstack.h"
#include "tmlrddoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

////
////  Message Map
////

IMPLEMENT_DYNCREATE(CTMLReadDoc, CDocument)

BEGIN_MESSAGE_MAP(CTMLReadDoc, CDocument)
  //{{AFX_MSG_MAP(CTMLReadDoc)
    // NOTE - the ClassExpress will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////
//// Constructor
////  Sets up data
////
CTMLReadDoc::CTMLReadDoc() :
           stackFileStack(100), wLastTopWord(0), wStartTopWord(0)
{
  TRACE ("CTMLReadDoc Constructor\n");

  apWordArray.SetSize(0,8000); // help arrays to grow by reasonable amounts
  apLinkArray.SetSize(0,100);
  apAnchorArray.SetSize(0,100);
  apImageArray.SetSize(0,20);
}

////
//// Destructor
////  Delete document contents and jumpback stack
////
CTMLReadDoc::~CTMLReadDoc()
{
  TRACE ("CTMLReadDoc Destructor\n");

  DeleteContents();

  // pop everything off stack and delete
  JumpBackPoint *jbp;
  while ((jbp = (JumpBackPoint *)stackFileStack.Pop()) != NULL)
    delete jbp;
}

////
//// OnNewDocument
////  Set up new document
////  Returns:
////    base class return value
////  In TMLRead, this is only called on startup, if there is no
////  command line argument
////
BOOL
CTMLReadDoc::OnNewDocument()
{
  TRACE ("CTMLReadDoc::OnNewDocument()\n");

  return CDocument::OnNewDocument();
}

////
//// OnOpenDocument
////  Read document, saves old doc in stack
////  Arguments:
////    pathname is the document path and filename
////  Returns:
////    TRUE if document opened
////
BOOL
CTMLReadDoc::OnOpenDocument(const char* pathname)
{
  TRACE ("CTMLReadDoc::OnOpenDocument()\n");

  if (!CDocument::OnOpenDocument(pathname)) // call base class; calls Serialize
    return FALSE;

  // push present document onto stack
  //  this can only be done here because the new document pathname has not
  //  been set up yet by the framework
  if (GetPathName().GetLength()) {
    JumpBackPoint *jbp = new JumpBackPoint; // make new jumpback object
    jbp->docpath = GetPathName();           // set it up
    jbp->iword = wLastTopWord;
    stackFileStack.Push((void *)jbp);       // push onto stack
  }

  SplitPathName(pathname);                  // divide out drive/dir
  ReadImages();                             // read images into memory

  return TRUE;
}

////
//// Serialize
////  Read data from TML file
////  Arguments:
////    ar is a CArchive from which the file can be read
////  This routine creates a parser object to do the work.
////
void
CTMLReadDoc::Serialize(CArchive& ar)
{
  TRACE ("CTMLReadDoc::Serialize()\n");

  if (ar.IsStoring()) {
    // does not occur in TMLRead
  }
  else {
    strDocTitle = ""; // in case there isn't one in the file
    int errorline = 0;
    // create the parser object, passing all the necessary pointers
    CTMLParser parser(ar, strDocTitle, errorline,
                        apWordArray, apLinkArray, apAnchorArray, apImageArray);
    // do the actual parsing
    int result = parser.Parse();
    if (result) { // process error
      char buf[120];
      wsprintf (buf, "Error parsing file: line %d\n%s",
                errorline+1, parser.ErrorMessage(result));
      AfxMessageBox (buf);
      DeleteContents();
      strDocTitle = "";
      strStartAnchor = "";
    }
  }
}

////
//// CTMLReadDoc diagnostics
////
#ifdef _DEBUG
void CTMLReadDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CTMLReadDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

////
//// DeleteContents
////  Delete document data
////  Deallocates all words, links, anchors, and images,
////  and clears the arrays.
////

void
CTMLReadDoc::DeleteContents()
{
  TRACE ("CTMLReadDoc::DeleteContents()\n");

  int size = apWordArray.GetSize();
  for (int i = 0; i < size; i++)
    delete (CTMLWord *) apWordArray[i];
  apWordArray.RemoveAll();

  size = apLinkArray.GetSize();
  for (i = 0; i < size; i++)
    delete (CTMLLink *) apLinkArray[i];
  apLinkArray.RemoveAll();

  size = apAnchorArray.GetSize();
  for (i = 0; i < size; i++)
    delete (CTMLAnchor *) apAnchorArray[i];
  apAnchorArray.RemoveAll();

  size = apImageArray.GetSize();
  for (i = 0; i < size; i++)
    delete (CTMLImage *) apImageArray[i];
  apImageArray.RemoveAll();
}

////
//// HyperLink
////  Jump to another file.
////  Arguments:
////    filename is the file to be jumped to
////    anchor is the anchor name to be jumped to
////  Called by view.  After hyperlink, the view must call on the document
////  to locate the word which should be scrolled to.
////
void
CTMLReadDoc::HyperLink(CString &filename, CString &anchor)
{
  strStartAnchor = anchor;

  if (filename[0] == '\\' || strchr(filename, ':') != NULL) // absolute path
    AfxGetApp()->OpenDocumentFile(filename);
  else   // filename relative to current document
    AfxGetApp()->OpenDocumentFile(strDocDriveDir+filename);
}

////
//// JumpBack
////  Jump back to previous file on stack
//// Called by view.  After jumpback, the view must call on the document
//// to locate the word which should be scrolled to.
////
void
CTMLReadDoc::JumpBack()
{
  JumpBackPoint *jbp = (JumpBackPoint *) stackFileStack.Pop();

  if (jbp) {
    wStartTopWord = jbp->iword;
    AfxGetApp()->OpenDocumentFile(jbp->docpath);
    delete jbp;
  }

  // the file from which the jumpback occurs is automatically
  //   pushed, so we have to pop it off and discard it
  delete (JumpBackPoint *) stackFileStack.Pop();
}

////
//// SplitPathName
////  Split out drive/directory from doc pathname, save in strDocDriveDir
////  Arguments:
////    pathname  full document path and filename
////
void
CTMLReadDoc::SplitPathName(const char *pathname)
{
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char name[MAXFILE];
  char ext[MAXEXT];

  fnsplit (pathname, drive, dir, name, ext);

  strDocDriveDir = drive;
  strDocDriveDir += dir;
}

////
//// ReadImages
////  Call on all images to read themselves from disk
////
void
CTMLReadDoc::ReadImages()
{
  int nimages = apImageArray.GetSize();
  for (int i = 0; i < nimages; i++)
    ((CTMLImage *) apImageArray[i])->Read(strDocDriveDir);
}

////
//// FindAnchor
////  Finds an anchor by name
////  Arguments:
////    aname is the anchor name
////    iword is where word number is returned
////  Returns:
////    TRUE if found, FALSE if not
////  This function searches the list of anchors to locate the
//// named anchor.  The index of the corresponding word in the document
//// is returned.  This is used by the view to find out where to jump to
//// after a hyperlink.
////
BOOL
CTMLReadDoc::FindAnchor(CString &aname, WORD &iword)
{
  BOOL anchorfound = FALSE;
  for (int i = 0; i < NumAnchors(); i++) {
    if (Anchor(i)->Used() && Anchor(i)->Anchor() == aname) { // found the anchor
      iword = Anchor(i)->FirstWord();
      anchorfound = TRUE;
      break;
    }
  }
  return anchorfound;
}
