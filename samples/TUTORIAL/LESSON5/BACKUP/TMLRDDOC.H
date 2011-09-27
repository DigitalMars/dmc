// tmlrddoc.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLReadDoc class declaration
//

////
//// CTMLReadDoc
////   Document class.  Contains parsed data from an TML file.
////  Contents include arrays of words, links, anchors, and images,
////  document title and filename, and a few things to assist in
////  hyperlinking and jumping back to previous files.
////

class CTMLReadDoc : public CDocument
{
  protected: // create from serialization only
    CTMLReadDoc();
    DECLARE_DYNCREATE(CTMLReadDoc)

  public:
    virtual ~CTMLReadDoc();             // destructor
    void DeleteContents();                // delete document data
    virtual void Serialize(CArchive& ar); // document I/O (in TMLRead, just I)

  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif

  protected:
    virtual BOOL  OnNewDocument();                       // new document
    virtual BOOL  OnOpenDocument(const char* pathname);  // open from file

  // Generated message map functions
  protected:
    //{{AFX_MSG(CTMLReadDoc)
      // NOTE - the ClassExpress will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()


  public:
    // Data access
    CString &DocTitle() { return strDocTitle; }
    CString &DocDriveDir() { return strDocDriveDir; }
    WORD NumWords ()   { return apWordArray.GetSize(); }
    WORD NumLinks ()   { return apLinkArray.GetSize(); }
    WORD NumAnchors () { return apAnchorArray.GetSize(); }
    WORD NumImages  () { return apImageArray.GetSize(); }
    CTMLWord   * Word   (WORD i) { return (CTMLWord *)   (apWordArray[i]); }
    CTMLLink   * Link   (WORD i) { return (CTMLLink *)   (apLinkArray[i]); }
    CTMLAnchor * Anchor (WORD i) { return (CTMLAnchor *) (apAnchorArray[i]); }
    CTMLImage  * Image  (WORD i) { return (CTMLImage *)  (apImageArray[i]); }

    // split out drive/directory from pathname
    void SplitPathName(const char *pathname);

    // read images
    void ReadImages ();

    // hyperlink support
    BOOL FindAnchor(CString &, WORD &);    // locate an anchor by name
    void HyperLink(CString &, CString &);   // hyperlink, called by view
    BOOL IsStartAnchor() { return strStartAnchor.GetLength() > 0; }
      // reset during get
    BOOL GetStartAnchor (WORD &x) {BOOL b = FindAnchor(strStartAnchor, x);
                                   strStartAnchor = ""; return b; }

    // jumpback support
    void JumpBack();
    void SetLastTopWord (WORD x) { wLastTopWord = x; }
    WORD GetLastTopWord () { return wLastTopWord; }
      // reset during get
    WORD GetStartTopWord () {WORD x = wStartTopWord; wStartTopWord = 0; return x;}
    BOOL IsPrevFile() { return (stackFileStack.IsEmpty() ? FALSE : TRUE); }

  // document data
  private:
    CString strDocTitle;       // document title from <TITLE> section
    CString strDocDriveDir;    // drive and directory

    CPtrArray apWordArray;     // pointers to CTMLWord
    CPtrArray apLinkArray;     // pointers to CTMLLink
    CPtrArray apAnchorArray;   // pointers to CTMLAnchor
    CPtrArray apImageArray;    // pointers to CTMLImage

    // for hyperlinks
    CString strStartAnchor;    // starting anchor name, when file hyperlinked to

    // for jumping back to previous file
    struct JumpBackPoint {
      CString docpath;    // full pathname of file
      WORD iword;         // word within the file
    };
    CPtrStack stackFileStack;    // stack of JumpBackPoints
    WORD wLastTopWord;           // current top word on screen
    WORD wStartTopWord;          // starting word number, when file jumped back to
};
