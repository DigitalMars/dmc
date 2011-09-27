// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <iosold\iostream.h>
#include <typeinfo.h>

#if Aiosiost1
iostream::iostream() : ios(0) {}
// dummy constructor for when derived classes initialize
// ios directly
#endif

#if Aiosiost2
iostream::iostream(streambuf *buf) : ios(buf) {}
#endif

#if Aiosiost3
iostream::~iostream() {}
#endif

