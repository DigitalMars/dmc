// tmlparse.cpp
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLParser member functions
// Notes:
//   An object of this type is created by the document and used to read
//    the input file.  The parser object builds the document's data arrays
//    based on the input file contents. The pointers to the data arrays
//    are set up in the parser constructor. The Parse() function is called
//    to do the parsing.
//   The command codes and interpretations thereof correspond largely to
//    standard HTML (as described in "A Beginner's Guide to HTML", available
//    at your nearest WWW browser).  One significant difference is that in
//    TMLRead, all images are placed on separate lines.  See example .TML files.

#include "stdafx.h"
#include "dibapi.h"

#include <ctype.h>
#include <string.h>

#include "tmlword.h"
#include "tmllink.h"
#include "tmlimage.h"
#include "tmlparse.h"


////
//// Constructor
////  Initializes class data
////
////
CTMLParser::CTMLParser (CArchive &a, CString &t, int &el,
                            CPtrArray &wa, CPtrArray &la,
                            CPtrArray &aa, CPtrArray &ia) :
   arcArchive(a), strTitle(t), nErrorLine (el),
   apWordArray(wa), apLinkArray(la),
   apAnchorArray(aa), apImageArray(ia), nNArchBuf(0), bArchEOF(FALSE)
{}

////
//// ReadChar
////  Reads a character from the archiver
////  Arguments:
////    pc is pointer to char where character is put
////  Returns:
////    number of characters read (1 or 0)
////
//// Buffering is necessary because CArchive only allows you to hit EOF once;
////  if you try to read after that, it goes off into nowhere...
//// It does not actually speed up the program, it takes about as long
////  to read the file buffered in this way as it did to read characters
////  one by one directly from CArchive.
int
CTMLParser::ReadChar (char *pc)
{
  // if nothing in buffer, or buffer is used up, read more
  if (!bArchEOF && (!nNArchBuf || nIArchBuf >= nNArchBuf)) {
    nNArchBuf = arcArchive.Read(sArchBuf, PARSE_ARCHBUFSIZE);
    nIArchBuf = 0;  // point to beginning of buffer
    if (nNArchBuf < PARSE_ARCHBUFSIZE)
      bArchEOF = TRUE;  // we hit EOF
  }

  // if there are characters in buffer, give one to caller
  if (nIArchBuf < nNArchBuf) {
    *pc = sArchBuf[nIArchBuf++];
    return 1;
  }
  else  // must be end of file
    return 0;
}

////
//// Parse
////  Parses input file
////  Returns:
////    error code, or 0 if no error
////  This is called from document's Serialize function;
//// all the necessary pointers and such have been set
//// up already in the constructor.  This function reads characters
//// and deals with some; it calls other functions to interpret
//// commands and escape sequences.
////
int
CTMLParser::Parse ()
{
  int error = 0;
  char c;        // current character

  nNumWords = 0; // start with no words
  nIChar = 0;    // and no characters in first word

  // continue loop while there are characters
  while (ReadChar(&c)) {
    if (nIChar >= PARSE_MAXWORDSIZE) {  // check for word too long here
      error = PARSE_ERROR_LONGLINE;
      break;
    }
    if (c == '<') {                          // markup command
      if ((error = EndWord ()) != 0) break;  // end previous word
      if ((error = ReadCode ()) != 0) break; // reads code and changes state
      // on return need to check for hrule or image...
      //  these codes generate their own word
      if (stateCurState.type == TMLWORD_TYPE_HRULE ||
          stateCurState.type == TMLWORD_TYPE_IMAGE) {
        if ((error = EndWord ()) != 0) break;
      }
    }
    // characters between commands
    else {
      if (c == '&')          // text escape
        c = ReadEscape ();   // replace with real character
      // linefeed, carriage return, formfeed
      if (c == 0x0d || c == 0x0a || c == '\f') {
        if (c == 0x0a) // keep track of line number in case of error
          ++nErrorLine;
        // preformatted text: use to break line
        if (stateCurState.contextblock == TMLWORD_CONTEXT_PREFORM) {
          if ((error = EndWord ()) != 0) break;
          stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
          continue;
        }
        // if not preformatted, just change to space
        else
          c = ' ';
      }
      if (c == '\t') // change tabs to spaces too
        c = ' ';

      // space
      if (c == ' ') {
        // preformatted and title keep spaces
        if (stateCurState.contextblock == TMLWORD_CONTEXT_PREFORM ||
            stateCurState.type == TMLWORD_TYPE_TITLE) {
          sCurWord[nIChar++] = c;
        }
        // otherwise, space breaks word
        else
          if ((error = EndWord ()) != 0) break;
      }
      // not a space; add character to word
      else
        sCurWord[nIChar++] = c;
    }
  }

  // end last word
  if (!error)
    error = EndWord();

  return error;
}

