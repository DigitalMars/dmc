/**********************************************************************

PROGRAM: SREC.C

PURPOSE: Generic template for a custom recognizer DLL.

TABS:    Every 4 columns.

COMMENTS:   SREC takes in raw pen data from the client application and
         accumulates it in its HRCinternal structure, which serves as
         the HRC object.  Input ends after a single stroke when the
         user lifts the pen from the tablet.

         The client application then calls ProcessHRC, which examines
         the stroke  data.  From the first and last points of the
         stroke, SREC calculates the nearest compass direction of the
         line formed by these endpoints.

         SREC then fills out a symbol graph structure with one of
         the following values:

            Value     Direction
            -------      -----------
            syvEast      right
            syvSouth     down
            syvWest      left
            syvNorth     up
            syvDot       point

         SREC stores results in an HRCRESULTinternal structure within
         the HRC.  Since this structure serves as the HRCRESULT object,
         calling DestroyHRC destroys both HRC and HRCRESULT.  (The
         function DestroyHRCRESULT simply returns HRC_OK.)


         Cross-references:
         ----------------

         SREC does little that isn't described in Chapter 8,
         "Writing a Recognizer."  Refer to that chapter for details
         about recognizers.  For descriptions of the requirements
         of a recognizer's exported functions, see also Chapter 10,
         "Pen API Functions."

         Other pertinent cross-references appear in function header
         blocks below.


         Application Requirements:
         ------------------------

         Any application that uses SREC must include PCM_PENUP
         in the dwPcm member of PCMINFO, as shown here:

         PCMINFO        pcminfo;
         .
         .
         .
         pcminfo.dwPcm = PCM_PENUP | (other flags)
         SendMessage( hWnd, PMSC_SETPCMINFO, (LPPCMINFO) &pcminfo );

CONTENTS:
         The source code in this file appears in three sections:

         1. REQUIRED FUNCTIONS   required exported functions
         2. OPTIONAL FUNCTIONS   optional exported functions
         3. INTERNAL FUNCTIONS   private to SREC (not exported)

**********************************************************************/

#define  NOPENAPPS
#define  NOPENDICT
#define  STRICT

#include <windows.h>
#include <penwin.h>
#include <string.h>
#include "main.h"

// Prototypes for internal and callback functions *********************

int            GetSYG( HRC hrc );
VOID           CalculateDirection( LPPOINT lpEndPoints, LPSYE lpSye );
VOID CALLBACK  TimerProc( HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime );
WORD WINAPI    WEP( int nParam );


// Macros *************************************************************

#define ABS(n)   ((n) < 0 ? -(n) : n)


// Constants **********************************************************

#define szID         "US Compass direction, single-stroke"


// Definitions of HRC and HRCRESULT objects ***************************

typedef struct                   // HRCRESULT object
{
   DWORD reserved;               // Reserve top DWORD
   SYG      syg;                 // Recognition results
} HRCRESULTinternal, FAR *LPHRCRESULTinternal;

typedef struct                   // HRC object
{
   DWORD          reserved;      // Reserve top DWORD
   HPENDATA       hpendata;      // Raw pen data to be recognized
   HREC           hrec;          // Module handle for this recognizer
   HRCRESULTinternal hrcresult;  // HRCRESULT structure
} HRCinternal, FAR *LPHRCinternal;

#define  lpHRC       ((LPHRCinternal) hrc)
#define  lpHRCRESULT    ((LPHRCRESULTinternal) hrcresult)


// Global variables ***************************************************

HINSTANCE   vhinst;                 // DLL instance handle
UINT        vfOutOfTime;            // Timing flag for ProcessHRC
SYE         vsye;                   // Symbol element with direction




/**********************************************************************

FUNCTION:   LibMain( hinst, wDataSeg, cbHeapSize, lpszCmdLine )

PURPOSE: Main DLL function, called by C start-up

RETURNS: TRUE

**********************************************************************/
int WINAPI  LibMain(
               HANDLE hinst,           // Instance handle
               WORD wDataSeg,          // Data segment
               WORD cbHeapSize,        // Size of heap
               LPSTR lpszCmdLine )     // Command line
{
   // Mention to prevent compiler warnings ....
   wDataSeg;
   lpszCmdLine;

   if (cbHeapSize)
      UnlockData( 0 );

   vhinst = hinst;               // Note instance handle for ProcessHRC
   return TRUE;
}



