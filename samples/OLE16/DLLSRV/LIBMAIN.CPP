#include "prgidmon.h"

HINSTANCE vDV_DLLhinst;

#ifdef __TURBOC__
extern "C"
int PASCAL FAR LibMain
(
	HINSTANCE   hinst,
	WORD        segDS,
	WORD        cbHeapSize,
	LPSTR       lpCmdLine
)
#else
extern "C"
BOOL PASCAL FAR LibMain
(
	HINSTANCE   hinst,
	HANDLE      segDS,
	UINT        cbHeapSize,
	LPSTR       lpCmdLine
)
#endif
{
	vDV_DLLhinst = hinst;

	UNREFERENCED(hinst);
	UNREFERENCED(cbHeapSize);
	UNREFERENCED(segDS);

	// Leave our DS unlocked when we're not running
	UnlockData( 0 );

	return TRUE;
}

extern "C" int FAR PASCAL _WEP(int)
{
	/* Your WEP functionality goes here */
	return 1;
}
