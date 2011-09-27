#include <windows.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "dib.h"
#include "cdib.h"

#if __NT__
#define HUGE
#else
#define HUGE __huge
#endif


#ifdef __BCPLUSPLUS__
#pragma option -a-
#else
#pragma pack(1)
#endif

class cDIBColor8Bit
{
  public:
    cDIBColor8Bit& operator=(cDIBColor8Bit&);
    cDIBColor8Bit& operator<<(cDIBColor8Bit&);
  private:
    BYTE pixel;
};

inline cDIBColor8Bit& cDIBColor8Bit::operator=(cDIBColor8Bit& from)
{
    pixel = from.pixel;
    return *this;
}

inline cDIBColor8Bit& cDIBColor8Bit::operator<<(cDIBColor8Bit& from)
{
    if (from.pixel)
        pixel = from.pixel;
    return *this;
}

class cDIBDuoColor8Bit
{
  public:
    cDIBDuoColor8Bit& operator=(cDIBDuoColor8Bit&);
    cDIBDuoColor8Bit& operator<<(cDIBDuoColor8Bit&);
  private:
    WORD duoPixel;
};

inline cDIBDuoColor8Bit& cDIBDuoColor8Bit::operator=(cDIBDuoColor8Bit& from)
{
    duoPixel = from.duoPixel;
    return *this;
}

inline cDIBDuoColor8Bit& cDIBDuoColor8Bit::operator<<(cDIBDuoColor8Bit& from)
{
    if (from.duoPixel)
        duoPixel = (((HIBYTE(from.duoPixel)) ? 
                        HIBYTE(from.duoPixel) : 
                        HIBYTE(duoPixel)) 
                      << 8) |
                   ((LOBYTE(from.duoPixel)) ? 
                        LOBYTE(from.duoPixel) : 
                        LOBYTE(duoPixel));
    return *this;
}

class cDIBQuadColor8Bit
{
  public:
    cDIBQuadColor8Bit& operator=(cDIBQuadColor8Bit&);
    cDIBQuadColor8Bit& operator<<(cDIBQuadColor8Bit&);
  private:
    DWORD quadPixel;
};

inline cDIBQuadColor8Bit& cDIBQuadColor8Bit::operator=(cDIBQuadColor8Bit& from)
{
    quadPixel = from.quadPixel;
    return *this;
}


#define DWMASKHI    0xFFFF0000L
#define DWMASKLO    0x0000FFFFL
#define DWMASKHIHI  0xFF000000L
#define DWMASKLOHI  0x00FF0000L
#define DWMASKHILO  0x0000FF00L
#define DWMASKLOLO  0x000000FFL

inline BOOL NonZeroHiWord(DWORD dw)
{
    return (dw & DWMASKHI) != 0L;
}

inline BOOL NonZeroLoWord(DWORD dw)
{
    return (dw & DWMASKLO) != 0L;
}

inline DWORD ShowThroughHi(DWORD dwBack, DWORD dwFront)
{
    return ((dwFront & DWMASKHIHI) ?
                (dwFront & DWMASKHIHI) :
                (dwBack & DWMASKHIHI)) |
           ((dwFront & DWMASKLOHI) ?
                (dwFront & DWMASKLOHI) :
                (dwBack & DWMASKLOHI));
}

inline DWORD ShowThroughLo(DWORD dwBack, DWORD dwFront)
{
    return ((dwFront & DWMASKHILO) ?
                (dwFront & DWMASKHILO) :
                (dwBack & DWMASKHILO)) |
           ((dwFront & DWMASKLOLO) ?
                (dwFront & DWMASKLOLO) :
                (dwBack & DWMASKLOLO));
}

inline cDIBQuadColor8Bit& cDIBQuadColor8Bit::operator<<(cDIBQuadColor8Bit& from)
{
    if (from.quadPixel)
    {
        quadPixel = ((NonZeroHiWord(from.quadPixel)) ? 
                        ShowThroughHi(quadPixel, from.quadPixel) :
                        (quadPixel & DWMASKHI)) |
                    ((NonZeroLoWord(from.quadPixel)) ?
                        ShowThroughLo(quadPixel, from.quadPixel) :
                        (quadPixel & DWMASKLO));
    }                   
    return *this;
}

#ifdef __BCPLUSPLUS__
#pragma option -a.
#else
#pragma pack(1)
#endif

