/* copyright 1995  - Symantec Corporation */
/* wrapper for the two different MFC header files*/

#if defined(_WIN32)
#include <32-bit\afxsock.inl>
#else
#include <16-bit\afxsock.inl>
#endif


