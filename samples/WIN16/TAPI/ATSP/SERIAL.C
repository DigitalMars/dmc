//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// PURPOSE.
//
// Copyright 1993-1995 Microsoft Corporation, all rights reserved.
#define USECOMM                 /* for Windows 95 windows.h */
#include <windows.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include "atsp.h"


int  SendModemCommand (int, const char *);
int  GetModemReply (int, char *, int);
int  SerialOpenComms (LPSTR, LPSTR);
void SerialCloseComms (int);
int  CheckSerialError (int);
void serialState (int, int, ATSPLineData *);
BOOL CALLBACK __export TalkDropProc (HWND, UINT, WPARAM, LPARAM);


extern HINSTANCE        hinst;
extern HWND                     myhwnd;

int                                     comevState       = 0;
ATSPLineData      *comevArg              = 0;

HWND                                    hwndTalkDrop = 0;
FARPROC                         procTalkDrop = 0;


#define state_ignore                            -1
#define state_makecall                  100
#define state_linedial                  101
#define state_sendinitstr               102
#define state_senddialstr               103
#define state_talkdropstart     104
#define state_talkdropend               105

#define RESPONSE_BUSY                   -5
#define RESPONSE_ERROR                  -8
#define RESPONSE_OK                             -11

// magic modem command strings

char s_dialFormat[]     = "ATS11=%luS8=%luS7=%lu%s%s\r";
char s_dialInit[]               = "X4E1S6=2";
char s_null[]                   = "";
char s_hangup[]         = "ATH\r";
char s_atd[]                    = "ATD\r";
char s_atd_s[]                  = "ATD;\r";
char s_r[]                              = "\r";

// The serial transmit and receive queue sizes.

#define TXQUEUE 512
#define RXQUEUE 512


long sendCompletion (ATSPLineData *pline, long res)
{
        long dwid = pline->dwRequestID;

        if (dwid == 0)
                return 0;
        else
        {
                pline->dwRequestID = 0;
                pline->lpfnCompletion (dwid, res);

                return dwid;
        }
}


long doAsyncCommand (int msg, ATSPLineData *pline)
{
        switch (msg)
        {
                case msgMakeCall:
                {
                        DebugMsg (("makecall"));
                        pline->hcd = SerialOpenComms (pline->port, pline->speed);

                        if (pline->hcd < 0)
                                return sendCompletion (pline, LINEERR_RESOURCEUNAVAIL);

                        serialState (state_makecall, 0, pline);
                        return sendCompletion (pline, 0);
                }
                break;

                case msgDial:
                {
                        DebugMsg (("dial"));

                        serialState (state_linedial, 0, pline);
                        return sendCompletion (pline, 0);
                }
                break;

                case msgDrop:
                {
                        if (pline->callState != LINECALLSTATE_IDLE &&
                                 pline->callState != 0)
                        {
                                DebugMsg (("drop"));
                                serialState (state_talkdropend, msgDrop, pline);
                        }

                        return sendCompletion (pline, 0);
                }
                break;
        }
}


int commEvent (int hcd, int flag)
{
        int  cbread;
        char szread[RXQUEUE];

        if (flag != CN_RECEIVE)
                return 0;

        GetCommEventMask (hcd, EV_RXFLAG);      // clear the flag

        // we got a string from the modem, go get it !

        cbread = ReadComm (hcd, szread, RXQUEUE);

        if (cbread < 0)
        {
                DebugMsg (("failed to read string"));
                CheckSerialError (hcd); // clear the error
        }
        else
        {
                char *str = szread;

                szread[cbread] = 0;
                AnsiUpper (szread);

                while (*str)
                {
                        int msg = 0;

                        DebugMsg (("Command: %s", str));

                        switch ((str[0] << 8) + str[1])
                        {
                                case 'ER':      // error
                                case 'NO':      // NO DIALTONE -> modem is not connected to anything
                                        msg = RESPONSE_ERROR;
                                        break;

                                case 'OK':
                                        msg = RESPONSE_OK;
                                        break;

                                case 'BU':
                                        msg = RESPONSE_BUSY;
                                        break;

                                case 'AT':      // do nothing with these strings,
                                default:                // they represent command echo or random junk
                                        break;
                        }

                        if (msg)
                                serialState (comevState, msg, comevArg);

                        // see if there is another string in the command buffer
                        while (*str && *str != '\r' && *str != '\n')    ++str;
                        while (*str && (*str == '\r' || *str == '\n')) ++str;
                }
        }

        return 0;
}