class cDIBManipulator
{
  public:
    cDIBManipulator(LPBITMAPINFO pAssociatedDib, short initX = 0, short initY = 0);
    int incX();
    int incY();
    int setX(int);
    int setY(int);
    int x();
    int y();
  protected:
    unsigned long index();
    unsigned DibWidthBytes();
    LPBITMAPINFO pDib;
    int _x, _y;
    union {
        void HUGE * pImage;
        cDIBColor8Bit HUGE * pImagePixels;
    };
};

inline cDIBManipulator::cDIBManipulator(LPBITMAPINFO pAssociatedDib, short initX, short initY)
{
    pImage = (void HUGE *)((BYTE *)pAssociatedDib + pAssociatedDib->bmiHeader.biSize + 
                           PaletteSize(pAssociatedDib));
    pDib = pAssociatedDib;
    setX(initX);
    setY(initY);
}

inline int cDIBManipulator::incX()
{
    return _x = (_x >= pDib->bmiHeader.biWidth - 1) ? 0 : _x+1;
}

inline int cDIBManipulator::incY()
{
    return _y = (_y >= pDib->bmiHeader.biHeight - 1) ? 0 : _y+1;
}

inline int cDIBManipulator::setX(int newX)
{
    int oldX = _x;
    _x = (newX >= pDib->bmiHeader.biWidth) ? pDib->bmiHeader.biWidth :
         (newX < 0) ? 0 :
         newX;
    return oldX;
}

inline int cDIBManipulator::setY(int newY)
{
    int oldY = _y;
    _y = (newY >= pDib->bmiHeader.biHeight) ? pDib->bmiHeader.biWidth :
         (newY < 0) ? 0 :
         newY;
    return oldY;
}

inline int cDIBManipulator::x()
{
    return _x;
}

inline int cDIBManipulator::y()
{
    return _y;
}

inline unsigned long cDIBManipulator::index()
{
    return long(DibWidthBytes()) * long(_y) + _x;
}

inline unsigned cDIBManipulator::DibWidthBytes()
{
    return (pDib->bmiHeader.biWidth % long(sizeof(long))) ?
                        (pDib->bmiHeader.biWidth + (sizeof(long) - 
                            (pDib->bmiHeader.biWidth % long(sizeof(long))))) :
                        pDib->bmiHeader.biWidth;
}

class cDIB8BitPixel : public cDIBManipulator {
  public:
    cDIB8BitPixel(LPBITMAPINFO pAssociatedDib, short initX = 0, short initY = 0);
    virtual cDIB8BitPixel& operator<<(cDIB8BitPixel&); // Transparent Copy
    
    cDIB8BitPixel& operator=(cDIB8BitPixel&);
};

cDIB8BitPixel::cDIB8BitPixel(LPBITMAPINFO pAssociatedDib, short initX, short initY)
    : cDIBManipulator(pAssociatedDib, initX, initY)
{
}


inline cDIB8BitPixel& cDIB8BitPixel::operator<<(cDIB8BitPixel& from)
{
    pImagePixels[index()] << from.pImagePixels[from.index()];
    return *this;
}

inline cDIB8BitPixel& cDIB8BitPixel::operator=(cDIB8BitPixel& from)
{
    pImagePixels[index()] = from.pImagePixels[from.index()];
    return *this;
}

class cDIB8BitQuadPixel : public cDIB8BitPixel {
  public:
    cDIB8BitQuadPixel(LPBITMAPINFO pAssociatedDib, short initX = 0, short initY = 0);
    virtual cDIB8BitQuadPixel& operator<<(cDIB8BitQuadPixel&); // Transparent Copy
    
    cDIB8BitQuadPixel& operator=(cDIB8BitQuadPixel&);
};

cDIB8BitQuadPixel::cDIB8BitQuadPixel(LPBITMAPINFO pAssociatedDib, short initX, short initY)
    : cDIB8BitPixel(pAssociatedDib, initX, initY)
{
}

inline cDIB8BitQuadPixel& cDIB8BitQuadPixel::operator<<(cDIB8BitQuadPixel& from)
{
    cDIBQuadColor8Bit * pImageQuads = (cDIBQuadColor8Bit *) &pImagePixels[index()];
    cDIBQuadColor8Bit * pFromImageQuads = (cDIBQuadColor8Bit *) &from.pImagePixels[from.index()];

    *pImageQuads << *pFromImageQuads;
    
    return *this;
}