/**********************************************************************

FUNCTION:   WEP( nParam )

PURPOSE: DLL termination function.  WEP stands for Windows Exit
         Procedure, and is required in all Windows DLLs.

COMMENTS:   Kept in code segment CODE2, which is marked FIXED in the
         module definition file.  This is recommended practice for
         a DLL, which should keep its WEP routine continually
         resident in memory.

RETURNS: TRUE

**********************************************************************/
#pragma        alloc_text( CODE2, WEP )

WORD WINAPI WEP( int nParam )
{
   // Mention to prevent compiler warnings ....
   nParam;

   return TRUE;
}


///////////////////////////////////////////////////////////////////////
/*
**       Section 1:  REQUIRED FUNCTIONS
**
** The following functions are required for all recognizers
*/
///////////////////////////////////////////////////////////////////////



/**********************************************************************

FUNCTION:   ConfigRecognizer( uSubFunc, wParam, lParam )

PURPOSE: Configure recognizer for special options.

RETURNS: (depends on wParam)

COMMENTS:   A recognizer must export this function.  ConfigRecognizer
         is called when the recognizer is initialized or terminated,
         and when an application calls ConfigHREC to alter recognizer
         configuration.

         SREC provides no custom configuration itself other than
         returning its identification string.  Dummy case
         statements for various values of the uSubFunc parameter
         are included here to indicate how a custom recognizer
         should handle them.

**********************************************************************/
UINT WINAPI ConfigRecognizer(
                     UINT uSubFunc,    // Configuration subfunction
                     WPARAM wParam,
                     LPARAM lParam )
{
   UINT  iRet = TRUE;

   switch ( uSubFunc )
   {
      case WCR_INITRECOGNIZER:                  // Required
         // This subfunction is called when an application calls
         // InstallRecognizer. A recognizer normally relies on this event
         // to initialize itself. The LPARAM value is a pointer the the
         // user's name.  SREC does not require this, so returns without
         // taking any action.
         break;

      case WCR_CLOSERECOGNIZER:                 // Required
         // This subfunction performs final clean-up before Windows
         // unloads the recognizer.
         //
         // Since WCR_CLOSERECOGNIZER is received only when an application
         // explicitly calls UninstallRecognizer, the recognizer can take
         // liberties in this routine it cannot safely take in its WEP.
         // For example, when control reaches here, the client application
         // application is guaranteed to be active so the recognizer can
         // conceivably send or post a message to the client.  It cannot
         // reliably do the same thing from its WEP, however, since the
         // client may already have terminated when the WEP executes.
         //
         // The disadvantage of WCR_CLOSERECOGNIZER is that a recognizer
         // cannot be certain the subfunction will execute, since the
         // client may not call UninstallRecognizer before terminating.
         // Since WEP is guaranteed to execute when Windows unloads the
         // recognizer, essential clean-up duties such as unhooking
         // interrupts should be done in the WEP.
         //
         // Since SREC has no clean-up duties, it returns without taking
         // any action.
         break;

      case WCR_CONFIGDIALOG:
         break;

      case WCR_RCCHANGE:
         break;

      case WCR_RECOGNAME:
         _fstrncpy( (LPSTR)lParam, (LPCSTR)szID, (size_t)wParam );
         break;

      case WCR_TRAIN:
         iRet = TRAIN_NONE;         // SREC does not support training
         break;

      case WCR_DEFAULT:             // Incapable of being system default
      case WCR_QUERY:               // Does not support configuration dialog
      case WCR_QUERYLANGUAGE:       // Does not support any language
      case WCR_TRAINMAX:
      case WCR_TRAINDIRTY:
      case WCR_TRAINCUSTOM:
      case WCR_TRAINSAVE:
      case WCR_USERCHANGE:
      case WCR_SETALCPRIORITY:
      case WCR_SETANSISTATE:
      case WCR_GETALCPRIORITY:
      case WCR_GETANSISTATE:
         iRet = FALSE;
         break;

      case WCR_PWVERSION:
      case WCR_VERSION:
         iRet = 0x0002;          // Recognizer version 2.0
         break;

      default:
         iRet = FALSE;
         break;
   }
   return iRet;
}



