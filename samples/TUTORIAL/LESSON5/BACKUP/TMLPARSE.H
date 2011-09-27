/// tmlparse.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   #defines for parser
//   CTMLParser class declaration
// Notes:
//   An object of this type is created by the document and used to read
//    the input file.  The parser object builds the document's data arrays
//    based on the input file contents.

#ifndef _TMLPARSE_H
#define _TMLPARSE_H

// Error codes returned by CTMLParser::Parse(),
//  correspond to error messages returned by CTMLParser::ErrorMessage()

#define PARSE_ERROR_LONGLINE 2
#define PARSE_ERROR_MAXWORDS 3
#define PARSE_ERROR_BADCODE  4
#define PARSE_ERROR_BADARG   5
#define PARSE_ERROR_MAXLINKS 6
#define PARSE_ERROR_MAXANCHORS 7
#define PARSE_ERROR_MAXIMAGES  8

#define PARSE_MAXWORDSIZE 1024  // maximum length of a word, in characters
#define PARSE_ARCHBUFSIZE 1024  // file input buffer size

//// CTMLParser
////  Class to read and parse an TML file.
////  A single instance of this object is created during the document's
////  serialize function.  Pointers to the file and to arrays where the data
////  should be stored are passed in the parser's constructor, then the
////  Parse() function is called to do the work.

class CTMLParser {
  public:
    CTMLParser (CArchive &, CString &, int &,
                  CPtrArray &, CPtrArray &,
                  CPtrArray &, CPtrArray &);
    ~CTMLParser () {}

    int Parse ();              // do parsing
    char *ErrorMessage(int);   // return error message

  protected:
    // utility functions
    int ReadCode();                    // read and interpret code
    int ReadArg(char *, char *, int);  // read code argument
    char ReadEscape();                 // read character escape
    int EndWord();                     // write word into output buffers
    int MakeLink();                    // make new link
    int MakeAnchor();                  // make new anchor
    int MakeImage();                   // make new image

  private:
    // these are references to document data; this is where
    //   we put all the information we read from the file
    CArchive &arcArchive;           // where to get file input
    CString &strTitle;         // where to store title
    int &nErrorLine;         // line number on error
    CPtrArray &apWordArray;   // where to add words
    CPtrArray &apLinkArray;   // where to put links
    CPtrArray &apAnchorArray; // where to put anchors
    CPtrArray &apImageArray;  // where to put images

    // this structure keeps track of the last context, style, etc.
    //  and is used to store data in the word
    struct State {
      State() { Reset(); } // reset when constructed
      BYTE type;     // type - WORD, LITERAL, HRULE, IMAGE
      BYTE style;    // character style bits (for WORD or LITERAL)
      WORD contextflags;// context bits - flags
      WORD contextblock;// context bits - block type
      WORD hlevel;   // header level (1 to 6, only in HEADING context)
      WORD olitem;   // item number (1 or more, only in OLIST context)
      WORD ulnest;   // nesting (1 or more, only in ULIST context)
      WORD ilink;    // link index (if context is LINK or ANCHOR)
      WORD iimage;   // image index (if IMAGE)
      CString arg1;  // arguments to hyperlink and image commands
      CString arg2;
      // reset the state
      void Reset() { type = TMLWORD_TYPE_WORD;
                     contextflags = contextblock = TMLWORD_CONTEXT_NONE;
                     style = TMLWORD_STYLE_NONE;
                     hlevel = olitem = ulnest = ilink = iimage = 0; }
    };

    // these are shared by parser functions
    WORD nNumWords;                     // words so far
    char sCurWord[PARSE_MAXWORDSIZE]; // current word
    int nIChar;                       // index into current word
    State stateCurState;                     // state


    // this buffers the input from CArhive
  protected:
    int  ReadChar(char *);  // gets next char; reads more if necessary

  private:
    char sArchBuf[PARSE_ARCHBUFSIZE];  // buffer for CArchive input
    int  nIArchBuf;                    // pointer in input buffer
    int  nNArchBuf;                    // number of chars in input buffer
    BOOL bArchEOF;                     // flag for archive end-of-file
};

#endif
