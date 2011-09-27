//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// PURPOSE.
//
// Copyright 1993-1995 Microsoft Corporation, all rights reserved.
#include <windows.h>
#include <windowsx.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "atsp.h"

BOOL CALLBACK __export ConfigDlgProc(HWND, UINT, WPARAM, LPARAM);

int     doConfigDialog (HWND, ATSPLineData *);
void    LoadIniStrings (DWORD, ATSPLineData *);
void    fillDialParams (LPLINEDIALPARAMS, LPLINEDIALPARAMS);
long    appCall (int msg, ATSPLineData *pline, long dwRequestID);


// The number of entries in the devices and speed array.

#define NUMPORTS 4
#define NUMSPEEDS 7

char *lpszCommDevArray[NUMPORTS] =
{ "COM1", "COM2", "COM3", "COM4" };
char *lpszCommSpeedArray[NUMSPEEDS] =
{ "300", "1200", "2400", "9600", "19200", "38400", "57600" };


// Various tags in the ini file.

char s_telephon_ini[] = "telephon.ini";
char s_atspexe_exe[]  = "atspexe.exe";

char s_one[]                    = "1";
char s_zero[]                   = "0";
char s_numlines[]               = "NumLines";
char s_numphones[]      = "NumPhones";
char s_providerx[]      = "Provider%d";

char s_port[]                   = "Port";
char s_speed[]                  = "Speed";
char s_initstr[]                = "InitString";
char s_ignore[]         = "IgnoreInternalString";
char s_pulse[]                  = "PulseDial";
char s_linename[]               = "LineName";
char s_lineaddr[]               = "LineAddress";

#define MIN_DURATION            50
#define DEF_DURATION            95
#define MAX_DURATION            255
#define MIN_DIALSPEED   50
#define DEF_DIALSPEED   95
#define MAX_DIALSPEED   255
#define MIN_DIALTONE            1000
#define DEF_DIALTONE            60000
#define MAX_DIALTONE            255000
#define MIN_DIALPAUSE   0
#define DEF_DIALPAUSE   2000
#define MAX_DIALPAUSE   255000

// The global module handle

HANDLE  hInst                   = NULL;

// Line object.
ATSPLineData *pline     = NULL;

// Provider description string
char gszProviderInfo[255];


///////////////////////////////////////////////////////////
// The required DLL functions
///////////////////////////////////////////////////////////

int FAR PASCAL LibMain(HANDLE hInstance, WORD wDataSegment,
                               WORD wHeapSize, LPSTR lpszCmdLine)
{
        if (hInst != NULL)
                return FALSE;

        hInst = hInstance;

        return TRUE;
}

VOID FAR PASCAL __export WEP (int bSystemExit)
{
        hInst = NULL;
}


///////////////////////////////////////////////////////////
// The Service Provider Basic Configuration Routines
///////////////////////////////////////////////////////////

static BOOL appstartup  = FALSE;
static BOOL initialised = FALSE;
static HWND atspexehwnd = 0;

LONG TSPIAPI TSPI_lineNegotiateTSPIVersion (
DWORD           dwDeviceID,
DWORD           dwLowVersion,
DWORD           dwHighVersion,
LPDWORD lpdwTSPIVersion)
{
        DebugMsg (("Entering TSPI_lineNegotiateTSPIVersion"));

#ifdef DEBUG
        if (!initialised)
                assert (dwDeviceID == INITIALIZE_NEGOTIATION);
#endif

        if (dwDeviceID == INITIALIZE_NEGOTIATION ||
                 pline && dwDeviceID == pline->lineID)                  // we support only one line
        {
                *lpdwTSPIVersion = ATSP_VERSION;

                if (dwLowVersion  > ATSP_VERSION ||     // the app is too new for us
                         dwHighVersion < ATSP_VERSION)  // we are too new for the app
                        return LINEERR_INCOMPATIBLEAPIVERSION;
                else
                        return 0;
        }

        return LINEERR_BADDEVICEID;             // The requested device doesn't exist
}


LONG TSPIAPI TSPI_providerInit (
DWORD             dwTSPIVersion,
DWORD             dwPermanentProviderID,
DWORD             dwLineDeviceIDBase,
DWORD             dwPhoneDeviceIDBase,
DWORD             dwNumLines,
DWORD             dwNumPhones,
ASYNC_COMPLETION  lpfnCompletionProc)
{
        int res;

        DebugMsg (("Entering TSPI_providerInit"));

        assert (dwTSPIVersion == ATSP_VERSION);
        assert (dwNumLines    == 1);
        assert (dwNumPhones   == 0);

        if (initialised)
                return LINEERR_NOMULTIPLEINSTANCE;

        // initialise our internal structures

        pline = (ATSPLineData *) GlobalAllocPtr (GMEM_SHARE | GMEM_ZEROINIT,
                                                                                                                  sizeof (ATSPLineData));

        if (pline == 0)
        {
                DebugMsg (("alloc for line data failed"));
                return LINEERR_NOMEM;
        }

        pline->lpfnCompletion   = lpfnCompletionProc;
        pline->lineID                           = dwLineDeviceIDBase;
        pline->dwMediaMode              = LINEMEDIAMODE_INTERACTIVEVOICE;

        pline->callState                                                                = 0;
        pline->dpDialParams.dwDialPause                 = 2000;
        pline->dpDialParams.dwDialSpeed                 = DEF_DIALSPEED;
        pline->dpDialParams.dwDigitDuration             = DEF_DURATION;
        pline->dpDialParams.dwWaitForDialtone   = 60000;

        LoadIniStrings (dwPermanentProviderID, pline);

        // launch our companion app to get an async thread

        appstartup = TRUE;
        res = WinExec (s_atspexe_exe, SW_SHOWMINIMIZED);
        appstartup = FALSE;

        if (res <= HINSTANCE_ERROR)
        {
                DebugMsg (("App didn't start"));
                GlobalFreePtr (pline);
                pline = 0;
                return LINEERR_NODRIVER;
        }

        initialised = TRUE;

        return 0;
}


