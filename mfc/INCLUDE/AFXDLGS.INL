/* copyright 1993  - Symantec Corporation */
/* wrapper for the two different MFC header files*/


#if defined(_WIN32)
#include <32-bit\afxdlgs.inl>
#else
#include <16-bit\afxdlgs.inl>
#endif



