#include <windows.h>
#include <ole2.h>

#pragma warning ( disable : 4355 )
/*
 *  Warning 4355 warns against using the *void* pointer in a
 *  constructor.  Since we do void frequently, and it is harmless,
 *  we disable the warning.
 */

#define UNREFERENCED(a) ((void)(a))


STDAPI  DllGetClassObject(REFCLSID clsid, REFIID iid, void FAR* FAR* ppv);
STDAPI CreateProgIdMoniker( LPSTR szProgId, LPMONIKER FAR * ppmk);

#ifdef DEFINE_PRGIDMON_GUID_HERE
#include <initguid.h>
#endif

DEFINE_GUID(CLSID_ProgIdMoniker, 0xD6286D00, 0xCF38, 0x1068, 0xB8, 0x7E, 0x00, 0xDD, 0x01, 0x07, 0xD0, 0xBD);