void serialState (int state, int msg, ATSPLineData *pline)
{
  char dial[255];

  DebugMsg (("serialState: %d, %d, %lx", state, msg, (long) pline));


  // each state which expects a response from the modem sets
  // comevState before returning. Each state only expects one reply
  // from the modem, so subsequent responses are ignored;

  comevState = state_ignore;


  switch (state)
  {
        case state_makecall:
        {
                comevArg                         = pline;                       // remember for future calls
                pline->callState = LINECALLSTATE_UNKNOWN;

                // detect dialtone iff required
                // bug, needs to check dialparams for dialtone check required flag

                if (SendModemCommand (pline->hcd, s_atd_s) != 0)
                        goto cleanup;

                comevState = state_sendinitstr;
        }
        break;


        case state_sendinitstr:
        {
                char *userinit = pline->initstr;

                // report result of dialtone check

                if (msg != RESPONSE_OK)
                {
                        // BUG return async fail of LINEERR_CALLUNAVAIL to lineMakeCall
                        goto cleanup;
                }


                // BUG return success to lineMakeCall

                pline->callState = LINECALLSTATE_DIALTONE;
                pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLSTATE,
                                                                        LINECALLSTATE_DIALTONE, 0, 0);
                if (pline->dwAddressStates & (LINEADDRESSSTATE_NUMCALLS | LINEADDRESSSTATE_INUSEONE))
                        pline->lpfnEventProc (pline->htLine, 0, LINE_ADDRESSSTATE, 0,
                                                                        LINEADDRESSSTATE_NUMCALLS | LINEADDRESSSTATE_INUSEONE, 0);
                if (pline->dwLineStates & LINEDEVSTATE_NUMCALLS)
                        pline->lpfnEventProc (pline->htLine, 0, LINE_LINEDEVSTATE,
                                                                        LINEDEVSTATE_NUMCALLS, 0, 0);


                // send internal initialisation strings

                if (userinit[0] == 'A' && userinit[1] == 'T')
                        userinit += 2;

                wsprintf (dial, s_dialFormat,
                                         pline->dpDialParams.dwDigitDuration,
                                         pline->dpDialParams.dwDialPause >> 10,
                                         pline->dpDialParams.dwWaitForDialtone >> 10,
                                         (LPSTR) (pline->ignore ? s_null : s_dialInit),
                                         userinit);

                if (SendModemCommand (pline->hcd, dial) != 0)
                        goto cleanup;

                comevState = state_senddialstr;
        }
        break;


        case state_senddialstr:
        {
                // first, check we got OK from the initialisation string

                if (msg != RESPONSE_OK)
                        goto cleanup;
        }
        // fall thru to same code as if lineDial was called

        case state_linedial:
        {
                // send user dial string

                wsprintf (dial, "ATD%c%s%s\r", pline->pulse ? 'P' : 'T',
                                                                                                 pline->DestAddress,
                                                                                                 pline->partial ? "" : ";");

                if (SendModemCommand (pline->hcd, dial) != 0)
                        goto cleanup;

                comevState = state_talkdropstart;
        }
        break;


        case state_talkdropstart:
        {
                if (msg != RESPONSE_OK)
                        goto cleanup;

                pline->callState = LINECALLSTATE_DIALING;
                pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLSTATE,
                                                                        LINECALLSTATE_DIALING, 0, 0);

                // if this dialstring ended in ";", we have not yet received
                // all of the number to dial so don't display the talkdrop dialog
                // until we get a subsequent lineDial

                if (pline->partial)
                        break;


                // display the dreaded talkdrop dialog

                procTalkDrop = MakeProcInstance (TalkDropProc, hinst);
                hwndTalkDrop = CreateDialog (hinst,  MAKEINTRESOURCE (IDD_TALKDROP),
                                                                                          myhwnd, procTalkDrop);

                // start busy detection

                pline->callState = LINECALLSTATE_PROCEEDING;
                pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLSTATE,
                                                                        LINECALLSTATE_PROCEEDING, 0, 0);

                if (SendModemCommand (pline->hcd, s_atd) != 0)
                        goto cleanup;


                // if we actually reach this point, then the talkdrop dialog
                // is up and the modem is in busy detection mode.
                // we will return if a button is pressed on the dialog, or
                // the modem returns a string (CONNECTED or BUSY)

                comevState = state_talkdropend;
        }
        break;


        default:
        {
                DebugMsg (("****bad state in serialState"));

cleanup:
                msg = RESPONSE_ERROR;
        }
        // either way, fall thru to tidy up our state

        case state_talkdropend:
        {
                // close the dreaded talkdrop dialog

                if (hwndTalkDrop)
                {
                        DestroyWindow (hwndTalkDrop);
                        FreeProcInstance (procTalkDrop);

                        procTalkDrop = 0;
                        hwndTalkDrop = 0;
                }

                // check the two states from modem responses

                if (msg == RESPONSE_BUSY)
                {
                        pline->callState = LINECALLSTATE_DISCONNECTED;
                        pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLSTATE,
                                                        LINECALLSTATE_DISCONNECTED, LINEDISCONNECTMODE_BUSY, 0);
                }
                else if (msg == RESPONSE_OK)
                {
                        pline->callState = LINECALLSTATE_CONNECTED;
                        pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLSTATE,
                                                                                LINECALLSTATE_CONNECTED, 0, 0);

                        // this is the end of this thread
                        // we come back when app calls lineDrop
                        break;
                }
                else if (msg == IDTALK || msg == IDDROP)
                {
                        // a button was pressed on the talkdrop dialog
                        // so modem is still off hook and we need to hang it up

                        SendModemCommand (pline->hcd, s_r);

                        if (msg == IDTALK)
                        {
                                pline->callState = LINECALLSTATE_CONNECTED;
                                pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLSTATE,
                                                                                        LINECALLSTATE_CONNECTED, 0, 0);

                                // this is the end of this thread
                                // we come back when app calls lineDrop
                                break;
                        }
                }


                // finally, we need to close the line and
                // tell the app that we are idle again

                if (pline->callState != LINECALLSTATE_IDLE &&
                         pline->callState != 0)
                {
                        SerialCloseComms (pline->hcd);
                        pline->callState = LINECALLSTATE_IDLE;
                        pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLSTATE,
                                                                                LINECALLSTATE_IDLE, 0, 0);
                        if (pline->dwAddressStates & (LINEADDRESSSTATE_NUMCALLS | LINEADDRESSSTATE_INUSEZERO))
                                pline->lpfnEventProc (pline->htLine, 0, LINE_ADDRESSSTATE, 0,
                                                                                LINEADDRESSSTATE_NUMCALLS | LINEADDRESSSTATE_INUSEZERO, 0);
                        if (pline->dwLineStates & LINEDEVSTATE_NUMCALLS)
                                pline->lpfnEventProc (pline->htLine, 0, LINE_LINEDEVSTATE,
                                                                                LINEDEVSTATE_NUMCALLS, 0, 0);

                }
        }
        break;


        case state_ignore:
                DebugMsg (("Ignoring serial data"));
                break;
  }
}


