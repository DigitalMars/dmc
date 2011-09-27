// Copyright (C) 1986-2003 by Digital Mars
// www.digitalmars.com

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __TYPEINFO_H
#define __TYPEINFO_H 1

#ifndef __cplusplus
#error	Use C++ compiler for typeinfo.h
#endif

extern "C++" {

#if _WIN32

// The .\ is needed to avoid confusion with \dm\stlport\stlport\typeinfo
#include	".\typeinfo"

#else

class Type_info
{
    public:
	void *pdata;

    private:
	__cdecl Type_info(const Type_info&);
	Type_info& __cdecl operator=(const Type_info&);

    public:
	virtual __cdecl ~Type_info();

	int __cdecl operator==(const Type_info&) const;
	int __cdecl operator!=(const Type_info&) const;
	int __cdecl before(const Type_info&)
#if _WIN32 || DOS386
 const
#endif
 ;
	const char * __cdecl name() const;
};

typedef Type_info type_info;	// for C++ 98


class Bad_cast { };

typedef Bad_cast bad_cast;	// for C++ 98

namespace std
{
    using ::type_info;
    using ::bad_cast;

    class bad_typeid
    {
    };
}

#endif

};

#endif
