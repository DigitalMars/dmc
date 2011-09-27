///  tmlword.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   #defines for word types, styles, contexts
//   CTMLWord class declaration
// Notes:
//   Objects of this type are created by the parser and stored in the 
//    document.  Each CTMLWord represents either a word of ordinary text,
//    a string of literal text, an image, or a horizontal rule.
//   Unions of data members have been used to minimize memory use.

#ifndef _TMLWORD_H
#define _TMLWORD_H

// word types
//   These are the different types of word.
//   Most TMLWords in a document will be WORDs.  WORDs have no spaces
//     in them, and must be wrapped.
//   LITERALS may contain spaces; they are the contents of PREFORM contexts.
//   HRULEs are horizontal rules.
//   IMAGEs are images.  In this program, both HRULEs and IMAGEs are displayed
//    on a separate line.
//   TITLE is used internally by the parser for the document title.

#define TMLWORD_TYPE_NONE    0x00  // empty - ignore
#define TMLWORD_TYPE_WORD    0x01  // contents is word, needs spaces, wrapping
#define TMLWORD_TYPE_LITERAL 0x02  // contents is literal - doesn't need
                                     //  spaces or wrapping
#define TMLWORD_TYPE_HRULE   0x04  // horizontal rule
#define TMLWORD_TYPE_IMAGE   0x08  // image - contents is file name

#define TMLWORD_TYPE_TITLE   0x10  // document title (used internally)

// character styles
//  these are set for every word that should have the given style

#define TMLWORD_STYLE_NONE    0x00  // no char styles - just plain
#define TMLWORD_STYLE_DFN     0x01  // word being defined
#define TMLWORD_STYLE_EM      0x02  // emphasis
#define TMLWORD_STYLE_CITE    0x03  // citation
#define TMLWORD_STYLE_VAR     0x04  // metasyntactic variable
#define TMLWORD_STYLE_I       0x05  // italic
#define TMLWORD_STYLE_CODE    0x06  // computer code sample
#define TMLWORD_STYLE_KBD     0x07  // keyboard entry
#define TMLWORD_STYLE_SAMP    0x08  // computer output
#define TMLWORD_STYLE_TT      0x09  // typewriter
#define TMLWORD_STYLE_STRONG  0x0a  // strong emphasis
#define TMLWORD_STYLE_B       0x0b  // bold

// context bits
//  NEWLINE and NEWPAR are set for the first word in a *forced* new line or
//    paragraph (word wrapping may occur anywhere, however)
//  ANCHOR and LINK are set for every word in an anchor or link
//  FIRST is set for the first word in a heading, list, quote, or preform
//  HEADING, ULIST, OLIST, QUOTE, and PREFORM are mutually exclusive contexts,
//   although ULISTs may be nested within each other

#define TMLWORD_CONTEXT_NONE     0x00  // no special context (ordinary paragraph)
#define TMLWORD_CONTEXT_NEWLINE  0x01  // word must start new line
#define TMLWORD_CONTEXT_NEWPAR   0x02  // word must start new paragraph
#define TMLWORD_CONTEXT_ANCHOR   0x04  // anchor
#define TMLWORD_CONTEXT_LINK     0x08  // hyperlink
#define TMLWORD_CONTEXT_FIRST    0x10  // first word in this context
#define TMLWORD_CONTEXT_SENT     0x20  // start of sentence
#define TMLWORD_CONTEXT_FLAGS    0xff

#define TMLWORD_CONTEXT_HEADING  0x100  // heading (wHLevel specifies level)
#define TMLWORD_CONTEXT_ULIST    0x200  // unordered list item
#define TMLWORD_CONTEXT_OLIST    0x300  // ordered list item
                                          //  (wOLItem specifies item #)
#define TMLWORD_CONTEXT_QUOTE    0x400  // block quote
#define TMLWORD_CONTEXT_PREFORM  0x500  // preformatted text
#define TMLWORD_CONTEXT_BLOCKS   0xf00


//// CTMLWord
////  Contains words read from the TML file.
////  An array of these objects is created by parser, and stored in the
////  document.They can represent words, literal strings, images, or
////  horizontal rules, and contain all the information necessary for
////  formatting themselves correctly.

class CTMLWord {
  public:
    CTMLWord() { Reset(); }
    ~CTMLWord() {}

    // #defines for types, styles, and contexts are in tmlword.h

    // data access functions
    WORD Type () { return byType; }
    BOOL IsType (BYTE x) { return byType == x; }
    WORD HLevel () { return wHLevel; }
    WORD OLItem () { return wOLItem; }
    WORD ULNest () { return wULNest; }
    WORD Style () { return byStyle; }
    BOOL IsStyle (BYTE x) { return byStyle == x; }
    WORD Context () { return wContext; }
    BOOL HasContextFlag (WORD x) { return (wContext & x) != 0; }
    BOOL IsContextBlock (WORD x) { return (wContext & TMLWORD_CONTEXT_BLOCKS) == x; }
    WORD ILink () { return wILink; }
    WORD IImage () { return wIImage; }
    CString & Contents () { return strContents; }
    WORD XSize () { return wXSize; }

    // data setting functions
    void Type (BYTE x)    { byType = x; }
    void HLevel (WORD x)  { wHLevel = x; }
    void OLItem (WORD x)  { wOLItem = x; }
    void ULNest (WORD x)  { wULNest = x; }
    void Style (BYTE x)   { byStyle = x; }
    void AddStyle (BYTE x) { byStyle |= x; }
    void Context (WORD x)  { wContext = x; }
    void AddContext (WORD x)  { wContext |= x; }
    void ILink (WORD x) { wILink = x; }
    void IImage (WORD x) { wIImage = x; }
    void Contents (CString x) { strContents = x; }
    void Contents (char *x)   { strContents = x; }
    void XSize (WORD x) { wXSize = x; }

  protected:
    // reset word to default state
    void Reset() { byType = TMLWORD_TYPE_WORD;
                   wContext = TMLWORD_CONTEXT_NONE;
                   byStyle = TMLWORD_STYLE_NONE;
                   wHLevel = wOLItem = wILink = wXSize = 0; }

  private:
    BYTE byType;     // type - WORD, LITERAL, HRULE, IMAGE
    BYTE byStyle;    // character style bits (for WORD or LITERAL)
    WORD wContext;   // context bits
    union {
      WORD wHLevel;  // header level (1 to 6, only in HEADING context)
      WORD wOLItem;  // item number (1 or more, only in OLIST context)
      WORD wULNest;  // nesting (1 or more, only in ULIST context)
    };
    WORD wILink;     // link index (if context is LINK or ANCHOR)

    CString strContents; // string contents

  // NOTE: wXSize is set, maintained, and used by the view
  //   wIImage is set by parser (view must use caution to not set
  //   wXSize for images).  These are union just to save memory.

    union {
      WORD wXSize;    // size of word, units specified by view
      WORD wIImage;   // image index (if type is IMAGE)
    };
};

#endif