LONG TSPIAPI TSPI_providerShutdown (DWORD dwTSPIVersion)
{
        DebugMsg (("Entering TSPI_providerShutdown"));

        // close our shadow app. We do it here as it registers with us
        // and therefore our module count doesn't hit 0 so WEP isn't called
        if (atspexehwnd)
        {
                SendMessage (atspexehwnd, WM_CLOSE, 0, 0);
                atspexehwnd = 0;
        }

        GlobalFreePtr (pline);
        pline = 0;

        initialised = FALSE;

        return 0;
}


int FAR PASCAL __export ATSPAppRegister (HWND hwnd)
{
        if (!appstartup)
                return 1;

        atspexehwnd = hwnd;
        return 0;
}


///////////////////////////////////////////////////////////
// The Line Specific Calls
///////////////////////////////////////////////////////////

LONG TSPIAPI TSPI_lineConfigDialog (
DWORD    dwDeviceID,
HWND     hwndOwner,
LPCSTR lpszDeviceClass)
{
        DebugMsg (("Entering TSPI_lineConfigDialog"));

        if (dwDeviceID != pline->lineID)
                return LINEERR_BADDEVICEID;

        doConfigDialog (hwndOwner, pline);

        return 0;
}


long lineDropCore (ATSPLineData *theLine, DWORD dwRequestID)
{
        // if the call is not idle, transition to idle and close the comms port

        return appCall (msgDrop, pline, dwRequestID);
}


LONG TSPIAPI TSPI_lineClose (HDRVLINE hdLine)
{
        DebugMsg (("Entering TSPI_lineClose"));

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        // call drop in case there is still an active call on the line
        lineDropCore (pline, 0);

        pline->callState = 0;
        return 0;
}


LONG TSPIAPI TSPI_lineCloseCall (HDRVCALL hdCall)
{
        DebugMsg (("Entering TSPI_lineCloseCall"));

        if (hdCall != (HDRVCALL) pline->htCall)
                return LINEERR_INVALCALLHANDLE;

        // call drop in case there is still an active call on the line
        lineDropCore (pline, 0);

        pline->callState = 0;
        return 0;
}


LONG TSPIAPI TSPI_lineDrop (
DRV_REQUESTID   dwRequestID,
HDRVCALL                        hdCall,
LPCSTR                  lpsUserUserInfo,
DWORD                           dwSize)
{
        // Transition a call to the IDLE state.

        DebugMsg (("Entering TSPI_lineDrop"));

        if (hdCall != (HDRVCALL) pline->htCall)
                return LINEERR_INVALCALLHANDLE;

        return lineDropCore (pline, dwRequestID);       // it was our active call
}

LONG CheckDestAddress (LPCSTR szAddr)
{
        char ch;
        int  i;

        for (i = 1; ch = *szAddr++; i++)
        {
                if (ch == '?')
                        return LINEERR_DIALPROMPT;
                else if (ch == '$')
                        return LINEERR_DIALBILLING;
        }

        if (i > TAPIMAXDESTADDRESSSIZE)
                return LINEERR_INVALADDRESS;

        return 0;
}

LONG TSPIAPI TSPI_lineDial (
DRV_REQUESTID   dwRequestID,
HDRVCALL                        hdCall,
LPCSTR                  lpszDestAddress,
DWORD                           dwCountryCode)
{
        long res;

        DebugMsg (("Entering TSPI_lineDial"));

        if (hdCall != (HDRVCALL) pline->htCall)
                return LINEERR_INVALCALLHANDLE;

        if      (pline->callState != LINECALLSTATE_DIALTONE &&
                 pline->callState != LINECALLSTATE_DIALING)
                return LINEERR_INVALCALLSTATE;

        // check for invalid dial string (wierd chars or length)
        // we can assume lpszDestAddress is a valid pointer as TAPI checks it

        if (res = CheckDestAddress (lpszDestAddress))
                return res;

        _fstrcpy (pline->DestAddress, lpszDestAddress);
        pline->partial = (lpszDestAddress[lstrlen (lpszDestAddress) - 1] == ';');

        // finally, set off the async call in our companion app

        return appCall (msgDial, pline, dwRequestID);
}


LONG TSPIAPI TSPI_lineGetAddressCaps (
DWORD                                   dwDeviceID,
DWORD                                   dwAddressID,
DWORD                                   dwTSPIVersion,
DWORD                                   dwExtVersion,
LPLINEADDRESSCAPS lpAddressCaps)

