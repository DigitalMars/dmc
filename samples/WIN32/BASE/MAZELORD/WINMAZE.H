/*************************************************
MODULE: WinMaze.h
AUTHOR: JohnMil
DATE:   7/21/92

This module contains typedefs, etc for Maze Lords.
Prototypes are contained in MazProto.h.
*************************************************/

#ifndef _WINMAZE_H_INC_
#define _WINMAZE_H_INC_

#ifndef _IN_RC_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <windows.h>
#include <mmsystem.h>
#include "net.h"
#include "MAZEDLG.h"

#ifdef WIN16

#define Sleep(x)                // Sleep doesn't exist.
#define PT_MOVETO 1
#define PT_LINETO 2
#define GET_WM_COMMAND_ID(wp, lp)                   (wp)
#define GET_WM_COMMAND_HWND(wp, lp)                 (HWND)LOWORD(lp)
#define GET_WM_COMMAND_CMD(wp, lp)                  HIWORD(lp)
typedef long INT;
typedef LPCSTR LPCTSTR;

#else  // Else if it's win32

#define SELECTOROF(a) a
#define GET_WM_COMMAND_ID(wp, lp)               LOWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp)             (HWND)(lp)
#define GET_WM_COMMAND_CMD(wp, lp)              HIWORD(wp)
#undef INT
#define INT int


#endif


#define PIC_X           100     // Bitmaps are 100 pixels wide.
#define PIC_Y           150     // Bitmaps are 150 pixels tall.

#define PIC_FISH        0
#define PIC_ROBOT       1
#define PIC_SMILEY      2
#define PIC_REAPER      3
#define NUM_PICS        4

#define PIC_DRONE       PIC_ROBOT
#define PIC_DEFAULT     PIC_FISH


#define IDM_FILE                  1000
#define IDM_F_HIGHSCORES               1050
#define IDM_F_NEWGAME                  1100
#define IDM_F_EXIT                     1200
#define IDM_OPTIONS               2000
#define IDM_O_PLAYERSTRETCH            2040
#define IDM_O_DRONESTRETCH             2041
#define IDM_O_DRONES                   2050
#define IDM_O_NET                      2055
#define IDM_O_LOCAL                    2085
#define IDM_O_PAUSE                    2100
#define IDM_O_PLAYERSET                2105
#define IDM_F_STOP                     2110
#define IDM_O_BITMAP                   2115
#define IDM_ABOUT                 3000
#define IDM_DRAWPLAYERS           1001
#define IDM_DRAWDRONES            1002
#define IDT_NEWMAIL               4001
#define IDM_REDRAW                5001


#define IDS_ERR_REGISTER_CLASS   1
#define IDS_ERR_CREATE_WINDOW    2

#define WC_UPDATESCORE           1000
#define WC_UPDATEDIRECTION       1010
#define WC_DELETESCORE           1020
#define WC_ADDSCORE              1030


                    // we can see up to 10 squares ahead or to either side
                    // (not including our current square)
#define MAX_DRAW_DIST 10

#define MAX_DRONES 100
#define MAX_DRONE_SPEED 5

#define ONE_SECOND 1000

#define PANEL_WIDTH 100
#define PANEL_HEIGHT 150

        // Flags for walls in a Cell
#define NORTH ((BYTE)0x01)
#define WEST  ((BYTE)0x02)
#define SOUTH ((BYTE)0x04)
#define EAST  ((BYTE)0x08)

#define AWAY        0
#define TOWARD      2

#define FORWARD     0
#define LEFT        1
#define BACK        2
#define RIGHT       3

        // # of Cells in a subgrid
#define X_CELLS_PER_SUBGRID 5
#define Y_CELLS_PER_SUBGRID 5
        // # of Subgrids in the Grid
#define X_SUBGRIDS_PER_GRID 10
#define Y_SUBGRIDS_PER_GRID 10

#define X_SIZE (X_CELLS_PER_SUBGRID * (X_SUBGRIDS_PER_GRID+2))
#define Y_SIZE (Y_CELLS_PER_SUBGRID * (Y_SUBGRIDS_PER_GRID+2))
        // We have SUBGRI00.TXT through SUBGRI06.TXT for players,
#define NUM_PLAYER_SUBGRIDS 7
        // And SUBGRI07.TXT through SUBGRI13.TXT for the palace
