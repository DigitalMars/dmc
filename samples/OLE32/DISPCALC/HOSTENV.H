/*** 
*hostenv.h
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  Generic host specific includes.
*
*Implementation Notes:
*
*****************************************************************************/

#if defined(_MAC)

#if defined(_PPCMAC)
#pragma data_seg("_FAR_DATA")
#pragma data_seg( )
#define MAXLONG 0x7fffffff
#define EventHandlerProcPtr AEEventHandlerUPP
#else //_PPCMAC
#define  GetMenuItemText(mApple,menuItem,daName)  GetItem(mApple,menuItem,daName)
#endif //_PPCMAC

#endif //_MAC

#if defined(_MAC)

#if defined(_MSC_VER)

# include <values.h>
# include <types.h>
# include <string.h>
# include <quickdra.h>
# include <fonts.h>
# include <events.h>
# include <resource.h>
# include <menus.h>
# include <textedit.h>
# include <dialogs.h>
# include <desk.h>
# include <toolutil.h>
# include <memory.h>
# include <files.h>
# include <osutils.h>
# include <osevents.h>
# include <diskinit.h>
# include <packages.h>
# include <traps.h>
# include <AppleEve.h>

#else //_MSC_VER

# include <values.h>
# include <types.h>
# include <strings.h>
# include <quickdraw.h>
# include <fonts.h>
# include <events.h>
# include <resources.h>
# include <windows.h>
# include <menus.h>
# include <textedit.h>
# include <dialogs.h>
# include <desk.h>
# include <toolutils.h>
# include <memory.h>
# include <files.h>
# include <osutils.h>
# include <osevents.h>
# include <diskinit.h>
# include <packages.h>
# include <traps.h>
# include <AppleEvents.h>

#endif //_MSC_VER

# include <ole2.h>
# include <olenls.h>
# include <dispatch.h>

# define TCHAR			char
# define TSTR(str)		str	     
# define STRING(str)	        (str)
# define WIDESTRING(str)        (str)

#elif defined(WIN32)

# include <windows.h>

# if defined(UNICODE)
    #define TCHAR		WCHAR
    #define TSTR(str)		L##str
    #define STRING(str)	        (str)	    
    #define WIDESTRING(str)	(str)	    	    
# else
    #define TCHAR		char
    #define TSTR(str)		str	
    #define STRING(str)	        AnsiString(str)
    #define WIDESTRING(str)	WideString(str)	    	    
    extern "C" char FAR* AnsiString(OLECHAR FAR* strIn);
    extern "C" OLECHAR FAR* WideString(char FAR* strIn);
# endif


#else /* WIN16 */

# include <windows.h>
# include <ole2.h>
# include <olenls.h>
# include <dispatch.h>

# define TCHAR			char
# define TSTR(str)		str	     
# define STRING(str)	        (str)
# define WIDESTRING(str)        (str)
#endif

