/***
*tibrowse.h
*
*  Copyright (C) 1992-93, Microsoft Corporation.  All Rights Reserved.
*
*Purpose:
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
# include <lists.h>
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
# include <commdlg.h>
# include <ole2.h>
# include <dispatch.h>

#else /* WIN16 */

# include <windows.h>
# include <commdlg.h>
# include <ole2.h>
# include <olenls.h>
# include <dispatch.h>

#endif

#include "resource.h"

#ifdef WIN32
# define EXPORT
#else
# define EXPORT _export
#endif

#ifdef _MAC
# define UNUSED(X) ((void)(void*)&(X))
#else
# define UNUSED(X) (X)
#endif

#define DIM(X) (sizeof(X) / sizeof(X[0]))

void AssertFail(char FAR*, int);
#define ASSERT(X) if (!(X)) { AssertFail(__FILE__, __LINE__); } else {}

#define CHECKRESULT(X) \
  {HRESULT hresult = (X); \
	if(hresult != NOERROR && FAILED(GetScode(hresult))) MethodError(hresult); }


HRESULT OpenFile(BSTR FAR* pbstrFileName);
void    OpenTypeLib(char FAR*);
void    SetSelectedType(unsigned long);
void    FillMemberList(ITypeInfo FAR *, TYPEATTR FAR *, int cltid);
void    SetSelectedMember(unsigned long);
void    SetSelectedParam(unsigned long dwIndex);
void    UpdateMemberInfo(MEMBERID memid);
void    AssertFail(char FAR*, int);
void    MethodError(HRESULT hresult);
void    Cleanup(void);
void    MemFree(void FAR*);
