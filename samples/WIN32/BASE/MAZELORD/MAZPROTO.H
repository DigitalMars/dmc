/*****************************************************************
MODULE: MazProto.h
AUTHOR: JohnMil
DATE:   7/21/92

This module contains function prototypes for maze lords. It should
ONLY be included after winmaze.h.
*****************************************************************/


#ifndef _MAZ_PROTO_H_
#define _MAZ_PROTO_H_

#include "winmaze.h"

#ifdef WIN16
                        /* Maze16.c */
BOOL MaskBlt(HDC hDCDest,int X, int Y, int nWidth, int nHeight,
             HDC hDCSrc, int XSrc, int YSrc, HBITMAP hbmMask, int xMask, int yMask, DWORD Rop3);

BOOL PolyDraw(HDC hDC, LPPOINT lpPoints, LPBYTE lpTypes, int nCount);

#endif
                        /* Bitmap.c */
void FadePic(int iPicNum,BYTE bFacing,HDC hDC,LPRECT lprFrom, LPRECT lprDRect);
FullPicType FAR *NewFullPic(int iPicNum,FullPicType FAR *next);
FullPicType FAR *DeleteFullPic(FullPicType FAR *fptTrav);
void DrawClippedPic(int iPicNum,BYTE bFacingIndex,HDC hDC,LPRECT lprDRect,
                    LPRECT lprClip,LPRECT lprFrom, int iRelx, int iRely);
BOOL AddPic(int iPicNum);
BOOL DelPic(int iPicNum);





                        /* Draw.c */

HoleType FAR *DelHole(HoleType FAR *h);
HoleType FAR *NewHole(int x1,int x2,HoleType FAR *next);
HoleType FAR *DrawRect(HoleType FAR *htTrav,int iRelX,int iRelY, BYTE bRelDir);
void DrawMaze(HDC hDC,RECT FAR *rDraw);
void DrawTopView(HDC hDC,BOOL bRedraw);
void DrawPlayers(HDC hDC,PlayerType FAR *ptTrav2,RECT FAR *rUpd);
PlayerDrawType FAR *NewPlayerDraw(PlayerType FAR *p,LPRECT rDraw, LPRECT rClip,
                              int iRelx,int iRely,PlayerDrawType FAR *next);


                        /* Drones.c */

PlayerType FAR *DeletePlayer(PlayerType FAR *ptTrav);
PlayerType FAR *NewPlayer(unsigned long ulID,DWORD dwPID,LPSTR cUserName,
                      LPSTR cComputerName,int ix,int iy, BYTE Facing,
                      int iScore,int iPicNum,int iGridNum, int iGridxPos,
                      int iGridyPos,
                      PlayerType FAR *next);
int RandRange(int iLow, int iHigh);
void InitDrones(void);
LRESULT CALLBACK  MoveDrone(HWND hWnd, UINT wMsg, UINT idTimer, DWORD dwTime);



                        /* InitMaze.c */

void InitMaze(void);
void Calc3DMaze(void);
void InsertSubGrid(int SubGridNo,int iXPos, int iYPos);



                        /* Maze.c */

BOOL InSanctuary(PositionType FAR *Pos);
LONG FAR PASCAL WndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam);
int RegisterClasses(void);
void UnRegisterClasses(void);
void OneTimeInit(void);
void EachTimeInit(void);
void OneTimeShutdown(void);
void EachTimeShutdown(void);
void StartDemo(void);
void StopDemo(void);


                        /* MazeDlg.c */

BOOL FAR PASCAL DroneDlg(HWND hWndDlg, UINT Message, UINT wParam, LONG lParam);
BOOL FAR PASCAL AboutDlg(HWND hWndDlg, UINT Message, UINT wParam, LONG lParam);
BOOL FAR PASCAL IntoneDlg(HWND hWndDlg, UINT Message, UINT wParam, LONG lParam);
BOOL FAR PASCAL PlayerDlg(HWND hWndDlg,UINT Message,WPARAM wParam,LPARAM lParam);
void cwCenter(HWND hWnd,int top);


                        /* MazeWnd.c */

void KillSelf(void);
LONG FAR PASCAL MazeWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam);
void StretchBitmaps(void);

                        /* Network.c */

unsigned long MemCRC(LPSTR buffer,int bytes);
BOOL SendNetMessage(unsigned long ulSender, unsigned long ulDest, LPVOID lpStruct, DWORD dwPacketType);
void MailWatchThread(HANDLE hMailSlot);
BOOL InitNetwork(void);
void ProcessNewMail(void);


                        /* ReadSGrd.c */
int ReadSubGrids(VOID);


                        /* ScoreWnd.c */

LONG FAR PASCAL ScoreWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam);


                        /* TextWnd.c */

LONG FAR PASCAL TextWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam);
void PrintTextLine(LPSTR cLine);
LPSTR lstrdup(LPSTR s);
                        /* TopWnd.c */

LONG FAR PASCAL TopViewWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam);


#endif
