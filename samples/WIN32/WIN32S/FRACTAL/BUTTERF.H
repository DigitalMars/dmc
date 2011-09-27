#ifndef BUTTERFLY_H
#define BUTTERFLY_H
#include "bench.h"

///////////////////////////////////////

class cButterflyBench : public cBenchmark {
  public:
    cButterflyBench(HWND, unsigned nItersToRun);
    cButterflyBench(cButterflyBench &from) : cBenchmark(from), numIters(from.numIters) {}
    virtual ~cButterflyBench();
    
    virtual operator int();
    virtual unsigned run();
    virtual int repaint();
    virtual int explain();
    
  private:
    int alloc(unsigned long dwAllocSize);
    int dealloc();
    unsigned numIters;
    HGLOBAL hAlloc;
    void HUGE * pAlloc;
};

//////////////////////////////

extern unsigned wButterflyIters;

#endif