/**********************************************************************

FUNCTION:   AddPenInputHRC( hrc, rgPnt, rgOem, fuOem, lpsi )

PURPOSE: Adds data to vhpendata as user writes

RETURNS: HRCR_OK if successful

COMMENTS:   A recognizer must export this function.  After application
         gets points from tablet via GetPenInput, it passes the
         point data to AddPenInputHRC.

         SREC stores stroke and point data in an HPENDATA block, the
         handle for which is contained in the HRC structure.  For a
         description of HPENDATA, see the section "The HPENDATA Object"
         in Chapter 4.

**********************************************************************/
int WINAPI  AddPenInputHRC( HRC hrc,
                     LPPOINT rgPnt,
                     LPVOID rgOem,
                     UINT fuOem,
                     LPSTROKEINFO lpsi )
{
   // Mention to prevent compiler warnings ....
   fuOem;

   if (hrc && AddPointsPenData( lpHRC->hpendata, rgPnt, rgOem, lpsi ))
      return HRCR_OK;
   else
      return HRCR_ERROR;
}



/**********************************************************************

FUNCTION:   AddPenDataHRC( hrc, hpndt )

PURPOSE: Adds given hpendata to vhpendata as user writes

RETURNS: HRCR_OK if successful

COMMENTS:   A recognizer must export this function.

         SREC stores stroke and point data in an HPENDATA block, the
         handle for which is contained in the HRC structure.  For a
         description of HPENDATA, see the section "The HPENDATA Object"
         in Chapter 4.

**********************************************************************/
int WINAPI  AddPenDataHRC( HRC hrc,
                     HPENDATA hpndt )
{
   if (hrc && (InsertPenData( lpHRC->hpendata, hpndt, IX_END ) == PDR_OK))
      return HRCR_OK;
   else
      return HRCR_ERROR;
}



/**********************************************************************

FUNCTION:   CreateCompatibleHRC( hrcTemplate, hrec )

PURPOSE: Creates a new HRC, optionally based on an existing HRC

RETURNS: Returns handle to new HRC if successful, otherwise NULL

COMMENTS:   A recognizer must export this function.

         CreateCompatibleHRC allows an application to modify the
         configurations used by a recognizer, such as word lists,
         coercion, and GUIDE structure.

         In SREC, this function allocates an HRCinternal structure
         in local heap, creates an HPENDATA block for the pen data,
         and returns a far pointer to the structure.  Since the
         HRC has no configurable elements, SREC ignores any
         template HRC provided in the first parameter.

**********************************************************************/
HRC WINAPI  CreateCompatibleHRC( HRC hrcTemplate, HREC hrec )
{
   HRC   hrc;

   // Mention to prevent compiler warnings ....
   hrcTemplate;

   hrc = (HRC) LocalAlloc( LPTR, sizeof( HRCinternal ) );
   if (hrc)
   {
      lpHRC->hrec  = hrec;
      lpHRC->hpendata = CreatePenData( NULL, 0, PDTS_STANDARDSCALE, 0 );
      if (lpHRC->hpendata)
         return (hrc);
   }

   LocalFree( (HLOCAL) LOWORD(hrc) );     // If error, free allocation
   return NULL;                           //   and return NULL
}



/**********************************************************************

FUNCTION:   DestroyHRC( hrc )

PURPOSE: Destroys an existing HRC object

RETURNS: Returns HRCR_OK if successful

COMMENTS:   A recognizer must export this function.

         In SREC, the HRC contains an HPENDATA handle.  This
         function first calls the Pen API function DestroyPendData
         to destroy the HPENDATA block, then frees the local memory
         occupied by the HRC structure.

**********************************************************************/
int WINAPI  DestroyHRC( HRC hrc )
{

   if (!hrc)                           // If no object,
      return HRCR_OK;                  //   exit

   DestroyPenData( lpHRC->hpendata );      // Else destroy its HPENDATA
   if (!LocalFree( (HLOCAL) LOWORD(hrc) )) //   and remove from heap
      return HRCR_OK;
   else
      return HRCR_ERROR;
}



