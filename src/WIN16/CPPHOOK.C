#include <windows.h>

int (FAR PASCAL *_pRawLibMain)(HANDLE,WORD,WORD,LPSTR);


int FAR PASCAL __call_rawlibmain(HANDLE hInstance, WORD dataseg, WORD heapsize, LPSTR commandline)
{
   if (_pRawLibMain)
     return _pRawLibMain(hInstance,dataseg,heapsize,commandline);

   return TRUE; 
}

