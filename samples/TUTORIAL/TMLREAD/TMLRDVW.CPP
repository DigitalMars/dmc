// tmlrdvw.cpp
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLReadView message map and member functions
// Functions are categorized as follows:
//    Constructor, Destructor, and Diagnostics
//    Initialization - set default preferences, allocate GDI objects
//    Cleanup - deallocate GDI objects, line, page, and jumpbox objects
//    Document Update - called when document read or updated
//    Data Preparation - measure fonts and words, wrap words, paginate
//    Drawing and Printing
//    Message Handlers - for sizing, scrolling, mouse clicks, keypresses,
//       erase background, and menu commands
//    Message Handling Utilities - decipher mouse clicks, handle scrolling
//    Miscellaneous Utilities - jump to anchor or word, etc.
//

#include "stdafx.h"
#include "dibapi.h"

#include "TMLRead.h"
#include "tmlword.h"
#include "tmllink.h"
#include "tmlimage.h"
#include "tmlparse.h"
#include "tmlvobjs.h"
#include "ptrstack.h"
#include "tmlrddoc.h"
#include "tmlrdvw.h"
#include "prefdial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

////
//// Message Map
////

IMPLEMENT_DYNCREATE(CTMLReadView, CView)

BEGIN_MESSAGE_MAP(CTMLReadView, CView)
	//{{AFX_MSG_MAP(CTMLReadView)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_PREVFILE,OnPrevfile)
	ON_UPDATE_COMMAND_UI(IDC_PREVFILE,OnUpdatePrevfile)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_PREVFILE,OnPrevfile)
	ON_UPDATE_COMMAND_UI(IDC_PREVFILE,OnUpdatePrevfile)
	ON_COMMAND(ID_VIEW_PREFS,OnViewPrefs)
  //}}AFX_MSG_MAP
  // Standard printing commands
  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

///////////////////////////////////
/////////////////////////////////// Constructor and Destructor
///////////////////////////////////
////
//// Constructor
////  Initializes class data
////  Also makes call to load fonts, pens, etc.
////
CTMLReadView::CTMLReadView() : CView()
{
  TRACE ("CTMLReadView Constructor\n");

  // make sure all class data is initialized!

  pDoc = NULL;
  nDispLogPixelsX = nDispLogPixelsY = 0;
  rectWindowRect.SetRect(0,0,0,0);
  nVScrollPos = nVScrollMax = 0;
  bWordsMeasured = FALSE;
  bWordsWrapped = FALSE;
  bPaginated = FALSE;
  bPrinting = FALSE;
  rectPageRect.SetRect(0,0,0,0);
  rectPageHeaderRect.SetRect(0,0,0,0);

  apLineArray.SetSize(0,4000); // help arrays to grow by reasonable amounts
  apLinkBoxArray.SetSize(0,200);
  apPageArray.SetSize(0,20);

  SetDefaultPrefs(); // initialize preferences data
  LoadGDIObjects();  // load up fonts etc.
}


////
//// Destructor
////  Free fonts & other GDI objects, clear arrays
////
CTMLReadView::~CTMLReadView()
{
  TRACE ("CTMLReadView Destructor\n");

  FreeGDIObjects();  // free fonts etc.
  DeleteContents();  // flushes arrays
}

////
//// PreCreateWindow
//// for SetCursor
////

BOOL CTMLReadView::PreCreateWindow( CREATESTRUCT &cs )
{
    cs.lpszClass = ::AfxRegisterWndClass( cs.style,
                                          0,         // NULL cursor (default)
                                          0,         // NULL brush  (default)
                                          0 );       // NULL icon   (default)
    return TRUE;
}

////
//// Diagnostics
////
#ifdef _DEBUG
void
CTMLReadView::AssertValid() const
{
  CView::AssertValid();
}
void
CTMLReadView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
CTMLReadDoc*
CTMLReadView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTMLReadDoc)));
  return (CTMLReadDoc*) m_pDocument;
}
#endif //_DEBUG

///////////////////////////////////
/////////////////////////////////// Initialization
///////////////////////////////////
////
//// SetDefaultPrefs
////  Sets default values for preferences info
////  Note: sizes of fonts, spacing, etc. are in logical
////  units, which are the same size as pixels.
////
void
CTMLReadView::SetDefaultPrefs()
{
  nParVSpace = eDftParVSpace;
  nMargin    = eDftMargin;
  nIndent    = eDftIndent; // arbitrary

  bHandleThumbTrack = TRUE; // draw while dragging scroll thumb


  // mapping to physical styles
  anStyleMap[TMLWORD_STYLE_NONE]   = 0; // 0 = normal
  anStyleMap[TMLWORD_STYLE_DFN]    = 1; // 1 = italic
  anStyleMap[TMLWORD_STYLE_EM]     = 1;
  anStyleMap[TMLWORD_STYLE_CITE]   = 1;
  anStyleMap[TMLWORD_STYLE_VAR]    = 1;
  anStyleMap[TMLWORD_STYLE_I]      = 1;
  anStyleMap[TMLWORD_STYLE_CODE]   = 3; // 3 = TT
  anStyleMap[TMLWORD_STYLE_KBD]    = 3;
  anStyleMap[TMLWORD_STYLE_SAMP]   = 3;
  anStyleMap[TMLWORD_STYLE_TT]     = 3;
  anStyleMap[TMLWORD_STYLE_STRONG] = 2; // 2 = bold
  anStyleMap[TMLWORD_STYLE_B]      = 2;

  // font info

  finParFontInfo.SetFontInfo  ("Arial",18, FALSE, FALSE);
  finTTFontInfo.SetFontInfo   ("Courier New",18, FALSE, FALSE);
  afinHdrFontInfo[1].SetFontInfo ("Arial",36, TRUE, FALSE);
  afinHdrFontInfo[2].SetFontInfo ("Arial",24, TRUE, FALSE);
  afinHdrFontInfo[3].SetFontInfo ("Arial",18, TRUE, FALSE);
  afinHdrFontInfo[4].SetFontInfo ("Arial",18, TRUE, FALSE);
  afinHdrFontInfo[5].SetFontInfo ("Arial",18, TRUE, FALSE);
  afinHdrFontInfo[6].SetFontInfo ("Arial",18, TRUE, FALSE);

  // text color
  colTextColor = RGB(0,0,0);
  // hyperlink color
  colHyperColor = RGB(0,150,50);
  // background color
//  colBkgColor = RGB(255,128,0); //orange
  colBkgColor = RGB(255,255,255);

  // image stuff
  nImgVSpace = 8;
  nImgLineHt = finParFontInfo.size;

  // printing stuff
  bPrtDoHeader = TRUE;
  bPrtDoHeaderHRule = TRUE;
  nPrtLMargin = 72;
  nPrtTMargin = 24;
  nPrtBMargin = nPrtRMargin = 48;
  finPrtHdrFontInfo.SetFontInfo ("Arial", 18, TRUE, FALSE);
}

