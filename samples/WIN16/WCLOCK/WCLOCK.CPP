/*  Symantec C++ Demonstration Program
 *  Copyright (C) 1993 Symantec, Corp.
 */

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "wclock.h"

#define	CLKCLASSNAME	(ClkClassName)
#define	CLKWINDOWNAME	(ClkWndName)
#define	CLKMENUNAME	"CLKMENU"

static char ClkClassName[]	= "Symantec Weird Clock Class";
static char ClkWndName[]	= "Weird Clock";

HANDLE clkhInst		;		// clock instance

#define	CLKTIMERID	42
#define	TIMERINTERVAL	475
#define	MAXARRAY	128


typedef enum paintform	{		// clock repaint form
    Hands, All
} paintform;


typedef enum ptform	{		// clock tick mark form
    HourPt, MinutePt
} ptform;


typedef enum handform	{		// clock hand form
    HourHand, MinuteHand, SecondHand
} handform;

static int array[MAXARRAY] = {0};
static int count = 0;

FARPROC	procCountPt	;
FARPROC procSamplePt	;

HBRUSH	hourBrush;			// brush for hour ticks
HBRUSH  minuteBrush;			// brush for minute/second ticks
HBRUSH	backgroundBrush;		// brush for clock background
HPEN	backgroundPen;			// pen   for clock background
struct	handPen {			// pens  for clock hands
 HPEN	hourPen  ,
        minutePen,
        secondPen;
} handPen;


typedef struct ClockTime {
    unsigned char hour, minute, second;
} ClockTime;


ClockTime curTime	= { 0, 0, 0 };	// contains current time as displayed


#define	FACEPTS	60			// number of points on the clock face

typedef struct facept {
  POINT	 pt;
  ptform form;
} facept;

facept	facePts[FACEPTS];		// array of face points
POINT	clockCenter	;		// center point for hands
RECT	clockRect	;		// current clock client rect

int	desktopWidth	;		// width of the windows desktop
int	desktopHeight	;		// height of the desktop

BOOL doShapes=1;

#define MAXPTWIDTH	16		// maximum hour point width
#define MAXPTHEIGHT	MAXPTWIDTH	// maximum height
#define MINPTWIDTH	2		// minimum width
#define MINPTHEIGHT	1		// minimum height
#define ELLIPSEADJ	2		// ellipse width adjustment

#define HOURWIDTH	7		// width of hour hand
#define	MINUTEWIDTH	4		// width of minute hand
#define SECONDWIDTH	1		// width of second hand

#define	HOURSCALELEN	45		// percentage scale for hour hand length
#define MINUTESCALELEN	70		//   for minute hand
#define	SECONDSCALELEN	85		//   for second hand

#define HOURFREQ	5		// frequency of hour ticks
#define	HourToIndex(h)	((h)*HOURFREQ)


#define BORDERRATIO	20		// constants for square clock face
#define SIDES		4
#define	STEPS		(FACEPTS / SIDES)


#define MAXRGB	8

struct rgbs {
    long rgb;
    char used;
} rgbs[MAXRGB] = {
	0x00000000,  0,			// black
	0x000000FF,  0,			// red
	0x0000FF00,  0,			// green
	0x0000FFFF,  0,			// yellow
	0x00FF0000,  0,			// blue
	0x00FF00FF,  0,			// pink
	0x00FFFF00,  0,			// cyan
	0x00FFFFFF,  0			// white
};


#define	Rand(n)	( !(n) ? 0 : (rand() % (n)) )

//----------------- function prototypes ------------------------------

