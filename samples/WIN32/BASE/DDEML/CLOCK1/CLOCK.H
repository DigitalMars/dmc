
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


/*--------------------------------------------------------------------------*/
/*  Typedefs and Structures                                                 */
/*--------------------------------------------------------------------------*/

/* Don't change without updating GETTIME.ASM. */
typedef struct tagTIME
  {
    INT     hour;   /* 0 - 11 hours for analog clock */
    INT     hour12; /* 12 hour format */
    INT     hour24; /* 24 hour format */
    INT     minute;
    INT     second;
    INT     ampm;   /* 0 - AM , 1 - PM */
  } TIME;

typedef struct
{
    SHORT sin;
    SHORT cos;
} TRIG;

TRIG CirTab[] = {     // circle sin, cos, table
    { 0,     -7999  },
    { 836,   -7956  },
    { 1663,  -7825  },
    { 2472,  -7608  },
    { 3253,  -7308  },
    { 3999,  -6928  },
    { 4702,  -6472  },
    { 5353,  -5945  },
    { 5945,  -5353  },
    { 6472,  -4702  },
    { 6928,  -4000  },
    { 7308,  -3253  },
    { 7608,  -2472  },
    { 7825,  -1663  },
    { 7956,  -836   },

    { 8000,  0      },
    { 7956,  836    },
    { 7825,  1663   },
    { 7608,  2472   },
    { 7308,  3253   },
    { 6928,  4000   },
    { 6472,  4702   },
    { 5945,  5353   },
    { 5353,  5945   },
    { 4702,  6472   },
    { 3999,  6928   },
    { 3253,  7308   },
    { 2472,  7608   },
    { 1663,  7825   },
    { 836,   7956   },

    { 0,     7999   },
    { -836,  7956   },
    { -1663, 7825   },
    { -2472, 7608   },
    { -3253, 7308   },
    { -4000, 6928   },
    { -4702, 6472   },
    { -5353, 5945   },
    { -5945, 5353   },
    { -6472, 4702   },
    { -6928, 3999   },
    { -7308, 3253   },
    { -7608, 2472   },
    { -7825, 1663   },
    { -7956, 836    },

    { -7999, -0     },
    { -7956, -836   },
    { -7825, -1663  },
    { -7608, -2472  },
    { -7308, -3253  },
    { -6928, -4000  },
    { -6472, -4702  },
    { -5945, -5353  },
    { -5353, -5945  },
    { -4702, -6472  },
    { -3999, -6928  },
    { -3253, -7308  },
    { -2472, -7608  },
    { -1663, -7825  },
    { -836 , -7956  }
};

typedef struct tagCLOCKDISPSTRUCT
  {
    /* Clock display format for main window/icon outut */
    /* either  IDM_ANALOG, or IDM_DIGITAL */
    WORD    wFormat;

    /* Various boolean settings for the clock style
     * and current settings */
    BOOL bMinimized;
    BOOL bMaximized;
    BOOL bTopMost;
    BOOL bNoTitle;
    BOOL bTmpHide;
    BOOL bIconic;
    BOOL bColor;
    BOOL bNewFont;

    /* X and Y offset within client area of window
     * or icon where digital clock will be displayed */
    POINT   line1;

    /* Y offset within icon area to second line where
     * minute will be displayed */
    INT     yline2;

    /* length of the digital clock readout string,
     * required by the text out function.
     * if the time format is 24 hour, this value is
     * set so that the AM/PM indicator is not output */
    WORD    wDigTimeLen;

    /* buffer to store digital time in.  the hour/minute/ampm
     * sections of this buffer are only modified if they
     * change from the previous call. */
    CHAR    szDigTime[20];

    /* buffer to hold the win.ini international indicators
     * for 1159, and 2359 AM/PM 12 hour time format.
     * szAMPM[0] holds AM, szAMPM[1] holds PM indicator */
    CHAR    szAMPM[2][7];

    /* intl time format (like DOS) 0 - 12 hour, 1 - 24 hour */
    WORD    wTimeFormat;

    /* intl time seperator character */
    CHAR    cTimeSep;
  } CLOCKDISPSTRUCT;


/*--------------------------------------------------------------------------*/
/*  Function Templates                                                      */
/*--------------------------------------------------------------------------*/

void NEAR GetTime(TIME *);
void NEAR ConvTime(TIME *);

LONG APIENTRY ClockWndProc(HWND, UINT, WPARAM, LONG);


/*--------------------------------------------------------------------------*/
/*  Constants                                                               */
/*--------------------------------------------------------------------------*/

#define IDD_ABOUT 400

    /* Main Menu ID defines */

#define IDM_ANALOG       1
#define IDM_DIGITAL      2
#define IDM_ABOUT        4
//--------------
#define IDM_TOPMOST      5
#define IDM_NOTITLE      6
//--------------


/* Temp ID for dialogs. */
#define ID_JUNK     0xCACC

    /* String Resource definitions */

#define IDS_APPNAME      1
#define IDS_DATA         2
#define IDS_TOOMANY      3
#define IDS_FONTFILE     4

#define IDS_INTL        16
#define IDS_ITIME       18
#define IDS_S1159       19
#define IDS_S2359       20
#define IDS_STIME       22
#define IDS_CLKFORMAT   23

#define IDS_1159        24
#define IDS_2359        25
#define IDS_TIMESEP             27
#define IDS_USNAME      29
#define IDS_INIFILE     30
//--------------
#define IDS_TOPMOST     31
//--------------

#define HOURSCALE       65
#define MINUTESCALE     80
#define HHAND           TRUE
#define MHAND           FALSE
#define SECONDSCALE     80
#define MAXBLOBWIDTH    25
#define BUFLEN          30
#define REPAINT         0
#define HANDPAINT       1

#define UPDATE                  0
#define REDRAW                  1

#define OPEN_TLEN               450
#define ICON_TLEN               45000

#define CLKSCALE                ((INT)8000)