#define SERIAL_OK                0
#define SERIAL_ERROR -1
#define SERIAL_FATAL -2

int CheckSerialError (int hcd)
{
        int res = GetCommError (hcd, NULL);

        if (res == 0)
                return SERIAL_OK;


        if (res & (CE_BREAK  | CE_CTSTO  | CE_DSRTO  | CE_FRAME | CE_OVERRUN |
                                  CE_RLSDTO | CE_RXOVER | CE_TXFULL | CE_RXPARITY))
        {
                DebugMsg (("SERIAL_ERROR"));
                return SERIAL_ERROR;
        }
        else
        {
                DebugMsg (("SERIAL_FATAL"));
                return SERIAL_FATAL;
        }
}

UINT myatoi (LPSTR speed)
{
        UINT i;

        for (i = 0; *speed; speed++)
                i = i * 10 + (*speed - '0');

        return i;
}

int SerialOpenComms (LPSTR port, LPSTR speed)
{
        int hcd = OpenComm (port, RXQUEUE, TXQUEUE);

        if (hcd >= 0)
        {
                DCB dcb;

                if (GetCommState (hcd, &dcb) == 0)
                {
                        memset (&dcb.BaudRate, 0, sizeof (DCB) - sizeof (dcb.Id));

                        dcb.BaudRate = myatoi (speed);
                        dcb.ByteSize = 8;
                        dcb.fBinary  = TRUE;
                        dcb.fChEvt   = TRUE;
                        dcb.XonChar  = 17;
                        dcb.XoffChar = 19;
                        dcb.EvtChar  = '\n';

                        if (SetCommState (&dcb) == 0 &&
                                 EnableCommNotification (hcd, myhwnd, 512, -1)) // send events
                        {
                                UINT *lpcommev;

                                lpcommev = SetCommEventMask (hcd, EV_RXFLAG);
                                DebugMsg (("Opened: %s:%s", port, speed));

                                return hcd;
                        }
                }

                CloseComm (hcd);
        }

        DebugMsg (("***Comm port failed to open"));
        return -1;
}


