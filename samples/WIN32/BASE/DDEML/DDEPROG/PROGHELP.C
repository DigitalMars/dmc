/*****************************************************************************\
* PROGHELP.C
*
* Main module to implement a simple DDEML service DLL that makes interaction
* with progman simple.
\*****************************************************************************/

#include <windows.h>
#include <ddeml.h>
#include <string.h>

typedef struct {
    HCONV hConv;
    DWORD idInst;
} THREADINFO;

DWORD TlsIndex;

/*
 * This is the default DDEML callback function for ProgHelp
 */
HDDEDATA CALLBACK ProgHelpDdeCallback(
UINT wType,
UINT wFmt,
HCONV hConv,
HSZ hsz1,
HSZ hsz2,
HDDEDATA hData,
DWORD dwData1,
DWORD dwData2)
{
    return(0);
}


/*
 * This is the DLL init routine called by the system at attach/detatch time.
 *
 * We allocate a Tls for each thread that calls this DLL so that their state
 * can be tracked independently.
 */
BOOLEAN APIENTRY DllMain(
IN PVOID hmod,
ULONG Reason,
IN PCONTEXT pctx OPTIONAL)
{
    THREADINFO *pti;
    UNREFERENCED_PARAMETER(hmod);
    UNREFERENCED_PARAMETER(pctx);

    switch (Reason) {
    case DLL_PROCESS_ATTACH:
        TlsIndex = TlsAlloc();

    case DLL_THREAD_ATTACH:
        pti = (THREADINFO *)LocalAlloc(LPTR, sizeof(THREADINFO));
        if (!pti) {
            return(FALSE);
        }
        TlsSetValue(TlsIndex, pti);
        break;

    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        pti = TlsGetValue(TlsIndex);
        LocalFree(pti);
        if (Reason == DLL_PROCESS_DETACH) {
            TlsFree(TlsIndex);
        }
        break;
    }
    return TRUE;
}

/*
 * An application must call this API first to establish a conversation
 * with progman.  DDEML initialization is done automatically if needed.
 */
BOOL ConnectToProgmanA()
{
    THREADINFO *pti;
    HSZ hszProgman;

    pti = TlsGetValue(TlsIndex);

    if (!pti->idInst) {
        if (DdeInitializeA(&(pti->idInst), ProgHelpDdeCallback,
                APPCMD_CLIENTONLY | CBF_SKIP_ALLNOTIFICATIONS,
                0L) != DMLERR_NO_ERROR) {
            return(FALSE);
        }
    }

    if (pti->hConv == 0) {
        hszProgman = DdeCreateStringHandleA(pti->idInst, "Progman", 0);
        pti->hConv = DdeConnect(pti->idInst, hszProgman, hszProgman, NULL);
    }
    return(pti->hConv != 0);
}



BOOL ConnectToProgmanW()
{
    THREADINFO *pti;
    HSZ hszProgman;

    pti = TlsGetValue(TlsIndex);

    if (!pti->idInst) {
        if (DdeInitializeW(&(pti->idInst), ProgHelpDdeCallback,
                APPCMD_CLIENTONLY | CBF_SKIP_ALLNOTIFICATIONS,
                0L) != DMLERR_NO_ERROR) {
            return(FALSE);
        }
    }

    if (pti->hConv == 0) {
        hszProgman = DdeCreateStringHandleA(pti->idInst, "Progman", 0);
        pti->hConv = DdeConnect(pti->idInst, hszProgman, hszProgman, NULL);
    }
    return(pti->hConv != 0);
}



/*
 * This API should be called when operations with progman are complete.
 * We shutdown DDEML here as well so we don't have to deal with it
 * at DLL detatch time.
 */
BOOL DisconnectFromProgman()
{
    THREADINFO *pti;

    pti = TlsGetValue(TlsIndex);

    if (!pti->idInst) {
        return(TRUE);
    }

    if (pti->hConv) {
        DdeDisconnect(pti->hConv);
        pti->hConv = 0;
    }

    DdeUninitialize(pti->idInst);
    pti->idInst = 0;
    return(TRUE);
}


/*
 * This API sends an ASCII execute string to progman.  It immediately returns
 * and the results of the execute are not known.  Calling applications should
 * be aware that these transactions are done asynchronously.  This allows
 * this API to work under modal-loop conditions.  DisconnectFromProgman()
 * will not succeed until all transactions are complete.
 */
BOOL ProgmanExecuteStringA(
LPSTR pszExec)
{
    THREADINFO *pti;
    DWORD dwResult;

    pti = TlsGetValue(TlsIndex);

    if (!pti->idInst || !pti->hConv) {
        return(FALSE);
    }
    if (!DdeClientTransaction((PBYTE)pszExec, strlen(pszExec) + 1,
            pti->hConv, 0, 0, XTYP_EXECUTE, TIMEOUT_ASYNC, &dwResult)) {
        return(FALSE);
    }
    return(TRUE);
}



BOOL ProgmanExecuteStringW(
LPWSTR pszExec)
{
    THREADINFO *pti;
    DWORD dwResult;

    pti = TlsGetValue(TlsIndex);

    if (!pti->idInst || !pti->hConv) {
        return(FALSE);
    }
    if (!DdeClientTransaction((PBYTE)pszExec,
            (wcslen(pszExec) + 1)  * sizeof(WCHAR),
            pti->hConv, 0, 0, XTYP_EXECUTE, TIMEOUT_ASYNC, &dwResult)) {
        return(FALSE);
    }
    return(TRUE);
}



