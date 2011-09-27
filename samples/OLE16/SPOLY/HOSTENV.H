/***
*hostenv.h
*
*  Copyright (C) 1992-93, Microsoft Corporation.  All Rights Reserved.
*
*Purpose:
*  Generic host specific includes.
*
*Implementation Notes:
*
*****************************************************************************/

#if defined(_MAC)

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

# include <LibraryManager.h>

# include <ole2.h>
# include <olenls.h>
# include <dispatch.h>

#elif defined(WIN32)

# include <windows.h>
# include <ole2.h>
# include <dispatch.h>

#else /* WIN16 */

# include <windows.h>
# include <ole2.h>
# include <olenls.h>
# include <dispatch.h>

#endif
