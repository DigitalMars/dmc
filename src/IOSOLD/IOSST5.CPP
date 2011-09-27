// STATICS.CPP
//
// Written by Steve Teale, April 1991
// Copyright (c) 1993 Digital Mars Corporation
//
// This translation unit contains the code to initialize the
// standard stream classes, and to set up appropriate ties
// and defaults.
//
// In the case of compilers generating native code there is no
// need for complicated provisions to ensure that static objects
// with constructors get initialized before they are used.
//
// Library objects are initialized before user objects, The
// stream classes are currently the only such objects in the
// libraries.

#include <typeinfo.h>
#include <iosold\fstream.hpp>

filebuf __fd4(4,0,0,ios::out | ios::translated);
ostream_withassign cprn(&__fd4);