{
        int cbLineAddr;

        DebugMsg (("Entering TSPI_lineGetAddressCaps"));

        // We support only one line and one address.

        if (dwDeviceID != pline->lineID)
                return LINEERR_BADDEVICEID;

        if (dwAddressID != 0)
                return LINEERR_INVALADDRESSID;

        cbLineAddr = _fstrlen (pline->lineaddr) + 1;

        lpAddressCaps->dwNeededSize += cbLineAddr;

        if (lpAddressCaps->dwTotalSize >= lpAddressCaps->dwUsedSize + cbLineAddr)
        {
                _fmemcpy((char *) lpAddressCaps + lpAddressCaps->dwUsedSize,
                                        pline->lineaddr, cbLineAddr);

                lpAddressCaps->dwAddressSize     = cbLineAddr;
                lpAddressCaps->dwAddressOffset = lpAddressCaps->dwUsedSize;
                lpAddressCaps->dwUsedSize                += cbLineAddr;
        }

        lpAddressCaps->dwLineDeviceID                   =       pline->lineID;

        lpAddressCaps->dwAddressSharing         =       LINEADDRESSSHARING_PRIVATE;
        lpAddressCaps->dwAddressStates          =       LINEADDRESSSTATE_INUSEZERO |
                                                                                                                LINEADDRESSSTATE_INUSEONE |
                                                                                                                LINEADDRESSSTATE_NUMCALLS |
                                                                                                                LINEADDRESSSTATE_CAPSCHANGE;
        lpAddressCaps->dwCallInfoStates         =       LINECALLINFOSTATE_APPSPECIFIC;
        lpAddressCaps->dwCallerIDFlags          =       LINECALLPARTYID_UNAVAIL;
        lpAddressCaps->dwCalledIDFlags          =       LINECALLPARTYID_UNAVAIL;
        lpAddressCaps->dwConnectedIDFlags       =       LINECALLPARTYID_UNAVAIL;
        lpAddressCaps->dwRedirectionIDFlags     =       LINECALLPARTYID_UNAVAIL;
        lpAddressCaps->dwRedirectingIDFlags     =       LINECALLPARTYID_UNAVAIL;
        lpAddressCaps->dwCallStates                     =       LINECALLSTATE_IDLE |
                                                                                                                LINECALLSTATE_DIALTONE |
                                                                                                                LINECALLSTATE_DIALING |
                                                                                                                LINECALLSTATE_CONNECTED |
                                                                                                                LINECALLSTATE_DISCONNECTED |
                                                                                                                LINECALLSTATE_PROCEEDING |
                                                                                                                LINECALLSTATE_UNKNOWN;
        lpAddressCaps->dwDialToneModes          =       LINEDIALTONEMODE_UNAVAIL;
        lpAddressCaps->dwBusyModes                              =       LINEBUSYMODE_UNAVAIL;
        lpAddressCaps->dwSpecialInfo                    =       LINESPECIALINFO_UNAVAIL;
        lpAddressCaps->dwDisconnectModes                =       LINEDISCONNECTMODE_BUSY;
        lpAddressCaps->dwMaxNumActiveCalls      =       1;
        lpAddressCaps->dwAddrCapFlags                   =       LINEADDRCAPFLAGS_DIALED |
                                                                                                                LINEADDRCAPFLAGS_PARTIALDIAL;
        lpAddressCaps->dwCallFeatures                   =       LINECALLFEATURE_DIAL |
                                                                                                                LINECALLFEATURE_DROP;
        lpAddressCaps->dwAddressFeatures                =       LINEADDRFEATURE_MAKECALL;

        return 0;
}

LONG TSPIAPI TSPI_lineGetAddressID (
HDRVLINE                 hdLine,
LPDWORD          lpdwAddressID,
DWORD                    dwAddressMode,
LPCSTR           lpsAddress,
DWORD                    dwSize)
{
        DebugMsg (("Entering TSPI_lineGetAddressID"));

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        assert (dwAddressMode == LINEADDRESSMODE_DIALABLEADDR);

        if (_fstrcmp (pline->lineaddr, lpsAddress))
                return LINEERR_INVALADDRESS;
        else
        {
                *lpdwAddressID = 0;     // we support only 1 address
                return 0;
        }
}

LONG TSPIAPI TSPI_lineGetAddressStatus (
HDRVLINE                                        hdLine,
DWORD                                           dwAddressID,
LPLINEADDRESSSTATUS     lpAddressStatus)
{
        DebugMsg (("Entering TSPI_lineGetAddressStatus"));

        if (dwAddressID)
                return LINEERR_INVALADDRESSID;

        // if we are idle a call can be made

        if (pline->callState == 0)              // our internal flag that line is not in use
                lpAddressStatus->dwAddressFeatures = LINEADDRFEATURE_MAKECALL;
        else
        {
                lpAddressStatus->dwNumInUse              = 1;
                lpAddressStatus->dwNumActiveCalls = 1;
        }

        return 0;
}

LONG TSPIAPI TSPI_lineGetCallAddressID (
HDRVCALL        hdCall,
LPDWORD lpdwAddressID)
{
        DebugMsg (("Entering TSPI_lineGetCallAddressID"));

        if (hdCall != (HDRVCALL) pline->htCall)
                return LINEERR_INVALCALLHANDLE;

        // There is but a single address where a call may exist.
        *lpdwAddressID = 0;

        return 0;
}

LONG TSPIAPI TSPI_lineGetCallInfo (
HDRVCALL                        hdCall,
LPLINECALLINFO  lpCallInfo)
{
        int cbDestAddr = _fstrlen (pline->DestAddress) + 1;

        DebugMsg (("Entering TSPI_lineGetCallInfo"));

        if (hdCall != (HDRVCALL) pline->htCall)
                return LINEERR_INVALCALLHANDLE;


        lpCallInfo->dwNeededSize += cbDestAddr;

        if (lpCallInfo->dwTotalSize >= lpCallInfo->dwUsedSize + cbDestAddr)
        {
                _fmemcpy((char *) lpCallInfo + lpCallInfo->dwUsedSize,
                                        pline->DestAddress, cbDestAddr);

                lpCallInfo->dwDisplayableAddressSize    = cbDestAddr;
                lpCallInfo->dwDisplayableAddressOffset  = lpCallInfo->dwUsedSize;
                lpCallInfo->dwUsedSize                                          += cbDestAddr;
        }

        lpCallInfo->dwLineDeviceID                                      =       pline->lineID;
        lpCallInfo->dwBearerMode                                        =       LINEBEARERMODE_VOICE;
        lpCallInfo->dwMediaMode                                         =       pline->dwMediaMode;
        lpCallInfo->dwAppSpecific                                       =       pline->dwAppSpecific;
        lpCallInfo->dwCallParamFlags                            =       LINECALLPARAMFLAGS_IDLE;
        lpCallInfo->dwCallStates                                        =       LINECALLSTATE_IDLE |
                                                                                                                        LINECALLSTATE_DIALTONE |
                                                                                                                        LINECALLSTATE_DIALING |
                                                                                                                        LINECALLSTATE_CONNECTED |
                                                                                                                        LINECALLSTATE_DISCONNECTED |
                                                                                                                        LINECALLSTATE_PROCEEDING |
                                                                                                                        LINECALLSTATE_UNKNOWN;
        lpCallInfo->dwOrigin                                                    =       LINECALLORIGIN_OUTBOUND;
        lpCallInfo->dwReason                                                    =       LINECALLREASON_UNAVAIL;
        lpCallInfo->dwCallerIDFlags                             =       LINECALLPARTYID_UNAVAIL;
        lpCallInfo->dwCalledIDFlags                             =       LINECALLPARTYID_UNAVAIL;
        lpCallInfo->dwConnectedIDFlags                  =       LINECALLPARTYID_UNAVAIL;
        lpCallInfo->dwRedirectionIDFlags                        =       LINECALLPARTYID_UNAVAIL;
        lpCallInfo->dwRedirectingIDFlags                        =       LINECALLPARTYID_UNAVAIL;

        lpCallInfo->DialParams                                          =       pline->dpDialParams;

        return 0;
}

