#ifndef STLPORT_PREFIX_H
# define STLPORT_PREFIX_H

# define __BUILDING_STLPORT 1

# if defined (_WIN32) || defined (WIN32)
#  ifdef __cplusplus
#   define WIN32_LEAN_AND_MEAN
#   define NOSERVICE
#  endif
#  if !(defined (__CYGWIN__) || defined(_WIN32_WCE))
#   define _STLP_REAL_LOCALE_IMPLEMENTED
#  endif
# endif

#   undef _STLP_NO_FORCE_INSTANTIATE

/* Please add extra compilation switches for particular compilers here */

# include <stl/_config.h>

# if defined (_STLP_USE_TEMPLATE_EXPORT) && defined (_STLP_USE_DECLSPEC) && ! defined (_STLP_EXPOSE_GLOBALS_IMPLEMENTATION)
#  define _STLP_EXPOSE_GLOBALS_IMPLEMENTATION
# endif

# ifdef __cplusplus

# include <ctime>
# if defined (_STLP_USE_NAMESPACES) && ! defined (_STLP_VENDOR_GLOBAL_CSTD)
using _STLP_VENDOR_CSTD::time_t;
# endif

// This section is only for compilers that support precompiled headers !
// Currently there are : Visual C++
// Please remember to make sure to turn on precompiled header option in
// the platform makefile when addinf compilers to this list, otherwise 
// this will result in worse build performance.

# if defined (_STLP_MSVC) || defined (__ICL)
# ifndef _STLP_USE_DECLSPEC
# include <climits>
# include <cmath>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <new>
# include <exception>
# include <functional>
# include <memory>
# include <utility>
# include <limits>
# include <algorithm>
# include <iosfwd>
# include <ios>

# include <stdexcept>
# include <string>
// # include <locale>

# ifdef _STLP_MSVC
#  pragma hdrstop
# endif

# endif

# endif /* precompiler headers */

# if defined (_STLP_FUNCTION_TMPL_PARTIAL_ORDER) || defined (__BORLANDC__)
#  define _STLP_OPERATOR_SPEC _STLP_DECLSPEC 
# else
#  define _STLP_OPERATOR_SPEC _STLP_TEMPLATE_NULL _STLP_DECLSPEC
# endif

# endif /* __cplusplus */

#endif /* PREFIX */

