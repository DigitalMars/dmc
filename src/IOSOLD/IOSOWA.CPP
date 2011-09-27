// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <iosold\iostream.h>
#include <typeinfo.h>

#if Aiosowa1
ostream_withassign::ostream_withassign(streambuf *buf)
    : ios(buf), assigned_to(0) {}

ostream_withassign::~ostream_withassign()
{
    if (assigned_to || (flags() & ios::stdio))
        set_buffer(0);
// This instance can not flush the buffer. Since these classes
// are used for the static streams, the buffer will probably
// already have been destroyed.
}
#endif

#if Aiosowa2
ostream_withassign::ostream_withassign()
    : ios(0), assigned_to(0) { clear(ios::badbit); }
#endif

#if Aiosowa3
// Assignment operators. Flush the old buffer, if any. Assign a new buffer
// and re-initialize the internal state.
ostream_withassign &ostream_withassign::operator=(streambuf *source)
{
    streambuf *old;
    if ((old = rdbuf()) != 0)
        flush();

    assigned_to = 1;
    set_buffer(source);
    if (!old) {
// Not previously used, to set up as from scratch
        clear(0);
        flags(ios::skipws|ios::dec);
        precision(6);
        width(0);
        tie(0);
    }
    return *this;
}
#endif

#if Aiosowa4
// Assignment operators. Flush the old buffer, if any. Assign a new buffer
// and re-initialize the internal state.

ostream_withassign &ostream_withassign::operator=(ostream &source)
{
    if (this == &source)
        return *this;
    assigned_to = 1;
    if (rdbuf())
        flush();
    set_buffer(source.rdbuf());
    clear(source.rdstate());
    flags(source.flags());
    precision(source.precision());
    width(source.width());
    tie(source.tie());
    return *this;
}
#endif