////
//// LoadGDIObjects
////  Allocates and loads GDI objects (fonts, pens, brushes)
////
void CTMLReadView::LoadGDIObjects()
{
  int i;

  // create the CFonts
  for (i = 0; i < 4; i++)
    apfonPFonts[i] = new CFont;
  for (i = 1; i < 7; i++)
    apfonHFonts[i] = new CFont;
  pfonPrtHdrFont = new CFont;

  // set up font defaults in logfont
  LOGFONT logfont;
  logfont.lfHeight = 1;
  logfont.lfWidth = logfont.lfEscapement = logfont.lfOrientation = 0;
  logfont.lfWeight = FW_NORMAL;
  logfont.lfItalic = logfont.lfUnderline = logfont.lfStrikeOut = 0;
  logfont.lfCharSet       = DEFAULT_CHARSET;
  logfont.lfOutPrecision  = OUT_TT_PRECIS;
  logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  logfont.lfQuality       = DEFAULT_QUALITY;
  logfont.lfPitchAndFamily= DEFAULT_PITCH | TMPF_TRUETYPE | FF_DONTCARE;
  logfont.lfFaceName[0] = 0;

  // now create fonts

  // normal paragraph font
  logfont.lfHeight = finParFontInfo.size;
  logfont.lfWeight = FW_NORMAL;
  logfont.lfItalic = 0;
  strcpy (logfont.lfFaceName, finParFontInfo.name);
  apfonPFonts[0]->CreateFontIndirect(&logfont);
  // italic
  logfont.lfItalic = 1;
  apfonPFonts[1]->CreateFontIndirect(&logfont);
  // bold
  logfont.lfWeight = FW_BOLD;
  logfont.lfItalic = 0;
  apfonPFonts[2]->CreateFontIndirect(&logfont);
  // TT font... same height
  logfont.lfWeight = finTTFontInfo.bold ? FW_BOLD : FW_NORMAL;
  logfont.lfItalic = finTTFontInfo.ital;
  strcpy (logfont.lfFaceName, finTTFontInfo.name);
  apfonPFonts[3]->CreateFontIndirect(&logfont);

    // header fonts
  for (i = 1; i < 7; i++) {
    logfont.lfHeight = afinHdrFontInfo[i].size;
    logfont.lfWeight = afinHdrFontInfo[i].bold ? FW_BOLD : FW_NORMAL;
    logfont.lfItalic = afinHdrFontInfo[i].ital;
    strcpy (logfont.lfFaceName, afinHdrFontInfo[i].name);
    apfonHFonts[i]->CreateFontIndirect(&logfont);
  }

  // printed page header font
  logfont.lfHeight = finPrtHdrFontInfo.size;
  logfont.lfWeight = finPrtHdrFontInfo.bold ? FW_BOLD : FW_NORMAL;
  logfont.lfItalic = finPrtHdrFontInfo.ital;
  strcpy (logfont.lfFaceName, finPrtHdrFontInfo.name);
  pfonPrtHdrFont->CreateFontIndirect(&logfont);

  // drawing pens and brushes
  ppenTextPen  = new CPen(PS_SOLID,0,colTextColor);
  ppenHyperPen = new CPen(PS_SOLID,0,colHyperColor);
  pbruTextBrush = new CBrush(colTextColor);
  pbruBkgBrush = new CBrush(colBkgColor);
}

///////////////////////////////////
/////////////////////////////////// Cleanup
///////////////////////////////////
////
//// DeleteContents
////  Deletes allocated data from view (lines, pages, boxes)
////
void
CTMLReadView::DeleteContents()
{
  DeleteLines();
  bWordsWrapped = FALSE;
  DeletePages();
  bPaginated = FALSE;
  DeleteBoxes();
}

////
//// DeleteLines
////  Deletes all lines, removes pointers from array
////
void
CTMLReadView::DeleteLines()
{
  int size = apLineArray.GetSize();
  for (int i = 0; i < size; i++)
    delete Line(i);
  apLineArray.RemoveAll();
}

////
//// DeletePages
////  Deletes all pages, removes pointers from array
////
void
CTMLReadView::DeletePages()
{
  int size = apPageArray.GetSize();
  for (int i = 0; i < size; i++)
    delete Page(i);
  apPageArray.RemoveAll();
}

////
//// DeleteBoxes
////  Deletes all linkboxes, removes pointers from array
////
void
CTMLReadView::DeleteBoxes()
{
  int size = apLinkBoxArray.GetSize();
  for (int i = 0; i < size; i++)
    delete LinkBox(i);
  apLinkBoxArray.RemoveAll();
}

////
//// FreeGDIObjects
////  Delete fonts, pens, and brushes, set pointers to NULL
////
void CTMLReadView::FreeGDIObjects()
{
  int i;
  for (i = 0; i < 4; i++) {
    delete apfonPFonts[i];
    apfonPFonts[i] = NULL;
  }
  for (i = 1; i < 7; i++) {
    delete apfonHFonts[i];
    apfonHFonts[i] = NULL;
  }
  delete pfonPrtHdrFont;
  pfonPrtHdrFont = NULL;

  delete ppenTextPen;
  ppenTextPen = NULL;
  delete ppenHyperPen;
  ppenHyperPen = NULL;
  delete pbruTextBrush;
  pbruTextBrush = NULL;
  delete pbruBkgBrush;
  pbruBkgBrush = NULL;
}

///////////////////////////////////
/////////////////////////////////// Document Update
///////////////////////////////////
////
//// OnInitialUpdate
////  Called by framework when new doc is read in
////  Saves pointer to document
////
void
CTMLReadView::OnInitialUpdate()
{
  TRACE ("CTMLReadView::OnInitialUpdate()\n");

  pDoc = GetDocument();  // store pointer to the document
                         // in reality this pointer is easily
                         // available at all times by using
                         // GetDocument(), but pDoc looks nicer
                         // in the code :>

  CView::OnInitialUpdate(); // call base class, which calls OnUpdate()
}

////
//// OnUpdate
////  Called when document is modified
////  Arguments:
////    (see MFC docs; we do not use them here)
////  In TMLRead, the document is only modified when a new file
////  is read, so this routine is only called once per doc file.
////
void
CTMLReadView::OnUpdate(CView *pSender, LPARAM lHint, CObject * pHint)
{
  TRACE ("CTMLReadView::OnUpdate()\n");

  CView::OnUpdate(pSender, lHint, pHint);  // call base class

  DeleteContents();         // delete all lines and boxes
  bWordsMeasured = FALSE;   // words not measured

  // if we have just loaded a new document because of a hyperlink or jumpback,
  //  we need to find where in the new document to jump

  WORD topword;
  BOOL anchornotfound = FALSE;

  // hyperlink
  if (pDoc->IsStartAnchor()) {
    if (!pDoc->GetStartAnchor(topword)) {
      anchornotfound = TRUE; // do not call AfxMessageBox here,
                             //  or OnDraw will get called before we have
                             //  set the jump word
      topword = 0;
    }
  }
  // jumpback or ordinary file open
  else
    topword = pDoc->GetStartTopWord();

  SetJumpWord(topword);

  if (anchornotfound)
    AfxMessageBox("Hyperlink anchor not found");
}

