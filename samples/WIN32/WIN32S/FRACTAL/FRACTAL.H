#ifndef FRACTAL_H
#define FRACTAL_H
#include "bench.h"
#include "cdib.h"

///////////////////////////////////////

class cFractalBench : public cBenchmark {
  public:
    cFractalBench(HWND, unsigned short wDistanceToPan);
    virtual ~cFractalBench();
    
    virtual operator int();
    virtual unsigned run();
    virtual int repaint();
    virtual int explain();
    
  private:
    unsigned short wPan;
    cDIB * pCanvas;
};

//////////////////////////////
extern unsigned short wFractalPanDistance;
#endif
