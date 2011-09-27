/***********************************************************************
File:   Maze.c
Author: JohnMil
Date:   6-13-92


Abstract:

    This is the main module for MazeLords. It Contains WinMain, as well as the
    WndProcs for the main modules.


Contents:

    InSanctuary() -- determines whether a maze position is in the Sanctuary
    WinMain() -- Main windows entrypoint
    WndProc() -- Main window processing entrypoint
    RegisterClasses() -- Register different window classes
    OneTimeInit() -- Initialization done first time window created
    EachTimeInit() -- Initialization done each new game
    OneTimeShutdown() -- cleanup done last time window destroyed
    EachTimeShutdown() -- cleanup done each time the game is STOPped.

Revision History:

10-30-92 (JohnMil):
    Modified to new personal comment format

************************************************************************/

#define FULL_REDRAW 200
#define IN_MAIN
#include "winmaze.h"
#include "mazproto.h"
#include "net.h"

int ngCmdShow;
HDC hHoldDC;



/*=====================================================================
Function: InSanctuary()

Inputs: Position

Outputs: TRUE if position in Sanctuary, else FALSE

Abstract:
    This function is used to determine if a given position is inside
    the sanctuary. If it is, then combat isn't allowed from/to that
    position, with the exception of shooting drones from outside the
    sanctuary.
======================================================================*/

BOOL InSanctuary(
    PositionType FAR *Pos
    )
{
    BOOL bRet;
    int iLeft,iTop;

    iLeft = 6*X_CELLS_PER_SUBGRID+1;
    iTop = 7*Y_CELLS_PER_SUBGRID+1;


    bRet = ((Pos->ix >= iLeft)&&(Pos->ix <= iLeft+2));
    bRet = bRet&&((Pos->iy >= iTop)&&(Pos->iy <= iTop+2));

    return(bRet);
}



/*=====================================================================
Function: WinMain()

Inputs: Standard winmain inputs

Outputs:success

Abstract:
    This is the WinMain for MazeLords. It is responsible for, among
    other things, registering the windows classes, and allocating some
    of the larger dynamic data structures.
======================================================================*/