///////////////////////////////////
/////////////////////////////////// Data Preparation
///////////////////////////////////
////
//// MeasureFontHeights
////  Measure font heights and width of spaces in current DC
////  Arguments:
////    pDC is device context
////  Selects fonts into DC and measures heights, ascender locations,
////  and widths of space character.
////
void
CTMLReadView::MeasureFontHeights(CDC *pDC)
{
  pDC->SaveDC(); // save for restoration before exit

  TEXTMETRIC tm;

  // regular fonts
  pDC->SelectObject(apfonPFonts[0]);  // select font into DC
  pDC->GetTextMetrics(&tm);           //  get metrics
  anPFontHt[0] = anPFontHt[1] = anPFontHt[2] = tm.tmHeight;    //  save height
  anPFontAsc[0] = anPFontAsc[1] = anPFontAsc[2] = tm.tmAscent; //  save baseline
  anPSpaceWid[0] = anPSpaceWid[1] = anPSpaceWid[2] = pDC->GetTextExtent(" ", 1).cx;

  // TT font
  pDC->SelectObject(apfonPFonts[3]);  // select font into DC
  pDC->GetTextMetrics(&tm);           //  get metrics
  anPFontHt[3] = tm.tmHeight;         //  save height
  anPFontAsc[3] = tm.tmAscent;        //  save baseline
  anPSpaceWid[3] = pDC->GetTextExtent(" ", 1).cx;

  // header fonts
  for (int i = 1; i < 7; i++) {
    pDC->SelectObject(apfonHFonts[i]);
    pDC->GetTextMetrics(&tm);
    anHFontHt[i] = tm.tmHeight;
    anHFontAsc[i] = tm.tmAscent;
    anHSpaceWid[i] = pDC->GetTextExtent(" ", 1).cx;
  }

  // print header font
  pDC->SelectObject(pfonPrtHdrFont); // select font into DC
  pDC->GetTextMetrics(&tm);          //  get metrics
  nPrtHdrFontHt = tm.tmHeight;       //  save height

  pDC->RestoreDC(-1);                // restore device context
}

////
//// MeasureWords
////  Measure all words' x sizes
////  Arguments:
////    pDC is device context
////    pInfo is print info structure (used to indicate if
////      caller is a print function)
////  Sets X Size of each word contained in document.
////
void
CTMLReadView::MeasureWords(CDC *pDC, CPrintInfo *pInfo)
{
  if (bPrinting && !pInfo)     // if printing, only do this for print functions
    return;

  MeasureFontHeights (pDC);    // measure font heights in this DC

  BeginWaitCursor();
  pDC->SaveDC(); // save for restoration before exit

  ASSERT (pDoc != NULL);

  int nwords = pDoc->NumWords();
  CFont *pfont, *pfontlast = NULL;

  // loop through all words in the document
  for (int i = 0; i < nwords; i++) {
    CTMLWord *word = pDoc->Word(i); // get next word
    // text - need to measure with GetTextExtent()
    if (word->IsType(TMLWORD_TYPE_WORD) ||
        word->IsType(TMLWORD_TYPE_LITERAL)) {
      // decide on font, depending on header level or word style
      if (word->IsContextBlock(TMLWORD_CONTEXT_HEADING)) {
        pfont = apfonHFonts[word->HLevel()];
      }
      else { // anything not a heading
        pfont = apfonPFonts[anStyleMap[word->Style()]];
      }
      // select font into DC only if changed
      if (pfont != pfontlast) {
        pDC->SelectObject(pfont);
        pfontlast = pfont;
      }
      // measure size and set
      int xsize = (pDC->GetTextExtent(word->Contents(), (word->Contents()).GetLength())).cx;
      word->XSize(xsize);
    }
    // images
    else if (word->IsType(TMLWORD_TYPE_IMAGE)) {
      /// DO NOT SET IMAGE XSIZE -- it is in union with iimage!
    }
    // hrules
    else
      word->XSize(29999);  // value never actually used
  }
  pDC->RestoreDC(-1); // restore device context
  EndWaitCursor();
  bWordsMeasured = TRUE;
}

////
//// WrapWords
////  Divide words into lines
////  Arguments:
////    wrapwidth is window or page width
////    pInfo is print info structure (used to indicate if
////      caller is a print function)
////  x
////  x
////
void
CTMLReadView::WrapWords(int wrapwidth, CPrintInfo *pInfo)
{
  if (bPrinting && !pInfo)     // if printing, only do this for print functions
    return;

  // early in the program, these may get tripped...
  if (wrapwidth <= 0 || !bWordsMeasured || pDoc == NULL)
    return;

  // flags indicating whether link boxes recorded and images broken
  BOOL doboxes = TRUE, breakimages = TRUE;
  if (pInfo)
    doboxes = breakimages = FALSE;

  WORD nlines = 0; // local counter for number of lines

  // clear arrays
  DeleteLines();
  if (doboxes)
    DeleteBoxes();

  // get number of words, return if none
  int nwords = pDoc->NumWords();
  if (!nwords)
    return;

  BeginWaitCursor();

  // Start with a line (lines are blank when constructed)
  CTMLLine *line = new CTMLLine(0, nParVSpace, nlines);
  apLineArray.Add((void *)line); // add to array
  nlines++;
  int xline = 0; // added to line so far
  long ynet = 0; // total vertical space

  // Pointer for last box on line
  CLinkBox *lastbox;

  for (int i = 0; i < nwords; i++) {
    CTMLWord *word = pDoc->Word(i); // get next word
    // new paragraphs and new lines... paragraphs generate blank line
    if (word->HasContextFlag(TMLWORD_CONTEXT_NEWLINE) ||
        word->HasContextFlag(TMLWORD_CONTEXT_NEWPAR)) {
      // generate blank line for paragraph
      if (word->HasContextFlag(TMLWORD_CONTEXT_NEWPAR)) {
        ynet = line->YBottom();
        line = new CTMLLine(ynet, nParVSpace, nlines);
        apLineArray.Add((void *)line); // add to array
        if (++nlines >= 16384) break;
        lastbox = NULL;
        xline = 0;
      }
      // now start (another) new line
      ynet = line->YBottom();
      line = new CTMLLine(ynet, nParVSpace, nlines);
      apLineArray.Add((void *)line); // add to array
      if (++nlines >= 16384) break;
      lastbox = NULL;
      xline = 0;
    }
    // deal with ordinary words
    if (word->IsType(TMLWORD_TYPE_WORD)) {
      int xword = 0;
      int yword = (word->IsContextBlock(TMLWORD_CONTEXT_HEADING) ?
                     anHFontHt[word->HLevel()] :
                     anPFontHt[0]);
      int xspace = (word->IsContextBlock(TMLWORD_CONTEXT_HEADING) ?
                     anHSpaceWid[word->HLevel()] :
                     anPSpaceWid[anStyleMap[word->Style()]]);
      // blank line - get right indentation, add word
      if (line->Blank()) {
        xword += nMargin;
        if (word->IsContextBlock(TMLWORD_CONTEXT_OLIST) ||
            word->IsContextBlock(TMLWORD_CONTEXT_QUOTE))
          xword += nIndent;
        else if (word->IsContextBlock(TMLWORD_CONTEXT_ULIST))
          xword += nIndent*word->ULNest();
        line->Indent(xword);  // value so far given to line for indent
        xword += word->XSize();
        line->AddWord(i);
        line->YSize(yword);
        xline += xword;
      }
      // line with something on it already -
      //   get interword space and word size, see if it fits
      else {
        xword += (word->HasContextFlag(TMLWORD_CONTEXT_SENT) ?
                 2*xspace : xspace);
        xword += word->XSize();
        if (xline + xword < wrapwidth - nMargin) { // fits - add to line
          line->AddWord(i);
          line->YSize(yword);
          xline += xword;
        }
        else {  // doesn't fit - start new line
          ynet = line->YBottom();
          line = new CTMLLine(ynet, yword, nlines);
          apLineArray.Add((void *)line); // add to array
          if (++nlines >= 16384) break;
          xline = 0;
          lastbox = NULL;
          //restart as above for blank line
          xword = nMargin;
          if (word->IsContextBlock(TMLWORD_CONTEXT_OLIST) ||
              word->IsContextBlock(TMLWORD_CONTEXT_QUOTE))
            xword += nIndent;
          else if (word->IsContextBlock(TMLWORD_CONTEXT_ULIST))
            xword += nIndent*word->ULNest();
          line->Indent(xword);  // value so far given to line for indent
          xword += word->XSize();
          line->AddWord(i);
          line->YSize(yword);
          xline += xword;
        }
      }
    }
    else {  // images, hrules, literals added to line always
      // do not care about xsize except to center image
      line->AddWord(i);
      if (word->IsType(TMLWORD_TYPE_IMAGE)) {
        CSize isize = pDoc->Image(word->IImage())->Size();
        line->YSize(isize.cy + nImgVSpace*2);
        int xtemp = (wrapwidth - isize.cx)/2;
        line->Indent(xtemp > nMargin ? xtemp : nMargin);
      }
      else if (word->IsType(TMLWORD_TYPE_HRULE))
        line->YSize(nParVSpace*2);
      else {//if (word->IsType(TMLWORD_TYPE_LITERAL))
        line->YSize(anPFontHt[3]);
        line->Indent(nMargin);
      }
    }
    // check link box
    if (doboxes && word->HasContextFlag(TMLWORD_CONTEXT_LINK)) {
      BOOL neednewbox = TRUE;
      if (lastbox && !word->IsType(TMLWORD_TYPE_IMAGE)) {
        if (lastbox->ILink() == word->ILink()) {
          lastbox->ExtendRight(xline);
          neednewbox = FALSE;
        }
      }
      if (neednewbox) {
        CLinkBox *newbox = new CLinkBox;
        apLinkBoxArray.Add((void *)newbox); // add to array
        newbox->ILink(word->ILink());
        // new box for image
        if (word->IsType(TMLWORD_TYPE_IMAGE)) {
          CSize isize = pDoc->Image(word->IImage())->Size();
          newbox->SetBox(line->Indent(),
                         line->YOffset() + nParVSpace,
                         line->Indent()+isize.cx,
                         line->YBottom() - nParVSpace);
          lastbox = NULL;
        }
        // new box for word
        else {
          newbox->SetBox(xline-word->XSize(),line->YOffset(),xline,line->YBottom());
          lastbox = newbox;
        }
      }
    }
    /// if an image, break line up into additional lines
    if (breakimages && word->IsType(TMLWORD_TYPE_IMAGE)) {
      int maxht = (nImgLineHt*3)/2; // break up if larger than this
      WORD iline0 = nlines-1; // index of first line
      int remainderht = line->YSize();
      while (remainderht > maxht) {
        remainderht -= nImgLineHt;
        line->YSize(nImgLineHt);
        ynet = line->YBottom();
        CTMLLine *line2 = new CTMLLine(ynet,
                                       remainderht,
                                       iline0);
        line2->FirstWord(i); // current word (which is an image)
        line2->LastWord(i);
        line2->Indent(line->Indent()); // same indent
        line = line2;
        apLineArray.Add((void *)line); // add to array
        if (++nlines >= 16384) break;
      }
      lastbox = NULL;
      xline = 0;
      if (nlines >= 16384) break;
    }
  }

  EndWaitCursor();
  bWordsWrapped = TRUE;

  if (nlines >= 16384)
    AfxMessageBox ("Too Many Lines!");
}

