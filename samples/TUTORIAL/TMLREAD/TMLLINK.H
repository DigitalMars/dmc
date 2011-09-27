// tmllink.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLLink class declaration
//   CTMLAnchor class declaration
// Notes:
//   Arrays of both types of object are kept as part of the document's
//     data.  CTMLLink objects are used to decide where to jump when a
//     word is clicked on, and CTMLAnchor objects give the association
//     between CTMLWord objects in the document and anchor names.
//     

#ifndef _TMLLINK_H
#define _TMLLINK_H

//// CTMLLink
////  Contains hypertext link information: the indices of words which
////  cause a jump, and the jump destination.
////  An array of these objects is created by the parser and stored in
////  the document.

class CTMLLink {
  public:
    CTMLLink() { wFirstWord = wLastWord = 0; bUsed = FALSE; }
    ~CTMLLink() {}

  // data access
  WORD FirstWord() { return wFirstWord; }
  WORD LastWord()  { return wLastWord; }
  CString File()   { return strFile; }
  CString Anchor() { return strAnchor; }
  BOOL Used() { return bUsed; }

  // data setting
  void FirstWord(WORD x) { wFirstWord = x; }
  void LastWord(WORD x)  { wLastWord = x; bUsed = TRUE;}
  void File(CString x)   { strFile = x; }
  void Anchor(CString x) { strAnchor = x; }

  private:
                       // indices of CTMLWords which cause jump when clicked:
    WORD wFirstWord;   //   first
    WORD wLastWord;    //   last
                       // destination of jump:
    CString strFile;   //   filename - within this file if empty
    CString strAnchor; //   anchor name
    BOOL bUsed;        // TRUE when connected to a word
};

//// CTMLAnchor
////  Contains hypertext anchor information: the indices of words which
////  are jumped to, and the anchor name.
////  An array of these objects is created by the parser and stored in
////  the document.

class CTMLAnchor {
  public:
    CTMLAnchor() { wFirstWord = wLastWord = 0; bUsed = FALSE;}
    ~CTMLAnchor() {}

  // data access
  WORD FirstWord() { return wFirstWord; }
  WORD LastWord()  { return wLastWord; }
  CString Anchor() { return strAnchor; }
  BOOL Used() { return bUsed; }

  // data setting
  void FirstWord(WORD x) { wFirstWord = x; }
  void LastWord(WORD x)  { wLastWord = x; bUsed = TRUE;}
  void Anchor(CString x) { strAnchor = x; }

  private:
                       // indices of CTMLWords which are jumped to:
    WORD wFirstWord;   //   first
    WORD wLastWord;    //   last
    CString strAnchor; // anchor name
    BOOL bUsed;        // TRUE when connected to a word
};

#endif