void SerialCloseComms (int hcd)
{
// windows bug, this call sends message to window NULL !
//      EnableCommNotification (hcd, NULL, 0, 0);       // stop events being sent

        SendModemCommand (hcd, s_hangup);

        // could sync wait for reply here if brave !

        {       long Wait = GetTickCount();
                while (GetTickCount() - Wait < 500);
        }

        EscapeCommFunction (hcd, CLRDTR);       // drop DTR

        if (CloseComm (hcd) == 0)
                DebugMsg (("Communications port is closed"));
        else
                DebugMsg (("Couldn't close the serial port in SerialCloseComms"));
}


int SendModemCommand (int hcd, const char *lpstr)
{
        int cbstr = _fstrlen (lpstr);

        if (cbstr > TXQUEUE)
                return SERIAL_ERROR;

        if (WriteComm (hcd, lpstr, cbstr) < 0)
        {
                DebugMsg (("failed to write string"));
                return CheckSerialError (hcd);
        }
        else
        {
                DebugMsg (("Sent: %i: %s", cbstr, lpstr));
                return 0;
        }
}


BOOL CALLBACK __export
TalkDropProc (HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
        switch (uiMsg)
        {
                case WM_INITDIALOG:
                {
                        int  BoxHeight, BoxWidth, NewTop, NewLeft;
                        RECT rcDeskTop, rcStateBox;

                        // Center the state box.
                        GetWindowRect (GetDesktopWindow (), &rcDeskTop);
                        GetWindowRect (hDlg, &rcStateBox);

                        BoxWidth = rcStateBox.right - rcStateBox.left;
                        BoxHeight = rcStateBox.bottom - rcStateBox.top;
                        NewTop = (rcDeskTop.bottom - BoxHeight) / 2;
                        NewLeft = (rcDeskTop.right - BoxWidth) / 2;

                        MoveWindow (hDlg, NewLeft, NewTop, BoxWidth, BoxHeight, TRUE);
                        return TRUE;
                }

                case WM_CLOSE:
                        wParam = IDDROP;
                        // fall thru

                case WM_COMMAND:
                {
                        if (wParam == IDCANCEL)
                                wParam = IDDROP;                        // we map esc to drop

                        if (wParam == IDDROP || wParam == IDTALK)
                        {
                                serialState (state_talkdropend, wParam, comevArg);
                                return TRUE;
                        }
                        else
                                return FALSE;
                }

                default:
                        return FALSE;
        }
}



#ifdef DEBUG
void CDECL SPTrace(LPCSTR pszFormat, ...)
{
        static char szBuffer[512];
        static char fmtBuffer[1024];
        static char szModuleBuffer[_MAX_PATH];
        static char szTemp[_MAX_PATH];
        static char szFName[_MAX_FNAME];
        const char* pszLocalFormat;

        int nBuf, count, localCount;
        va_list args;

        pszLocalFormat = pszFormat;

        va_start (args, pszFormat);

        nBuf = wvsprintf (szBuffer, pszLocalFormat, args);

        // Convert formatting to readable format.
        for (count = 0, localCount = 0; count < nBuf; count++, localCount++)
        {
                if (szBuffer[count] == '\r')
                {
                        fmtBuffer[localCount++] = '\\';
                        fmtBuffer[localCount] = 'r';
                }
                else if (szBuffer[count] == '\n')
                {
                        fmtBuffer[localCount++] = '\\';
                        fmtBuffer[localCount] = 'n';
                }
                else
                        fmtBuffer[localCount] = szBuffer[count];
        }

        fmtBuffer[localCount] = '\0';

        GetModuleFileName (hinst, szModuleBuffer, sizeof (szModuleBuffer));
        _splitpath (szModuleBuffer, szTemp, szTemp, szFName, szTemp);
        wsprintf (szBuffer, "%s: %s\n\r", (LPSTR) szFName, (LPSTR) fmtBuffer);

        OutputDebugString (szBuffer);
}
#endif
