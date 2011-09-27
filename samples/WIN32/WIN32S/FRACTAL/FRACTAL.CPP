#include <windows.h>
#include "dib.h"
#include "fractal.h"

#define CLOUDSPEED      2
#define LANDSPEED       32
#define FARLANDSPEED    15

char * pszFractExplain = "Fractal Landscape tests access time across\n"
                        "large memory spaces.  The technique here\n"
                        "uses non-optimal animation to simulate\n"
                        "real-world memory-intensive applications.\n"
                        "\n"
                        "This benchmark requires a 256-color configuration\n"
                        "Landscape built using FRACTINT.\n";

cFractalBench::cFractalBench(HWND hwndOwner, unsigned short wDistanceToPan)
	: cBenchmark(hwndOwner, "Fractal Landscape Thrash"), wPan(wDistanceToPan)
{
    pCanvas = new cDIB;
}

cFractalBench::~cFractalBench()
{
    if (pCanvas)
        delete pCanvas;
}

cFractalBench::operator int()
{
	return (pCanvas != NULL);
}

unsigned cFractalBench::run()
{
    if (!pCanvas)
        return 0;
        
	cDIB sky("stsky.bmp");
	cDIB land("stland.bmp");
	cDIB farland("stfarlnd.bmp");
	cDIB clouds("stcloud.bmp");
	RECT winrect;
    
    if (!sky || !land || !farland || !clouds)
    {
        MessageBox(NULL, "Not enough memory to run Fractal Lanscape", NULL, 
                   MB_OK);
        return 0;
    }
    
    MessageBox(NULL, "Preparing benchmark...", name(), MB_OK);
    sky.conformTo(land);
    clouds.conformTo(land);
    farland.conformTo(land);
    land.conformTo(land);
    clouds.setY(480-256);
    clouds.setX(-(CLOUDSPEED * int(wPan) + 1));
    farland.setY(80);
    
    MessageBox(NULL, "Starting Fractal Landscape", "Benchmark", MB_OK);
    MoveWindow(hwnd(), 0, 0, 640, 480, TRUE);
    GetClientRect(hwnd(), &winrect);
    MoveWindow(hwnd(), 0, 0, 640 + (640 - winrect.right), 
               480 + (480 - winrect.bottom), TRUE);
    HCURSOR hcurOld = SetCursor(LoadCursor(NULL, IDC_WAIT));
    HDC hdc = GetDC(hwnd());
    beginTimer();
    HBITMAP hBm;
    for (unsigned short i = 0; i < wPan; i++)
    {
        *pCanvas = sky;
        clouds.drawOnto(*pCanvas, cDIB::DIBSRCTRANSPARENT);
        farland.drawOnto(*pCanvas, cDIB::DIBSRCTRANSPARENT);
        land.drawOnto(*pCanvas, cDIB::DIBSRCTRANSPARENT);
        hBm = pCanvas->getBm();
        DrawBitmap(hdc, 0, 0, hBm, SRCCOPY);
        DeleteObject(hBm);
        clouds.setX(clouds.x() + CLOUDSPEED);
        farland.setX(farland.x() - FARLANDSPEED);
        land.setX(land.x() - LANDSPEED);
    }
	endTimer();
	ReleaseDC(hwnd(), hdc);
	SetCursor(hcurOld);
    MessageBox(NULL, "Benchmark done", "Benchmark", MB_OK);
	return 1;

}

int cFractalBench::repaint()
{
    if (pCanvas)
    {
        if (!(*pCanvas))
        {
            return 0;
        }
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd(), &ps);
        assert(hdc != NULL);
        pCanvas->drawInto(hdc);
        EndPaint(hwnd(), &ps);
        return 1;
    }
    return 0;
}

int cFractalBench::explain()
{
    static char buf[512];
    
    sprintf(buf,"%s", pszFractExplain);
    MessageBox(NULL, buf, name(), MB_OK);
    
    return 1;
}