#define NUM_PALACE_SUBGRIDS 7
        // SUBGRI14.TXT is completely closed, for unused grids

#define NUM_SUBGRIDS (NUM_PLAYER_SUBGRIDS + NUM_PALACE_SUBGRIDS + 1)
#define SUBGRID_DEFAULT 1


// A maze is an array of characters, with each character being a combination
// of the NORTH, SOUTH, EAST, and WEST bit flags.  If the flag is set, there
// is a wall there. If not, there isn't. Note that both the flag of the
// current cell and of the adjacent cell in that direction must be checked.


        // Adjacent returns the coordinates of the cell in that direction
        // This applies to both Grid and Maze coordinates.
#define ADJ_X(x,dir) ((((BYTE)(dir)) & EAST) ? ((x)+1) : \
              ((((BYTE)(dir)) & WEST) ? ((x)-1) : (x)))
#define ADJ_Y(y,dir) ((((BYTE)(dir)) & SOUTH) ? ((y)+1) : \
              ((((BYTE)(dir)) & NORTH) ? ((y)-1) : (y)))
        // To calculate SubGrid # from a coordinate
#define X_TO_SUBGRID(x) ((int) (x) / X_CELLS_PER_SUBGRID)
#define Y_TO_SUBGRID(y) ((int) (y) / Y_CELLS_PER_SUBGRID)
        // To get origin of a subgrid in coordinates from subgrid #'s
#define SUBGRID_TO_X(x) ((int) (x) * X_CELLS_PER_SUBGRID)
#define SUBGRID_TO_Y(y) ((int) (y) * Y_CELLS_PER_SUBGRID)

        // Convert relative direction to absolute
#define LEFT_TO_ABS(dir) (((((BYTE)dir)) & EAST) ? NORTH : ((BYTE)(((BYTE) (dir))<<1)))
#define RIGHT_TO_ABS(dir) ((((BYTE)(dir)) & NORTH) ? EAST : ((BYTE)(((BYTE) (dir))>>1)))
#define BACK_TO_ABS(dir)  (LEFT_TO_ABS(LEFT_TO_ABS(dir)))


#define RECTWIDTH(r) ((r).right - (r).left)
#define RECTDEPTH(r) ((r).bottom - (r).top)
#define GREATEROF(a,b) (((a)>(b))?(a):(b))
#define LESSEROF(a,b) (((a)<(b))?(a):(b))

struct S_BitMapPicType {
    HBITMAP hBitmap;
    int xSize,ySize;
    int xOrg,yOrg;
    };
typedef struct S_BitMapPicType BitMapPicType;

struct S_PicType {
    BitMapPicType P[4];
    };
typedef struct S_PicType PicType;

struct S_FullPicType {
    BitMapPicType P[4];     // BitMap
    BitMapPicType M[4];     // Black and white Mask
    int     iPicNum;
    struct S_FullPicType FAR *next;
    };
typedef struct S_FullPicType FullPicType;


struct _SLineType {
   int x[2],y[2];
   };

typedef struct _SLineType LineType;



        // A cell is a single square in a sub-grid
typedef BYTE CellType;

        // SubGridCellsType is the array of characters that make up a sub-grid
struct S_SubGridCellsType {
    CellType Cell[X_CELLS_PER_SUBGRID][Y_CELLS_PER_SUBGRID];
    };
typedef struct S_SubGridCellsType SubGridCellsType;

typedef struct {
  int ix, iy;
  BYTE Facing;
  } PositionType;


struct _s_PlayerType {
  unsigned long ulID;
  DWORD dwPID;
  char cUserName[MAX_USERNAME_LENGTH];
  char cComputerName[MAX_COMPUTERNAME_LENGTH];
  PositionType Pos;
  BOOL Drawn;
  RECT rDrawn;
  RECT rFrom;
  int iScore;
  int iPicNum;
  int iGridNum;
  POINT pGridLoc;
  BYTE dwDirectShown;
  struct _s_PlayerType FAR *next;
  };
typedef struct _s_PlayerType PlayerType;


struct _s_PlayerDrawType {
  PlayerType FAR *p;            // Pointer to the player to draw
  RECT rDraw,                   // Rectangle the FULL bitmap would be drawn in
       rClip;                   // Rectangle available for drawing in.
  int iRelx,iRely;
  struct _s_PlayerDrawType FAR *next;
  };

typedef struct _s_PlayerDrawType PlayerDrawType;



        // SubGridType is a full description of a SubGrid.