////
//// Paginate
////  Divide lines into pages, setting up page array
////  Arguments:
////    maxpageht is page vertical size
////
void
CTMLReadView::Paginate(int maxpageht)
{
  int nlines = apLineArray.GetSize();
  if (!nlines)
    return;

  // Start with a page (pages are blank when constructed)
  CTMLPage *page = new CTMLPage;
  apPageArray.Add((void *)page); // add to array
  page->FirstLine(0);
  int pageht = 0;

  for (int i = 0; i < nlines; i++) {
    CTMLLine *line = Line(i); // get next line
    // if line doesn't fit on page, and page not blank,
    //   make a new page
    if (pageht + line->YSize() > maxpageht && !page->Blank()) {
      page = new CTMLPage;
      apPageArray.Add((void *)page); // add to array
      page->FirstLine(i);
      pageht = 0;
    }
    // add line to the current page
    page->LastLine(i+1);
    pageht += line->YSize();
  }
  bPaginated = TRUE;
}

///////////////////////////////////
/////////////////////////////////// Drawing and Printing
///////////////////////////////////
////
//// OnPreparePrinting
////  Called by framework before printing
////  Arguments:
////    pInfo is printing information
////  Returns:
////    Base class return value
////  Here we just call base class
////
BOOL
CTMLReadView::OnPreparePrinting(CPrintInfo* pInfo)
{
  TRACE ("CTMLReadView::OnPreparePrinting()\n");

  return DoPreparePrinting(pInfo);  // default preparation
}

////
//// DoPreparePrinting
////  Called by framework before printing
////  Arguments:
////    pInfo is printing information
////  Returns:
////    Base class return value
////  Here we just call base class
////
BOOL
CTMLReadView::DoPreparePrinting(CPrintInfo* pInfo)
{
  TRACE ("CTMLReadView::DoPreparePrinting()\n");

  return CView::DoPreparePrinting(pInfo);  // default preparation
}

////
//// OnBeginPrinting
////  Gets things ready for printing
////  Arguments:
////    pDC is printing device context
////    pInfo is printing information
////  This is our first access to the printing DC, so we set up
////  several things needed later when OnPrint is called, as well
////  as rewrapping the words for the printed page and paginating.
////
void
CTMLReadView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
  TRACE ("CTMLReadView::OnBeginPrinting()\n");

  bPrinting = TRUE;
  SetJumpWord(GetTopWord());  // set word at top of screen for next screen redraw

  // set mapping mode so that a logical unit is the same size as a pixel
  //  on the display
  pDC->SetMapMode(MM_ANISOTROPIC);
  pDC->SetWindowExt(MulDiv(pDC->GetDeviceCaps(HORZRES), nDispLogPixelsX,
                           pDC->GetDeviceCaps(LOGPIXELSX)),
                    MulDiv(pDC->GetDeviceCaps(VERTRES), nDispLogPixelsY,
                           pDC->GetDeviceCaps(LOGPIXELSY)));
  pDC->SetViewportExt(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));

  // set up the rectangles in which drawing will occur
  rectPageRect = CRect (0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
  pDC->DPtoLP(rectPageRect); // entire page
  // now inset by margins
  rectPageRect.left += nPrtLMargin;
  rectPageRect.right -= nPrtRMargin;
  rectPageRect.top += nPrtTMargin;
  rectPageRect.bottom -= nPrtBMargin;

  // measure words and font heights
  MeasureWords (pDC, pInfo);
  // set up header area
  if (bPrtDoHeader) {
    rectPageHeaderRect = rectPageRect;
    rectPageHeaderRect.bottom = rectPageHeaderRect.top + 2*nPrtHdrFontHt;
    rectPageRect.top = rectPageHeaderRect.bottom;
  }

  // wrap words based on modified page width
  WrapWords (rectPageRect.Width(), pInfo);
  TRACE2("wrapped words for printing  width=%d  nlines=%d\n",
         rectPageRect.Width(), apLineArray.GetSize());

  // separate lines into pages
  Paginate (rectPageRect.Height());

  // set number of pages
  pInfo->SetMinPage(1);
  pInfo->SetMaxPage(apPageArray.GetSize());
}