LONG TSPIAPI TSPI_lineGetCallStatus (
HDRVCALL                          hdCall,
LPLINECALLSTATUS lpCallStatus)
{
        DebugMsg (("Entering TSPI_lineGetCallStatus"));

        if (hdCall != (HDRVCALL) pline->htCall)
                return LINEERR_INVALCALLHANDLE;

        lpCallStatus->dwCallState = pline->callState;

        switch (pline->callState)
        {
                case LINECALLSTATE_IDLE:
                case LINECALLSTATE_UNKNOWN:
                        break;

                case LINECALLSTATE_DIALTONE:
                        lpCallStatus->dwCallStateMode   =       LINEDIALTONEMODE_UNAVAIL;
                        // fall thru

                case LINECALLSTATE_DIALING:
                        lpCallStatus->dwCallFeatures    =       LINECALLFEATURE_DIAL |
                                                                                                                LINECALLFEATURE_DROP;
                        break;

                case LINECALLSTATE_DISCONNECTED:
                        lpCallStatus->dwCallStateMode   =       LINEDISCONNECTMODE_BUSY;
                        lpCallStatus->dwCallFeatures    =       LINECALLFEATURE_DROP;
                        break;

                case LINECALLSTATE_CONNECTED:
                case LINECALLSTATE_PROCEEDING:
                        lpCallStatus->dwCallFeatures    =       LINECALLFEATURE_DROP;
                        break;
        }

        return 0;
}

LONG TSPIAPI TSPI_lineGetDevCaps (
DWORD dwDeviceID,
DWORD dwTSPIVersion,
DWORD dwExtVersion,
LPLINEDEVCAPS lpLineDevCaps)
{
        int cbname = _fstrlen (pline->linename) + 1;
        int cbinfo = _fstrlen (gszProviderInfo) + 1;

        DebugMsg (("Entering TSPI_lineGetDevCaps"));

        if (dwDeviceID != pline->lineID)
                return LINEERR_BADDEVICEID;


        lpLineDevCaps->dwNeededSize += cbinfo + cbname;


        if (lpLineDevCaps->dwTotalSize >= lpLineDevCaps->dwUsedSize + cbinfo)
        {
                // Copy in the provider info

                _fmemcpy ((char *)lpLineDevCaps + lpLineDevCaps->dwUsedSize,
                                                                                                gszProviderInfo, cbinfo);

                lpLineDevCaps->dwProviderInfoSize       = cbinfo;
                lpLineDevCaps->dwProviderInfoOffset     = lpLineDevCaps->dwUsedSize;
                lpLineDevCaps->dwUsedSize                         += cbinfo;
        }

        if (lpLineDevCaps->dwTotalSize >= lpLineDevCaps->dwUsedSize + cbname)
        {
                // Copy in the line name

                _fmemcpy((char *) lpLineDevCaps + lpLineDevCaps->dwUsedSize,
                                                                                                pline->linename, cbname);

                lpLineDevCaps->dwLineNameSize           = cbname;
                lpLineDevCaps->dwLineNameOffset = lpLineDevCaps->dwUsedSize;
                lpLineDevCaps->dwUsedSize                 += cbname;
        }


        lpLineDevCaps->dwPermanentLineID                = (pline->dwppID << 16) + 0;

        lpLineDevCaps->dwAddressModes                   =       LINEADDRESSMODE_ADDRESSID;
        lpLineDevCaps->dwNumAddresses                   =       1;
        lpLineDevCaps->dwMaxNumActiveCalls      =       1;
        lpLineDevCaps->dwStringFormat                   =       STRINGFORMAT_ASCII;
        lpLineDevCaps->dwBearerModes                    =       LINEBEARERMODE_VOICE;
        lpLineDevCaps->dwMediaModes                     =       LINEMEDIAMODE_INTERACTIVEVOICE;
        lpLineDevCaps->dwGenerateDigitModes     =       LINEDIGITMODE_PULSE |
                                                                                                                LINEDIGITMODE_DTMF;
        lpLineDevCaps->dwLineStates                     =       LINEDEVSTATE_NUMCALLS |
                                                                                                                LINEDEVSTATE_CAPSCHANGE;

        lpLineDevCaps->MinDialParams.dwDialSpeed                                = MIN_DIALSPEED;
        lpLineDevCaps->DefaultDialParams.dwDialSpeed                    = DEF_DIALSPEED;
        lpLineDevCaps->MaxDialParams.dwDialSpeed                                = MAX_DIALSPEED;

        lpLineDevCaps->MinDialParams.dwDigitDuration                    = MIN_DURATION;
        lpLineDevCaps->DefaultDialParams.dwDigitDuration        = DEF_DURATION;
        lpLineDevCaps->MaxDialParams.dwDigitDuration                    = MAX_DURATION;

        lpLineDevCaps->MinDialParams.dwWaitForDialtone          = MIN_DIALTONE;
        lpLineDevCaps->DefaultDialParams.dwWaitForDialtone      = DEF_DIALTONE;
        lpLineDevCaps->MaxDialParams.dwWaitForDialtone          = MAX_DIALTONE;

        lpLineDevCaps->MinDialParams.dwDialPause                                = MIN_DIALPAUSE;
        lpLineDevCaps->DefaultDialParams.dwDialPause                    = DEF_DIALPAUSE;
        lpLineDevCaps->MaxDialParams.dwDialPause                                = MAX_DIALPAUSE;

        lpLineDevCaps->dwLineFeatures   = LINEFEATURE_MAKECALL;

        // we make an assumption in this sample code that a "stock"
        // AT modem will understand @ and W but not $ modifiers

        lpLineDevCaps->dwDevCapFlags    =       LINEDEVCAPFLAGS_CLOSEDROP |
                                                                                                LINEDEVCAPFLAGS_DIALQUIET |
                                                                                                LINEDEVCAPFLAGS_DIALDIALTONE;

        return 0;
}