int PASCAL WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine,
    int nCmdShow
    )
{
    MSG     msg;
    int     i,j,nRc;
    HGLOBAL hMem;

    //
    // DYNAMIC ALLOCATION OF GLOBAL VARS
    //
    hMem = GlobalAlloc(GHND,(MAX_DRAW_DIST*2+2+1) * sizeof(LPPOINT FAR *));
    pPost = (LPPOINT FAR * FAR *) GlobalLock(hMem);
    for(i=0;i<(MAX_DRAW_DIST*2+2+1);i++) {
        hMem = GlobalAlloc(GHND,(MAX_DRAW_DIST+2+1)*sizeof(LPPOINT));
        pPost[i] = (LPPOINT FAR *) GlobalLock(hMem);
        for(j=0;j<(MAX_DRAW_DIST+2+1);j++) {
            hMem = GlobalAlloc(GHND,2*sizeof(POINT));
            pPost[i][j] = (LPPOINT) GlobalLock(hMem);
            }
        }

    hMem = GlobalAlloc(GHND,NUM_PICS * sizeof(FullPicType FAR *));
    PreStretch = (FullPicType FAR * FAR *) GlobalLock(hMem);
    for(i=0;i<NUM_PICS;i++) {
        hMem = GlobalAlloc(GHND,MAX_DRAW_DIST * sizeof(FullPicType));
        PreStretch[i] = (FullPicType FAR *) GlobalLock(hMem);
        }

    hMem = GlobalAlloc(GHND,X_SIZE * sizeof(BYTE FAR *));
    bMaze = (BYTE FAR * FAR *) GlobalLock(hMem);
    for(i=0;i<X_SIZE;i++) {
        hMem = GlobalAlloc(GHND,Y_SIZE * sizeof(BYTE));
        bMaze[i] = (LPBYTE) GlobalLock(hMem);
        }

    hMem = GlobalAlloc(GHND,X_SUBGRIDS_PER_GRID * sizeof(SubGridType FAR *));
    Grid = (SubGridType FAR * FAR *) GlobalLock(hMem);
    for(i=0;i<X_SUBGRIDS_PER_GRID;i++) {
        hMem = GlobalAlloc(GHND,Y_SUBGRIDS_PER_GRID * sizeof(SubGridType));
        Grid[i] = (SubGridType FAR *) GlobalLock(hMem);
        }

    hMem = GlobalAlloc(GHND,NUM_SUBGRIDS * sizeof(SubGridCellsType));
    SubGrids = (SubGridCellsType FAR *) GlobalLock(hMem);



    lstrcpy(szAppName, "MAZE");
    hInst = hInstance;

    //
    // We need to register the different windows and child
    // windows we'll be using if this is the first instance
    // of MazeLords.
    //
    if(!hPrevInstance) {
      if ((nRc = RegisterClasses()) == -1) {
        LoadString(hInst, IDS_ERR_REGISTER_CLASS, szString, sizeof(szString));
        MessageBox((HWND)NULL, szString, (LPSTR)NULL, MB_ICONEXCLAMATION);
        return nRc;
        }
      }

    //
    // Display the Main window for MazeLords
    //
    hWndMain = CreateWindow(
                  szAppName,
                  "Maze Lords",
                  WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX |
                             WS_THICKFRAME|WS_CLIPCHILDREN|WS_OVERLAPPED,
                  CW_USEDEFAULT, 0,
                  CW_USEDEFAULT, 0,
                  (HWND)NULL,
                  (HMENU)NULL,
                  hInst,
                  NULL);

    OneTimeInit();

    if(hWndMain == (HWND)NULL){
        LoadString(hInst, IDS_ERR_CREATE_WINDOW, szString, sizeof(szString));
        MessageBox((HWND)NULL, szString, "WinMain", MB_ICONEXCLAMATION);
        return IDS_ERR_CREATE_WINDOW;
        }


    ShowWindow(hWndMain, nCmdShow);
    ngCmdShow = nCmdShow;



    while(GetMessage(&msg, (HWND)NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }

    UnRegisterClasses();
    return msg.wParam;
}


/*=====================================================================
Function: Wndproc()

Inputs: Standard windows message processing entrypoint parms

Outputs:returns Success

Abstract:
    This is the window procedure for the main window. It takes care of
    spawning and destroying child windows, interpreting menu-item
    command messages, and most of the none-movement keystrokes.
======================================================================*/

LONG FAR PASCAL WndProc(
    HWND hWnd,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    HMENU           hMenu=0;
    HBITMAP         hBitmap=0;
    HDC             hDC,hBMDC;
    PAINTSTRUCT     ps;
    int             nRc=0;
    INT             i,j,MazexDim,MazeyDim,TopxDim,TopyDim;
    UINT            uCmdId,uCmdCmd;
    HWND            hCmd;


    switch (Message) {


        case WM_TIMER:
            if ((!UserIntCount)&&(!bDemoMode)&&(bDemoEnable)) {
                StartDemo();
                }
            UserIntCount = 0;
            break;


        case WM_INITMENU:
            if (bDemoMode) {
                StopDemo();
                }
            UserIntCount++;
            return DefWindowProc(hWnd, Message, wParam, lParam);

        case WM_COMMAND:
            uCmdId = GET_WM_COMMAND_ID(wParam,lParam);
            uCmdCmd = GET_WM_COMMAND_CMD(wParam,lParam);
            hCmd = GET_WM_COMMAND_HWND(wParam,lParam);
            hMenu = GetMenu(hWnd);

            switch (uCmdId) {

                case IDT_NEWMAIL:
                    ProcessNewMail();
                    break;

                case IDM_F_HIGHSCORES:
                    bDemoEnable = FALSE;
                    MessageBox((HWND)NULL,"High Scores aren't implemented.","High Scores",
                               MB_ICONHAND | MB_APPLMODAL);
                    bDemoEnable = TRUE;
                    break;

                case IDM_F_NEWGAME:

                    bDemoEnable = FALSE;

                    SetCursor(LoadCursor((HINSTANCE)NULL,IDC_WAIT));

                    EachTimeInit();

                    GetClientRect(hWndMain,&rMain);


                    MazexDim = (rMain.right - rMain.left)/2;
                    MazeyDim = (rMain.bottom - rMain.top)*2/3;
                    MazexDim = MazeyDim = (MazexDim < MazeyDim) ? MazexDim : MazeyDim;

                    TopxDim = (rMain.right - rMain.left)/4;
                    TopyDim = (rMain.bottom - rMain.top)/3;
                    TopxDim = TopyDim = (TopxDim < TopyDim) ? TopxDim : TopyDim;
                    rTopView.right = rMain.right - 10;
                    rTopView.bottom = rMain.bottom - 10;
                    rTopView.left = rTopView.right - TopxDim;
                    rTopView.top = rTopView.bottom - TopyDim;

                    rText.left = rMain.left + 10;
                    rText.right = rMain.right - 10 - TopxDim - 10;
                    rText.top = rMain.top + 10 + MazeyDim + 10;
                    rText.bottom = rMain.bottom-10;

                    rScore.left = rMain.left + 10 + MazexDim +10;
                    rScore.right = rMain.right - 10;
                    rScore.top = rMain.top + 10;
                    rScore.bottom = rMain.bottom - 10 - TopyDim - 10;

                    //
                    // create the 3-d view window
                    //
                    rMaze.left = 0;rMaze.right = 0;
                    rMaze.right = MazexDim;
                    rMaze.bottom = MazeyDim;

                    InitMaze();
                    Calc3DMaze();
                    InitNetwork();

                    rMaze.left = rMain.left+10;
                    rMaze.top = rMain.top +10;
                    rMaze.right = rMaze.left + MazexDim;
                    rMaze.bottom = rMaze.top + MazeyDim;

                    hWndMaze = CreateWindow((LPSTR) "MazeDrawWindow",
                                            (LPSTR) "",
                                            WS_CHILD | WS_DLGFRAME,
                                            rMaze.left,rMaze.top,
                                            rMaze.right - rMaze.left,
                                            rMaze.bottom - rMaze.top,
                                            hWndMain,
                                            (HMENU) NULL,
                                            hInst,
                                            NULL);

                    if(hWndMaze == (HWND)NULL) {
                        return IDS_ERR_CREATE_WINDOW;
                        }

                    StretchBitmaps();

                    GetClientRect(hWndMaze,&rMaze);
                    ShowWindow(hWndMaze, ngCmdShow);

                    //
                    // Create the top-view window
                    //
                    hWndTopView = CreateWindow((LPSTR) "TopViewWindow",
                                               NULL,
                                               WS_CHILD | WS_DLGFRAME,
                                               rTopView.left,rTopView.top,
                                               rTopView.right - rTopView.left,
                                               rTopView.bottom - rTopView.top,
                                               hWndMain,
                                               (HMENU)NULL,
                                               hInst,
                                               NULL);

                    if(hWndTopView == (HWND)NULL) {
                        LoadString(hInst, IDS_ERR_CREATE_WINDOW, szString, sizeof(szString));
                        MessageBox((HWND)NULL, szString, "Main", MB_ICONEXCLAMATION);
                        return IDS_ERR_CREATE_WINDOW;
                        }

                    GetClientRect(hWndTopView,&rTopView);
                    ShowWindow(hWndTopView, ngCmdShow);

                    //
                    // Create the Text window
                    //
                    hWndText = CreateWindow((LPSTR) "TextWindow",
                                            NULL,
                                            WS_CHILD | WS_DLGFRAME,
                                            rText.left,rText.top,
                                            rText.right - rText.left,
                                            rText.bottom - rText.top,
                                            hWndMain,
                                            (HMENU)NULL,
                                            hInst,
                                            NULL);

                    if(hWndText == (HWND)NULL) {
                        LoadString(hInst, IDS_ERR_CREATE_WINDOW, szString, sizeof(szString));
                        MessageBox((HWND)NULL, szString, "Main", MB_ICONEXCLAMATION);
                        return IDS_ERR_CREATE_WINDOW;
                        }

                    GetClientRect(hWndText,&rText);
                    ShowWindow(hWndText, ngCmdShow);
                    PrintTextLine("You materialize in the Sanctuary.");
                    PrintTextLine("Nothing can harm you here.");


                    //
                    // Create the Score window
                    //
                    hWndScore = CreateWindow((LPSTR) "ScoreWindow",
                                             NULL,
                                             WS_CHILD | WS_DLGFRAME,
                                             rScore.left,rScore.top,
                                             rScore.right - rScore.left,
                                             rScore.bottom - rScore.top,
                                             hWndMain,
                                             (HMENU) NULL,
                                             hInst,
                                             NULL);

                    if(hWndScore == (HWND)NULL) {
                        LoadString(hInst, IDS_ERR_CREATE_WINDOW, szString, sizeof(szString));
                        MessageBox((HWND)NULL, szString, "Main", MB_ICONEXCLAMATION);
                        return IDS_ERR_CREATE_WINDOW;
                        }

                    GetClientRect(hWndScore,&rScore);
                    ShowWindow(hWndScore, ngCmdShow);

                    //
                    // The game is started. reset the cursor as well.
                    //
                    GameStarted = TRUE;
                    SetCursor(LoadCursor((HINSTANCE)NULL,IDC_ARROW));
                    break;

                case IDM_F_EXIT:
                    SendMessage(hWnd,WM_CLOSE,0,0);
                    break;

                case IDM_O_NET:
                    bNetworked = TRUE;
                    CheckMenuItem(hMenu,IDM_O_NET,MF_CHECKED);
                    CheckMenuItem(hMenu,IDM_O_LOCAL,MF_UNCHECKED);
                    break;

                case IDM_O_LOCAL:
                    bNetworked = FALSE;
                    CheckMenuItem(hMenu,IDM_O_NET,MF_UNCHECKED);
                    CheckMenuItem(hMenu,IDM_O_LOCAL,MF_CHECKED);
                    break;

                case IDM_O_PLAYERSTRETCH:
                    bPlayerPrestretch = !bPlayerPrestretch;
                    CheckMenuItem(hMenu,IDM_O_PLAYERSTRETCH,
                    bPlayerPrestretch?MF_CHECKED:MF_UNCHECKED);
                    if (bPlayerPrestretch) {
                        bDronePrestretch = TRUE;
                        CheckMenuItem(hMenu,IDM_O_DRONESTRETCH,MF_CHECKED);
                        }
                    if (GameStarted) {
                        StretchBitmaps();
                        }
                    break;

                case IDM_O_DRONESTRETCH:
                    bDronePrestretch = !bDronePrestretch;
                    CheckMenuItem(hMenu,IDM_O_DRONESTRETCH,
                              bDronePrestretch?MF_CHECKED:MF_UNCHECKED);
                    if (bPlayerPrestretch) {
                        bDronePrestretch = TRUE;
                        CheckMenuItem(hMenu,IDM_O_DRONESTRETCH,MF_CHECKED);
                        }
                    if (GameStarted) {
                        StretchBitmaps();
                        }
                    break;

                case IDM_O_BITMAP:
                    if (bBitmapDraw) {
                        CheckMenuItem(hMenu,IDM_O_BITMAP,MF_UNCHECKED);
                        bBitmapDraw = FALSE;
                        hDC = hHoldDC;
                        hHoldDC = hMazeDC;
                        hMazeDC = hDC;
                        }
                    else {
                        CheckMenuItem(hMenu,IDM_O_BITMAP,MF_CHECKED);
                        bBitmapDraw = TRUE;
                        if ((((HWND)hMaze3DBM) == (HWND)NULL)&&(GameStarted)) {
                            hHoldDC = hMazeDC;
                            GetClientRect(hWnd,&rMaze);
                            hDC = GetDC(hWnd);
                            if (hMazeDC != NULL) {
                                DeleteDC(hMazeDC);
                            }
                            hMazeDC = CreateCompatibleDC(hDC);
                            if (hMaze3DBM != NULL) {
                                DeleteObject(hMaze3DBM);
                            }
                            hMaze3DBM = CreateCompatibleBitmap(hDC,rMaze.right-rMaze.left,
                                                               rMaze.bottom-rMaze.top);
                            SelectObject(hMazeDC,hMaze3DBM);
                            ReleaseDC(hWnd,hDC);
                            }
                        else {
                            hDC = hHoldDC;
                            hHoldDC = hMazeDC;
                            hMazeDC = hDC;
                            }
                        }
                    break;

                case IDM_O_PLAYERSET:
                    bDemoEnable = FALSE;
                    nRc = DialogBox(hInst,"PLAY_CONF_DLG",hWnd,PlayerDlg);
                    bDemoEnable = TRUE;
                    break;

                case IDM_O_DRONES:
                    bDemoEnable = FALSE;
                    nRc = DialogBox(hInst,"DRONE_DLG", hWnd, DroneDlg);
                    if (GameStarted) {
                        if (uiTimer != (UINT) NULL) {
                            KillTimer((HWND)NULL,uiTimer);
                            }

                        if ((iNumDrones)&&(iDroneSpeed != 0)) {
                            if (! (uiTimer = SetTimer((HWND)NULL,0,ONE_SECOND/iDroneSpeed,MoveDrone))) {
                                MessageBox((HWND)NULL,"Unable to create timer. Killing Game.","FATAL ERROR",
                                           MB_ICONEXCLAMATION|MB_APPLMODAL);
                                SendMessage(hWndMain,WM_CLOSE,0,0);
                                }
                            }

                        InitDrones();
                        }
                    bDemoEnable = TRUE;
                    break;

                case IDM_O_PAUSE:
                    GamePaused = ! GamePaused;
                    break;

                case IDM_F_STOP:
                    if (GameStarted) {
                        nRc = MessageBox((HWND)NULL,"Do you really want to stop the game?",
                                         "Stop Game",MB_YESNO);
                        //
                        // If they really want to stop the game, do it...
                        //
                        if (nRc) {
                            SendMessage(hWndScore,WM_CLOSE,0,0);
                            SendMessage(hWndText,WM_CLOSE,0,0);
                            SendMessage(hWndTopView,WM_CLOSE,0,0);
                            SendMessage(hWndMaze,WM_CLOSE,0,0);
                            EachTimeShutdown();
                            bDemoEnable = TRUE;
                            }
                        }

                    break;

                case IDM_ABOUT:
                    bDemoEnable = FALSE;
                    nRc = DialogBox(hInst,"MDLG_ABOUT", hWnd, AboutDlg);
                    bDemoEnable = TRUE;
                    break;

                default:
                    return DefWindowProc(hWnd, Message, wParam, lParam);
                }
            break;


        case WM_KEYDOWN:

            if (bDemoMode) {
                StopDemo();
                }
            UserIntCount++;

            switch(wParam) {

                //
                // X = Quit the game
                //
                case 'X':
                    if (!SendNetMessage(0,0,NULL,NP_LEAVINGGAME)) {
                        MessageBox((HWND)NULL,"Unable to send Packet","WndProc",
                                     MB_ICONEXCLAMATION|MB_APPLMODAL);
                        }

                    SendMessage(hWndMain,WM_CLOSE,0,0);
                    return(0);
                    break;

                //
                // movement messages go to the MazeWindow
                //
                case VK_SPACE:
                case VK_UP:
                case VK_LEFT:
                case VK_RIGHT:
                case VK_DOWN:
                case 'w':
                case 'W':
                case 's':
                case 'S':
                    SendMessage(hWndMaze,WM_KEYDOWN,wParam,lParam);
                    break;
                }
            break;

        case WM_CREATE:
            //
            // Set timer for 30-second intervals to go into demo-mode.
            //
            SetTimer(hWnd,(UINT) NULL, (UINT) (30*1000),(TIMERPROC) NULL);
            //
            // We decide on the basis of user interaction whether to
            // start demo-mode or not.
            //
            UserIntCount = 0;
            bDemoMode = FALSE;
            CheckMenuItem(hMenu,IDM_O_NET,MF_CHECKED);
            CheckMenuItem(hMenu,IDM_O_LOCAL,MF_UNCHECKED);
            bNetworked = TRUE;
            break;

        case WM_MOVE:
            break;

        case WM_SIZE:
            GetClientRect(hWndMain,&rMain);

            MazexDim = (rMain.right - rMain.left)/2;
            MazeyDim = (rMain.bottom - rMain.top)*2/3;
            MazexDim = MazeyDim = (MazexDim < MazeyDim) ? MazexDim : MazeyDim;

            rMaze.left = rMain.left+10;
            rMaze.top = rMain.top +10;
            rMaze.right = rMaze.left + MazexDim;
            rMaze.bottom = rMaze.top + MazeyDim;
            MoveWindow(hWndMaze,rMaze.left,rMaze.top,
                       rMaze.right-rMaze.left,rMaze.bottom-rMaze.top,TRUE);

            TopxDim = (rMain.right - rMain.left)/4;
            TopyDim = (rMain.bottom - rMain.top)/3;
            TopxDim = TopyDim = (TopxDim < TopyDim) ? TopxDim : TopyDim;
            rTopView.right = rMain.right - 10;
            rTopView.bottom = rMain.bottom - 10;
            rTopView.left = rTopView.right - TopxDim;
            rTopView.top = rTopView.bottom - TopyDim;
            MoveWindow(hWndTopView,rTopView.left,rTopView.top,
                       rTopView.right - rTopView.left,
                       rTopView.bottom - rTopView.top,TRUE);

            rText.left = rMain.left + 10;
            rText.right = rMain.right - 10 - TopxDim - 10;
            rText.top = rMain.top + 10 + MazeyDim + 10;
            rText.bottom = rMain.bottom-10;

            MoveWindow(hWndText,rText.left,rText.top,
                       rText.right - rText.left,
                       rText.bottom - rText.top,TRUE);

            rScore.left = rMain.left + 10 + MazexDim +10;
            rScore.right = rMain.right - 10;
            rScore.top = rMain.top + 10;
            rScore.bottom = rMain.bottom - 10 - TopyDim - 10;

            MoveWindow(hWndScore,rScore.left,rScore.top,
                    rScore.right - rScore.left,
                    rScore.bottom - rScore.top,TRUE);
            break;

        case WM_PAINT:
            memset(&ps, 0x00, sizeof(PAINTSTRUCT));
            hDC = BeginPaint(hWnd, &ps);

            SetBkMode(hDC, TRANSPARENT);
            hBitmap = LoadBitmap(hInst,"MAZE");
            hBMDC = CreateCompatibleDC(hDC);
            SelectObject(hBMDC,hBitmap);
            GetClientRect(hWndMain,&rMain);
            //
            // Fill the destination with the bitmap
            //
            for(i=rMain.left;i<rMain.right;i+=54) {
                for(j=rMain.top;j<rMain.bottom;j+=42) {
                    BitBlt(hDC,i,j,54,42,
                           hBMDC,0,0,
                           SRCCOPY
                          );
                    }
                }
            DeleteDC(hBMDC);
            DeleteObject(hBitmap);

            EndPaint(hWnd, &ps);
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            if (hWnd == hWndMain) {
                PostQuitMessage(0);
                EachTimeShutdown();
                OneTimeShutdown();
                }
            break;

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
        }

    return(0);
}



/*=====================================================================
Function:RegisterClasses()

Inputs: None

Outputs:returns Success/failure

Abstract:
    This is responsible for registering the main window class and the
    child window classes as well.
======================================================================*/

int RegisterClasses(
    void
    )
{
    WNDCLASS   wndclass;


    memset(&wndclass, 0x00, sizeof(WNDCLASS));
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInst;
    wndclass.hIcon = LoadIcon(hInst, "MAZE");
    wndclass.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wndclass.lpszMenuName = szAppName;
    wndclass.lpszClassName = szAppName;

    if(!RegisterClass(&wndclass)) {
        return(-1);
        }

    //
    // Register the 3-d window class
    //
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE | CS_OWNDC | CS_BYTEALIGNWINDOW;
    wndclass.lpfnWndProc = MazeWndProc;
    wndclass.hIcon = (HICON)NULL;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = (LPSTR) "MazeDrawWindow";

    if(!RegisterClass(&wndclass)) {
        return(-1);
        }

    //
    // Register the top view window class
    //
    wndclass.lpfnWndProc = TopViewWndProc;
    wndclass.lpszClassName = (LPSTR) "TopViewWindow";

    if(!RegisterClass(&wndclass)) {
        return(-1);
        }

    //
    // Register the text window class
    //
    wndclass.lpfnWndProc = TextWndProc;
    wndclass.lpszClassName = (LPSTR) "TextWindow";

    if(!RegisterClass(&wndclass)) {
        return(-1);
        }

    //
    // Register the Score window class
    //
    wndclass.lpfnWndProc = ScoreWndProc;
    wndclass.lpszClassName = (LPSTR) "ScoreWindow";

    if(!RegisterClass(&wndclass)) {
        return(-1);
        }

    return(0);
}



/*=====================================================================
Function: UnRegisterClasses

Inputs: none

Outputs:none

Abstract:
    This deletes the windows classes we registered for MazeLords.
======================================================================*/

void UnRegisterClasses(
    void
    )
{
    WNDCLASS   wndclass;
    memset(&wndclass, 0x00, sizeof(WNDCLASS));

    UnregisterClass(szAppName, hInst);
    UnregisterClass((LPSTR) "MazeDrawWindow",hInst);
    UnregisterClass((LPSTR) "TopViewWindow",hInst);
    UnregisterClass((LPSTR) "TextWindow",hInst);
    UnregisterClass((LPSTR) "ScoreWindow",hInst);
}



/*=====================================================================
Function: OneTimeInit()

Inputs: none

Outputs:none

Abstract:
    This initialization routine is called the first time a game is started.
    It reads in the subgrids, initializes the network, makes a mailslot,
    and sets many of the global variables.
======================================================================*/

void OneTimeInit()
{
    DWORD dwScratch,dwSize,dwType;
    HMENU hMenu;
    BOOL bRet;
    int i,j,k;
#ifdef WIN32
    HKEY hKey;
#endif

    if (!ReadSubGrids()) {
        MessageBox((HWND)NULL,"Unable to read SubGrids from text files","Init Error",
                   MB_ICONEXCLAMATION);
        }


    dwScratch = MAX_USERNAME_LENGTH;
    strcpy(ptSelf.cUserName,"<A Player>");
    strcpy(ptSelf.cComputerName,"MachineName");
    dwSize =MAX_COMPUTERNAME_LENGTH;
    ptSelf.ulID = 0;
    ptSelf.iPicNum = PIC_DEFAULT;
    ptSelf.iGridNum = SUBGRID_DEFAULT;

    hMenu = GetMenu(hWndMain);
    EnableMenuItem(hMenu,IDM_F_STOP,MF_GRAYED);
    EnableMenuItem(hMenu,IDM_O_PAUSE,MF_GRAYED);

#ifdef WIN32
    if (!(bRet = GetUserName(ptSelf.cUserName,&dwScratch))) {
        MessageBox((HWND)NULL,"Unable to get UserName","Net Initialization",
                   MB_ICONEXCLAMATION|MB_APPLMODAL);
        }
    dwType = REG_SZ;
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                 "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName",
                 0,
                 KEY_READ,
                 &hKey);
    RegQueryValueEx(hKey,"ComputerName",NULL,&dwType,
                    ptSelf.cComputerName,&dwSize);
    ptSelf.dwPID = GetCurrentProcessId();
#endif
    ptSelf.ulID = MemCRC((LPSTR) &ptSelf.ulID,(int) sizeof(ptSelf));
    bDemoMode = FALSE;
    bDemoEnable = TRUE;
    iNumDrones = 10;
    iDroneSpeed = 2;
    iDroneMoveAlg = 1;
    bBitmapDraw = FALSE;
    bPlayerPrestretch = FALSE;
    bDronePrestretch = FALSE;
    for(i=0;i<NUM_PICS;i++) {
        for (j=0;j<MAX_DRAW_DIST;j++) {
            for(k=0;k<4;k++) {
                PreStretch[i][j].P[k].hBitmap = (HBITMAP)NULL;
                PreStretch[i][j].M[k].hBitmap = (HBITMAP)NULL;
                }
            }
        }

    return;
}



