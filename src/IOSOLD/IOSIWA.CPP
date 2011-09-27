// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <iosold\iostream.h>
#include <typeinfo.h>

#if Aiosiwa1
istream_withassign::istream_withassign()
    : ios(0), assigned_to(0) { clear(ios::badbit); }
// Stream constructed this way is not in a useable state
#endif

#if Aiosiwa2
istream_withassign::istream_withassign(streambuf *buf)
    : ios(buf), assigned_to(0) {}

istream_withassign::~istream_withassign()
{
    if (assigned_to || (flags() & ios::stdio))
        set_buffer(0);
// This instance can not flush the buffer. Since these classes
// are used for the static streams, the buffer will probably
// already have been destroyed.
}
#endif

#if Aiosiwa3
// Assignment operators. Flush the old buffer, if any. Assign a new buffer
// and re-initialize the internal state.
istream_withassign &istream_withassign::operator=(streambuf *source)
{
    streambuf *old;
    if ((old = rdbuf()) != 0)
        sync();

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

#if Aiosiwa4
// Assignment operators. Flush the old buffer, if any. Assign a new buffer
// and re-initialize the internal state.
istream_withassign &istream_withassign::operator=(istream &source)
{
    if (this == &source)
        return *this;
    assigned_to = 1;
    if (rdbuf())
        sync();
    set_buffer(source.rdbuf());
    clear(source.rdstate());
    flags(source.flags());
    precision(source.precision());
    width(source.width());
    tie(source.tie());
    return *this;
}
#endif
