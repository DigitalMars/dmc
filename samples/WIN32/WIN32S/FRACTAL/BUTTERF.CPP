#include <stdlib.h>
#include "butterf.h"

#define AllocSize ((long)(1024L*1024L*2L))

/////////////////////////////
class TestClass {
	public:
		TestClass();
		TestClass & operator = (TestClass&);
	
	private:
		unsigned long var1, var2;
};
/////////////////////////////
inline TestClass::TestClass()
{
	var1 = (unsigned short) (rand()) * (unsigned short) (rand());
	var2 = (unsigned short) (rand()) * (unsigned short) (rand());
}

inline TestClass & TestClass::operator=(TestClass &tcCopyFrom)
{
	var1 = tcCopyFrom.var1;
	var2 = tcCopyFrom.var2;
	return *this;
}	
/////////////////////////////

static const char * pszBFExplain = "Butterfly Thrash tests access time across\n"
                                    "large memory spaces.  It is performs no\n"
                                    "somersaults to optimize for Intel segment\n"
                                    "limitations and is non-graphical.\n";

inline int cButterflyBench::alloc(unsigned long dwAllocSize)
{
    if (hAlloc)
        dealloc();
	hAlloc = GlobalAlloc(GPTR, dwAllocSize);
	return (hAlloc) ? (((pAlloc = GlobalLock(hAlloc)) != NULL) ? 1 : 0) : 0;
}

inline int cButterflyBench::dealloc()
{
	GlobalUnlock(hAlloc);
	hAlloc = GlobalFree(hAlloc);
	pAlloc = NULL;
	return (hAlloc != NULL);
}

cButterflyBench::cButterflyBench(HWND hwndOwner, unsigned nNumIters)
	: cBenchmark(hwndOwner, "Butterfly Thrash"), numIters(nNumIters)
{
    alloc(AllocSize);
}

cButterflyBench::~cButterflyBench()
{
    dealloc();
}

cButterflyBench::operator int()
{
	return (hAlloc != NULL);
}

unsigned cButterflyBench::run()
{
    HCURSOR hcurOld;
	long lNumTests = AllocSize / sizeof(TestClass);
	TestClass HUGE * ptcTests = (TestClass HUGE *) pAlloc;
	TestClass tcTemp;
	
	if (ptcTests)
	{
	    MessageBox(NULL, "Starting Butterfly Thrash", "Benchmark", MB_OK);
	    hcurOld = SetCursor(LoadCursor(NULL, IDC_WAIT));
	    beginTimer();
		long l;
		for (l = 0; l < lNumTests; l++)
		{
		    TestClass tcValueGenerator;
			ptcTests[l] = tcValueGenerator;
		}
		for (int i = 0; i < numIters; i++)
		{
			for (l = 0; l < lNumTests/2L; l++)
			{
				tcTemp = ptcTests[l];
				ptcTests[l] = ptcTests[(lNumTests-1)-l];
				ptcTests[(lNumTests-1)-l] = tcTemp;
				
			}
		}
		endTimer();
		SetCursor(hcurOld);
	    MessageBox(NULL, "Benchmark done", "Benchmark", MB_OK);
		return 1;
	}
	return 0;
}

int cButterflyBench::repaint()
{
   return 0;
}

int cButterflyBench::explain()
{
    static char buf[512];
    
    sprintf(buf,"%sThe block size used is %lu.", pszBFExplain, AllocSize);
#if _WINDOWS
    MessageBox(NULL, buf, name(), MB_OK);
#else
    printf("[%s]\n\n%s",name(), buf);
#endif
    return 1;
}


