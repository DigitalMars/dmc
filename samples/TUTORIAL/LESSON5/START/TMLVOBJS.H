// tmlvobjs.h  (view-related classes)
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLLine class declaration
//   CTMLPage class declaration
//   CLinkBox class declaration
// Notes:
//   These are utility classes for the view.  The view keeps arrays of
//     each type of object to aid in drawing, printing, and interpreting
//     mouse clicks.
//   CTMLLine represents a line on the screen or page.  It contains pointers
//     (indices actually) into the document's word array indicating which
//     words are on the line, as well as some positional and other information.
//     An array of lines is created by the view's WordWrap() function.
//   CTMLPage represents a printed page.  It contains starting and ending line
//     numbers.  An array of pages is created by the view's Paginate() function.
//     Pages are only used during printing.
//   CLinkBox is a semi-generic class which could be used to store a list
//     of hitboxes on the screen.  In TMLRead, an array of linkboxes is created
//     during word wrapping, and is consulted whenever the user clicks on the
//     screen.  The link index stored in each linkbox object points into the
//     document's array of links, which contain information used to perform
//     hyperlinks.
//     

#ifndef _TMLVOBJS_H
#define _TMLVOBJS_H

//// CTMLLine
////  Defines lines of text which may be displayed in view.
////  An array of these objects is created by view's DoWrap().
////  A CTMLLine object contains the indices of the first and last
////  words on the line, and some information about the line dimensions.

class CTMLLine {
  public:
    CTMLLine (DWORD yoff, WORD blanksize, WORD fl) :
              lYOffset(yoff), nYSize(blanksize), wFirstLine(fl),
              bBlank(TRUE), wIndent(0)  {}
    ~CTMLLine () {}

    // Data access functions
    WORD FirstWord () { return wFirstWord; }
    WORD LastWord ()  { return wLastWord; }
    WORD NWords()     { return (bBlank ? 0 : wLastWord-wFirstWord+1); }
    long YOffset ()   { return lYOffset; }
    int  YSize ()     { return nYSize; }
    long YBottom ()   { return lYOffset+nYSize; }
    WORD Indent ()    { return wIndent; }
    BOOL Blank ()     { return bBlank; }
    WORD FirstLine()  { return wFirstLine; }
    BOOL ContainsWord(WORD x) { return (bBlank ? FALSE :
                                                (x>=wFirstWord && x<=wLastWord)); }

    // Data setting functions
    void FirstWord (WORD x) { wFirstWord = wLastWord = x; bBlank = FALSE; }
    void LastWord (WORD x)  { wLastWord = x; }
    void AddWord (WORD x)   { bBlank ? FirstWord(x) : LastWord(x); }
    void YOffset (long x)   { lYOffset = x; }
    void YSize (int x)      { nYSize = x; }
    void Indent (WORD x)    { wIndent = x; }
    void Blank (BOOL x)     { bBlank = x; }
    void FirstLine (WORD x) { wFirstLine = x; }

  private:
    WORD wFirstWord;  // first and last words on line (indices to wordarray)
    WORD wLastWord;
    long lYOffset;    // cumulative total of nYSize before this line
    int  nYSize;      // vertical size of line
    BOOL bBlank;      // TRUE if this is a blank line
    WORD wIndent;     // line indentation
    WORD wFirstLine;  // if an image, connects back to first line of image
                      // could also be used for connection back to first line
                      //   of a paragraph or other similar things
};

//// CTMLPage
////  Defines pages of text which may be printed (or previewed).
////  An array of these objects is created by view's Paginate().
////  A CTMLPage object contains the indices of the first and last+1
////  lines on the page.

/// defines pages of text, consisting of some number of lines

class CTMLPage {
  public:
    CTMLPage () : bBlank(TRUE) {}
    ~CTMLPage () {}

    // Data access functions
    WORD FirstLine () { return wFirstLine; }
    WORD LastLine ()  { return wLastLine; }
    BOOL Blank ()     { return bBlank; }

    // Data setting functions
    void FirstLine (WORD x) { wFirstLine = wLastLine = x; bBlank = FALSE; }
    void LastLine (WORD x)  { wLastLine = x; }

  private:
    WORD wFirstLine;  // first and last lines on page
    WORD wLastLine;
    BOOL bBlank;      // TRUE if this is a blank page
};


//// CLinkBox
////  A CLinkBox object contains coordinates of a box on the screen
////  where clicking causes a hyperlink jump.  The vertical coordinates are
////  actually relative to the beginning of the first line in the line array.
////  An array of CLinkBoxes is created during line wrapping.

class CLinkBox {
  public:
    CLinkBox() { wX0=wX1=0; lY0=lY1=0; wILink = 0; }
    ~CLinkBox() {}

    // data access functions
    WORD ILink () { return wILink; }

    // data setting functions
    void ILink (WORD x) { wILink = x; }
    void SetBox (WORD xl, long yt, WORD xr, long yb)
                 { wX0=xl; lY0=yt; wX1=xr; lY1=yb; }
    void ExtendRight (int xr) { wX1 = xr; } // you can only extend to right!

    // check if x,y modified by offset is in box
    BOOL IsHit (WORD x, WORD y, long yoffset)
                 { return x>=wX0 && x<wX1 && y+yoffset>=lY0 && y+yoffset<lY1; }
    BOOL IsHit (CPoint p, long yoffset)
                 { return IsHit(p.x,p.y, yoffset); }

  private:
    WORD wX0, wX1;  // coordinates of box...
    long lY0, lY1;  // y positions could exceed WORD
    WORD wILink;    // where to jump (index into linkarray)
};

#endif