extern "C" {
  void CalcClockFace(void);
  void SetClockRect(int w, int h);
  void GetTime(ClockTime *t);
  void DrawClockFace(HDC hDC);
  void FAR PASCAL CountPt(int x, int y, LPSTR lpData);
  void FAR PASCAL SamplePt(int xP, int yP, LPSTR lpData);
  void AdjustIndex(int *index);
  void RenderHand(HDC	 hDC	,
		handform hform	,
		int	 x1	,
		int	 y1	,
		int	 x2	,
		int	 y2	);
  void DrawClockHand(HDC    hDC   ,
		   handform  hform ,
		   paintform pform ,
		   int	     oldh  ,
		   int	     newh  ,
		   HPEN	     pen   );
  void DrawClockHands(HDC hDC, paintform pform);
  void RedrawFaceAndHands(HDC hDC, paintform pform);
  void PaintClock(HWND hWnd, HDC hDC, paintform pform);
  void UpdateClock(HWND hWnd);
  void PaintBackground(HWND hWnd, HDC hDC);
  long GetRandomRGB(int init);
  void InitPensAndBrushes(void);
  void DeletePensAndBrushes(void);
  void InfiniteLoop(void);
  void MemoryOverwrite(void);
  void RepaintClock(HWND hWnd);
  long FAR PASCAL ClkWndProc(HWND hWnd	,
		   unsigned msg		,
		   WPARAM   wParam	,
		   LPARAM   lParam	);
  void InitScalingValues(void);
  int  InitClockTimer(HWND hWnd);
  int  InitClock(HANDLE hInst	,
	       HANDLE hPrevInst	,
	       int    nCmdShow	);
  int  ClockMessageLoop(void);
  int  PASCAL WinMain(HANDLE hInst	,
		   HANDLE hPrevInst	,
		   LPSTR  lpCmdLine	,
		   int	  nCmdShow	);
};

//------------------------------------------------------------------
// shape class stuff

class shape{
public:
virtual void draw(HDC hDC)=0;
void setlefttop(POINT p){lefttop.x=p.x;lefttop.y=p.y;};
void setlefttop(int xx, int yy){lefttop.x=xx;lefttop.y=yy;};
void getlefttop(POINT& p){p.x=lefttop.x;p.y=lefttop.y;};
protected:
POINT lefttop;
};

class ellipse: public shape{
public:
virtual void draw(HDC hDC)
   {Ellipse(hDC,lefttop.x,lefttop.y,lefttop.x+x,lefttop.y+y);};
void setaxes(int xx,int yy){x=xx;y=yy;};
void getaxes(int& xx, int& yy){xx=x;yy=y;};
protected:
int x;
int y;
};

class circle: public ellipse{
public:
void setradius(int r){x=r;y=r;};
void getradius(int&r){r=x;};
};

class rectangle:public shape{
public:
virtual void draw(HDC hDC)
   {Rectangle(hDC,lefttop.x,lefttop.y,lefttop.x+h,lefttop.y+v);};
void setsides(int hh, int vv){h=hh;v=vv;};
void getsides(int& hh, int& vv){hh=h;vv=v;};
protected:
int h;
int v;
};

class square:public rectangle{
public:
void setside(int s){h=s;v=s;};
void getside(int& s){s=h;};
};

//------------------------------------------------------------------

void CalcClockFace(void)		// calculates a square clock face
{
    int	   i, j, k;
    int	   width, height;
    int	   hStep, vStep;
    RECT   border;
    facept *p;
    ptform f;

    width	  = (clockRect.right  - clockRect.left);
    height	  = (clockRect.bottom - clockRect.top );

    border.left	  = width  / BORDERRATIO;
    border.top	  = height / BORDERRATIO;

    hStep         = (width  - border.left * 2) / STEPS;
    vStep	  = (height - border.top  * 2) / STEPS;

    border.right  = hStep * (STEPS + 1);
    border.bottom = vStep * (STEPS + 1);

    for (i = 0, j = hStep / 2, k = 3; i < STEPS; ++i, j += hStep, ++k) {
        f	= (!(k % HOURFREQ)) ? HourPt : MinutePt;

        p	= &facePts[i];
        p->pt.x = border.left + j;
        p->pt.y	= border.top;
        p->form	= f;

	p	= &facePts[i+STEPS*2];
	p->pt.x = border.right - j;
	p->pt.y = border.bottom;
	p->form = f;
    }

    for (i = 0, j = vStep / 2, k = 3; i < STEPS; ++i, j += vStep, ++k) {
        f	= (!(k % HOURFREQ)) ? HourPt : MinutePt;

	p	= &facePts[i+STEPS];
	p->pt.x	= border.right;
	p->pt.y = border.top + j;
	p->form	= f;

	p	= &facePts[i+STEPS*3];
	p->pt.x	= border.left;
	p->pt.y = border.bottom - j;
	p->form	= f;
    }

    clockCenter.x = (width  / 2) + Rand(width  / 4) * ((Rand(100) < 50) ? 1 : -1);
    clockCenter.y = (height / 2) + Rand(height / 4) * ((Rand(100) < 50) ? 1 : -1);
}


