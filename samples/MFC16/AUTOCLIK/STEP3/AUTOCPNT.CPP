// autocpnt.cpp : implementation file
//

#include "stdafx.h"
#include "autoclik.h"
#include "autocpnt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClikPoint

IMPLEMENT_DYNCREATE(CClikPoint, CCmdTarget)

CClikPoint::CClikPoint()
{
	EnableAutomation();

	// To keep the application running as long as an OLE automation
	//  object is active, the constructor calls AfxOleLockApp.

	AfxOleLockApp();
}

CClikPoint::~CClikPoint()
{
	// To terminate the application when all objects created with
	//  with OLE automation, the destructor calls AfxOleUnlockApp.

	AfxOleUnlockApp();
}

void CClikPoint::OnFinalRelease()
{
	// When the last reference for an automation object is released
	//  OnFinalRelease is called.  This implementation deletes the
	//  object.  Add additional cleanup required for your object before
	//  deleting it from memory.

	delete this;
}

BEGIN_MESSAGE_MAP(CClikPoint, CCmdTarget)
	//{{AFX_MSG_MAP(CClikPoint)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CClikPoint, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CClikPoint)
	DISP_PROPERTY(CClikPoint, "x", m_x, VT_I2)
	DISP_PROPERTY(CClikPoint, "y", m_y, VT_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClikPoint message handlers