inline cDIB8BitQuadPixel& cDIB8BitQuadPixel::operator=(cDIB8BitQuadPixel& from)
{
    cDIBQuadColor8Bit * pImageQuads = (cDIBQuadColor8Bit *) &pImagePixels[index()];
    cDIBQuadColor8Bit * pFromImageQuads = (cDIBQuadColor8Bit *) &from.pImagePixels[from.index()];

    *pImageQuads = *pFromImageQuads;
    return *this;
}

cDIB::cDIB() :
    _x(0), _y(0)

{
    hDib = NULL;
    pDib = NULL;
}

cDIB::cDIB(const char * pszFile) :
    _x(0), _y(0)
{
    hDib = OpenDIB((char*)pszFile);
    if (hDib)
    {
        pDib = (LPBITMAPINFO) GlobalLock(hDib);
    }
    else
    {
        pDib = NULL;
        MessageBox(NULL, "DIB load failure", NULL, MB_OK);
    }
}

cDIB::cDIB(HBITMAP hBmInit) :
    _x(0), _y(0)

{
    hDib = NULL;
    pDib = NULL;
    MessageBox(NULL, "cDIB::cDIB(HBITMAP) unimplemented", NULL, MB_OK);
}

cDIB::cDIB(cDIB &cdibCopyFrom)
{
    MessageBox(NULL, "cDIB::cDIB(cDIB&) unimplemented", NULL, MB_OK);
}

cDIB::~cDIB()
{
    if (hDib)
    {
        while(GlobalUnlock(hDib))
            ;
        GlobalFree(hDib);
    }
}

/**********************/
int cDIB::drawOnto(cDIB& cdibTarg, eDrawModes mode, short clipX, short clipY)
{
    if (!hDib)
        return 0;
        
    if (clipX != -1 || clipY != -1)
    {
        MessageBox(NULL, "Dib clipping unimplemented", NULL, MB_OK);
    }
    
    RECT rcToDraw;
    POINT ptTargetStart;
    
    rcToDraw.top = ((_y + pDibInfo->biHeight) > cdibTarg.pDibInfo->biHeight - 1) ?
                        ((cdibTarg.pDibInfo->biHeight-1) - _y) :
                   ((_y + pDibInfo->biHeight) < 0) ?
                        -1 :
                        pDibInfo->biHeight - 1;
    
    rcToDraw.left = (_x < 0) ? -_x :
                    (_x > cdibTarg.pDibInfo->biWidth) ?
                        -1 :
                        0;
    rcToDraw.bottom = (_y < 0) ? -_y :
                      (_y > cdibTarg.pDibInfo->biHeight) ?
                        -1 :
                        0;

    rcToDraw.right = ((_x + pDibInfo->biWidth) > cdibTarg.pDibInfo->biWidth - 1) ?
                        ((cdibTarg.pDibInfo->biWidth - 1) - _x) :
                     ((_x + pDibInfo->biWidth) < 0) ?
                        -1 :
                        pDibInfo->biWidth - 1;
    

    ptTargetStart.x = (_x < 0) ? 0 : _x;
    ptTargetStart.y = (_y < 0) ? 0 : _y;
    
    
    if (rcToDraw.top == -1 || rcToDraw.left == -1 || rcToDraw.bottom == -1 ||
        rcToDraw.right == -1)
    {
        return 1;
    }
    
    /*******/
    int iQuads, iPixelsLeft;
    iQuads = ((rcToDraw.right - rcToDraw.left) + 1) / sizeof(long);
    iPixelsLeft = ((rcToDraw.right - rcToDraw.left) + 1) % sizeof(long);
    int i, j;
    
    if (iQuads <= 0)
        return 0;  // small copies unimplemented
    
    cDIB8BitPixel FromByteScan(pDib);
    cDIB8BitQuadPixel FromQuadScan(pDib, rcToDraw.left, rcToDraw.bottom);
    
    cDIB8BitPixel ToByteScan(cdibTarg.pDib);
    cDIB8BitQuadPixel ToQuadScan(cdibTarg.pDib, ptTargetStart.x, 
                                 ptTargetStart.y);
    
    if (mode == DIBSRCCOPY)
    {
        for (i = 0; i <= rcToDraw.top - rcToDraw.bottom; i++)
        {
            ToQuadScan.setX(ptTargetStart.x);
            FromQuadScan.setX(rcToDraw.left);
            for (j = 0; j < iQuads; j++)
            {
                ToQuadScan = FromQuadScan;
                FromQuadScan.setX(FromQuadScan.x()+4);
                ToQuadScan.setX(ToQuadScan.x()+4);
            }
            if (iPixelsLeft)
            {
                FromByteScan.setX(FromQuadScan.x());
                FromByteScan.setY(FromQuadScan.y());
                ToByteScan.setX(ToQuadScan.x());
                ToByteScan.setY(ToQuadScan.y());
                for (j = 0; j < iPixelsLeft; j++)
                {
                    ToByteScan = FromByteScan;
                    FromByteScan.setX(FromByteScan.x()+1);
                    ToByteScan.setX(ToByteScan.x()+1);
                }
            }
            FromQuadScan.incY();
            ToQuadScan.incY();
        }
    }
    else
    {
        for (i = 0; i <= rcToDraw.top - rcToDraw.bottom; i++)
        {
            ToQuadScan.setX(ptTargetStart.x);
            FromQuadScan.setX(rcToDraw.left);
            for (j = 0; j < iQuads; j++)
            {
                ToQuadScan << FromQuadScan;
                FromQuadScan.setX(FromQuadScan.x()+4);
                ToQuadScan.setX(ToQuadScan.x()+4);
            }
            if (iPixelsLeft)
            {
                FromByteScan.setX(FromQuadScan.x());
                FromByteScan.setY(FromQuadScan.y());
                ToByteScan.setX(ToQuadScan.x());
                ToByteScan.setY(ToQuadScan.y());
                for (j = 0; j < iPixelsLeft; j++)
                {
                    ToByteScan << FromByteScan;
                    FromByteScan.setX(FromByteScan.x()+1);
                    ToByteScan.setX(ToByteScan.x()+1);
                }
            }
            FromQuadScan.incY();
            ToQuadScan.incY();
        }
    }
    
    return 0;
}

