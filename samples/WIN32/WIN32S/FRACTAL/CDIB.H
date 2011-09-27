#include "dib.h"

#ifndef CDIB_H
#define CDIB_H

class  cDIB {
  public:
    cDIB();
    cDIB(const char * pszFile);
    cDIB(HBITMAP hBmInit);
    cDIB(cDIB &cdibCopyFrom);
    virtual ~cDIB();
    
    enum eDrawModes {
        DIBSRCCOPY,
        DIBSRCTRANSPARENT
    };
    
    virtual int drawOnto(cDIB& cdibTarg, eDrawModes mode = DIBSRCCOPY, 
                         short clipX = -1, short clipY = -1);
  	virtual int drawInto(HDC hdc, DWORD dwGDImode = SRCCOPY);
  	
  	BOOL realizePalette(HDC hdc) const;

  	////////// Modification
  	cDIB& operator=(cDIB& cdibAssignFrom);

	BOOL clear(RGBQUAD * pClearColor = NULL);   // default to black
	
  	short setX(short nNewXpos);                    // returns old x pos for drawing
	short setY(short nNewYpos);	                // returns old y
	
	int conformTo(cDIB & cdib);
	int conformTo(HPALETTE hpal);
	
  	////////// Query
    operator int() const;
    BOOL operator!() const;
    
  	int width() const;
  	int height() const;
  	BYTE planes() const;
  	BYTE bitsPixel() const;
  	unsigned long numColors() const;
  	short x() const;
  	short y() const;
  	
  	HBITMAP getBm();
  	HPALETTE getPalette();
  	
  private:
    short _x, _y;
    union {
        LPBITMAPINFO pDib;
        LPBITMAPINFOHEADER pDibInfo;
    };
    HANDLE hDib;
};


inline BOOL cDIB::realizePalette(HDC hdc) const
{
    HPALETTE hpal, hpalTemp;
    
    hpal = CreateBIPalette(pDibInfo);
    hpalTemp = SelectPalette(hdc, hpal, FALSE);
    RealizePalette(hdc);
    SelectPalette(hdc, hpalTemp, FALSE);
    DeleteObject(hpal);
    return 0;
}

inline short cDIB::setX(short nNewXpos)
{
    short oldX = _x;
    _x = nNewXpos;
    return oldX;
}

inline short cDIB::setY(short nNewYpos)
{
    short oldY = _y;
    _y = nNewYpos;
    return oldY;
}

inline int cDIB::conformTo(cDIB & cdib)
{
    HPALETTE hpalFrom = CreateBIPalette(cdib.pDibInfo);
    assert(hpalFrom);
    hDib = SimplePaletteConform(hDib, hpalFrom);
    DeleteObject(hpalFrom);
    if (hDib != NULL)
    {
        pDib = (LPBITMAPINFO) GlobalLock(hDib);
        return 1;
    }
    else
        return 0;
}

inline int cDIB::conformTo(HPALETTE hpal)
{
    GlobalUnlock(hDib);
    hDib = SimplePaletteConform(hDib, hpal);
    if (hDib != NULL)
    {
        pDib = (LPBITMAPINFO) GlobalLock(hDib);
        return 1;
    }
    else
        return 0;
}

inline cDIB::operator int() const
{
    return (hDib != NULL)? 1 : 0;
}

inline BOOL cDIB::operator!() const
{
    return (hDib == NULL)? 1 : 0;
}

inline int cDIB::width() const
{
    return pDibInfo->biWidth;
}

inline int cDIB::height() const
{
    return pDibInfo->biHeight;
}

inline BYTE cDIB::planes() const
{
    return pDibInfo->biPlanes;
}

inline BYTE cDIB::bitsPixel() const
{
    return pDibInfo->biBitCount;
}

inline unsigned long cDIB::numColors() const
{
    return (pDibInfo->biBitCount == 1) ? 1 :
           (pDibInfo->biBitCount == 4) ? 16 :
           (pDibInfo->biBitCount == 8) ? 256 :
           (256L*256L*256L);
}

inline short cDIB::x() const
{
    return _x;
}

inline short cDIB::y() const
{
    return _y;
}

inline HBITMAP cDIB::getBm()
{
    HPALETTE hpal = CreateBIPalette(pDibInfo);
    HBITMAP hBm = BitmapFromDib(hDib, hpal);
    DeleteObject(hpal);
    return hBm;
}

inline HPALETTE cDIB::getPalette()
{
    return CreateBIPalette(pDibInfo);
}

#endif