////
//// OnEndPrinting
////  Clean things up after printing
////  Clears page and line arrays, sets flags so that words
////  are remeasured in display device context
////
void
CTMLReadView::OnEndPrinting(CDC *, CPrintInfo *)
{
  TRACE ("CTMLReadView::OnEndPrinting()\n");

  // delete pages and lines, set flags so next screen
  //  redraw will redo everything

  DeletePages();
  bPaginated = FALSE;
  DeleteLines();
  bWordsWrapped = FALSE;
  bWordsMeasured = FALSE;
  bPrinting = FALSE;
}

////
//// OnEndPrintPreview
////  Called by framework when Print Preview done
////  Arguments:
////    (see MFC docs; we do not use them here)
////
void
CTMLReadView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView *pView)
{
  TRACE ("CTMLReadView::OnEndPrintPreview()\n");

  CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

////
//// OnPrepareDC
////  Prepare device context before OnPrint() or OnDraw()
////  Arguments:
////    pDC is printing device context
////    pInfo is printing information
////  Calls base class function.  Since we use OnPrint and OnDraw
////  separately for printing and screen display, we do all necessary
////  device context setup in those routines.
////
void
CTMLReadView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  TRACE ("CTMLReadView::OnPrepareDC()\n");
  CView::OnPrepareDC(pDC, pInfo);
}

////
//// OnDraw
////  Redraws display
////  Arguments:
////    pDC is pointer to display device context
////  Sets up a few things which are not set up elsewhere;
////  remeasures words, wraps words, does jump if necessary;
////  sets up DC and draws display
////
void
CTMLReadView::OnDraw(CDC* pDC)
{
  TRACE ("CTMLReadView::OnDraw()\n");

  // set up display resolution for later use
  if (!nDispLogPixelsX) {  // check if we did it already
    nDispLogPixelsX = pDC->GetDeviceCaps(LOGPIXELSX);
    nDispLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);
  }

  if (pDoc != NULL) {
    // The following cannot be done in OnInitialUpdate
    //   because the information is not ready by then
    if (pDoc->DocTitle().GetLength()) { // sets window title
      pDoc->SetTitle(pDoc->DocTitle());
    }
    if (!bPrinting) {
      // do any of the following that are necessary
      if (!bWordsMeasured)
        MeasureWords (pDC, NULL);                  // remeasure words
      if (!bWordsWrapped)
        WrapWords (rectWindowRect.Width(), NULL);  // rewrap words
      if (IsJumpSet()) {
        WORD x;
        ResetScroll(x = GetJumpWord());  // move display to particular word
        TRACE1 ("Jumping to word %d\n", x);
      }
      // set up DC
      pDC->SetTextAlign(TA_TOP|TA_LEFT|TA_UPDATECP);
      // the following three are essentially equivalent to MM_TEXT
      pDC->SetMapMode(MM_ANISOTROPIC);
      pDC->SetWindowExt(rectWindowRect.Width(), rectWindowRect.Height());
      pDC->SetViewportExt(rectWindowRect.Width(), rectWindowRect.Height());

      // draw
      DrawView(pDC, nVScrollPos, apLineArray.GetSize(), rectWindowRect, NULL);

      // save top word on screen for possible later jumpback
      pDoc->SetLastTopWord(GetTopWord());
    }
  }
}

////
//// OnPrint
////  Do a page of printing
////  Arguments:
////    pDC is printing device context
////    pInfo is printing information
////  Note: base class function calls OnDraw(), so DO NOT CALL BASE
////
void
CTMLReadView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
  TRACE ("CTMLReadView::OnPrint()\n");

  // set mapping mode so that a logical unit is the same size as a pixel
  //  on the display
  pDC->SetTextAlign(TA_TOP|TA_LEFT|TA_UPDATECP);
  pDC->SetMapMode(MM_ANISOTROPIC);
  pDC->SetWindowExt(MulDiv(pDC->GetDeviceCaps(HORZRES), nDispLogPixelsX,
                           pDC->GetDeviceCaps(LOGPIXELSX)),
                    MulDiv(pDC->GetDeviceCaps(VERTRES), nDispLogPixelsY,
                           pDC->GetDeviceCaps(LOGPIXELSY)));
  pDC->SetViewportExt(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));

  // print page header
  if (bPrtDoHeader) {
    // set up name
    CString name = pDoc->DocTitle();
    if (name.GetLength() == 0)
      name = pDoc->DocDriveDir();
    // set up page number
    char pgnum[40];
    wsprintf (pgnum, "Page %d", pInfo->m_nCurPage);

    // now draw the two strings
    pDC->SaveDC();
    pDC->SelectObject(pfonPrtHdrFont);
    pDC->SetTextColor(colTextColor);
    pDC->SetTextAlign(TA_LEFT|TA_TOP);
    pDC->TextOut(rectPageHeaderRect.left, rectPageHeaderRect.top, name, name.GetLength());
    pDC->SetTextAlign(TA_RIGHT|TA_TOP);
    pDC->TextOut(rectPageHeaderRect.right, rectPageHeaderRect.top, pgnum, strlen(pgnum));
    // draw a line below the header
    if (bPrtDoHeaderHRule) {
      pDC->SelectObject(ppenTextPen);
      pDC->MoveTo(rectPageHeaderRect.left, rectPageHeaderRect.top+(5*nPrtHdrFontHt)/4);
      pDC->LineTo(rectPageHeaderRect.right, rectPageHeaderRect.top+(5*nPrtHdrFontHt)/4);
    }
    pDC->RestoreDC(-1);
  }
  // call DrawView to draw the rest
  int ipage = pInfo->m_nCurPage-1;
  int firstline = Page(ipage)->FirstLine();
  int lastline  = Page(ipage)->LastLine();
  DrawView(pDC, firstline, lastline, rectPageRect, pInfo);
}