////
//// TML code ID's, used to associate each code with a number
////
enum MLCodeID {
  ML_NONE,
  ML_TITLE, ML_xTITLE,
  ML_H1, ML_xH1, ML_H2, ML_xH2, ML_H3, ML_xH3,
  ML_H4, ML_xH4, ML_H5, ML_xH5, ML_H6, ML_xH6,
  ML_P, ML_BR, ML_HR, ML_IMG,
  ML_UL, ML_xUL, ML_OL, ML_xOL, ML_LI,
  ML_PRE, ML_xPRE,
  ML_BLOCKQUOTE, ML_xBLOCKQUOTE,
  ML_DL, ML_DT, ML_DD, ML_xDL,
  ML_A, ML_xA,
  ML_DFN, ML_xDFN, ML_EM, ML_xEM, ML_CITE, ML_xCITE, ML_VAR, ML_xVAR,
  ML_I, ML_xI, ML_CODE, ML_xCODE, ML_KBD, ML_xKBD, ML_SAMP, ML_xSAMP,
  ML_TT, ML_xTT, ML_STRONG, ML_xSTRONG, ML_B, ML_xB
};

////
//// association between code IDs and TML code strings
////
static struct {
  char *mlcode;
  MLCodeID id;
} codelist[] = {
  { "TITLE",      ML_TITLE },
  { "/TITLE",     ML_xTITLE },
  { "H1",         ML_H1 },
  { "/H1",        ML_xH1 },
  { "H2",         ML_H2 },
  { "/H2",        ML_xH2 },
  { "H3",         ML_H3 },
  { "/H3",        ML_xH3 },
  { "H4",         ML_H4 },
  { "/H4",        ML_xH4 },
  { "H5",         ML_H5 },
  { "/H5",        ML_xH5 },
  { "H6",         ML_H6 },
  { "/H6",        ML_xH6 },
  { "P",          ML_P },
  { "BR",         ML_BR },
  { "HR",         ML_HR },
  { "IMG",        ML_IMG },
  { "UL",         ML_UL },
  { "/UL",        ML_xUL },
  { "OL",         ML_OL },
  { "/OL",        ML_xOL },
  { "LI",         ML_LI },
  { "PRE",        ML_PRE },
  { "/PRE",       ML_xPRE },
  { "BLOCKQUOTE",   ML_BLOCKQUOTE },
  { "/BLOCKQUOTE",  ML_xBLOCKQUOTE },
  { "DL",         ML_DL },
  { "DT",         ML_DT },
  { "DD",         ML_DD },
  { "/DL",        ML_xDL },
  { "A",          ML_A },
  { "/A",         ML_xA },
  { "DFN",        ML_DFN },
  { "/DFN",       ML_xDFN },
  { "EM",         ML_EM },
  { "/EM",        ML_xEM },
  { "CITE",       ML_CITE },
  { "/CITE",      ML_xCITE },
  { "VAR",        ML_VAR },
  { "/VAR",       ML_xVAR },
  { "I",          ML_I },
  { "/I",         ML_xI },
  { "CODE",       ML_CODE },
  { "/CODE",      ML_xCODE },
  { "KBD",        ML_KBD },
  { "/KBD",       ML_xKBD },
  { "SAMP",       ML_SAMP },
  { "/SAMP",      ML_xSAMP },
  { "TT",         ML_TT },
  { "/TT",        ML_xTT },
  { "STRONG",     ML_STRONG },
  { "/STRONG",    ML_xSTRONG },
  { "B",          ML_B },
  { "/B",         ML_xB },
  { NULL, ML_NONE }            // note terminator
};


