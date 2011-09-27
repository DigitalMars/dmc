/*** 
*statbar.h
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
*
*Implementation Notes:
*  This file requires windows.h and ole2.h
*
*****************************************************************************/

class CStatBar : public IUnknown {
public:
    static CStatBar FAR* Create(HINSTANCE hinst, HWND hwndFrame);

    // IUnknown methods
    //
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
    STDMETHOD_(unsigned long, AddRef)(void);
    STDMETHOD_(unsigned long, Release)(void);

    // Introduced methods
    //
    void Show(void);
    inline void Move(void);
    inline void Update(void);

    inline int GetX(void);
    inline void SetX(int x);

    inline int GetY(void);
    inline void SetY(int y);

    inline int GetHeight(void);
    inline void SetHeight(int height);

    inline int GetWidth(void);
    inline void SetWidth(int width);

    //inline HFONT GetFont(void);
    void SetFont(HFONT hfont);

    //char FAR* GetText(void);
    inline void SetText(OLECHAR FAR* sz);

    void WMPaint(void);
    BOOL Register(HINSTANCE);

private:
    CStatBar();
    ~CStatBar();

    unsigned long	m_refs;

    HWND	m_hwnd;			// the status bar window handle

    int		m_x;			// x coordinate of upper left corner
    int		m_y;			// y coordinate of upper left corner
    int		m_height;
    int		m_width;

    HFONT	m_hfont;
    int		m_dyFont;		// font height
    int		m_dxFont;		// font width

    BSTR	m_bstrMsg;		// the status bar text

    static TCHAR FAR* m_szWndClass;
};

inline void
CStatBar::Move()
{
    MoveWindow(m_hwnd, m_x, m_y, m_width, m_height, TRUE);
}

inline void
CStatBar::Update()
{
    InvalidateRect(m_hwnd, NULL, TRUE);
    UpdateWindow(m_hwnd);
}

inline int
CStatBar::GetX()
{
    return m_x;
}

inline void
CStatBar::SetX(int x)
{
    m_x = x;
}

inline int
CStatBar::GetY(void)
{
    return m_y;
}

inline void
CStatBar::SetY(int y)
{
    m_y = y;
}

inline int
CStatBar::GetHeight(void)
{
    return m_height;
}

inline void
CStatBar::SetHeight(int height)
{
    m_height = height;
}

inline int
CStatBar::GetWidth(void)
{
    return m_width;
}

inline void
CStatBar::SetWidth(int width)
{
    m_width = width;
}

inline void
CStatBar::SetText(OLECHAR FAR* sz)
{
    SysFreeString(m_bstrMsg);
    m_bstrMsg = SysAllocString(sz);
}

extern "C" void
SBprintf(CStatBar FAR* psb, TCHAR FAR* szFmt, ...);