LONG TSPIAPI TSPI_lineGetID (
HDRVLINE                        hdLine,
DWORD                           dwAddressID,
HDRVCALL                        hdCall,
DWORD                           dwSelect,
LPVARSTRING             lpDeviceID,
LPCSTR                  lpszDeviceClass)
{
        DebugMsg (("Entering TSPI_lineGetID"));

        // Since we have only one device, we don't have to
        // check the location of the line, address, or call.

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        if (_fstrcmp (lpszDeviceClass, "tapi/line") == 0)
        {
                lpDeviceID->dwNeededSize += sizeof (DWORD);

                if (lpDeviceID->dwTotalSize >= lpDeviceID->dwUsedSize + sizeof (DWORD))
                {
        lpDeviceID->dwStringFormat      = STRINGFORMAT_BINARY;
        lpDeviceID->dwStringSize        = sizeof (DWORD);
        lpDeviceID->dwStringOffset      = lpDeviceID->dwUsedSize;

        *((DWORD *) ((char *) lpDeviceID + lpDeviceID->dwUsedSize)) = pline->lineID;

                        lpDeviceID->dwUsedSize          += sizeof (DWORD);
                }

                return 0;
        }

#ifdef COMMSUPPORT
        if (_fstrcmp (lpszDeviceClass, "comm") == 0)
        {
      int cbport = _fstrlen (pline->port) + 1;

      lpDeviceID->dwNeededSize += cbport;

                if (lpDeviceID->dwTotalSize >= lpDeviceID->dwUsedSize + cbport)
                {
                        lpDeviceID->dwStringFormat = STRINGFORMAT_ASCII;
                        lpDeviceID->dwStringSize        = cbport;
                        lpDeviceID->dwStringOffset = lpDeviceID->dwUsedSize;

                        _fmemcpy ((char *) lpDeviceID + lpDeviceID->dwUsedSize, pline->port, cbport);

                        lpDeviceID->dwUsedSize          += cbport;
                }

                return 0;
        }
#endif

        return LINEERR_NODEVICE;
}

LONG TSPIAPI TSPI_lineGetLineDevStatus (
HDRVLINE                         hdLine,
LPLINEDEVSTATUS lpLineDevStatus)
{
        DebugMsg (("Entering TSPI_lineGetLineDevStatus"));

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        lpLineDevStatus->dwRoamMode              = LINEROAMMODE_UNAVAIL;

        lpLineDevStatus->dwDevStatusFlags = LINEDEVSTATUSFLAGS_CONNECTED |
                                                                                                        LINEDEVSTATUSFLAGS_INSERVICE;

        if (pline->callState == 0)
        {
                lpLineDevStatus->dwNumActiveCalls = 0;
                lpLineDevStatus->dwLineFeatures  = LINEFEATURE_MAKECALL;
        }
        else
        {
                lpLineDevStatus->dwNumActiveCalls = 1;
        }

        return 0;
}

LONG TSPIAPI TSPI_lineGetNumAddressIDs (
HDRVLINE        hdLine,
LPDWORD lpNumAddressIDs)
{
        DebugMsg (("Entering TSPI_lineGetNumAddressIDs"));

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        *lpNumAddressIDs = 1;   // We only support one address
        return 0;
}

LONG TSPIAPI TSPI_lineMakeCall (
DRV_REQUESTID                           dwRequestID,
HDRVLINE                                                hdLine,
HTAPICALL                                       htCall,
LPHDRVCALL                                      lphdCall,
LPCSTR                                          lpszDestAddress,
DWORD                                                   dwCountryCode,
LPLINECALLPARAMS const  lpCallParams)
{
        long res;

        DebugMsg (("Entering TSPI_lineMakeCall"));

        // check for invalid dial string (wierd chars or length)
        if (lpszDestAddress && (res = CheckDestAddress (lpszDestAddress)))
                return res;

        if (pline->callState != 0)
                return LINEERR_CALLUNAVAIL;

        if (lpCallParams)
        {
                if (lpCallParams->dwBearerMode != LINEBEARERMODE_VOICE)
                        return LINEERR_INVALBEARERMODE;

                if (lpCallParams->dwMediaMode != LINEMEDIAMODE_INTERACTIVEVOICE)
                        return LINEERR_INVALMEDIAMODE;

                if (lpCallParams->dwCallParamFlags & ~(LINECALLPARAMFLAGS_IDLE))
                        return LINEERR_INVALCALLPARAMS;

                fillDialParams (&lpCallParams->DialParams, &pline->dpDialParams);
        }

        // fill in fields of the call record

        pline->htCall = htCall;                 // we have no hdcall of our own
        *lphdCall = (HDRVCALL) htCall;

        pline->dwAppSpecific = 0;

        if (lpszDestAddress)
        {
                _fstrcpy (pline->DestAddress, lpszDestAddress);
                pline->partial = (lpszDestAddress[lstrlen (lpszDestAddress) - 1] == ';');
        }
        else
        {
                pline->DestAddress[0] = 0;
                pline->partial = FALSE;
        }

        // finally, set off the async call in our companion app

        return appCall (msgMakeCall, pline, dwRequestID);
}

