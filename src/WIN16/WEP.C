#include <windows.h>
#include <process.h>

extern char __dll_started;

int FAR PASCAL _WEP (int parm);
int  (FAR PASCAL *_pRawWEP)(int);

int FAR PASCAL WEP (int parm)
{
  int retcode;

  if (!__dll_started)
    goto dont;
  __asm
  {
    mov  AX,DS
    lar  AX,AX
    jnz  dont
    test AX,8000h
    jz   dont
  }
  retcode = _WEP (parm);
  _cexit ();
  if (_pRawWEP)
    retcode = _pRawWEP(parm);
  return retcode;
dont:
  return 1;
}