void SetClockRect(int w, int h)
{
    clockRect.top    = 0;
    clockRect.left   = 0;
    clockRect.right  = w;
    clockRect.bottom = h;
}


void GetTime(ClockTime *t)
{
#ifndef  PURE_ANSI
    union REGS r;
#else
    union _REGS r;  // _MSC_VER
#endif

    r.h.ah = 0x2C;		// get system time
#ifndef  PURE_ANSI
    int86(0x21, &r, &r);
#else
    _int86(0x21, &r, &r);       
#endif

    t->hour   = r.h.ch;
    t->minute = r.h.cl;
    t->second = r.h.dh;
}


void DrawClockFace(HDC hDC)
{
    RECT     rect;
    facept   *p;
    unsigned i, hWidth, hHeight, width, height;

    hWidth  = (MAXPTWIDTH  * (clockRect.right  - clockRect.left)) / desktopWidth ;
    hHeight = (MAXPTHEIGHT * (clockRect.bottom - clockRect.top )) / desktopHeight;

    if (hHeight < MINPTHEIGHT) hHeight = MINPTHEIGHT;
    if (hWidth  < MINPTWIDTH ) hWidth  = MINPTWIDTH ;

    for (i = 0; i < FACEPTS; ++i) {
        p = &facePts[i];
        if (p->form == HourPt) {
            width  = hWidth;
            height = hHeight;
        } else {
            width  = height = 1;
        }
        rect.left   = p->pt.x   - width  / 2;
        rect.right  = rect.left + width     ;
        rect.top    = p->pt.y   - height / 2;
        rect.bottom = rect.top  + height    ;

	if (p->form == HourPt) {
	    SelectObject(hDC, hourBrush);
	    RoundRect(hDC		  ,
	              rect.left		  ,
	              rect.top		  ,
	              rect.right	  ,
	              rect.bottom	  ,
		      width  + ELLIPSEADJ ,
		      height + ELLIPSEADJ );
	} else
            FillRect(hDC, &rect, minuteBrush);
    }
    
if(doShapes)
    {	
    int j = 50;
    shape *z;        
    for(int k=0;k<2;k++,j *= 2)
    {    
    if((i=rand())<8000)
       {
       circle c;
       c.setradius(j*width);
       z=&c;
       }
    else if(i<16000)
       {
       rectangle r;
       r.setsides(j*width,2*j*height);
       z=&r;
       }
    else if(i<24000)
       {
       square s;
       s.setside(j*width);
       z=&s;
       }
   else
       {
       ellipse e;
       e.setaxes(j*width,3*j*height);
       z = &e;
       }
     z->setlefttop(clockCenter.x-k*j,clockCenter.y-k*j);
     z->draw(hDC);  
     }
     }
}



typedef struct samplepoint {
    int	   count, max;
    int	   x, y;
} samplepoint;    


void FAR PASCAL CountPt(int x, int y, LPSTR lpData)
{
    ++*(unsigned int far *)lpData;
}