////
//// ReadCode
////  Reads and interprets a markup code
////  Returns:
////    error code, 0 if no error
////  Note that the leading '<' has been read already.
////
int
CTMLParser::ReadCode ()
{
  char buf[PARSE_MAXWORDSIZE];   // code buffer
  int  ibuf = 0;                 // index into code buffer
  int  error = 0;

  // part one: get code out of file
  char c;
  while (ReadChar(&c)) {
    if (ibuf >= PARSE_MAXWORDSIZE)  // check for word too long here
      return PARSE_ERROR_BADCODE;
    if (c == '>')           // markup command end
      break;
    else                    // just another character
      buf[ibuf++] = c;
  }
  buf[ibuf] = 0;            // terminate string


  // part two: divide into code and arg
  char *parg, *parg2;
  for (parg = buf; *parg && !isspace(*parg); parg++){}; // find null or space
  parg2 = parg; // save this point
  for (; *parg && isspace(*parg); parg++){};  // continue through white space
  *parg2 = 0;   // ends first part, which is code
  // at this point parg either points to null or to start of argument


  // part three: identify the code by trying to match them
  //  one-by-one
  MLCodeID code = ML_NONE;
  for (int i = 0; codelist[i].mlcode != NULL; i++) {
    if (stricmp(buf, codelist[i].mlcode) == 0) {
      code = codelist[i].id;
      break;
    }
  }


  // part four: deal with the code... giant switch
  //  This is where we interpret the code to mean something,
  //  by setting up the stateCurState with flags and other
  //  information
  switch (code) {
    case ML_NONE: // code not found, so do nothing...
      break;
    case ML_TITLE:    // Title requires special extraction
      stateCurState.type = TMLWORD_TYPE_TITLE;
      break;
    case ML_xTITLE:   // end of title
      stateCurState.Reset();
      break;
    case ML_H1:       // headers
    case ML_H2:
    case ML_H3:
    case ML_H4:
    case ML_H5:
    case ML_H6:
      stateCurState.Reset();
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      stateCurState.contextblock = TMLWORD_CONTEXT_HEADING;
      switch (code) {
        case ML_H1: stateCurState.hlevel = 1; break;
        case ML_H2: stateCurState.hlevel = 2; break;
        case ML_H3: stateCurState.hlevel = 3; break;
        case ML_H4: stateCurState.hlevel = 4; break;
        case ML_H5: stateCurState.hlevel = 5; break;
        case ML_H6: stateCurState.hlevel = 6; break;
      }
      break;
    case ML_xH1:      // end of headers
    case ML_xH2:
    case ML_xH3:
    case ML_xH4:
    case ML_xH5:
    case ML_xH6:
      stateCurState.Reset();
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
      break;
    case ML_P:        // new paragraph
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      break;
    case ML_BR:       // line break
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
      break;
    case ML_HR:       // horizontal rule
      stateCurState.type = TMLWORD_TYPE_HRULE;
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
      break;
    case ML_IMG:      // image
      stateCurState.type = TMLWORD_TYPE_IMAGE;
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
      if ((error = ReadArg ("SRC", parg, 1)) == 0)
        error = MakeImage ();
      break;
    case ML_OL:       // ordered list
      stateCurState.Reset();
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      stateCurState.contextblock = TMLWORD_CONTEXT_OLIST;
      stateCurState.olitem = 0;
      break;
    case ML_LI:       // item in ordered or unordered list
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      stateCurState.olitem++;
      break;
    case ML_PRE:      // preformatted
      stateCurState.Reset();
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      stateCurState.contextblock = TMLWORD_CONTEXT_PREFORM;
      stateCurState.type = TMLWORD_TYPE_LITERAL;
      stateCurState.style = TMLWORD_STYLE_TT;
      break;
    case ML_BLOCKQUOTE: // block quote
      stateCurState.Reset();
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      stateCurState.contextblock = TMLWORD_CONTEXT_QUOTE;
      break;
    case ML_xOL: // same exit for any of these contexts
    case ML_xPRE:
    case ML_xBLOCKQUOTE:
      stateCurState.Reset();
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      break;

    case ML_UL:     // unordered lists may be nested
      if (stateCurState.contextblock == TMLWORD_CONTEXT_ULIST)
        stateCurState.ulnest++;
      else {
        stateCurState.Reset();
        stateCurState.contextblock = TMLWORD_CONTEXT_ULIST;
        stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
        stateCurState.ulnest = 1;
      }
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      break;
    case ML_xUL:    // end uf unordered list
      if (stateCurState.ulnest > 1) {
        stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
        stateCurState.ulnest--;
      }
      else {
        stateCurState.Reset();
        stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      }
      break;

    case ML_DL:     // definition list
    case ML_xDL:
      stateCurState.Reset();
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWPAR;
      break;
    case ML_DT:     // term to be defined
      stateCurState.contextblock = TMLWORD_CONTEXT_NONE;
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      break;
    case ML_DD:     // definition of term
      stateCurState.contextflags |= TMLWORD_CONTEXT_NEWLINE;
      stateCurState.contextflags |= TMLWORD_CONTEXT_FIRST;
      stateCurState.contextblock = TMLWORD_CONTEXT_QUOTE;
      break;

    case ML_A:      // anchor/link
      if ((error = ReadArg ("NAME", parg, 1)) == 0) {
        if ((error = MakeAnchor ()) == 0)
          stateCurState.contextflags |= TMLWORD_CONTEXT_ANCHOR;
      }
      else if ((error = ReadArg ("HREF", parg, 2)) == 0) {
        if ((error = MakeLink ()) == 0)
          stateCurState.contextflags |= TMLWORD_CONTEXT_LINK;
      }
      break;
    case ML_xA:    // end anchor/link
      stateCurState.contextflags &= ~(TMLWORD_CONTEXT_ANCHOR|TMLWORD_CONTEXT_LINK);
      break;

    // character styles
    case ML_DFN:
      stateCurState.style = TMLWORD_STYLE_DFN;
      break;
    case ML_EM:
      stateCurState.style = TMLWORD_STYLE_EM;
      break;
    case ML_CITE:
      stateCurState.style = TMLWORD_STYLE_CITE;
      break;
    case ML_VAR:
      stateCurState.style = TMLWORD_STYLE_VAR;
      break;
    case ML_I:
      stateCurState.style = TMLWORD_STYLE_I;
      break;
    case ML_CODE:
      stateCurState.style = TMLWORD_STYLE_CODE;
      break;
    case ML_KBD:
      stateCurState.style = TMLWORD_STYLE_KBD;
      break;
    case ML_SAMP:
      stateCurState.style = TMLWORD_STYLE_SAMP;
      break;
    case ML_TT:
      stateCurState.style = TMLWORD_STYLE_TT;
      break;
    case ML_STRONG:
      stateCurState.style = TMLWORD_STYLE_STRONG;
      break;
    case ML_B:
      stateCurState.style = TMLWORD_STYLE_B;
      break;
    case ML_xDFN:
    case ML_xEM:
    case ML_xCITE:
    case ML_xVAR:
    case ML_xI:
    case ML_xCODE:
    case ML_xKBD:
    case ML_xSAMP:
    case ML_xTT:
    case ML_xSTRONG:
    case ML_xB:
      stateCurState.style = TMLWORD_STYLE_NONE;
      break;
  }

  return error; // either no errors, or error in parsing of IMAGE or ANCHOR
}

