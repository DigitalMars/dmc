/***
*spoly.h - Application-wide definitions
*
*  Copyright (C) 1992-93, Microsoft Corporation.  All Rights Reserved.
*
*Purpose:
*
*Implementation Notes:
*
*****************************************************************************/

#include "hostenv.h"
#include "resource.h"
#include "clsid.h"

#if defined(_MAC)
# define PASCAL pascal
# define STRSTR strstr
#elif defined(WIN32)
# include "statbar.h"
# define STRSTR strstr
#else /* WIN16 */
# include "statbar.h"
# define STRSTR _fstrstr
#endif

#ifdef _MAC
# define UNUSED(X) ((void)(void*)&(X))
#else
# define UNUSED(X) (X)
#endif

#define DIM(X) (sizeof(X) / sizeof(X[0]))

extern "C" void Assert(int, char FAR*, int, char FAR*);
#define ASSERT(X) Assert(X, __FILE__, __LINE__, NULL)
#define ASSERTSZ(X, MSG) Assert(X, __FILE__, __LINE__, MSG)



// Description of a single named parameter.
//
typedef struct tagPARAMDESC {
	char FAR* szName;           // parameter name
} PARAMDESC;

// Description of a single member.
//
typedef struct tagMEMBERDESC {
	char FAR* szName;           // member name
	DISPID id;                  // member id
	PARAMDESC FAR* rgpd;        // ptr to array of PARAMDESCs
	unsigned int cParams;               // number of parameters
} MEMBERDESC;

STDAPI
SPolyGetIDsOfNames(
	MEMBERDESC FAR* pmd,
	unsigned int cMethods,
	char FAR* FAR* rgszNames,
	unsigned int cNames,
	LCID lcid,
	DISPID FAR* rgid);


STDAPI InitOle();
STDAPI UninitOle();
