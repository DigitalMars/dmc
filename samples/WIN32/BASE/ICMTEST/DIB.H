// **************************************************************************
// 								    
// FILE	: DIB.H 						    
// 								    
// DESCRIPTION : Header/include file for DIB.C.			    
// 								    
// ***************************************************************************  

// Macro to swap two values   
#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))

// flags for _lseek   
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#define MAXREAD  32768		// Number of bytes to be read during   
				                  // each read operation. 	       

#define PALVERSION      0x300
#define MAXPALETTE	256	// max. # supported palette entries   

// Header signatutes for various resources   
#define BFT_BITMAP 0x4d42   // 'BM'   

// macro to determine if resource is a DIB   
#define ISDIB(bft) ((bft) == BFT_BITMAP)

// Macro to determine to round off the given value to the closest byte   
#define WIDTHBYTES(i)	((i+31)/32*4)

DWORD PASCAL lread(int fh, VOID FAR *pv, DWORD ul);
DWORD PASCAL lwrite(int fh, VOID FAR *pv, DWORD ul);

BOOL	InitDIBs(void);
BOOL	InitIndividualDIB (UINT uiBMPIndex);
HANDLE	OpenDIB(LPSTR szFile);
HANDLE	ReadDibBitmapInfo (int fh);
WORD	DibNumColors (VOID FAR * pv);
HBITMAP BitmapFromDib (HANDLE hdib, HPALETTE hpal);
BOOL	DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop);
BOOL	CreateandDrawBitmap(HDC hdc, int x, int y, HANDLE hdib, DWORD rop);
HPALETTE CreateDibPalette (HANDLE hdib);
