BOOL bDoOpenDlg ;



OPENFILENAME ofn ;

#define MAXBUF 300

#define MAXFILTERS     10
#define MAXCUSTFILTER  MAXBUF
#define FILENAMESIZE   MAXBUF
#define FILETITLESIZE  MAXBUF
#define DLGTITLESIZE   MAXBUF
#define INITDIRSIZE    MAXBUF
#define DEFEXTSIZE     40
#define TEMPNAMESIZE   30

BOOL APIENTRY OpenFunc(HWND, UINT, UINT, LONG) ;
void FillOpenDlg(HWND, LPOPENFILENAME) ;
void InitOpenStruct(HWND, LPOPENFILENAME) ;
void GetOpenDlg(HWND, LPOPENFILENAME) ;
void InitFilterString(void) ;
void InitCustFilterString(void) ;

TCHAR szFileTitle[FILETITLESIZE]  ;
TCHAR szDlgTitle[DLGTITLESIZE]    ;
TCHAR szFileName[FILENAMESIZE]    ;
TCHAR szInitialDir[INITDIRSIZE]   ;
TCHAR szDefExt[DEFEXTSIZE]        ;
TCHAR szTempName[TEMPNAMESIZE]    ;

TCHAR szFilterInits[MAXFILTERS][30] ;
TCHAR szFilterString[MAXBUF] ;
LPTSTR lpszFilterString ;

TCHAR  szCustFiltInits[3][30] ;
TCHAR  szCustFilterString[MAXBUF] ;
LPTSTR lpszCustFilterString ;


DWORD dwFlags ;


void DoOpenDialog(HWND) ;


#define ID_FILTERO                   400
#define ID_CUSTFILTO                 401
#define ID_MAXCUSTFILTO              402
#define ID_FILTINDEXO                403
#define ID_SZFILEO                   404
#define ID_MAXSZFILEO                405
#define ID_SZFILETITLEO              406
#define ID_MAXSZFILETITLEO           407
#define ID_SZINITDIRO                408
#define ID_SZTITLEO                  409
#define ID_FLAGSO                    410
#define ID_FILEOFFO                  411
#define ID_FILEEXTO                  412
#define ID_SZDEFEXTO                 413
#define ID_ERRORO                    414
#define ID_RETURNO                   415
#define ID_OPENDIALOG                606
#define ID_STRUCTSIZEO               416
#define ID_HWNDOWNERO                417
#define ID_HINSTANCEO                418
#define ID_CUSTDATAO                 419
#define ID_HOOKO                     420
#define ID_TEMPLATEO                 421
#define ID_ADD1O                     422
#define ID_ADD2O                     423
#define ID_CLEAR1O                   424
#define ID_CLEAR2O                   425
#define ID_RESETOPEN                 601
#define ID_NULLSTRUCTO               602
#define ID_USEHINSTO                 607
#define ID_MULTIOPEN                 608
#define ID_FILEPREVIEW               609
