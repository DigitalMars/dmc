#include <iosold\iostream.h>
#include <string.h>
#include <iosold\stdiostr.h>
#include <typeinfo.h>

void ostream::flush_stdio()
{
    fflush(stdout);
    fflush(stderr);
}

void ios::sync_with_stdio()
{
    stdiobuf *inb = 0, *soutb = 0, *serrb = 0;
    inb = new stdiobuf(stdin);
    if (!inb) goto nomem;
    soutb = new stdiobuf(stdout);
    if (!soutb) goto nomem;
    serrb = new stdiobuf(stderr);
    if (!serrb) goto nomem;
    cin.set_buffer(inb);
    cout.set_buffer(soutb);
    cerr.set_buffer(serrb);
    cout.setf(ios::stdio);
    cerr.setf(ios::stdio);
#if M_UNIX || M_XENIX
    clog.set_buffer(serrb);
    clog.setf(ios::stdio);
#endif
    return;
nomem:
    delete inb;
    delete soutb;
    delete serrb;
}