////
//// ReadArg
////  Extract and argument from the passed buffer
////  Arguments:
////    arg points to the argument name
////    buf points to the buffer
////    type is 1 if we are looking for a single argument,
////      or 2 if a double argument (separated by # sign)
////  Returns:
////    error code, or 0 if no error
////    puts arguments it finds into stateCurState
////  Example:
////    ReadArg ("ADDRESS", "name=\"myname\" address=\"MyAdrs\"", 1)
////   puts "MyAdrs" into stateCurState.arg1.
////   Double arg (type = 2) used for hrefs ("file#label").
////
int
CTMLParser::ReadArg (char *arg, char *buf, int type)
{
  int error = PARSE_ERROR_BADARG;

  while (*buf) {
    for (; *buf && isspace(*buf); buf++){};  // skip white space
    // is this the right argument?
    if (strncmpi(arg, buf, strlen(arg)) == 0) {
      buf = strchr(buf, '\"');     // find first quote
      if (buf == NULL) break;
      char *buf2 = ++buf;        // first char past quote
      buf2 = strchr(buf2, '\"'); // find second quote
      if (buf2 == NULL) break;
      *buf2 = 0;
      if (type == 1) { // single argument
        stateCurState.arg1 = buf;
        stateCurState.arg2 = "";
        error = 0;
      }
      else {  // double argument, second preceded by #
        buf2 = strchr (buf, '#');
        if (buf2 == NULL) {  // second part of arg not there
          stateCurState.arg1 = buf;
          stateCurState.arg2 = "";
        }
        else {               // both parts of arg exist
          *buf2 = 0;
          stateCurState.arg1 = buf;
          stateCurState.arg2 = ++buf2;
        }
        error = 0;
      }
    }
    // wrong argument, skip past it
    else {
      buf = strchr(buf, '\"'); // find first quote
      if (buf == NULL) break;
      buf = strchr(++buf, '\"'); // find second quote
      if (buf == NULL) break;
      buf++;                     // past second quote
    }
  }

  return error;
}

