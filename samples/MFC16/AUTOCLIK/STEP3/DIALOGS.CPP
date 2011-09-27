// dialogs.cpp : implementation file
//

#include "stdafx.h"
#include "autoclik.h"
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeText dialog


CChangeText::CChangeText(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeText)
	m_str = "";
	//}}AFX_DATA_INIT
}

void CChangeText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeText)
	DDX_Text(pDX, IDC_EDIT1, m_str);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChangeText, CDialog)
	//{{AFX_MSG_MAP(CChangeText)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChangeText message handlers