long appCall (int msg, ATSPLineData *pline, long dwRequestID)
{
        long res;

        if (atspexehwnd == 0)
        {
                DebugMsg (("companion app has not yet registered"));
                return LINEERR_NODRIVER;
        }

        if (pline->dwRequestID)
        {
                DebugMsg (("can't do two async requests at the same time!"));
                return LINEERR_INVALCALLSTATE;
        }

        pline->dwRequestID = dwRequestID;

        if ((res = SendMessage (atspexehwnd, WM_EXEMSG, msg, (long) pline)) < 0)
                DebugMsg (("send message to companion app failed: %li", res));

        return res;
}

void fillDialParams (LPLINEDIALPARAMS source, LPLINEDIALPARAMS dest)
{
        // this could also be called from lineSetCallParams
        // Set the dial params if there are any.

        DWORD   pause           = source->dwDialPause;
        DWORD   speed           = source->dwDialSpeed;
        DWORD   duration = source->dwDigitDuration;
        DWORD   dialtone = source->dwWaitForDialtone;

        if (pause == 0)
                pause = DEF_DIALPAUSE;
        else if (pause < MIN_DIALPAUSE)
                pause = MIN_DIALPAUSE;
        else if (pause > MAX_DIALPAUSE)
                pause = MAX_DIALPAUSE;

        if (speed == 0)
                speed = DEF_DIALSPEED;
        else if (speed < MIN_DIALSPEED)
                speed = MIN_DIALSPEED;
        else if (speed > MAX_DIALSPEED)
                speed = MAX_DIALSPEED;

        if (duration == 0)
                duration = DEF_DURATION;
        else if (duration < MIN_DURATION)
                duration = MIN_DURATION;
        else if (duration > MAX_DURATION)
                duration = MAX_DURATION;

        if (dialtone == 0)
                dialtone = DEF_DIALTONE;
        else if (dialtone < MIN_DIALTONE)
                dialtone = MIN_DIALTONE;
        else if (dialtone > MAX_DIALTONE)
                dialtone = MAX_DIALTONE;

        dest->dwDialPause                       = pause;
        dest->dwDialSpeed                       = speed;
        dest->dwDigitDuration   = duration;
        dest->dwWaitForDialtone = dialtone;
}

LONG TSPIAPI TSPI_lineOpen (
DWORD                   dwDeviceID,
HTAPILINE       htLine,
LPHDRVLINE      lphdLine,
DWORD                   dwTSPIVersion,
LINEEVENT       lpfnEventProc)
{
        DebugMsg (("Entering TSPI_lineOpen"));

        if (dwDeviceID != pline->lineID)
                return LINEERR_BADDEVICEID;

        // Since we only support outgoing calls, we
        // don't open the serial port until we need to make a call

        pline->lpfnEventProc = lpfnEventProc;
        pline->htLine                    = htLine;
        *lphdLine                        = (HDRVLINE) pline;

        return 0;
}

LONG TSPIAPI TSPI_lineSetAppSpecific (
HDRVCALL        hdCall,
DWORD           dwAppSpecific)
{
        DebugMsg (("Entering TSPI_lineSetAppSpecific"));

        if ((hdCall != (HDRVCALL) pline->htCall) ||
                (pline->callState = 0))
                return LINEERR_INVALCALLHANDLE;

        pline->dwAppSpecific = dwAppSpecific;

        pline->lpfnEventProc (pline->htLine, pline->htCall, LINE_CALLINFO,
                                                          LINECALLINFOSTATE_APPSPECIFIC, 0, 0);
        return 0;
}

LONG TSPIAPI TSPI_lineConditionalMediaDetection (
HDRVLINE          hdLine,
DWORD             dwMediaModes,
LPLINECALLPARAMS  const lpCallParams)
{
        DebugMsg (("Entering TSPI_lineConditionalMediaDetection"));

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        if (dwMediaModes != LINEMEDIAMODE_INTERACTIVEVOICE)
                return LINEERR_INVALMEDIAMODE;

        if ((lpCallParams) &&
                ((lpCallParams->dwBearerMode != LINEBEARERMODE_VOICE) ||
                        (lpCallParams->dwMediaMode != LINEMEDIAMODE_INTERACTIVEVOICE) ||
                        (lpCallParams->dwCallParamFlags & ~(LINECALLPARAMFLAGS_IDLE))))
                return LINEERR_INVALMEDIAMODE;

        return 0;
}

LONG TSPIAPI TSPI_lineSetDefaultMediaDetection (
HDRVLINE        hdLine,
DWORD           dwMediaModes)
{
        DebugMsg (("Entering TSPI_lineSetDefaultMediaDetection"));

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        if (dwMediaModes && dwMediaModes != LINEMEDIAMODE_INTERACTIVEVOICE)
                return LINEERR_INVALMEDIAMODE;

        pline->dwLineMediaModes = dwMediaModes;

        return 0;
}

LONG TSPIAPI TSPI_lineSetMediaMode (
HDRVCALL        hdCall,
DWORD           dwMediaMode)
{
        DebugMsg (("Entering TSPI_lineSetMediaMode"));

        if (hdCall != (HDRVCALL) pline->htCall)
                return LINEERR_INVALCALLHANDLE;

        if (dwMediaMode != LINEMEDIAMODE_INTERACTIVEVOICE)
                return LINEERR_INVALMEDIAMODE;

        pline->dwMediaMode = dwMediaMode;

        return 0;
}

LONG TSPIAPI TSPI_lineSetStatusMessages (
HDRVLINE        hdLine,
DWORD           dwLineStates,
DWORD           dwAddressStates)
{
        DebugMsg (("Entering TSPI_lineSetStatusMessages"));

        if (hdLine != (HDRVLINE) pline)
                return LINEERR_INVALLINEHANDLE;

        pline->dwLineStates             = dwLineStates;
        pline->dwAddressStates  = dwAddressStates;

        return 0;
}


///////////////////////////////////////////////////////////
// The configuration trio
///////////////////////////////////////////////////////////

// These routines are called from the control panel applet
// TAPI may not be running at this point, so one cannot assume
// we have been initialised


