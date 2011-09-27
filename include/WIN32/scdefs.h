/* Copyright (C) 1994-2001 by Digital Mars. $Revision: 4 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#if _WIN32

#ifndef _X86_
#define _X86_ 1
#endif

#ifndef WIN32
#define WIN32 1
#endif

#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE 1
#endif


#endif