void FAR PASCAL SamplePt(int xP, int yP, LPSTR lpData)
{
    samplepoint far *s = (samplepoint far *)lpData;

    if (s->count++ == s->max) {
        s->x = xP;
        s->y = yP;
    }
}


void RenderHand(HDC	 hDC	,
		handform hform	,
		int	 x1	,
		int	 y1	,
		int	 x2	,
		int	 y2	)
{
    unsigned int count, scale;
    samplepoint sPt;

    // implementation of hand as straight line

    switch (hform) {

    case HourHand   :
        scale = HOURSCALELEN   ;
        break;

    case MinuteHand :
        scale = MINUTESCALELEN ;
        break;

    case SecondHand :
        scale = SECONDSCALELEN ;
        break;
    }

    count = 0;
    LineDDA(x1, y1, x2, y2, procCountPt, (LPARAM)((int far *)(&count)));

    sPt.max   = (count * scale) / 100;
    sPt.count = 0;
    LineDDA(x1, y1, x2, y2, procSamplePt, (LPARAM)((int far *)(&sPt)));

    MoveTo(hDC, x1, y1);
    LineTo(hDC, sPt.x, sPt.y);
}


void AdjustIndex(int *index)
{
    // adjust time index to correspond to facePt array format
    if ((*index += 7) >= 60) *index -= 60;
}


void DrawClockHand(HDC	     hDC   ,
		   handform  hform ,
		   paintform pform ,
		   int	     oldh  ,
		   int	     newh  ,
		   HPEN	     pen   )
{
    SetROP2(hDC, R2_NOT);
    SelectObject(hDC, pen);

    AdjustIndex(&oldh);
    AdjustIndex(&newh);

    if ((pform == All) || (oldh != newh)) {	// draw newh hand
        RenderHand(hDC		     ,
        	   hform	     ,
        	   clockCenter.x     ,
        	   clockCenter.y     ,
        	   facePts[newh].pt.x ,
        	   facePts[newh].pt.y );
    }

    if ((pform != All) && (oldh != newh)) {	// erase oldh hand
        RenderHand(hDC		     ,
        	   hform	     ,
        	   clockCenter.x     ,
        	   clockCenter.y     ,
        	   facePts[oldh].pt.x ,
        	   facePts[oldh].pt.y );
    }
}


void DrawClockHands(HDC hDC, paintform pform)
{
    ClockTime newTime;

    GetTime(&newTime);

    DrawClockHand(hDC			    ,
                  SecondHand		    ,
                  pform			    ,
                  curTime.second	    ,
                  newTime.second	    ,
                  handPen.secondPen	    );

    DrawClockHand(hDC			    ,
                  MinuteHand		    ,
                  pform			    ,
                  curTime.minute	    ,
                  newTime.minute	    ,
                  handPen.minutePen	    );

    DrawClockHand(hDC			    ,
                  HourHand		    ,
                  pform			    ,
                  HourToIndex(curTime.hour) ,
                  HourToIndex(newTime.hour) ,
                  handPen.hourPen	    );

    curTime.second = newTime.second;
    curTime.minute = newTime.minute;
    curTime.hour   = newTime.hour  ;
};


void RedrawFaceAndHands(HDC hDC, paintform pform)
{
    DrawClockFace (hDC);
    DrawClockHands(hDC, pform);
}


void PaintClock(HWND hWnd, HDC hDC, paintform pform)
{
    if (pform == All) {
        CalcClockFace();
	RedrawFaceAndHands(hDC, All);
    } else
	DrawClockHands(hDC, Hands);
}



void UpdateClock(HWND hWnd)
{
    ClockTime t;
    HDC	hDC;

    GetTime(&t);

    if ((t.second != curTime.second) ||
        (t.minute != curTime.minute) ||
        (t.hour   != curTime.hour  )) {

	hDC = GetDC(hWnd);
        PaintClock(hWnd, hDC, Hands);
        ReleaseDC(hWnd, hDC);
    }
}


