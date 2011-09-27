// window_enable_scope_test.cpp : Defines the entry point for the console application.
//

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <winstl.h>
#include <winstl_window_enable_scope.h>

winstl_ns_using(window_enable_scope)

namespace W
{
	struct Window
	{
		Window(HWND h)
			: m_h(h)
		{}

		HWND	m_h;
	};
}

////////////////////////////////////////////////////////////////////////////////
// If Koenig lookup is not supported, then we must wrap get_hwnd(Window &w)
// inside the winstl (stlsoft::winstl_project) namespace in order for it to be
// visible in the instantiation of window_enable_scope.


#ifdef __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT
namespace W
{

	inline HWND get_hwnd(Window &w)
	{
		return w.m_h;
	}

} // namespace W
#else
# ifndef _WINSTL_NO_NAMESPACE
#  ifdef _STLSOFT_NO_NAMESPACE
/* There is no stlsoft namespace, so must define ::winstl */
namespace winstl
{
#  else
/* Define stlsoft::winstl_project */

namespace stlsoft
{

namespace winstl_project
{

#  endif /* _STLSOFT_NO_NAMESPACE */
# endif /* !_WINSTL_NO_NAMESPACE */

	inline HWND get_hwnd(::W::Window &w)
	{
		return w.m_h;
	}

# ifndef _WINSTL_NO_NAMESPACE
#  ifdef _STLSOFT_NO_NAMESPACE
} // namespace winstl
#  else
} // namespace winstl_project
} // namespace stlsoft
#  endif /* _STLSOFT_NO_NAMESPACE */
# endif /* !_WINSTL_NO_NAMESPACE */
#endif /* __STLSOFT_CF_KOENIG_LOOKUP_SUPPORT */


////////////////////////////////////////////////////////////////////////////////

int main(int /* argc */, char* /* argv */[])
{
	HWND				_hwnd	=	::GetDesktopWindow();
	W::Window			hwnd(_hwnd);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
	window_enable_scope	scope(hwnd, true, false);
#else
	window_enable_scope	scope(get_hwnd(hwnd), true, false);
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

	return 0;
}
