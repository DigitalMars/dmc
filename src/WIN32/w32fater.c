#include <windows.h>
#include <string.h>
#include <stdlib.h>


/* For MessageBox */
#pragma comment(lib,"user32.lib")   


void __cdecl _win32_stderrmsg(const char *msg, const char *title)
{
    HANDLE stdErrHandle = GetStdHandle(STD_ERROR_HANDLE);
    DWORD numWritten;
    
    
    BOOL bDoMessageBox = (stdErrHandle == INVALID_HANDLE_VALUE ||
                          !WriteConsole(stdErrHandle,msg,strlen(msg),
                                        &numWritten,NULL)
                          );
    
    if (bDoMessageBox)
    {
        MessageBox(NULL,msg,title,MB_OK|MB_ICONSTOP|MB_TASKMODAL);
    }
   
}

void __cdecl _win32_faterr(const char *msg)
{
    
    _win32_stderrmsg(msg,"");
    _Exit(1);
}
