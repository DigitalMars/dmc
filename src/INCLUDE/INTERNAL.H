/* Items needed from MS's internal.h and mtdll.h*/

#if !defined (CRTDLL) || !defined (DLL_FOR_WIN32S)
int _callnewh();
extern int _newmode;    /* malloc new() handler mode */
#define _HEAP_MAXREQ  0xFFFFFFE0
#endif  /* !defined (CRTDLL) || !defined (DLL_FOR_WIN32S) */