/**********************************************************************

FUNCTION:   DestroyHRCRESULT( hrcresult )

PURPOSE: Destroys an existing HRCRESULT object

RETURNS: Returns HRCR_OK if successful

COMMENTS:   A recognizer must export this function.

         In SREC, an HRCRESULT is a pointer to an HRCRESULTinternal
         structure contained in the HRC.  Since destroying the HRC
         also destroys the HRCRESULT, this function returns HRCR_OK
         to satisfy the caller.

**********************************************************************/
int WINAPI  DestroyHRCRESULT( HRCRESULT hrcresult )
{
   // Mention to prevent compiler warnings ....
   hrcresult;

   return HRCR_OK;
}



/**********************************************************************

FUNCTION:   GetSymbolCountHRCRESULT( hrcresult )

PURPOSE: Returns the number of symbols in the recognition result

RETURNS: Returns symbol count if successful

COMMENTS:   A recognizer must export this function.

         SREC can make only one interpretation of a stroke.  Therefore
         this function simply returns 1.

**********************************************************************/
int WINAPI  GetSymbolCountHRCRESULT( HRCRESULT hrcresult )
{
   // Mention to prevent compiler warnings ....
   hrcresult;

   return 1;
}



/**********************************************************************

FUNCTION:   GetSymbolsHRCRESULT( hrcresult, iSyv, rgsyv, cSyv )

PURPOSE: Returns the symbol values from the recognition result

RETURNS: Returns number of symbols retrieved if successful

COMMENTS:   A recognizer must export this function.

         SREC generates only one symbol for a stroke.  This function
         copies the symbol value from HRCRESULTinternal.syg.lpsye->syv
         to the caller's buffer at rgsyv.

**********************************************************************/
int WINAPI  GetSymbolsHRCRESULT( HRCRESULT hrcresult, // Result object
                         UINT iSyv,          // Index
                         LPSYV rgsyv,        // Buffer
                         UINT cSyv )         // Buffer size
{
   // If size of rgsyv = 0, return zero
   if (!cSyv)
      return 0;

   // If index = 0 and result exists, copy it to rgsyv
   if (!iSyv && lpHRCRESULT->syg.cSye)
   {
      *rgsyv = (lpHRCRESULT->syg.lpsye->syv);
      return 1;
   }
   else
      return HRCR_ERROR;
}



/**********************************************************************

FUNCTION:   EndPenInputHRC( hrc )

PURPOSE: Halts pen collection for this input session

RETURNS: HRCR_OK

COMMENTS:   A recognizer must export this function.

         EndPenInputHRC signals the recognition engine that input has
         stopped and no more delayed strokes will be forthcoming.

         For some recognizers such as GRECO.DLL, an application must
         call EndPenInputHRC before getting results.  (DoDefaultPenInput
         calls EndPenInput internally.)  SREC does no on-the-fly
         recognition; therefore, this function simply returns HRCR_OK.

**********************************************************************/
int WINAPI  EndPenInputHRC( HRC hrc )
{
   // Mention to prevent compiler warnings ....
   hrc;

   return HRCR_OK;
}



/**********************************************************************

FUNCTION:   GetHRECFromHRC( hrc )

PURPOSE: Returns module handle attached to hrc

RETURNS: Module handle HREC

COMMENTS:   A recognizer must export this function.

         In SREC, the HRC object contains the module handle as
         member hrec.  GetHRECFromHRC simply passes the handle
         back to the caller.

**********************************************************************/
HREC WINAPI GetHRECFromHRC( HRC hrc )
{
   if (hrc)
      return (lpHRC->hrec);
   else
      return NULL;
}