int cDIB::drawInto(HDC hdc, DWORD dwGDImode)
{
    HPALETTE hpal, hpalTemp;
    
    hpal = CreateBIPalette(pDibInfo);
    hpalTemp = SelectPalette(hdc, hpal, FALSE);
    RealizePalette(hdc);

    DibBlt(hdc, _x, _y, pDibInfo->biWidth, pDibInfo->biHeight, hDib, 
           0, 0, dwGDImode);
    SelectPalette(hdc, hpalTemp, FALSE);
    DeleteObject(hpal);
    return 1;
}
/**********************/

cDIB& cDIB::operator=(cDIB& cdibAssignFrom)
{
    HDC hdcDesktop;
    int iBitsPixel;
    
    hdcDesktop = GetDC(NULL);
    iBitsPixel = GetDeviceCaps(hdcDesktop, BITSPIXEL);
    ReleaseDC(NULL, hdcDesktop);
    if (iBitsPixel == 8)
    {
        HPALETTE hpalFrom = CreateBIPalette(cdibAssignFrom.pDibInfo);
        HBITMAP hBm = BitmapFromDib(cdibAssignFrom.hDib, hpalFrom);
        if (hBm != NULL)
        {
            if (hDib)
            {
                while (GlobalUnlock(hDib))
                    ;
                GlobalFree(hDib);
            }
            hDib = DibFromBitmap(hBm, BI_RGB, 8, hpalFrom);
            if (!hDib)
            {
                MessageBox(NULL, "Out of memory", NULL, MB_OK);
                exit(EXIT_FAILURE);
            }
            DeleteObject(hBm);
            DeleteObject(hpalFrom);
            pDib = (LPBITMAPINFO) GlobalLock(hDib);
            return *this;
        }
        else
        {
            DeleteObject(hpalFrom);
        }
    }
    if (hDib)
    {
        while (GlobalUnlock(hDib))
            ;
        GlobalFree(hDib);
    }
    
    hDib = GlobalAlloc(GHND, GlobalSize(cdibAssignFrom.hDib));
    if (hDib)
    {
        pDib = (LPBITMAPINFO) GlobalLock(hDib);
        memcpy(pDib, cdibAssignFrom.pDib, pDibInfo->biSize + 
               PaletteSize(pDibInfo));
        cdibAssignFrom.drawOnto(*this, DIBSRCCOPY);
    }
    else
    {
        MessageBox(NULL, "Out of memory", NULL, MB_OK);
        exit(EXIT_FAILURE);
    }
    return *this;
}

BOOL cDIB::clear(RGBQUAD * pClearColor)
{
    MessageBox(NULL, "cDIB::clear unimplemented!", NULL, MB_OK);
    return 0;
}