void PaintBackground(HWND hWnd, HDC hDC)
{
    RECT rect;

    GetClientRect(hWnd, &rect);
    FillRect(hDC, &rect, backgroundBrush);
}


long GetRandomRGB(int init)
{
    int i;

    if (init) {
	for (i = 0; i < MAXRGB; ++i)
	    rgbs[i].used = FALSE;
    }

    do
      i = Rand(MAXRGB);
    while (rgbs[i].used);
        
    rgbs[i].used = TRUE;

    return rgbs[i].rgb;
}


void InitPensAndBrushes(void)
{
    hourBrush	      = CreateSolidBrush(GetRandomRGB(TRUE ));
    minuteBrush	      = CreateSolidBrush(GetRandomRGB(FALSE));
    backgroundBrush   = CreateSolidBrush(GetRandomRGB(FALSE));

    handPen.hourPen   = CreatePen(PS_SOLID, HOURWIDTH  , 0);
    handPen.minutePen = CreatePen(PS_SOLID, MINUTEWIDTH, 0);
    handPen.secondPen = CreatePen(PS_SOLID, SECONDWIDTH, 0);

    backgroundPen     = CreatePen(PS_SOLID, 1, GetRandomRGB(FALSE));
};


void DeletePensAndBrushes(void)
{
    DeleteObject(hourBrush	  );
    DeleteObject(minuteBrush	  );
    DeleteObject(backgroundBrush  );

    DeleteObject(handPen.hourPen  );
    DeleteObject(handPen.minutePen);
    DeleteObject(handPen.secondPen);

    DeleteObject(backgroundPen    );
}


void InfiniteLoop(void)
{
    int inCount = 0, outCount = 0;

    while (inCount < 20) {
        ++outCount;
    }

    MessageBox(0, "Escaped from infinite loop!", "Note", MB_ICONINFORMATION);
}



void MemoryOverwrite(void)
{
    int i;

    for (i = 0; i <= MAXARRAY; ++i)
        array[i] = i;
}


void RepaintClock(HWND hWnd)
{
    PAINTSTRUCT paint;

    DeletePensAndBrushes();
    InitPensAndBrushes();

    InvalidateRect(hWnd, (LPRECT)NULL, TRUE);

    BeginPaint(hWnd, &paint);
    PaintClock(hWnd, paint.hdc, All);
    EndPaint(hWnd, &paint);
}