struct S_SubGridType {
    unsigned long ulOwnerID;
    POINT Pos;
        // Since there are a limited # of subgrids, we might share them.
    SubGridCellsType *SubGrid;
    };

typedef struct S_SubGridType SubGridType;

struct _SObjectType {
    DWORD Class;
    DWORD ObjectID;
    POINT MazePos;
    BYTE Facing;
    struct _SObjectType * next;
};

typedef struct _SObjectType ObjectType;

struct _SHoleType {
    int x[2];
    struct _SHoleType FAR *next;
    };

typedef struct _SHoleType HoleType;

struct _S_ScoreType {
    unsigned long ulID;
    int ix,iy;          // Upper left corner of score entry
    int iScore;
    BYTE bDir;
    struct _S_ScoreType FAR *next;
    };

typedef struct _S_ScoreType ScoreType;

typedef struct {
    int iLoudness;
    PositionType Pos;
    DWORD dwLen;
    char cBuff[132];
    } IntoneType;


typedef struct {
    int xSize,ySize;
    int xOrg,yOrg;
    } PicPosType;

typedef struct {
   char cBase[5];
   int iPicNum;
   PicPosType PicPos[4];
   } PicSpecType;







////////////////////////////////////////////////////////////////////////////

/**********************Function definitions***************************/

int ReadSubGrids(VOID);
int InitGrid(VOID);


/**********************GLOBAL VARIABLES ****************************/
                // If we're in the main function, the globals
                // will be declared. Otherwise they'll just
                // be given as externs.
#ifndef IN_MAIN
#define EXT_DECL extern
#else
#define EXT_DECL
#endif

EXT_DECL SubGridType FAR  * FAR *Grid;
EXT_DECL SubGridCellsType FAR *SubGrids;
EXT_DECL FILE  *hFile;
EXT_DECL int xc,yc,xscale,yscale;
EXT_DECL RECT CurView;
EXT_DECL char szString[128];   /* variable to load resource strings         */

EXT_DECL char szAppName[20];   /* class name for the window               */
EXT_DECL HINSTANCE hInst;
EXT_DECL HWND hWndMain,hWndMaze,hWndTopView,hWndText,hWndScore;
EXT_DECL RECT rMain,rMaze,rTopView,rText,rScore;
EXT_DECL BYTE FAR * FAR *bMaze;
EXT_DECL LPPOINT FAR * FAR *pPost;
EXT_DECL int iNumDrones,iDroneSpeed,iDroneMoveAlg;
EXT_DECL UINT uiTimer;
EXT_DECL HBITMAP hFishBitmap[4],hFishMask[4];
EXT_DECL HBITMAP hRoboBitmap[4],hRoboMask[4];
EXT_DECL int ViewEnd;           // This is how many squares ahead we can see
EXT_DECL BOOL GamePaused,GameStarted,bNetworked,bSelfInSanct;

EXT_DECL FullPicType fptPic;
EXT_DECL HBITMAP hbScratch;
EXT_DECL float scale;

EXT_DECL PlayerType ptSelf;
EXT_DECL PlayerType ptPlayers;
EXT_DECL PlayerType ptDrones;
EXT_DECL HANDLE hMailSlot;

EXT_DECL PositionType ptLastPos;

EXT_DECL int iPlayersKilled,iDronesKilled;
EXT_DECL int iTimesKilled,iKilledByDrones;
EXT_DECL ScoreType Scores;
EXT_DECL int iWhisperDist, iShoutDist,iLoudness;

EXT_DECL HBITMAP hShotBM[2],hFadeBM[2];

EXT_DECL HBITMAP hMaze3DBM;
EXT_DECL HDC     hMazeDC;
EXT_DECL BOOL    bBitmapDraw;
EXT_DECL BOOL    bPlayerPrestretch,bDronePrestretch;
EXT_DECL FullPicType FAR * FAR *PreStretch;

EXT_DECL BOOL    bDemoMode;
EXT_DECL BOOL    bDemoEnable;
EXT_DECL INT     UserIntCount;


extern PicSpecType PicSpec[];
extern int iNumPicSpecs;

/*******************END GLOBALS *************************************/

#define VIEWER_DIST              (PANEL_WIDTH)
#define MC_TO_SC(x,z) (scale*(VIEWER_DIST * (x)/((z)+(VIEWER_DIST))))



#endif
