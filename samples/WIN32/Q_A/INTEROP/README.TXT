Title: Calling Routines in a Win32 DLL from 16-bit Code (WOW and Win32s)

The INTEROP sample demonstrates two general methods for calling routines 
in a Win32 DLL from 16-bit Code: thunks and SendMessage(). There are two 
different thunking methods, one for each platform: Generic Thunks on 
Windows NT and Universal Thunks on Win32s. The message used is 
WM_COPYDATA, a new message introduced by Windows NT and Win32s. All 
three methods provide a way to call functions and pass data across the 
16-32 boundary, translating any pointers in the process. The advantages 
of WM_COPYDATA over thunks are that it is fast the exact same code runs 
on either platform (the thunk used will depend on the platform). The 
disadvantage of WM_COPYDATA is that a method must be devised to get a 
function return value (other than true or false) back to the calling 
application.

The sample consists of the following source files:

   app16.c    -  Win16 Application                    
   dll16.c    -  16-bit side of Universal Thunk/Generic Thunk     
   stub32.c   -  32-bit stub that loads the 32-bit DLLs on Win32s 
   utdll32.c  -  32-bit side of the Universal Thunk               
   disp32.c   -  Dispatch calls sent through WM_COPYDATA                                       
   dll32.c    -  Win32 DLL

Generic Thunk
-------------

Under Windows NT, it is possible to call routines in a Win32 DLL from a 
Win16 application using an interface called Generic Thunks. The SDK file 
doc\sdk\misc\genthunk.txt describes the interface.

Here is a picture of the way the pieces fit together in INTEROP:
 
                             dll32
                           ---------
                          |  Win32  |
    32-bit                |   DLL   |
                           ---------
                              /|\      
             ------------------|-------
                               |       
                ---------     ------------
               | Win 3.1 |<->| 16-bit DLL |
    16-bit     |   app.  |   |   (GT)     |
                ---------     ------------
                  app16           dll16

DLL16 is loaded when APP16 is loaded. If it detects that WOW is present, 
then it loads DLL32.

WOW presents a few new 16-bit application programming interfaces (APIs) 
that allow you to load the Win32 DLL, get the address of the DLL 
routine, call the routine (passing it up to thirty-two 32-bit 
arguments), convert 16:16 (WOW) addresses to 0:32 addresses (useful if 
you need to build up a 32-bit structure that contains pointers and pass 
a pointer to it), and free the Win32 DLL. These functions are:

   DWORD FAR PASCAL LoadLibraryEx32W( LPCSTR, DWORD, DWORD );
   DWORD FAR PASCAL GetProcAddress32W( DWORD, LPCSTR );
   DWORD FAR PASCAL CallProc32W( DWORD, ..., LPVOID, DWORD, DWORD );
   DWORD FAR PASCAL GetVDMPointer32W( LPVOID, UINT );
   BOOL FAR PASCAL FreeLibrary32W( DWORD );

When linking the Win16 application, you need to put the following 
statements in the .DEF file, indicating that the functions will be 
imported from the WOW kernel:

   IMPORTS
      kernel.LoadLibraryEx32W
      kernel.FreeLibrary32W
      kernel.GetProcAddress32W
      kernel.GetVDMPointer32W
      kernel.CallProc32W

Note that although these functions are called in 16-bit code, they
need to be provided with 32-bit handles, and they return 32-bit
handles.

In addition, be sure that your Win32 DLL entry points are declared with 
the _stdcall convention; otherwise, you will get an access violation.


Universal Thunk
---------------

Under Win32s, it is possible to call routines in a Win32 DLL from a 
Win16 application using an interface called Universal Thunks. The 
interface is described in the Win32s Programmer’s Reference. The sample 
UTSAMPLE, shows the opposite (and more typical) case, a Win32 
application calling 16-bit routines.

Here is a picture of the way the pieces fit together in INTEROP:

                         stub           utdll32         dll32     
                      -----------     -----------     ---------
                     |   Win32   |-->| Win32 DLL |<->|  Win32  |
    32-bit           |    EXE    |   |    (UT)   |   |   DLL   |
                      -----------     -----------     ---------
                              /|\      /|\
             ------------------|--------|-------------------------
                               |       \|/
                ---------     ------------
               | Win 3.1 |<->| 16-bit DLL |
    16-bit     |   app.  |   |   (UT)     |
                ---------     ------------
                  app16           dll16

The load order is as follows: The Windows 3.1 application loads the
16-bit DLL. The 16-bit DLL checks to see whether the 32-bit side has
been initialized. If it has not been initialized, then the DLL spawns
the 32-bit EXE (stub), which then loads the 32-bit DLL that sets up
the Universal Thunks with the 16-bit DLL. Once all of the components
are loaded and initialized, when the Windows 3.x application calls an
entry point in the 16-bit DLL, the 16-bit DLL uses the 32-bit
Universal Thunk callback to pass the data over to the 32-bit side.
Once the call has been received on the 32-bit side, the proper Win32
DLL entry point can be called.

WM_COPYDATA
-----------

The wParam and lParam for this message are as follows:

   wParam = (WPARAM) (HWND) hwndFrom;   /* handle of sending window */
   lParam = (LPARAM) (PCOPYDATASTRUCT) pcds; 

Where hwndFrom is the handle of the sending window and COPYDATASTRUCT is 
defined as follows:

   typedef struct tagCOPYDATASTRUCT {  
       DWORD dwData;
       DWORD cbData;
       PVOID lpData;
   } COPYDATASTRUCT;

The INTEROP sample uses dwData as a function code, indicating which 
Win32 DLL entry point should be calling and lpData to contain a pointer 
to the data structure to be passed to the function.

Here is a picture of the way the pieces fit together in INTEROP:

                         disp           dll32     
                      -----------     ---------
                     |   Win32   |-->|  Win32  |
    32-bit           |    EXE    |   |   DLL   |
                      -----------     ---------
                              /|\      
             ------------------|----------------------------------
                               |       
                ---------     ------------
               | Win 3.1 |<->| 16-bit DLL |
    16-bit     |   app.  |   |  (THUNK)   |
                ---------     ------------
                  app16           dll16

DLL16 is loaded when APP16 is loaded. DISP is spawned to handle 
WM_COPYDATA messages, regardless of platform. DISP dispatches the calls 
to DLL32, marshalling the arguments.