/**********************************************************************

FUNCTION:   GetResultsHRC( hrc, uType, rghresult, cResults )

PURPOSE: Places the recognizer's results into HRCRESULT objects.
         Each object represents a separate guess by the recognizer.

RETURNS: Returns 1 as number of HRCRESULT objects filled.
         If rghresult is NULL, returns HCR_ERROR.

COMMENTS:   A recognizer must export this function.

         SREC makes only one guess; it therefore fills only the
         first HRCRESULT in the array and ignores the others.  The
         returned HRCRESULT points to the structure HRCRESULTinternal,
         which is contained in the HRC structure.

**********************************************************************/
int WINAPI  GetResultsHRC( HRC hrc,          // HRC handle
                     UINT uType,             // Result type
                     LPHRCRESULT rghresult,  // Array of HRCRESULTs
                     UINT cResult )          // Size of array
{
   // If requesting gesture or array size is zero, return zero
   if (uType == GRH_GESTURE || !cResult)
      return 0;

   if (rghresult)
   {
      rghresult[0] = (HRCRESULT) &(lpHRC->hrcresult);
      return 1;
   }
   else
      return HRCR_ERROR;
}



/**********************************************************************

FUNCTION:   ProcessHRC( hrc, dwTimeMax )

PURPOSE: Initiates recognition of the pen data in vhpendata

RETURNS: Returns HRCR_OK if successful

COMMENTS:   A recognizer must export this function.

         SREC calls the internal routine GetSYG which in turn calls
         CalculateDirection.  The latter routine makes simple calculations
         that easily finish in the allotted time.

         However, a recognizer with more demanding abilities must
         monitor elapsed time to ensure it does not exceed the limit
         specified in dwTimeMax.  To demonstrate one way to monitor
         time, ProcessHRC calls SetTimer when dwTimeMax = PH_MIN or
         PH_DEFAULT with time-out periods of 50 milliseconds and 200
         milliseconds, respectively.  If the time-out period elapses
         before GetSYG finishes, the callback function TimerProc
         sets an out-of-time flag.  If GetSYG finds this flag set, it
         immediately aborts with a return value of HRCR_INCOMPLETE.

**********************************************************************/
int WINAPI  ProcessHRC( HRC hrc, DWORD dwTimeMax )
{
   UINT     idTimer, uTimeout;
   int         iRet;

   vfOutOfTime = FALSE;             // Initialize time-out flag

   if (dwTimeMax != PH_MAX)
   {
      uTimeout  = (dwTimeMax == PH_MIN) ? 50 : 200;
      idTimer   = SetTimer( NULL, NULL, uTimeout, (TIMERPROC) *TimerProc );
      iRet    = GetSYG( hrc );
      KillTimer( NULL, idTimer );
   }
   else
      iRet = GetSYG( hrc );

   return (iRet);
}



/**********************************************************************

   Callback function used when ProcessHRC called with PH_MIN or PH_DEFAULT.
   This function simply raises a global flag to inform GetSYG the specified
   time has elapsed and it should quit immediately.  In this case, GetSYG
   returns HRCR_INCOMPLETE to inform the application the time-out occurred
   before recognition could finish.

**********************************************************************/
VOID CALLBACK  TimerProc( HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime )
{
   // Mention to prevent compiler warnings ....
   hwnd;
   msg;
   idTimer;
   dwTime;

   vfOutOfTime = TRUE;
   return;
}



/**********************************************************************

FUNCTION:   SetGuideHRC( hrc, lpguide, nFirstVisible )

PURPOSE: Sets a guide structure in an HRC

COMMENTS:   A recognizer must export this function.  If the recognizer
         does not support box guides, it should return HRCR_OK
         only if the lpguide parameter is zero or if all fields in
         the GUIDE structure are zero.  Either case indicates the
         caller wants free-form input without guides.

RETURNS: HRCR_OK is success

**********************************************************************/
int WINAPI  SetGuideHRC(
                  HRC hrc,
                  LPGUIDE lpguide,
                  UINT nFirstVisible )

{
   // Mention to prevent compiler warnings ....
   nFirstVisible;
   hrc;

   if (lpguide           &&
      lpguide->xOrigin  || lpguide->yOrigin  ||
      lpguide->cxBox    || lpguide->cyBox    ||
      lpguide->cxBase   || lpguide->cyBase   ||
      lpguide->cHorzBox || lpguide->cVertBox || lpguide->cyMid)
      return HRCR_INVALIDGUIDE;
   else
      return HRCR_OK;
}



