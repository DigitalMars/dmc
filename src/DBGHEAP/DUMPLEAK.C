#ifdef _DEBUG
#include <scrtl.h>
#include <crtdbg.h>

void __DumpMemLeaks()
{
       if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_LEAK_CHECK_DF)
        {
            _CrtSetDumpClient(NULL);
            _CrtDumpMemoryLeaks();
        }
}


void (*_DumpMemLeaksp)(void) = __DumpMemLeaks;

#endif