////
//// DrawView
////  Draws words into a device context
////  Arguments:
////    pDC is the device context
////    linestart is the line in linearray at the top of the screen/page
////    linemax is the bottom+1 line to (potentially) draw -
////       - for screen display, this is the end of linearray, but for
////       printing, we may end the display short of the bottom of the
////       page so as not to get partial lines or images
////    drawrect is the rectangle within which the lines are displayed -
////       - for screen, it is entire window area, but for printer, the
////       margins and header are removed from the rectangle
////    pInfo is print info structure (used to indicate if
////      caller is a print function)
////  Traverses the line array, checks if line is within invalid area, and
////  if so, draws.  Attends to numerous details.
////
void
CTMLReadView::DrawView(CDC *pDC, int linestart, int linemax,
                               CRect drawrect, CPrintInfo *pInfo)
{
  if (bPrinting && !pInfo)  // if printing, only do this for print functions
    return;

  ASSERT(pDoc != NULL);

  pDC->SaveDC(); // save for restoration before exit

  int py = drawrect.top;              // starting y coordinate
  COLORREF colorlast = colTextColor;  // last text color
  pDC->SetTextColor(colTextColor);
  pDC->SetBkMode(TRANSPARENT);        // transparent characters
  pDC->SelectObject(ppenTextPen);     // pen for hrules
  pDC->SelectObject(pbruTextBrush);   // brush for bullets

  CFont *pfontlast = apfonHFonts[1];  // initial font selection
  pDC->SelectObject(pfontlast);

  CRect cliprect;                     // get clipping rectangle
  pDC->GetClipBox(&cliprect);

  CTMLLine *line;
  for (int iline = linestart;              // first line
       iline < linemax && py < drawrect.bottom;   //  continue while lines & visible
       py += line->YSize(), ++iline) {     //  increment vertical pos & line num
    line = Line(iline);                    // get line from array

    // blank line, no work
    if (line->Blank())
      continue;

    BOOL imageline = FALSE; // set later if line has image

    // check if line falls within the invalid rectangle
    if (py+line->YSize() <= cliprect.top || py >= cliprect.bottom)
      continue;

    // start line by indenting
    int px = drawrect.left + line->Indent();
    pDC->MoveTo(px,py);

    int last_ilink = -1; // previous word's ilink
    CPoint linkxy;       // start of link underline

    // stuff needed for loop through words on line
    int iword;
    CTMLWord *word, *nextword;
    char textoutbuf[1024];
    textoutbuf[0] = 0;

    // loop through words on the line
    for (iword = line->FirstWord(),
          word = pDoc->Word(iword),
          nextword = NULL;
         iword <= line->LastWord();
         iword++, word = nextword) {
      nextword = (iword+1 <= line->LastWord() ?
                  pDoc->Word(iword+1) :
                  NULL);
      // image
      if (word->IsType(TMLWORD_TYPE_IMAGE)) {
        // need to draw image from first line of image
        CTMLLine *imgline = Line(line->FirstLine());
        CSize isize = pDoc->Image(word->IImage())->Size();
        CRect drect;
        drect.top = py - (line->YOffset()-imgline->YOffset()) + nParVSpace;
        drect.bottom = drect.top + isize.cy;
        drect.left = drawrect.left + line->Indent();
        drect.right = drect.left + isize.cx;
        pDoc->Image(word->IImage())->Draw(pDC, drect);
        imageline = TRUE;
      }
      // horizontal rule
      else if (word->IsType(TMLWORD_TYPE_HRULE)) {
        pDC->MoveTo(drawrect.left,py+line->YSize()/2);
        pDC->LineTo(drawrect.right,py+line->YSize()/2);
      }
      else { // LITERAL and WORD treated together
        CFont *pfont; // our font
        COLORREF color; // color

        // decide color
        if (word->HasContextFlag(TMLWORD_CONTEXT_LINK)) {
          color = colHyperColor;
        }
        else
          color = colTextColor;

        // decide font
        if (word->IsContextBlock(TMLWORD_CONTEXT_HEADING))
          pfont = apfonHFonts[word->HLevel()];
        else   // anything not a heading
          pfont = apfonPFonts[anStyleMap[word->Style()]];

        // select font and color
        if (pfont != pfontlast) {
          pDC->SelectObject(pfont);
          pfontlast = pfont;
        }
        if (color != colorlast) {
          pDC->SetTextColor(color);
          colorlast = color;
        }

        // literal text... no leading spaces
        if (word->IsType(TMLWORD_TYPE_LITERAL)) {
            pDC->TextOut(0, 0, word->Contents(), (word->Contents()).GetLength());
        }
        else { // normal word
          if (iword == line->FirstWord()) {
            // bullet item
            if (word->IsContextBlock(TMLWORD_CONTEXT_ULIST) &&
                word->HasContextFlag(TMLWORD_CONTEXT_FIRST)) {
              CPoint oldpos = pDC->GetCurrentPosition();
              CPoint bulpos(oldpos);
              CSize  bulsize(4,4);
              bulpos.Offset(-anPSpaceWid[0]*2, anPFontHt[0]/2);
              bulpos.Offset(-bulsize.cx, -bulsize.cy/2);
              CRect bullet(bulpos, bulsize);
              pDC->Ellipse(&bullet);
              pDC->MoveTo(oldpos);
            }
            // numbered item
            else if (word->IsContextBlock(TMLWORD_CONTEXT_OLIST) &&
                     word->HasContextFlag(TMLWORD_CONTEXT_FIRST)) {
              pDC->SaveDC();
              pDC->SetTextColor(colTextColor);
              pDC->SetTextAlign(TA_RIGHT|TA_TOP|TA_UPDATECP);
              char buf[40];
              wsprintf (buf, "%d. ", word->OLItem());
              pDC->TextOut(0,0, buf, strlen(buf));
              pDC->RestoreDC(-1);
            }
          }
          else { // not first on line
            if (word->HasContextFlag(TMLWORD_CONTEXT_SENT))
              strcat (textoutbuf, "  ");
            else
              strcat (textoutbuf, " ");
          }
          strcat (textoutbuf, word->Contents());

          // skip TextOut if possible to speed things up...
          // we can skip writing the textoutbuf only under certain conditions:
          if (nextword != NULL &&
              word->Type() == nextword->Type() &&
              word->Style() == nextword->Style() &&
              !word->HasContextFlag(TMLWORD_CONTEXT_LINK) &&
              !nextword->HasContextFlag(TMLWORD_CONTEXT_LINK)) {
          }
          // can't skip, so write out text
          else {
            pDC->TextOut (0, 0, textoutbuf, strlen(textoutbuf));
            textoutbuf[0] = 0;
          }
        }
        // underline hyperlinks
        if (word->HasContextFlag(TMLWORD_CONTEXT_LINK)) {
          if (last_ilink == word->ILink()) { // connect to previous
          }
          else {                 // start fresh
            linkxy = pDC->GetCurrentPosition();
            int wordbaseline =
                  (word->IsContextBlock(TMLWORD_CONTEXT_HEADING) ?
                        anHFontAsc[word->HLevel()] :
                        anPFontAsc[anStyleMap[word->Style()]]);
            linkxy.Offset(-word->XSize(), wordbaseline+1);
            last_ilink = word->ILink();
          }
          CPoint save = pDC->MoveTo(linkxy);
          pDC->SelectObject(ppenHyperPen); // underline in same color
          pDC->LineTo(save.x, linkxy.y);
          pDC->MoveTo(save);
          pDC->SelectObject(ppenTextPen);
        }
        else
          last_ilink = -1;  //not in link
      } // end of LITERAL and WORD
    }  // end of for()

    // if we just drew an image, step past subsequent lines
    //   containing pointer to same image
    // stop when we reach last line in image
    if (imageline) {
      WORD firstline = line->FirstLine(); // save first image line #
      for (int jline = iline+1; jline < linemax; jline++) {
        CTMLLine *nextline = Line(jline);
        if (nextline->FirstLine() != firstline)
          break;
        else {
          py += line->YSize();
          ++iline;
          line = nextline;
        }
      }
    }
  } // end of for()
  pDC->RestoreDC(-1); // restore device context
}

///////////////////////////////////
/////////////////////////////////// Message Handlers
///////////////////////////////////

