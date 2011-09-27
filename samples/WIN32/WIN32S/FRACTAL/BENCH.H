#ifndef BENCH_H
#define BENCH_H

#ifdef _WINDOWS
#include <windows.h>
#ifndef __NT__
#define HUGE __huge
#else
#define HUGE
#endif
#else
#ifndef DOS386
#error Sthrash requires 386 DOS if not targeting Windows
#define NWINDOWS
#define HUGE
typedef int HWND;
#endif
#endif

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef _strdup			/* For Borland */
#define _strdup strdup
#endif

///////////////////////////////////////

class cBenchmark {
  public:
    cBenchmark(HWND, const char * pszInitName);
    cBenchmark(cBenchmark&);
    virtual ~cBenchmark();
    
    virtual cBenchmark& operator=(cBenchmark&);
    
    virtual operator int() = 0;
    BOOL operator!();
    HWND associate(HWND);
    HWND hwnd();
    
    const char * name();
    
    virtual unsigned run() = 0;  // Return time result, 0 for err
    virtual int repaint() = 0;
    unsigned result();
    BOOL ran();
    
    virtual int explain() = 0;
    virtual int displayResult();
    
  protected:
    time_t beginTimer();
    time_t endTimer();
    
  private:
    HWND hwndOwner;
    time_t beginTime, endTime;
    char * pszName;
};

//////////////////////////////

inline cBenchmark::cBenchmark(HWND hwndOwn, const char * pszInitName)
    : hwndOwner(hwndOwn), beginTime(0), endTime(0)
{
    pszName = (pszInitName)? _strdup(pszInitName) : NULL;
}

inline cBenchmark::cBenchmark(cBenchmark &cbmkFrom)
    : hwndOwner(cbmkFrom.hwndOwner), beginTime(cbmkFrom.beginTime), 
      endTime(cbmkFrom.endTime)
{
    pszName = (cbmkFrom.pszName)? _strdup(cbmkFrom.pszName) : NULL;
}

inline cBenchmark::~cBenchmark()
{
    if (pszName)
        free(pszName);
}

inline cBenchmark& cBenchmark::operator=(cBenchmark& cbmkFrom)
{
    assert(0);
    if (pszName)
        free(pszName);
    pszName = (cbmkFrom.pszName)? _strdup(cbmkFrom.pszName) : NULL;
    beginTime = cbmkFrom.beginTime;
    endTime = cbmkFrom.endTime;
    hwndOwner = cbmkFrom.hwndOwner;
    return *this;
}

inline BOOL cBenchmark::operator!()
{
    return !(BOOL(*this));
}

inline HWND cBenchmark::associate(HWND hwndNewOwn)
{
    if (hwndNewOwn)
    {
        HWND hwndRetval = hwndOwner;
        hwndOwner = hwndNewOwn;
        return hwndRetval;
    }
    else
        return hwndOwner;
}

inline HWND cBenchmark::hwnd()
{
    return hwndOwner;
}

inline const char * cBenchmark::name()
{
    return pszName;
}

inline int cBenchmark::displayResult()
{
    char pszBuf[64];
    sprintf(pszBuf, "Time result was %u", unsigned(endTime - beginTime));
#if _WINDOWS
    return MessageBox(NULL, pszBuf, (pszName) ? pszName : "Benchmark", MB_OK);
#else
    return printf("%s: %s", name(), pszBuf);
#endif
}

inline unsigned cBenchmark::result()
{
    return unsigned(endTime - beginTime);
}

inline time_t cBenchmark::beginTimer()
{
    endTime = 0;
    return beginTime = time(NULL);
}

inline time_t cBenchmark::endTimer()
{
    return endTime = time(NULL);
}

inline BOOL cBenchmark::ran()
{
    return (endTime != 0);
}

///////////////////////////////////////
#endif