LONG TSPIAPI TSPI_providerInstall (HWND hwnd, DWORD dwPermanentProviderId)
{
        int res;
        OFSTRUCT OpenBuff;
        ATSPLineData tmpline;

        DebugMsg (("Entering TSPI_providerInstall"));

        if (!initialised)
                LoadIniStrings (dwPermanentProviderId, &tmpline);
        else if (dwPermanentProviderId != pline->dwppID)
        {
                res = ID_MULTIPLE_INST;
                goto error;
        }

        // check that all our required components are available
        if (OpenFile (s_atspexe_exe, &OpenBuff, OF_EXIST) == HFILE_ERROR)
        {
                res = ID_NO_DRIVER;
                goto error;
        }

        // display the config screen
        if (!doConfigDialog (hwnd, (initialised ? pline : &tmpline)))
                return 0;
        else
                return LINEERR_OPERATIONFAILED;

error:
        {
                char szerr[255];
                char title[255];

                LoadString (hInst, res, szerr, sizeof (szerr));
                LoadString (hInst, ID_ATSP_ERROR, title, sizeof (title));

                MessageBox (hwnd, szerr, title, MB_APPLMODAL | MB_ICONEXCLAMATION);

                return LINEERR_OPERATIONFAILED;
        }
}


LONG TSPIAPI TSPI_providerRemove (HWND hwnd, DWORD dwPermanentProviderId)
{
        DebugMsg (("Entering TSPI_providerRemove"));

        // The control panel removes all of our junk for us
        // (and then some) when the provider is removed.

        return 0;
}


LONG TSPIAPI TSPI_providerConfig (HWND hwnd, DWORD dwPermanentProviderId)
{
        ATSPLineData tmpline;
        DebugMsg (("Entering TSPI_providerConfig"));

        if (!initialised)
                LoadIniStrings (dwPermanentProviderId, &tmpline);
        else if (pline && dwPermanentProviderId != pline->dwppID)
                return LINEERR_NOMULTIPLEINSTANCE;

        doConfigDialog (hwnd, (initialised ? pline : &tmpline));

        return 0;
}


LONG TSPIAPI TSPI_providerEnumDevices(
    DWORD    dwPermanentProviderID,
    LPDWORD  lpdwNumLines,
    LPDWORD  lpdwNumPhones,
    HPROVIDER   hProvider,
    LINEEVENT   lpfnLineCreateProc,
    PHONEEVENT  lpfnPhoneCreateProc)
{
        DebugMsg (("Entering TSPI_providerEnumDevices"));

        if (pline && dwPermanentProviderID != pline->dwppID)
                return LINEERR_NOMULTIPLEINSTANCE;

        *lpdwNumLines  = 1;
        *lpdwNumPhones = 0;

        return 0;
}


///////////////////////////////////////////////////////////
// Internal support routines
///////////////////////////////////////////////////////////


void LoadIniStrings (DWORD ppID, ATSPLineData *plinedata)
{
        char section[sizeof (s_providerx) + 5];         // room for 65535

#ifdef DEBUG
        if (initialised)
                DebugMsg (("***LoadIniStrings incorrectly called multiple times"));
#endif

        wsprintf (section, s_providerx, (int) ppID);
        plinedata->dwppID = ppID;

        // user preferences come from the telephon.ini file

        plinedata->ignore = GetPrivateProfileInt (section, s_ignore,
                                                                                                                        0, s_telephon_ini);
        plinedata->pulse  = GetPrivateProfileInt (section, s_pulse,
                                                                                                                        0, s_telephon_ini);

        GetPrivateProfileString (section, s_port, "COM1", plinedata->port,
                                                                                        sizeof (plinedata->port), s_telephon_ini);
        GetPrivateProfileString (section, s_speed, "9600", plinedata->speed,
                                                                                        sizeof (plinedata->speed), s_telephon_ini);
        GetPrivateProfileString (section, s_initstr, "", plinedata->initstr,
                                                                                        sizeof (plinedata->initstr), s_telephon_ini);
        GetPrivateProfileString (section, s_linename, "", plinedata->linename,
                                                                                        sizeof (plinedata->linename), s_telephon_ini);
        GetPrivateProfileString (section, s_lineaddr, "", plinedata->lineaddr,
                                                                                        sizeof (plinedata->lineaddr), s_telephon_ini);

        // the provider info string comes from the resource file

        gszProviderInfo[0] = 0;         // in case loadstring fails

        LoadString (hInst, ID_PROVIDER_INFO, gszProviderInfo,
                                                                                                sizeof (gszProviderInfo));
}


ATSPLineData *cfgdlgline = NULL;

int doConfigDialog (HWND hwnd, ATSPLineData *pline)
{
        cfgdlgline = pline;
        return DialogBox (hInst, MAKEINTRESOURCE (IDD_CFGDLG), hwnd, ConfigDlgProc);
}