/*=====================================================================
Function: EachTimeInit()

Inputs: none

Outputs:none

Abstract:
    This function is called each time a new game is started (initially,
    and any time after a STOP selection from the menu) This loads some
    bitmaps, changes the state on menu-items whose validity is changed by
    a game in progress, and resets some global variables.
======================================================================*/

void EachTimeInit(
    void
    )
{
    HMENU hMenu;

    hMenu = GetMenu(hWndMain);

    EnableMenuItem(hMenu,IDM_O_NET,MF_GRAYED);
    EnableMenuItem(hMenu,IDM_O_LOCAL,MF_GRAYED);
    EnableMenuItem(hMenu,IDM_O_PLAYERSET,MF_GRAYED);
    EnableMenuItem(hMenu,IDM_F_NEWGAME,MF_GRAYED);
    EnableMenuItem(hMenu,IDM_F_STOP,MF_ENABLED);
    EnableMenuItem(hMenu,IDM_O_PAUSE,MF_ENABLED);
    uiTimer = (UINT) NULL;
    ptDrones.next = NULL;
    fptPic.next = NULL;
    bSelfInSanct = TRUE;

    hShotBM[0]=LoadBitmap(hInst,"SHOT1");
    hShotBM[1]=LoadBitmap(hInst,"SHOT2");
    hFadeBM[0]=LoadBitmap(hInst,"FADE1");
    hFadeBM[1]=LoadBitmap(hInst,"FADE2");

    hMaze3DBM = (HBITMAP)NULL;
    hMazeDC = (HDC)NULL;
    iWhisperDist = 2;
    iShoutDist = 10;
    iPlayersKilled = iTimesKilled = iDronesKilled = 0;
    ptSelf.iScore = iKilledByDrones = 0;
    GamePaused = GameStarted = FALSE;
    Scores.next = NULL;

    return;
}



