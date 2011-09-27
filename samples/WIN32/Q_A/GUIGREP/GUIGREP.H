
#define IDM_ABOUT 100
#define IDM_DEFGREP 101
#define IDM_INSTALL 102
#define IDM_STRINGEDIT 103
#define IDM_TERMBOX 104
#define IDM_STARTGREP 105
#define IDM_UNINSTALL 106
#define IDM_SORTBOX 107
#define IDM_CASESENS 108
#define IDM_CANCEL 109

#define WM_INQUIREFILES WM_USER+0xc001
#define WM_LAUNCH       WM_USER+0xc002
#define WM_FREEAGAIN    WM_USER+0xc003
#define WM_ADDITEM      WM_USER+0xc004
#define WM_TELLHANDLE   WM_USER+0xc005
#define WM_STARTSYNC    WM_USER+0xc006


#define MAXLINELEN      256
#define FILBUFSIZE      16384
#define MAX_CONCURRENT_THREADS 30


BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long FAR PASCAL MainWndProc(HWND, UINT, UINT, LONG);
BOOL FAR PASCAL About(HWND, UINT, UINT, LONG);


typedef struct tagTHREADPBLOCK
{
 int  iLoop;
 HWND hWndGlobal;
 HWND hWndFileBox;
} THREADPBLOCK, *LPTHREADPBLOCK;


/*********************************************************************/
/*                                                                   */
/*     C++ declarations here                                         */
/*                                                                   */
/*********************************************************************/

/*********************************************************************/
/*                                                                   */
/*     The memory class is a simple fast allocator. It provides      */
/*     one static memory block from which little chunks are          */
/*     allocated by each thread.                                     */
/*                                                                   */
/*********************************************************************/

class memory
{
private:  LPSTR lpBasePointer;  /* Probably don't need this one at all... */
	  LPSTR lpCurrentPointer;
	  LPSTR lpReturnPointer;
	  int iCurrentPointer;
	  int iActualBlockSize;
	  HANDLE hBlock;
	  CRITICAL_SECTION csAllocGuard;
public:   memory(int);
	  ~memory(void);
	  LPSTR Alloc (int iSize)
	  {      EnterCriticalSection(&csAllocGuard);
#ifdef MIPS
	/* MIPS requires all DWORDs to be aligned on DWORD boundaries.
	   That's why under MIPS, we round up all alignments to
	   multiples of sizeof(DWORD).   */
	   if (iSize != (int)((iSize / sizeof (DWORD)) * sizeof (DWORD)))
	      iSize = (1 + (iSize / sizeof (DWORD))) * sizeof (DWORD);
#endif
	     if (iSize + iCurrentPointer > iActualBlockSize)
		{               MessageBox(GetFocus(),"No memory left",
					"Grepper",MB_OK);
				    return((LPSTR)0);
		};
		iCurrentPointer+= iSize;
		lpReturnPointer = lpCurrentPointer;
	_try {
		lpCurrentPointer = &lpCurrentPointer[iSize];
		}
		_except (EXCEPTION_EXECUTE_HANDLER) {
		 MessageBox(GetFocus(),"Access Violation","Memory Allocator",MB_OK);
		};
		LeaveCriticalSection(&csAllocGuard);
		return (lpReturnPointer);
	   };
	   void Reset()
	   {lpCurrentPointer = lpBasePointer;
	    iCurrentPointer = 0;
	   };
	   void Free (LPSTR lpBlockPt)
	   {};
};

memory::memory(int iSize)
{
  hBlock = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT,iSize);
  lpBasePointer = (LPSTR) LocalLock(hBlock);
  lpCurrentPointer = lpBasePointer;
  iActualBlockSize = iSize;
  iCurrentPointer = 0;
  InitializeCriticalSection(&csAllocGuard);
}

memory::~memory()
{ LocalUnlock(hBlock);
  LocalFree(hBlock);
  DeleteCriticalSection(&csAllocGuard);
}

/*********************************************************************/
/*                                                                   */
/*     GrowMemory provides a growable block of memory.               */
/*     It is used for the local handle of the edit box that          */
/*     accomodates the hits.                                         */
/*                                                                   */
/*********************************************************************/

class growmemory
{
private: int iCurrentSize,iCurrentPt,iIncrement;
		 HANDLE hCurrentBlock;
		 LPSTR lpStartPt;
		 CRITICAL_SECTION csAllocGuard;
	 char lpszCRLF[3];
	 int ilpszCRLF;

public: growmemory(int iInitSize, int iInc);
	    ~growmemory(void);
	    BOOL add(LPSTR lpSource, int iSize)
/* watch out here: We assume that the requested size is never bigger than
   the increment. This is safe with the hardcoded values but may fail in
   the general case. */

		   { int iSizeOld = iSize;
		     EnterCriticalSection(&csAllocGuard);
/* adjust the size for the line feed to follow */
			     iSize+=ilpszCRLF;
		     if (iCurrentPt+iSize > iCurrentSize)
		      {iCurrentSize+=iIncrement;
		   hCurrentBlock = LocalReAlloc(hCurrentBlock,iCurrentSize,LMEM_MOVEABLE);
				  };
				  lpStartPt = (LPSTR)LocalLock(hCurrentBlock);
				  if (!lpStartPt)
				   { MessageBeep(0);
				     return(FALSE);     };
				  strncpy(&lpStartPt[iCurrentPt],lpSource,iSizeOld);
/* add the CRLF at the end. */
				  strncpy(&lpStartPt[iCurrentPt+iSizeOld],lpszCRLF,ilpszCRLF);
				  LocalUnlock(hCurrentBlock);
				  iCurrentPt+=iSize;
				  LeaveCriticalSection(&csAllocGuard);
				  return(TRUE);
			    };
		HANDLE GetHandle(void)
			{ return (hCurrentBlock);
			};
};

growmemory::growmemory(int iInitSize, int iInc)
		   { iIncrement=iInc;
		     iCurrentPt=0;
		     iCurrentSize=iInitSize;
		     hCurrentBlock = LocalAlloc(LMEM_MOVEABLE,iCurrentSize);
				 InitializeCriticalSection(&csAllocGuard);
				 strcpy(lpszCRLF,"\r\n");
				 ilpszCRLF=strlen(lpszCRLF);
		   }
growmemory::~growmemory(void)
		   {DeleteCriticalSection(&csAllocGuard);
		    LocalFree(hCurrentBlock);
		   }
