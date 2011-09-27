//_ typeinfo.cpp
// RTL support for Type_info

#include <typeinfo.h>
#include <string.h>

/***********************************
 */

__cdecl Type_info::Type_info(const Type_info& ti)
{
    pdata = ti.pdata;
}

Type_info& __cdecl Type_info::operator=(const Type_info& ti)
{
    pdata = ti.pdata;
    return *this;
}

__cdecl Type_info::~Type_info()
{
}

int __cdecl Type_info::operator==(const Type_info& ti) const
{
    return this == &ti;
}

int __cdecl Type_info::operator!=(const Type_info& ti) const
{
    return this != &ti;
}

int __cdecl Type_info::before(const Type_info& ti)
#if _WIN32 || DOS386
 const
#endif
{
    return strcmp(name(),ti.name()) < 0;
}

const char * __cdecl Type_info::name() const
{   char *p;
    unsigned nbases;

    p = (char *)pdata;
    switch (*p)
    {
	case 0:
	    p++;
	    break;
	case 1:
	    p += 2 + sizeof(p);
	    break;
	case 2:
	    p++;
	    nbases = *(unsigned short *) p;
	    p += 2 + nbases * (sizeof(int) + sizeof(p));
	    break;
	default:
	    p = "bad Type_info";
	    break;
    }
    return p;
}

/* ========================== C++ 98 Implementation ================== */

#if _WIN32

namespace std
{

/***********************************
 */

type_info::type_info()
{
}

__cdecl type_info::type_info(const type_info& ti)
{
    pdata = ti.pdata;
}

type_info& __cdecl type_info::operator=(const type_info& ti)
{
    pdata = ti.pdata;
    return *this;
}

__cdecl type_info::~type_info()
{
}

bool __cdecl type_info::operator==(const type_info& ti) const
{
    return this == &ti;
}

bool __cdecl type_info::operator!=(const type_info& ti) const
{
    return this != &ti;
}

bool __cdecl type_info::before(const type_info& ti) const
{
    return strcmp(name(),ti.name()) < 0;
}

const char * __cdecl type_info::name() const
{   char *p;
    unsigned nbases;

    p = (char *)pdata;
    switch (*p)
    {
	case 0:
	    p++;
	    break;
	case 1:
	    p += 2 + sizeof(p);
	    break;
	case 2:
	    p++;
	    nbases = *(unsigned short *) p;
	    p += 2 + nbases * (sizeof(int) + sizeof(p));
	    break;
	default:
	    p = "bad type_info";
	    break;
    }
    return p;
}

bad_cast::~bad_cast() throw() { }
const char* bad_cast::what() const throw() { return "class bad_cast"; }

bad_typeid::~bad_typeid() throw() { }
const char* bad_typeid::what() const throw() { return "class bad_typeid"; }

}

#endif