/*=====================================================================
Function: OneTimeShutdown()

Inputs: none

Outputs:none

Abstract:
    This function does NOTHING right now. It'll be called at the very
    end of everything.
======================================================================*/

void OneTimeShutdown(
    void
    )
{
    return;
}



/*=====================================================================
Function: EachTimeShutdown()

Inputs: none

Outputs:none

Abstract:
    This function is called each time a STOP command is selected and
    approved. It changes menu items to the correct state, changes some
    global state variables, and notifies the network that we're quitting
    the game.
======================================================================*/

void EachTimeShutdown(
    void
    )
{
    HMENU hMenu;

    hMenu = GetMenu(hWndMain);

    DeleteObject(hShotBM[0]);
    DeleteObject(hShotBM[1]);
    DeleteObject(hFadeBM[0]);
    DeleteObject(hFadeBM[1]);
    DeleteObject(hMaze3DBM);
    DeleteDC(hMazeDC);


    EnableMenuItem(hMenu,IDM_O_NET,MF_ENABLED);
    EnableMenuItem(hMenu,IDM_O_LOCAL,MF_ENABLED);
    EnableMenuItem(hMenu,IDM_O_PLAYERSET,MF_ENABLED);
    EnableMenuItem(hMenu,IDM_F_STOP,MF_GRAYED);
    EnableMenuItem(hMenu,IDM_O_PAUSE,MF_GRAYED);
    EnableMenuItem(hMenu,IDM_F_NEWGAME,MF_ENABLED);


    if (!SendNetMessage(0,0,NULL,NP_LEAVINGGAME)) {
        MessageBox((HWND)NULL,"Unable to send 'leaving game' Packet","EachTimeShutDown",
                   MB_ICONEXCLAMATION|MB_APPLMODAL);
        }

    bNetworked = TRUE;
    GameStarted = FALSE;

    iNumDrones = 0;
    iDroneSpeed = 0;
    iDroneMoveAlg = 1;
    bBitmapDraw = FALSE;

    return;
}




/*=====================================================================
Function: StartDemo()

Inputs: none

Outputs:none

Abstract:
    This function is responsible for causing MazeLords to enter Demo
    mode.
======================================================================*/

void StartDemo(
    void
    )
{
    bDemoMode = TRUE;
    SetWindowText(hWndMain,"Maze Lords - Demo Mode");
    SendMessage(hWndMain,WM_COMMAND,MAKELONG(IDM_F_NEWGAME,0),(LPARAM)NULL);
    return;
}



/*=====================================================================
Function: StopDemo()

Inputs: none

Outputs:none

Abstract:
    This function is responsible for ending the mazelords demo and
    putting it into a state that the user can interact with.
======================================================================*/


void StopDemo(
    void
    )
{
    bDemoMode = FALSE;
    SendMessage(hWndScore,WM_CLOSE,0,0);
    SendMessage(hWndText,WM_CLOSE,0,0);
    SendMessage(hWndTopView,WM_CLOSE,0,0);
    SendMessage(hWndMaze,WM_CLOSE,0,0);
    EachTimeShutdown();
    iNumDrones  = 10;
    iDroneSpeed = 2;
    bDemoEnable = TRUE;
    UserIntCount++;
    SetWindowText(hWndMain,"Maze Lords");
    return;
}