///////////////////////////////////////////////////////////////////////
/*
**       Section 2:  OPTIONAL FUNCTIONS
**
** The following functions are optional for a recognizer.
**
** Since SREC serves as a template for a custom recognizer,
** some functions are only stub versions, returning the
** values FALSE or HRCR_UNSUPPORTED.
*/
///////////////////////////////////////////////////////////////////////



/**********************************************************************

FUNCTION:   CreatePenDataHRC( hrc )

PURPOSE: Creates and fills an HPENDATA object from the given HRC

RETURNS: Returns handle to new HPENDATA if successful, otherwise NULL

COMMENTS:   In SREC, the HRC contains an HPENDATA handle.  This function
         clones the internal HPENDATA and returns the new handle.

**********************************************************************/
HPENDATA WINAPI   CreatePenDataHRC( HRC hrc )
{
   if (hrc)
      return (DuplicatePenData( lpHRC->hpendata, 0 ));
   else
      return NULL;
}



/**********************************************************************

FUNCTION:   GetMaxResultsHRC( hrc )

PURPOSE: Gets the maximum number of guesses the recognizer will make
         before terminating

RETURNS: Returns number of guesses

COMMENTS:   SREC makes only one guess, so simply returns 1.

         The maximum number of guesses for a recognizer is set with
         the SetMaxResultsHRC function.

**********************************************************************/
int WINAPI  GetMaxResultsHRC( HRC hrc )
{
   if (hrc)
      return 1;
   else
      return HRCR_ERROR;
}



/**********************************************************************

FUNCTION:   SetMaxResultsHRC( hrc, cMaxResults )

PURPOSE: Sets the maximum number of guesses the recognizer can make
         before terminating

RETURNS: Returns HRCR_OK if successful

COMMENTS:   SREC can make only one guess, so returns HRCR_UNSUPPORTED

**********************************************************************/
int WINAPI  SetMaxResultsHRC( HRC hrc, UINT cMaxResults )
{
   // Mention to prevent compiler warnings ....
   hrc;
   cMaxResults;

   return HRCR_UNSUPPORTED;
}



/**********************************************************************

FUNCTION:   GetHotspotsHRCRESULT( hrcresult, iSyv, rgPnt, iPnt )

PURPOSE: Gets the hotspots of the recognized symbol

RETURNS: Returns count if successful

COMMENTS:   SREC assumes two hotpots, which are the first and last
         points of the stroke.

**********************************************************************/
int WINAPI  GetHotspotsHRCRESULT(
               HRCRESULT hrcresult, // Results
               UINT iSyv,           // Symbol index
               LPPOINT rgPnt,       // Buffer for receiving hotspots
               UINT iPnt )          // Size of buffer in POINTs
{
   int i;

   //
   // Make sure (a) recognition result exists; (b) caller has requested
   // only the first symbol; (c) buffer exists; and (d) buffer can hold
   // at least two hotspots.
   //
   if (!hrcresult  ||  iSyv  ||  !rgPnt  ||  iPnt < 2)
      return HRCR_ERROR;

   for (i = 0; i < 2; i++, rgPnt++)
   {
      rgPnt->x = lpHRCRESULT->syg.rgpntHotSpots[i].x;
      rgPnt->y = lpHRCRESULT->syg.rgpntHotSpots[i].y;
   }

   return (lpHRCRESULT->syg.cHotSpot);
}



/**********************************************************************

FUNCTION:   TrainHREC( hrec, lpsyv, hpendata, uConflict )

PURPOSE: Perform context-independent training

RETURNS: HRCR_OK if ink in hpendata has been successfully trained

COMMENTS:   SREC does not support training

**********************************************************************/
BOOL WINAPI TrainHREC(
                  HREC hrec,        // Recognizer handle
                  LPSYV lpsyv,      // Ptr to symbol values
                  UINT cSyv,        // Count of symbol values
                  HPENDATA hpendata,// Pendata handle
                  UINT uConflict )  // How to handle conflicts
{
   // Mention to prevent compiler warnings ....
   hrec;
   lpsyv;
   cSyv;
   hpendata;
   uConflict;

   return HRCR_UNSUPPORTED;
}



