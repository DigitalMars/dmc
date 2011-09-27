
// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

// Updated: 11th January 2004

#define	_STLSOFT_STRICT

#include <windows.h>
#include <time.h>

//#define	_COMSTL_NO_UUIDOF

#include <stlsoft.h>

#ifdef __SYNSOFT_COMPILER_IS_DMC
 #error Alas the Digital Mars compiler cannot handle the interface casts yet. Maybe 8.35 ... ?
#endif /* __SYNSOFT_COMPILER_IS_DMC */

#ifdef __STLSOFT_COMPILER_IS_MSVC
#pragma warning(disable : 4100)
#pragma warning(disable : 4508)
#pragma warning(disable : 4511)
#pragma warning(disable : 4512)
//#error
#endif

#include <comstl.h>

#include <stlsoft_argument_proxies.h>

stlsoft_ns_using(pointer_proxy)

#include <stlsoft_exceptions.h>

stlsoft_ns_using(null_exception)


#include <comstl_interface_cast.h>
#include <comstl_initialisers.h>
#include <comstl_string_access.h>

stlsoft_ns_using(c_str_ptr_a)
comstl_ns_using(ignore_interface_cast_exception)
comstl_ns_using(interface_cast_addref)
comstl_ns_using(interface_cast_noaddref)
comstl_ns_using(interface_cast_test)

#include <string>

comstl_ns_using_std(string)

#include <stdio.h>

comstl_ns_using(com_initialiser)

#ifndef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
stlsoft_ns_using(get_ptr)
#endif /* __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */



int main(int /* argc */, char ** /*argv*/)
{
    com_initialiser	coinit;	// Initialise COM
    HRESULT			hr;

	fprintf(stderr, "interface_cast_test\n");

    if(!coinit)
    {
		fprintf(stderr, "Failed to initialise COM libraries\n");

        hr = E_UNEXPECTED;
    }
    else
    {
		LPSTREAM	pstm;

		hr	=	::CreateStreamOnHGlobal(NULL, true, &pstm);

		if(SUCCEEDED(hr))
		{
			{
				if(interface_cast_test<IStream*>(pstm))
				{
					interface_cast_noaddref<IStream*>	stm(pstm);

					stm->Revert();

#if 0 // Enable this to demonstrate how Release() is inaccessible
# ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
					stm->Release();
# endif // __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
#endif // 0

					IStream	*pstm2	=	get_ptr(stm);

					fprintf(stderr, "Cast to IStream* %s\n", "succeeded");

					STLSOFT_SUPPRESS_UNUSED(pstm2);
				}
				else
				{
					fprintf(stderr, "Cast to IStream* %s\n", "failed");
				}
			}

			{
				if(interface_cast_test<IStorage*>(pstm))
				{
					fprintf(stderr, "Cast to IStorage* %s\n", "succeeded");
				}
				else
				{
					fprintf(stderr, "Cast to IStorage* %s\n", "failed");
				}
			}

			{
				try
				{
					fprintf(stderr, "Trying excepting cast\n");

					interface_cast_noaddref<IStorage*>	stg(pstm);

					stg->SetClass(GUID_NULL);

					fprintf(stderr, "Cast succeeded\n");
				}
				catch(comstl::bad_interface_cast &x)
				{
					fprintf(stderr, "exception caught: %s\n", x.what());
				}
				catch(...)
				{
					fprintf(stderr, "exception caught\n");
				}
			}

			{
				LPUNKNOWN	punk	=	pstm;
				LPSTREAM	pstm3	=	interface_cast_addref<IStream*>(punk);

#if 0 // Enable this to demonstrate how Release() is inaccessible
# ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
				interface_cast_addref<IStream*>(punk)->Release();
# endif // __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
#endif // 0
				fprintf(stderr, "Cast %s\n", pstm3 == 0 ? "failed" : "succeeded");

				pstm3->Release();

				pstm->Release();
			}
		}
		else
		{
			fprintf(stderr, "Failed to create stream\n");
		}
    }

    return FAILED(hr);
}


#ifdef x__STLSOFT_COMPILER_IS_INTEL
int WINAPI WideCharToMultiByte(
    UINT     CodePage,
    DWORD    dwFlags,
    LPCWSTR  lpWideCharStr,
    int      cchWideChar,
    LPSTR   lpMultiByteStr,
    int      cbMultiByte,
    LPCSTR   lpDefaultChar,
    LPBOOL  lpUsedDefaultChar)
{
	HINSTANCE	hinst	=	::LoadLibrary("KERNEL32");
	int (WINAPI *pfnWideCharToMultiByte)(
    UINT     ,
    DWORD    ,
    LPCWSTR  ,
    int      ,
    LPSTR   ,
    int      ,
    LPCSTR   ,
    LPBOOL  );

	(FARPROC&)pfnWideCharToMultiByte = ::GetProcAddress(hinst, "WideCharToMultiByte");

	return pfnWideCharToMultiByte (
     CodePage,
     dwFlags,
     lpWideCharStr,
     cchWideChar,
    lpMultiByteStr,
     cbMultiByte,
     lpDefaultChar,
    lpUsedDefaultChar);
}
#endif