////
//// ReadEscape
////  Read a character escape and return its value
////  Returns:
////    character represented by escape sequence
////    '*' if unrecognized
////  The leading '&' is already read.
////
char
CTMLParser::ReadEscape ()
{
  char buf[PARSE_MAXWORDSIZE];   // escape code buffer
  int  ibuf = 0;                 // index into escape code buffer
  int  error = 0;
  char retval = '*';

  // part one: get code out of file
  char c;
  while (ReadChar(&c)) {
    if (ibuf >= PARSE_MAXWORDSIZE) {  // check for word too long here
      ibuf = 0;
      break; // could just not break, and keep eating file...
    }
    if (c == ';')           // escape sequence end
      break;
    else                    // add to buffer
      buf[ibuf++] = c;
  }
  buf[ibuf] = 0;


  // part two: interpret the escape code
  // we only handle four...
  if (stricmp(buf,"quot") == 0)
    retval = '\"';
  else if (stricmp(buf,"amp") == 0)
    retval = '&';
  else if (stricmp(buf,"lt") == 0)
    retval = '<';
  else if (stricmp(buf,"gt") == 0)
    retval = '>';

  return retval;
}

////
//// EndWord
////  Adds word to document's array
////  Returns:
////    error code, or 0 if no error
////  This routine saves the state and text in a word and
//// adds it to the document's array of words.  It can also
//// allocate and add images, links, and anchors.
////
int
CTMLParser::EndWord ()
{
  // if called by consecutive white space chars, return
  if (nIChar == 0 &&
      stateCurState.type != TMLWORD_TYPE_HRULE &&
      stateCurState.type != TMLWORD_TYPE_IMAGE)
    return 0;

  sCurWord[nIChar] = 0; // end word

  if (stateCurState.type == TMLWORD_TYPE_TITLE) // title is special
    strTitle = sCurWord;
  else {
    // output the word...
    // some of these things assume arrays were empty at start
    // but doc is responsible for that
    CTMLWord *word = new CTMLWord;
    // set up word parameters
    word->Type(stateCurState.type);
    word->Style(stateCurState.style);
    word->Context(stateCurState.contextflags|stateCurState.contextblock);
    // union, so only assign one
    if (stateCurState.contextblock == TMLWORD_CONTEXT_HEADING)
      word->HLevel(stateCurState.hlevel);
    else if (stateCurState.contextblock == TMLWORD_CONTEXT_OLIST)
      word->OLItem(stateCurState.olitem);
    else if (stateCurState.contextblock == TMLWORD_CONTEXT_ULIST)
      word->ULNest(stateCurState.ulnest);
    word->ILink(stateCurState.ilink);

    // connect links/anchors back to word list
    if (stateCurState.contextflags & TMLWORD_CONTEXT_LINK) {
      ((CTMLLink *)apLinkArray[stateCurState.ilink])->LastWord(nNumWords);
    }
    else if (stateCurState.contextflags & TMLWORD_CONTEXT_ANCHOR) {
      ((CTMLAnchor *)apAnchorArray[stateCurState.ilink])->LastWord(nNumWords);
    }

    // set contents
    if (stateCurState.type == TMLWORD_TYPE_HRULE) {
      // nothing more, no contents
    }
    else if (stateCurState.type == TMLWORD_TYPE_IMAGE) {
      // no contents, but must set image index
      word->IImage(stateCurState.iimage);
    }
    else { // words and literals
      word->Contents(sCurWord);
    }
    apWordArray.Add((void *)word);
    nNumWords++;
  }

  // reset things for next word
  if (stateCurState.type == TMLWORD_TYPE_HRULE ||
      stateCurState.type == TMLWORD_TYPE_IMAGE) {
    stateCurState.type = TMLWORD_TYPE_WORD;
    stateCurState.contextflags &= ~(TMLWORD_CONTEXT_FIRST |
                            TMLWORD_CONTEXT_SENT  |
                            TMLWORD_CONTEXT_ANCHOR);
    // keep newline/newpar
  }
  else {
    stateCurState.contextflags &= ~(TMLWORD_CONTEXT_NEWLINE |
                            TMLWORD_CONTEXT_NEWPAR |
                            TMLWORD_CONTEXT_FIRST |
                            TMLWORD_CONTEXT_SENT  |
                            TMLWORD_CONTEXT_ANCHOR);
    // check for end of sentence... this could be done in more complex
    //  manner if necessary.  It has to be attached to next word.
    if (stateCurState.type == TMLWORD_TYPE_WORD) {
      if (sCurWord[nIChar-1] == '.' ||
          sCurWord[nIChar-1] == '?' ||
          sCurWord[nIChar-1] == '!')
        stateCurState.contextflags |= TMLWORD_CONTEXT_SENT;
    }
  }
  sCurWord[0] = 0;
  nIChar = 0;

  if (nNumWords >= 16384)
    return PARSE_ERROR_MAXWORDS;
  else
    return 0;
}

