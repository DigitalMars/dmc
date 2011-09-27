// tmlrdvw.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLReadView class declaration
//

////
//// CTMLReadView
////   View class.  Contains all the functions and data necessary
////   to draw the document on the screen, to print, and to handle input
////

class CTMLReadView : public CView
{
  protected: // create from serialization only
    CTMLReadView();
    DECLARE_DYNCREATE(CTMLReadView)

  public:
    virtual ~CTMLReadView();
    CTMLReadDoc* GetDocument();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
    // Generated message map functions
    //{{AFX_MSG(CTMLReadView)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPrevfile();
    afx_msg void OnUpdatePrevfile(CCmdUI *pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

  public:
    // document read/updated
    void OnInitialUpdate();
    void OnUpdate(CView *, LPARAM, CObject *);
    BOOL PreCreateWindow( CREATESTRUCT &cs ); // for SetCursor

  protected:
    // initialization
    void SetDefaultPrefs();     // set up defaults for fontinfo etc.
    void LoadGDIObjects();      // load fonts etc.
    // cleanup
    void DeleteContents();      // deletes any allocated arrays
    void DeleteLines();         // get rid of individual arrays
    void DeleteBoxes();
    void DeletePages();
    void FreeGDIObjects();      // free fonts etc.
    // data preparation
    void MeasureFontHeights(CDC *);           //  record heights of loaded fonts
    void MeasureWords(CDC *,  CPrintInfo *);  // measure all words
    void WrapWords(int, CPrintInfo *);        // do all word wrapping
    void Paginate(int);                       // break lines into pages
    // drawing and printing support
    BOOL OnPreparePrinting(CPrintInfo* pInfo);
    BOOL DoPreparePrinting(CPrintInfo* pInfo);
    void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT, CPreviewView *);
    void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
    void OnDraw(CDC* pDC);                                 // draw view on screen
    void OnPrint(CDC* pDC, CPrintInfo* pInfo);             // print view
    void DrawView(CDC *, int, int, CRect,  CPrintInfo *);  // called to draw or print
    // message handling utilities
    int GetILinkBoxFromPt(CPoint);             // search box array for hit
    void HandleScroll(UINT, UINT);             // handle scrolls and cursor keys
    int  HandleScrollPage(UINT);               //  helps above handle page up / down
    // miscellaneous utilities for moving around
    WORD GetTopWord();             // returns index of first word on screen
    void ResetScroll(WORD);        // reset scroll bar range and position after wrapping
    void JumpToAnchor(CString);    // reposition to given anchor name

    // Jump support - used to tell OnDraw where to start display after
    //  hyperlink or jumpback

    // set word to jump to in next redraw
    void SetJumpWord (WORD x) { wJumpWord = x; bJumpSet = TRUE; }
    // check if word jump set for next redraw
    BOOL IsJumpSet() { return bJumpSet; }
    // get word to jump to in next redraw
    WORD GetJumpWord() { bJumpSet = FALSE; return wJumpWord; }

    // access to data arrays
    CTMLLine * Line (WORD i)    { return (CTMLLine *) (apLineArray[i]); }
    CLinkBox * LinkBox (WORD i) { return (CLinkBox *) (apLinkBoxArray[i]);  }
    CTMLPage * Page (WORD i)    { return (CTMLPage *) (apPageArray[i]); }

  private:
    // data - misc.
    CTMLReadDoc *pDoc;          // pointer to document
    int nDispLogPixelsX,        // pix per logical inch of display
        nDispLogPixelsY;
    CRect rectWindowRect;       // window rectangle, saved from last OnSize
    int nVScrollPos;            // current scroll position
    int nVScrollMax;            // max scroll position
    BOOL bWordsMeasured;        // TRUE if words all measured
    CPtrArray apLineArray;      // pointers to CTMLLine
    BOOL bWordsWrapped;         // TRUE if wordwrapping done and apLineArray set up
    CPtrArray apLinkBoxArray;   // pointers to CLinkBox
    CPtrArray apPageArray;      // pointers to CTMLPage for printer pages
    BOOL bPaginated;            // TRUE if paginated
    BOOL bPrinting;             // TRUE from Begin to End Print
    CRect rectPageRect;         // rect for printer page
    CRect rectPageHeaderRect;   // rect for printer page header

    struct FontInfo {  // enough information to construct a font
      // constructors
      FontInfo () { SetFontInfo("", 8, FALSE, FALSE); }
      FontInfo (char *n, int s, BOOL b = FALSE, BOOL i = FALSE)
                  { SetFontInfo (n, s, b, i); }
      // set the info
      void SetFontInfo (char *n, int s, BOOL b, BOOL i)
                    { name = n; size = s; bold = b; ital = i; }
      // data
      CString name;  // typeface name
      int size;      // size
      BOOL bold;     // style flags
      BOOL ital;
    };

public:
    // These values are public because CPrefDialog will need access to them.
    // "private" is resumed immediately afterwards.
    enum
    {
        eDftParVSpace = 12,
        eDftMargin    = 10,
        eDftIndent    = 40  // very arbitrary
    };

private:
    // preferences-type data
    FontInfo finParFontInfo;     // paragraph font
    FontInfo finTTFontInfo;      // typewriter font (must be same size as parfont)
    FontInfo afinHdrFontInfo[7]; // header fonts (indexed 1 to 6)
    int nIndent;         // indentation for blockquote, lists (Logical Units)
    int nMargin;         // indentation for every line (LU)
    int nParVSpace;      // vertical space between paragraphs (LU)
    int nImgVSpace;      // vertical space around images (LU)
    int nImgLineHt;      // height of lines within images (LU)
    int anStyleMap[16];  // mapping of character styles to normal=0,
                         //  italic=1, bold=2, TT=3
    COLORREF colTextColor;  // normal text color
    COLORREF colHyperColor; // color for hyperlinks
    COLORREF colBkgColor;   // color for screen
    BOOL bHandleThumbTrack; // handle thumbtrack messages

    // printer preferences
    FontInfo finPrtHdrFontInfo;  // page header font
    BOOL bPrtDoHeader;           // should we print page headers
    BOOL bPrtDoHeaderHRule;      // should we put line under header
    int nPrtLMargin, nPrtRMargin, // margins (LU)
        nPrtTMargin, nPrtBMargin;

    // font heights - used to look up line height based on first word style
    //  these values set in MeasureFontHeights()
    int anPFontHt[4];    // paragraph fonts
    int anHFontHt[7];    // header fonts
    int nPrtHdrFontHt;   // print page header font
    // ascents (baselines)
    int anPFontAsc[4];
    int anHFontAsc[7];
    // width of spaces, for between words
    int anPSpaceWid[4];
    int anHSpaceWid[7];

    // GDI Objects - created by LoadGDIObjects(), freed by FreeGDIObjects()
    CFont  *apfonPFonts[4];    // paragraph font in 3 styles (normal, ital, bold),
                               //  plus TT (Teletype) font
    CFont  *apfonHFonts[7];    // header fonts
    CFont  *pfonPrtHdrFont;    // print page header font
    CPen   *ppenTextPen;       // pens for lines in text colors
    CPen   *ppenHyperPen;
    CBrush *pbruTextBrush;     // for bullets
    CBrush *pbruBkgBrush;      // brush to erase background

    BOOL bJumpSet;       // is word jump set for next draw?
    WORD wJumpWord;      //  if so, jump to this word
};

#ifndef _DEBUG	// debug version in tmlrdvw.cpp
inline CTMLReadDoc* CTMLReadView::GetDocument()
   { return (CTMLReadDoc*) m_pDocument; }
#endif
