/// tmlimage.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CTMLImage class declaration
// Notes:
//   An array of objects of this type is stored in the document, one
//    to represent each image found in the TML source file.

#ifndef _TMLIMAGE_H
#define _TMLIMAGE_H

//// CTMLImage
////  Contains image data.
////  An array of these objects is created by parser, and stored in the
////  document.  The images themselves are.read by the document, through
////  calls to the DIB API.
////  Image words in the document's word array contain indices which point
////  to these objects.  When they need to be drawn, the view calls the
////  DIB API to draw them.

class CTMLImage {
  public:
    CTMLImage();
    ~CTMLImage();

  // read, initialize, remove
    BOOL Read(const char *);  // read from file into memory  TRUE if error
    void UnRead();            // remove from memory
    void InitDIBData();       // initialize DIB data

  // draw
    void Draw(CDC *, CRect);

  // data access
    CString File()   { return strFilename; }
    WORD XSize()     { return sizImageSize.cx; }
    WORD YSize()     { return sizImageSize.cy; }
    CSize &Size()      { return sizImageSize; }

  // data setting
    void File(CString x)   { strFilename = x; }

  private:
    CString strFilename;       // image filename
    CSize   sizImageSize;      // image size
    CPalette *pPalette;        // pointer to palette
    HDIB    hDIB;              // bitmap handle

};

#endif