////
//// MakeAnchor
////  Make a new anchor
////  Returns:
////    error code, or 0 if no error
////
int
CTMLParser::MakeAnchor ()
{
  CTMLAnchor *anchor = new CTMLAnchor;
  anchor->Anchor(stateCurState.arg1);
  anchor->FirstWord(nNumWords);// set to next word...
                          //  note implications if we are at end of file
                          //  so must check Used()

  apAnchorArray.Add((void *)anchor); // add to array

  int nanchors = apAnchorArray.GetSize();

  stateCurState.ilink = nanchors-1;        // save array reference

   if (nanchors >= 16384)
     return PARSE_ERROR_MAXANCHORS;
   else
     return 0;
}

////
//// MakeLink
////  Make a new link
////  Returns:
////    error code, or 0 if no error
////
int
CTMLParser::MakeLink ()
{
  CTMLLink *link = new CTMLLink;
  link->File(stateCurState.arg1);
  link->Anchor(stateCurState.arg2);
  link->FirstWord(nNumWords);// set to next word...
                          //  note implications if we are at end of file
                          //  so must check Used()

  apLinkArray.Add((void *)link); // add to array

  int nlinks = apLinkArray.GetSize();

  stateCurState.ilink = nlinks-1;        // save array reference

   if (nlinks >= 16384)
     return PARSE_ERROR_MAXLINKS;
   else
     return 0;
}

////
//// MakeImage
////  Make a new image
////  Returns:
////    error code, or 0 if no error
////  Images are actually read by the document
////
int
CTMLParser::MakeImage ()
{
  CTMLImage *image = new CTMLImage;
  image->File(stateCurState.arg1);

  apImageArray.Add((void *)image); // add to array

  int nimages = apImageArray.GetSize();

  stateCurState.iimage = nimages-1;        // save array reference

   if (nimages >= 16384)
     return PARSE_ERROR_MAXIMAGES;
   else
     return 0;
}

////
//// Array of parser error messages
////
static char * parser_errormsg[] = {
  "No error",
  "What is error 1?",
  "Line too long",            //  PARSE_ERROR_LONGLINE 2
  "Too many words in file",   //  PARSE_ERROR_MAXWORDS 3
  "Bad <> pair - possibly unterminated <",  //  PARSE_ERROR_BADCODE  4
  "Bad or missing argument to <A> or <IMG>",//  PARSE_ERROR_BADARG   5
  "Too many hyperlinks",      //  PARSE_ERROR_MAXLINKS 6
  "Too many anchors",         //  PARSE_ERROR_MAXANCHORS 7
  "Too many images"           //  PARSE_ERROR_MAXIMAGES 8
};

////
//// ErrorMessage
////  Gets error message text
////  Arguments:
////    i  index of error message
////  Returns:
////    pointer to character string for error
//// Note: these strings could be made into resources.
////
char *
CTMLParser::ErrorMessage(int i)
{
  return parser_errormsg [i];

}