BOOL CALLBACK __export
ConfigDlgProc (HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
  int CurrSel;

  switch (uiMsg)
  {
        case WM_INITDIALOG:
   {
                for (CurrSel = 0; CurrSel < NUMPORTS; CurrSel++)
                {
                        // List the port in the combo box.
                        SendDlgItemMessage (hDlg, ID_PORT, CB_ADDSTRING,
                                                                        0, (LPARAM) ((LPSTR) lpszCommDevArray[CurrSel]));
                }

                for (CurrSel = 0; CurrSel < NUMSPEEDS; CurrSel++)
                {
                        SendDlgItemMessage(hDlg, ID_SPEED, CB_ADDSTRING,
                                                                0, (LPARAM) ((LPSTR) lpszCommSpeedArray[CurrSel]));
                }

                CurrSel = (int) SendDlgItemMessage (hDlg, ID_PORT,  CB_FINDSTRING, 0,
                                                                                                        (LPARAM) (LPSTR) cfgdlgline->port);
                SendDlgItemMessage (hDlg, ID_PORT,              CB_SETCURSEL,
                                                                                                        CurrSel, 0);
                CurrSel = (int) SendDlgItemMessage (hDlg, ID_SPEED, CB_FINDSTRING, 0,
                                                                                                        (LPARAM) (LPSTR) cfgdlgline->speed);
                SendDlgItemMessage (hDlg, ID_SPEED,             CB_SETCURSEL,
                                                                                                        CurrSel, 0);
                SendDlgItemMessage (hDlg, ID_LINENAME, WM_SETTEXT, 0,
                                                                                                        (LPARAM) (LPSTR) cfgdlgline->linename);
                SendDlgItemMessage (hDlg, ID_LINEADDR, WM_SETTEXT, 0,
                                                                                                        (LPARAM) (LPSTR) cfgdlgline->lineaddr);
                SendDlgItemMessage (hDlg, ID_INITSTR,   WM_SETTEXT, 0,
                                                                                                        (LPARAM) (LPSTR) cfgdlgline->initstr);
                SendDlgItemMessage (hDlg, ID_IGNORE,    BM_SETCHECK,
                                                                                                        cfgdlgline->ignore, 0);
                SendDlgItemMessage (hDlg, ID_PULSE,             BM_SETCHECK,
                                                                                                        cfgdlgline->pulse, 0);
        }
        break;

        case WM_COMMAND:
        {
          switch (wParam)
          {
                case IDOK:
                {
                        // This code is a little brutal, it writes out all the
                        // (new) values when OK is pressed, even if nothing
                        // changed

                        char Buffer[20];        // for itoa of small ints

                        char szp[sizeof (s_providerx) + 5];             // room for 65535
                        wsprintf (szp, s_providerx, (int) cfgdlgline->dwppID);

                        // Port

                        CurrSel = (int) SendDlgItemMessage (hDlg, ID_PORT,
                                                                                                                                CB_GETCURSEL, 0, 0);
                        SendDlgItemMessage (hDlg, ID_PORT, CB_GETLBTEXT, CurrSel,
                                                                                                                                (LPARAM) (LPSTR) cfgdlgline->port);
                        WritePrivateProfileString (szp, s_port, cfgdlgline->port, s_telephon_ini);

                        // Speed

                        CurrSel = (int) SendDlgItemMessage (hDlg, ID_SPEED,
                                                                                                                                CB_GETCURSEL, 0, 0);
                        SendDlgItemMessage (hDlg, ID_SPEED, CB_GETLBTEXT, CurrSel,
                                                                                                                                (LPARAM) (LPSTR) cfgdlgline->speed);
                        WritePrivateProfileString (szp, s_speed, cfgdlgline->speed, s_telephon_ini);

                        // Line Name

                        SendDlgItemMessage (hDlg, ID_LINENAME, WM_GETTEXT,
                                                        sizeof (cfgdlgline->linename), (LPARAM) (LPSTR) cfgdlgline->linename);
                        WritePrivateProfileString (szp, s_linename, cfgdlgline->linename,
                                                                                                                                                                s_telephon_ini);

                        // Line Address

                        SendDlgItemMessage (hDlg, ID_LINEADDR, WM_GETTEXT,
                                                        sizeof (cfgdlgline->lineaddr), (LPARAM) (LPSTR) cfgdlgline->lineaddr);
                        WritePrivateProfileString (szp, s_lineaddr, cfgdlgline->lineaddr,
                                                                                                                                                                s_telephon_ini);

                        // Initialisation String

                        SendDlgItemMessage (hDlg, ID_INITSTR, WM_GETTEXT,
                                                                sizeof (cfgdlgline->initstr), (LPARAM) (LPSTR) cfgdlgline->initstr);
                        AnsiUpper (cfgdlgline->initstr);
                        WritePrivateProfileString (szp, s_initstr, cfgdlgline->initstr,
                                                                                                                                                                s_telephon_ini);

                        // "Don't use internal string" checkbox

                        cfgdlgline->ignore = (int) SendDlgItemMessage (hDlg, ID_IGNORE,
                                                                                                                                                BM_GETCHECK, 0, 0);
                        _itoa(cfgdlgline->ignore, Buffer, 10);
                        WritePrivateProfileString (szp, s_ignore, Buffer, s_telephon_ini);

                        // "Use pulse dialing"

                        cfgdlgline->pulse = (int) SendDlgItemMessage(hDlg, ID_PULSE,
                                                                                                                                                BM_GETCHECK, 0, 0);
                        _itoa(cfgdlgline->pulse, Buffer, 10);
                        WritePrivateProfileString (szp, s_pulse, Buffer, s_telephon_ini);

                        // Flush the ini file cache

                        WritePrivateProfileString (0, 0, 0, s_telephon_ini);

                        // Finally, send the appropriate notifications to TAPI
                        if (pline && pline->lpfnEventProc)
                        {
                                if (pline->dwAddressStates & LINEADDRESSSTATE_CAPSCHANGE)
                                        pline->lpfnEventProc (pline->htLine, 0, LINE_ADDRESSSTATE,
                                                                                         0, LINEADDRESSSTATE_CAPSCHANGE, 0);
                                if (pline->dwLineStates & LINEDEVSTATE_CAPSCHANGE)
                                        pline->lpfnEventProc (pline->htLine, 0, LINE_LINEDEVSTATE,
                                                                                         LINEDEVSTATE_CAPSCHANGE, 0, 0);
                        }

                        EndDialog (hDlg, 0);
                }
                break;

                case IDCANCEL:
                        EndDialog (hDlg, -1);
                        break;
          }
        }
        break;

        default:
                return FALSE;
  }

  return TRUE;
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

        GetModuleFileName (hInst, szModuleBuffer, sizeof (szModuleBuffer));
        _splitpath (szModuleBuffer, szTemp, szTemp, szFName, szTemp);
        wsprintf (szBuffer, "%s: %s\n\r", (LPSTR) szFName, (LPSTR) fmtBuffer);

        OutputDebugString (szBuffer);
}
#endif