////
//// OnLButtonDown
////  Respond to left button click
////  Arguments:
////    nFlags is flags
////    point is click point
////  Look for a link box under the click point; if
////  found, do the hyperlink.
////
void
CTMLReadView::OnLButtonDown(UINT nFlags, CPoint point)
{
  TRACE ("CTMLReadView::OnLButtonDown()\n");

  int linkto = GetILinkBoxFromPt(point); // search for link box

  if (linkto >= 0) { // if found, do link stuff
    CTMLLink *link = pDoc->Link(linkto);
    if ((link->File()).GetLength() > 0) { // have to jump files
      CString filename = link->File();
      CString anchor = link->Anchor();
      pDoc->HyperLink(filename, anchor);  // make document try to load new file
    }
    else // have to find link in current file
      JumpToAnchor(link->Anchor());
  }
}

////
//// OnSetCursor
////

BOOL CTMLReadView::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
    BOOL         bRetval = FALSE;                           // msg not handled
    CTMLReadApp* pApp    = (CTMLReadApp*)::AfxGetApp();
    HCURSOR      hcur;

    if ( nHitTest == HTCLIENT )
    {
        // Otherwise, cursor is in client area: we handle it:
        // get mouse cursor as a POINT;
        // convert to client coords (ScreenToClient)
        POINT pt;
        ::GetCursorPos( &pt);
        ScreenToClient( &pt);

        // Figure out which cursor to use
        if ( GetILinkBoxFromPt(pt) != -1 )
            hcur = pApp->LoadHyperlinkCursor();             // index finger cursor
        else
            hcur = pApp->LoadStandardCursor( IDC_ARROW);    // default cursor

        bRetval = TRUE;                                     // handle the msg
    }
    else if ( nHitTest == HTVSCROLL )       // odd that this should be needed...
    {
        hcur = pApp->LoadStandardCursor( IDC_ARROW);        // default cursor
        bRetval = TRUE;                                     // handle the msg
    }

    if ( bRetval )
        SetCursor( hcur);

    return bRetval;
}


////
//// OnSize
////  Respond to window resize message
////  Arguments:
////    nType is sizing message type
////    cx, cy are new size
////  Saves the new window size, rewraps words if necessary
////
void
CTMLReadView::OnSize(UINT nType, int cx, int cy)
{
  TRACE2 ("CTMLReadView::OnSize(%d,%d)\n",cx,cy);

  if (rectWindowRect.right != cx && cx > 0 && bWordsWrapped) {
    WORD topword;
    topword = GetTopWord(); // save top word for later restoration to top line
    WrapWords(cx, NULL);
    ResetScroll(topword);
  }

  rectWindowRect.SetRect(0,0,cx,cy);
}

////
//// OnVScroll
////  Handle vertical scroll messages
////  Arguments:
////    nSBCode is the scrolling message type
////    nPos is the scrollbar position
////  Calls utility function to deal with it
////
void
CTMLReadView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
  TRACE ("CTMLReadView::OnVScroll()\n");

  HandleScroll (nSBCode, nPos);
}

////
//// OnKeyDown
////  Respond to keypresses
////  Arguments:
////    nChar is character code
////  The useful characters are the cursor keys, which we
////  change to a scroll bar message code and pass to HandleScroll()
////  There are also keypresses to show some internal program data,
////  currently only implemented if _DEBUG defined during compile.
////
void
CTMLReadView::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
  TRACE ("CTMLReadView::OnKeyDown()\n");

  BOOL movementkey = TRUE;  // assume it's a cursor movement key
  UINT scrollcode;
  // set scroll code according to key pressed
  switch (nChar) {
    case VK_HOME:  scrollcode = SB_TOP;    break;
    case VK_END:   scrollcode = SB_BOTTOM; break;
    case VK_UP:    scrollcode = SB_LINEUP; break;
    case VK_DOWN:  scrollcode = SB_LINEDOWN; break;
    case VK_PRIOR: scrollcode = SB_PAGEUP;   break;
    case VK_NEXT:  scrollcode = SB_PAGEDOWN; break;
    default: movementkey = FALSE; break; // oops, not a movement key
  }
  // call utility function to deal with scrolling
  if (movementkey)
    HandleScroll(scrollcode, 0);

#ifdef _DEBUG
// keys for debugging or other things
  if (nChar >= 0x31 && nChar <= 0x39) {
    char buf[120];
    buf[0] = 0;
    switch (nChar) {
      case 0x31: // '1'
        wsprintf (buf, "Document: %d words, %d links, %d anchors",
             pDoc->NumWords(), pDoc->NumLinks(), pDoc->NumAnchors());
        break;
      case 0x32: // '2'
        wsprintf (buf, "Display: %d lines, %d linkboxes",
                       apLineArray.GetSize(), apLinkBoxArray.GetSize());
        break;
      case 0x33: // '3'
        bHandleThumbTrack = (bHandleThumbTrack ? FALSE : TRUE);
        wsprintf (buf, "Thumb tracking toggled, now %d", (int)bHandleThumbTrack);
        break;
      case 0x39: // '9'
        TRACE("Tripping assertion failure:\n");
        ASSERT (FALSE);
        break;
    }
    if (buf[0])
      AfxMessageBox (buf);
  }
#endif
}

////
//// OnEraseBkgnd
////  Handle Erase Background message
////  Arguments:
////    pDC is device context to be erased
////  Returns:
////    TRUE
////  This allows us to paint the background whatever color we like
////
BOOL CTMLReadView::OnEraseBkgnd(CDC* pDC)
{
  TRACE ("CTMLReadView::OnEraseBkgnd()\n");

  CRect rect;
  pDC->GetClipBox(&rect);
  pDC->FillRect(&rect, pbruBkgBrush);

  return TRUE;
}

////
//// OnPrevfile
////  Handle the Previous File menu command (and toolbar icon)
////  This command causes us to jump back to the previously
////  displayed file, at the point where we left it
////
void
CTMLReadView::OnPrevfile()
{
  // TODO: Add your control command handler code here
  pDoc->JumpBack();
}

////
//// OnUpdatePrevfile
////  Enables/Disables Previous File menu command (and toolbar icon)
////  Arguments:
////    pCmdUI is command UI used to enable/disable menu item
////  Command is enabled if there are any previous files on
////  the document's stack
////
void
CTMLReadView::OnUpdatePrevfile(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(pDoc->IsPrevFile());
}

///////////////////////////////////
/////////////////////////////////// Message Handling Utilities
///////////////////////////////////
////
//// GetILinkBoxFromPt
////  Searches for hit on linkbox
////  Arguments:
////    point is click point
////  Returns:
////    link box index, or -1 if no hit
////
int
CTMLReadView::GetILinkBoxFromPt(CPoint point)
{
  int linkto = -1;

  // double-check that there are lines and that we are within bounds
  if (apLineArray.GetSize() != 0 && nVScrollPos < apLineArray.GetSize()) {
    long yoffset = Line(nVScrollPos)->YOffset();
    int nboxes = apLinkBoxArray.GetSize();
    // look through boxes start to finish
    // this is inelegant but adequate
    for (int i = 0; i < nboxes; i++) {
      if (LinkBox(i)->IsHit(point, yoffset)) {
        linkto = LinkBox(i)->ILink();
        break;
      }
    }
  }
  return linkto;
}

