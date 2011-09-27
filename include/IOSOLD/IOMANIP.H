// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars 1990-1992. All Rights Reserved.

#ifndef __IOMANIP_H
#define __IOMANIP_H

#include <iosold\iomdefs.h>

#pragma pack(__DEFALIGN)
ios &_iomanip_resetiosflags(ios&, long);
inline SMANIP<long> resetiosflags(long f) 
	{ return SMANIP<long>(_iomanip_resetiosflags,f); }

ios &_iomanip_setfill(ios&, int);
inline SMANIP<int> setfill(int filler)
	{ return SMANIP<int>(_iomanip_setfill,filler); }

ios &_iomanip_setiosflags(ios&, long);
inline SMANIP<long> setiosflags(long f)
	{ return SMANIP<long>(_iomanip_setiosflags,f); }

ios &_iomanip_setprecision(ios&, int);
inline SMANIP<int> setprecision(int n)
	{ return SMANIP<int>(_iomanip_setprecision,n); }

ios &_iomanip_setw(ios&, int);
inline SMANIP<int> setw(int w)
	{ return SMANIP<int>(_iomanip_setw,w); }

ios &_iomanip_setbase(ios&, int);
inline SMANIP<int> setbase(int n)
	{ return SMANIP<int>(_iomanip_setbase,n); }

#pragma pack()
#endif  // __IOMANIP_H