long FAR PASCAL ClkWndProc(HWND	    hWnd	,
			   unsigned msg		,
			   WPARAM   wParam	,
			   LPARAM   lParam	)
{
    switch (msg) {

    case WM_COMMAND	:

        switch (wParam) {
        
	case IDM_SHAPE:
	     HMENU hMenu = GetMenu(hWnd);
	     BOOL bChecked = GetMenuState(hMenu,IDM_SHAPE,MF_BYCOMMAND);
	     if(bChecked&MF_CHECKED)
	     	{
	     	CheckMenuItem(hMenu,IDM_SHAPE,MF_BYCOMMAND|MF_UNCHECKED);
	     	doShapes=0;
	     	}
	     else
	     	{
	     	CheckMenuItem(hMenu,IDM_SHAPE,MF_BYCOMMAND|MF_CHECKED);
	     	doShapes = 1;
	     	}
	     RepaintClock(hWnd);
	     break;
	     	     
        case IDM_NEWCOLORS :
            RepaintClock(hWnd);
            break;

	case IDM_INFINITE  :
	    InfiniteLoop();
	    break;

	case IDM_OVERWRITE :
	    MemoryOverwrite();
	    break;

        case IDM_CRASH	   :
            TraverseTree();
            break;

	case IDM_ABOUT	   :
	    AboutClock(hWnd, clkhInst);
	    break;
        }
        break;

    case WM_SIZE	:
        SetClockRect(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_PAINT	:
	RepaintClock(hWnd);
	break;

    case WM_ERASEBKGND	:
        PaintBackground(hWnd, (HDC)wParam);
	break;

    case WM_TIMER	:
    case WM_TIMECHANGE	:
        UpdateClock(hWnd);
        break;

    case WM_DESTROY	:
        KillTimer(hWnd, CLKTIMERID);
        DeletePensAndBrushes();

        PostQuitMessage(0);
        break;

    default		:
        return DefWindowProc(hWnd, msg, wParam, lParam);

    }
    return 0L;
}


void InitScalingValues(void)
{
    HDC hDC;

    hDC		  = GetDC(NULL);
    desktopWidth  = GetDeviceCaps(hDC, HORZRES);
    desktopHeight = GetDeviceCaps(hDC, VERTRES);

    ReleaseDC(NULL, hDC);
}


int InitClockTimer(HWND hWnd)
{
    ClockTime t;

    GetTime(&t);
    GetTime(&curTime);

    while ((t.second == curTime.second) &&
           (t.minute == curTime.minute) &&
           (t.hour   == curTime.hour  ))
        GetTime(&curTime);

    if (!SetTimer(hWnd, CLKTIMERID, TIMERINTERVAL, 0L))
        return FALSE;

    return TRUE;
}


int InitClock(HANDLE hInst	,
	      HANDLE hPrevInst	,
	      int    nCmdShow	)
{
    WNDCLASS	wclass;
    unsigned	clkWidth, clkHeight;
    HWND	hWndClk;
   
    if (!hPrevInst) {
        wclass.lpszClassName	= CLKCLASSNAME;
        wclass.lpfnWndProc	= ClkWndProc;
        wclass.style		= CS_HREDRAW | CS_VREDRAW;
        wclass.cbClsExtra	= 0;
        wclass.cbWndExtra	= 0;
        wclass.hbrBackground	= (HBRUSH)NULL;
        wclass.hInstance	= hInst;
        wclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
        wclass.hIcon		= NULL;
        wclass.lpszMenuName	= NULL;

        if (!RegisterClass(&wclass))
            return FALSE;
    } else
        return FALSE;

    BuildDataBase();

    InitScalingValues();

    clkhInst	 = hInst;
    procCountPt	 = MakeProcInstance((FARPROC)CountPt , hInst);
    procSamplePt = MakeProcInstance((FARPROC)SamplePt, hInst);

    clkWidth	 = desktopWidth  / 2 + GetSystemMetrics(SM_CXFRAME) * 2;
    clkHeight	 = desktopHeight / 2 + GetSystemMetrics(SM_CYCAPTION) +
				      GetSystemMetrics(SM_CYMENU)    +
				      GetSystemMetrics(SM_CYFRAME) * 2;

    hWndClk = CreateWindow(CLKCLASSNAME			,
			   CLKWINDOWNAME		,
			   WS_OVERLAPPEDWINDOW		,
			   10				,
			   10				,
			   clkWidth			,
			   clkHeight			,
			   NULL				,
			   LoadMenu(hInst, CLKMENUNAME)	,
			   hInst			,
			   NULL				);

    if (!InitClockTimer(hWndClk)) {
        DeletePensAndBrushes();
        return FALSE;
    }
    srand(curTime.second*60+curTime.minute);

    InitPensAndBrushes();

    ShowWindow(hWndClk, nCmdShow);
    UpdateWindow(hWndClk);

    return TRUE;
}



int ClockMessageLoop(void)
{
    MSG	msg;
    
    while (GetMessage(&msg, NULL, 0, 0xFFFF)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
    }
    return msg.wParam;
}




int PASCAL WinMain(	HINSTANCE  hInst	,
			HINSTANCE  hPrevInst	, 
                   	LPSTR      lpCmdLine	,
                   	int	   nCmdShow )
{

        if (!InitClock(hInst, hPrevInst, nCmdShow))
               return FALSE;
        return ClockMessageLoop();
}