////
//// HandleScroll
////  Utility function to handle mouse or keyboard scrolling
////  Arguments:
////    code is scroll message code
////    pos is scroll position (thumb messages)
////  x
////
void
CTMLReadView::HandleScroll(UINT code, UINT pos)
{
  int nlines = apLineArray.GetSize(); // for easy access

  // easy cases:
  if (nlines <= 1) return;

  // things we know at start:
  //   int nlines     - number of lines
  //   int nVScrollPos - first line on screen
  //   int windowy    - window y size

  int vinc = 0;  // scroll increment, in lines

  // +++be careful for signed vs. unsigned, long vs. short arithmetic+++
  // set scroll increment based on code from caller
  switch (code) {
    case SB_THUMBTRACK:
      if (bHandleThumbTrack)
        vinc = (int)pos - nVScrollPos;
      break;
    case SB_THUMBPOSITION:
      vinc = (int)pos - nVScrollPos;
      break;
    case SB_LINEUP:
      if (nVScrollPos > 0)
        vinc = -1;
      break;
    case SB_LINEDOWN:
      if (nVScrollPos < nlines-1)
        vinc = 1;
      break;
    case SB_TOP:
      vinc = -nVScrollPos;
      break;
    case SB_BOTTOM:
      vinc = nlines - 1 - nVScrollPos;
      break;
    case SB_PAGEUP:
    case SB_PAGEDOWN:
      vinc = HandleScrollPage(code);
      break;
    default:
      break;
  }
  // do scrolling
  if (vinc) {
    int newscrollpos = nVScrollPos + vinc;
    long vscrollpix = Line(nVScrollPos)->YOffset() - Line(newscrollpos)->YOffset();
    // invalidate entire window if necessary
    if ((vscrollpix < 0 && vscrollpix < -rectWindowRect.Height()) &&
        (vscrollpix > 0 && vscrollpix > rectWindowRect.Height())) {
      Invalidate (TRUE);
    }
    // scroll part of the window if possible
    else {
      ScrollWindow(0, (int)vscrollpix, NULL, NULL);
    }
    // update scroll bar
    nVScrollPos = newscrollpos;
    SetScrollRange (SB_VERT, 0, nVScrollMax, FALSE);
    SetScrollPos(SB_VERT, nVScrollPos, TRUE);
  }
}

////
//// HandleScrollPage
////  Utility function to figure out proper scroll increment for pageup/down
////  Arguments:
////    code is scroll message code (SB_PAGEUP or SB_PAGEDOWN)
////  Returns:
////    scroll increment
////  This is tricky as it requires us to walk through the line array until
////   we have the right amount of vertical space to scroll properly.
////
int
CTMLReadView::HandleScrollPage(UINT code)
{
  int nlines = apLineArray.GetSize(); // for easy access
  int vinc = 0;
  long yoffset0 = Line(nVScrollPos)->YOffset();
  CTMLLine *line;

  // deal with PageUp
  if (code == SB_PAGEUP) {
    if (nVScrollPos > 0) {
      // walk through array until we either hit top or have overstepped
      //  the window height
      for (int i = nVScrollPos-1; i >= 0; i--) {
        if (yoffset0 - Line(i)->YOffset() > rectWindowRect.Height())
          break;
      }
      // we have gone one too far...
      vinc = i-nVScrollPos+1;
      if (!vinc) vinc = -1;  // be sure to go up one, in any case
    }
  }

  // deal with PageDown
  else {
    if (nVScrollPos < nlines-1) {
      // walk through array until we either hit bottom or have
      //  overstepped the window height
      for (int i = nVScrollPos+1; i < nlines; i++) {
        if (Line(i)->YOffset() - yoffset0 > rectWindowRect.Height())
          break;
      }
      // we have gone one too far...
      vinc = i - nVScrollPos - 1;
      if (!vinc) vinc = 1;  // be sure to go down one, in any case
    }
  }
  return vinc;
}

///////////////////////////////////
/////////////////////////////////// Miscellaneous Utilites
///////////////////////////////////
////
//// GetTopWord
////  Return index of top word on screen
////  Returns:
////    index of top word on screen
////  This is only slightly tricky because the first line on the screen
////  may be blank.  Walk through lines until we find a word.
////
WORD
CTMLReadView::GetTopWord()
{
  int nlines = apLineArray.GetSize(); // for easy access
  WORD retval;

  if (!nlines || pDoc == NULL || !pDoc->NumWords())
    retval = 0;
  else {
    retval = pDoc->NumWords() - 1; // default
    CTMLLine *line;
    // step through lines, starting at top line on screen, until
    //   we find one with a word in it
    for (int iline = nVScrollPos; iline < nlines; ++iline) {
      line = Line(iline); // get line from array
      if (line->Blank())
        continue;
      else {
        retval = line->FirstWord();
        break;
      }
    }
  }
  return retval;
}

////
//// ResetScroll
////  Resets scroll bar range and position
////  Arguments:
////    topword is index of word that should be on top line
////  Used to jump to a particular word in the file
////
void
CTMLReadView::ResetScroll(WORD topword)
{
  int nlines = apLineArray.GetSize(); // for easy access

  nVScrollMax = nlines - 1;

  // often we just want to go to top of file
  if (topword == 0)
    nVScrollPos = 0;
  else {
    CTMLLine *line;
    // walk through lines to find the one with the particular word
    for (int iline = 0; iline < nlines; ++iline) {
      line = Line(iline); // get line from array
      if (line->Blank())
        continue;
      else {
        if (line->ContainsWord(topword)) {
          nVScrollPos = iline;
          break;
        }
      }
    }
  }
  if (nVScrollMax <= 0)
    nVScrollMax = 1;      // to make sure we always have a scroll bar

  SetScrollRange (SB_VERT, 0, nVScrollMax, FALSE);
  SetScrollPos(SB_VERT, nVScrollPos, TRUE);
}

////
//// JumpToAnchor
////  Jump to the named anchor
////  Arguments:
////    aname is name of anchor
////  Calls document to find the anchor, then resets scrolling
////  position to that word.  Displays error box if anchor unknown.
////
void
CTMLReadView::JumpToAnchor(CString aname)
{
  WORD iword;
  if (pDoc->FindAnchor(aname, iword)) { // call doc to locate
    ResetScroll (iword);
    Invalidate(); // force redraw
  }
  else
    AfxMessageBox("Hyperlink anchor not found");
}

////
//// OnViewPrefs
////  Command handler for View | Preferences menu item
////  Initializes a CPrefDialog Preferences dialog,
////  displays it modally, and updates CTMLReadView if user clicked OK
////
void CTMLReadView::OnViewPrefs()
{
	// TODO: Add your control command handler code here
    CPrefDialog dlgPref;

    // Initialize CPrefDialog data members with current
    // values from CTMLReadView
    dlgPref.nParVSpace = nParVSpace;
    dlgPref.nMargin    = nMargin;
    dlgPref.nIndent    = nIndent;

    // Display as a modal dialog.
    // If user clicks OK, DoDataExchange will be called
    // to validate data in the controls. If the controls
    // hold valid values, their contents will be transferred
    // to the CPrefDialog data members.  In that case,
    // we must transfer the values to the corresponding
    // CTMLReadView data members.
    if ( dlgPref.DoModal() == IDOK )
    {
        // Transfer the data to our view class
        nParVSpace = dlgPref.nParVSpace;
        nMargin    = dlgPref.nMargin;
        nIndent    = dlgPref.nIndent;

        // Make sure that the view is redrawn to reflect
        // the new preferences
        bWordsWrapped = FALSE;
    	OnUpdate( NULL, 0L, NULL);
    }
}