///////////////////////////////////////////////////////////////////////
/*
**       Section 3:  INTERNAL FUNCTIONS
**
** The following functions are internal to SREC.
*/
///////////////////////////////////////////////////////////////////////



/**********************************************************************

FUNCTION:   GetSYG( hrc )

PURPOSE: Internal function that fills the SYG structure contained
         in the HRC (which points to an HRCinternal structure)

RETURNS: HRCR_COMPLETE if SYG structure correctly updated

COMMENTS:   GetSYG monitors the time-out flag vfOutOfTime.  If the
         TimerProc callback function set up by ProcessHRC sets the
         flag before GetSYG finishes -- highly unlikely -- GetSYG
         returns the value HRCR_INCOMPLETE.

**********************************************************************/
int      GetSYG( HRC hrc )
{
   POINT    rgpntEndPoints[2];
   UINT     i, iPoint;           // Loop counters
   DWORD    dwcPoint;            // Number of points in stroke


   //
   // Check for empty buffers
   //
   if (!hrc || !lpHRC->hpendata)
      return HRCR_NORESULTS;

   //
   // Get number of points in first (and only) stroke.  This tells us
   // the index number of the last point in stroke.
   //
   if (GetStrokeAttributes( lpHRC->hpendata, 0,
                      (LPDWORD) &dwcPoint, GSA_SIZE ) != PDR_OK)
      return HRCR_ERROR;

   //
   // Get stroke endpoints.  These are also the hotpoints.
   //
   for (i = 0, iPoint = 0; i < 2; i++, iPoint = LOWORD(dwcPoint) - 1)
   {
      ExtractPenDataPoints( lpHRC->hpendata, 0, iPoint, 1,
                       (LPPOINT) &rgpntEndPoints[i], NULL, 0 );
      lpHRC->hrcresult.syg.rgpntHotSpots[i].x = rgpntEndPoints[i].x;
      lpHRC->hrcresult.syg.rgpntHotSpots[i].y = rgpntEndPoints[i].y;
   }

   //
   // Build symbol graph
   //
   lpHRC->hrcresult.syg.cHotSpot  = 2;
   lpHRC->hrcresult.syg.lRecogVal = 0L;   // Not used

   if (vfOutOfTime)                 // If time-out,
      return HRCR_INCOMPLETE;       //   quit now

   CalculateDirection( rgpntEndPoints, &vsye );
   lpHRC->hrcresult.syg.lpsye = (LPSYE) &vsye;
   lpHRC->hrcresult.syg.cSye  = 1;

   return HRCR_COMPLETE;
}



/**********************************************************************

FUNCTION:   CalculateDirection( lpEndPoints, lpSye )

PURPOSE: Private function to calculate the closest compass
         direction of the line defined by the first and last
         points of a stroke.

RETURNS: nothing

COMMENTS:   The created symbol element is composed of a
         recognizer-specific symbol value (SYV), confidence level,
         and recognizer value.

         This algorithm does not take into account the tablet
         aspect ratio.

**********************************************************************/
VOID  CalculateDirection(
                     LPPOINT lpEndPoints, // Ptr to 2 POINT structs
                     LPSYE   lpSye )         // Ptr to SYE structure
{
   int  dx;
   int  dy;
   BOOL fIsEastward;
   BOOL fIsSouthward;
   BOOL fIsHoriz;

   dx = (lpEndPoints+1)->x - lpEndPoints->x;
   dy = (lpEndPoints+1)->y - lpEndPoints->y;

   fIsEastward  = dx > 0 ? TRUE : FALSE;
   fIsSouthward = dy > 0 ? TRUE : FALSE;
   fIsHoriz     = ABS(dx) > ABS(dy) ? TRUE : FALSE;

   if (fIsHoriz)
      lpSye->syv = fIsEastward ? syvEast : syvWest;
   else
      lpSye->syv = fIsSouthward ? syvSouth : syvNorth;

   if (dx == 0 && dy == 0)
      lpSye->syv = syvDot;

   lpSye->cl        = 100;             // Set confidence level
   lpSye->lRecogVal = 0L;              // Not used
}
