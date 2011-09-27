// ccvuiDlg.cpp : implementation file
//
// Updated: 30th May 2004
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ccvui.h"
#include "ccvuiDlg.h"

////////////////////////////////////////////////////////////////////////////

typedef enum_simple_sequence<   IEnumGUID,
                                GUID,
                                GUID_policy,
                                GUID const &,
                                input_cloning_policy<IEnumGUID>,
                                10>     enum_guid_t;

// Value policy for CATEGORYINFO
struct CATEGORYINFO_policy
{
    public:
        typedef CATEGORYINFO        value_type;

    public:
        /// Initialises an instance
        static void init(value_type *)
        {}
        /// Initialises an instance from another
        static void copy(value_type *dest, value_type const *src)
        {
            *dest = *src;
        }
        /// Releases an instance
        static void clear(value_type *)
        {}
};

typedef enum_simple_sequence<   IEnumCATEGORYINFO,
                                CATEGORYINFO,
                                CATEGORYINFO_policy,
                                CATEGORYINFO const &,
                                input_cloning_policy<IEnumCATEGORYINFO>,
                                10>     enum_catinfo_t;

////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnButtonStlsoft();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_BUTTON_STLSOFT, OnButtonStlsoft)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCcvDlg dialog

CCcvDlg::CCcvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCcvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCcvDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCcvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCcvDlg)
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCcvDlg, CDialog)
	//{{AFX_MSG_MAP(CCcvDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCcvDlg message handlers

BOOL CCcvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    this->SetWindowText("COM Category Viewer");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCcvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCcvDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCcvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCcvDlg::OnQuery()
{
    HTREEITEM	hNode;
	HTREEITEM	hImp;
//	HTREEITEM	hReq;

    CHAR		buff[500];

    m_tree.DeleteAllItems();

	window_enable_scope	scopeQuery(GetDlgItem(IDC_QUERY)->GetSafeHwnd());
    CWaitCursor			scopeCursor;

    // ICatRegister
    ICatInformation	*pci;

    HRESULT	hr = ::CoCreateInstance(CLSID_StdComponentCategoriesMgr,
									NULL,
                                    CLSCTX_ALL,
									IID_ICatInformation,
									(void**)&pci);

	if(SUCCEEDED(hr))
	{
		IEnumCATEGORYINFO	*penci;
		LCID				lcid = ::GetUserDefaultLCID();

        hr = pci->EnumCategories(lcid, &penci);

		if(SUCCEEDED(hr))
		{
			enum_catinfo_t				categories(penci, false); // false eats the reference
			enum_catinfo_t::iterator	begin = categories.begin();
			enum_catinfo_t::iterator	end	  = categories.end();

			for(; begin != end; ++begin)
			{
				CATEGORYINFO const	&info = *begin;
				IEnumCLSID			*penImpl;
//				IEnumCLSID			*penReq;
				CATID				catids[1];

				catids[0] = info.catid;

				//////////////////////////////////
				// Category

                wsprintf(	buff
						,	"%s: \"%s\"\0"
						,	static_cast<char const *>((c_str_ptr_a(info.catid)))
						,	static_cast<char const *>(w2a(info.szDescription)));

                hNode = m_tree.InsertItem(buff);

				UpdateWindow(); // Update the display, since enum of classes takes a long time

				//////////////////////////////////
				// Implementing classes

				hImp = m_tree.InsertItem("Implementing Classes", hNode);

				hr = pci->EnumClassesOfCategories(1, catids, 0, NULL, &penImpl);

				if(SUCCEEDED(hr))
				{
					enum_guid_t				classes(penImpl, false); // false eats the reference
					enum_guid_t::iterator	begin	=	classes.begin();
					enum_guid_t::iterator	end		=	classes.end();

					for(; begin != end; ++begin)
					{
                        m_tree.InsertItem(c_str_ptr_a(*begin), hImp);
					}
				}

				//////////////////////////////////
				// Requiring classes

#if 0
				hReq = m_tree.InsertItem("Requring Classes");

                // This does not work. Yet to figure out why.
				hr = pci->EnumClassesOfCategories(0, NULL, 1, catids, &penReq);

				if(SUCCEEDED(hr))
				{
					enum_guid_t				classes(penReq, false); // false eats the reference
					enum_guid_t::iterator	begin	=	classes.begin();
					enum_guid_t::iterator	end		=	classes.end();

					for(; begin != end; ++begin)
					{
                        m_tree.InsertItem(c_str_ptr(*begin), hReq);
					}
				}
#endif /* 0 */
			}
		}

		pci->Release();
	}

}

void CCcvDlg::OnAbout()
{
    CAboutDlg().DoModal();
}

void CAboutDlg::OnButtonStlsoft()
{
    ShellExecute(NULL, "open", "http://www.stlsoft.org/", NULL, NULL, SW_NORMAL);
}
