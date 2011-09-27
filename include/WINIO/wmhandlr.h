/*
WMHANDLR.H
Event (WM_ message) handlers - interface
Part of the WINIO Library

from "Undocumented Windows" (Addison-Wesley, 1992)
by Andrew Schulman, Dave Maxey and Matt Pietrek.

Copyright (c) Dave Maxey and Andrew Schulman 1991-1992. All rights reserved.

Visit Andrew's web site at www.undoc.com
*/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_EXTRA_WMS		16

typedef long (* WMHANDLER)(HWND, unsigned, WORD, LONG);
typedef WMHANDLER	WMHANDLERTABLE[WM_USER + MAX_EXTRA_WMS];
typedef struct {
	WMHANDLERTABLE	wmhandlertable;
	WORD			wExtraMsg[MAX_EXTRA_WMS];
	} WMHANDLERDATA, FAR * WMTAB;

/* This structure is pointed at by the lParam of the WM_CREATE. The
 * wmTab field is the handler table for the new window, the lpdata
 * is for any other data the app needs to store with the window. The
 * window class must allow for at least 8 window extra bytes.
 */
typedef struct {
	WMTAB wmTab;
	LPVOID lpData;
	} CREATEPARAMS, FAR * LPCREATEPARAMS;
	

/* The generic window procedure - invokes the appropriate
	message handler */
long FAR PASCAL _export wmhandler_wndproc(HWND, WORD, WORD, LONG);

/* wmhandler_get returns current handler for an event for a window */
WMHANDLER wmhandler_get(HWND hWnd, WORD wMessage);

/* wmhandler_set also returns current handler, and then
   makes the supplied handler current for the message type for the
   specified window (the ...tab version operates on a known WMTAB) */
WMHANDLER wmhandler_set(HWND, WORD, WMHANDLER);
WMHANDLER wmhandler_settab(WMTAB, WORD, WMHANDLER);

/* optional function to create an invisible "object window" to
   be used as a recipient for events */
HWND wmhandler_hwnd(char *name);

/* Allocate and initialize a new WM_ handler table */
WMTAB wmhandler_create(void);

/* Free (destroy) a previously created WM_ handler table */
void wmhandler_destroy(HWND);

/* Check for, receive, and dispatch messages */
void wmhandler_yield(void);

/* An application's main event loop */
void wmhandler_mainloop(void);

#ifdef __cplusplus
}
#endif

