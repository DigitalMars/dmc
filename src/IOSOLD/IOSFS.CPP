// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <typeinfo.h>
#include <iosold\fstream.h>

#if Aiosfs1
fstream_common::fstream_common() : ios(&buffer) {}
#endif

#if Aiosfs2
void fstream_common::attach(int file_descriptor, int io_mode)
{
    filebuf *fb = buffer.attach(file_descriptor, io_mode);
    if (!fb)
        clear(ios::failbit | ios::badbit | rdstate());
    else
        clear();
}
#endif

#if Aiosfs3
void fstream_common::close()
{
    filebuf *fb = buffer.close();
    if (!fb)
        clear(ios::failbit | ios::badbit | rdstate());
}
#endif

#if Aiosfs4
void fstream_common::open(const char *name, int io_mode,
            int protection)
{
    filebuf *fb = buffer.open(name, io_mode, protection);
    if (!fb)
        clear(ios::failbit | ios::badbit | rdstate());
    else
        clear();
}
#endif

#if Aiosfs5
ifstream::ifstream() : ios(&buffer) { clear(ios::badbit); }
#endif

#if Aiosfs6
ifstream::ifstream(int file_descriptor, int io_mode)
    : ios(&buffer)
{
    fstream_common::attach(file_descriptor, io_mode);
}
#endif

#if Aiosfs7
ifstream::ifstream(const char *name, int io_mode, int protection)
    : ios(&buffer)
{
    fstream_common::open(name, io_mode, protection);
}
#endif

#if Aiosfs8
ifstream::ifstream(int file_descriptor, char *memory, int length, int io_mode)
    : ios(&buffer)
{
    fstream_common::setbuf(memory, length);
    fstream_common::attach(file_descriptor, io_mode);
}
#endif

#if Aiosfs9
ifstream::~ifstream()
{
}
#endif

#if Aiosfs10
ofstream::ofstream() : ios(&buffer) { clear(ios::badbit); }
#endif

#if Aiosfs11
ofstream::ofstream(int file_descriptor, int io_mode)
    : ios(&buffer)
{
    fstream_common::attach(file_descriptor, io_mode);
}
#endif

#if Aiosfs12
ofstream::ofstream(const char *name, int io_mode, int protection)
    : ios(&buffer)
{
    fstream_common::open(name, io_mode, protection);
}
#endif

#if Aiosfs13
ofstream::ofstream(int file_descriptor, char *memory, int length, int io_mode)
    : ios(&buffer)
{
    fstream_common::setbuf(memory, length);
    fstream_common::attach(file_descriptor, io_mode);
}
#endif

#if Aiosfs14
ofstream::~ofstream() {}
#endif

#if Aiosfs15
fstream::fstream() : ios(&buffer) { clear(ios::badbit); }
#endif

#if Aiosfs16
fstream::fstream(int file_descriptor, int io_mode)
    : ios(&buffer)
{
    fstream_common::attach(file_descriptor, io_mode);
}
#endif

#if Aiosfs17
fstream::fstream(const char *name, int io_mode, int protection)
    : ios(&buffer)
{
    fstream_common::open(name, io_mode, protection);
}
#endif

#if Aiosfs18
fstream::fstream(int file_descriptor, char *memory, int length, int io_mode)
    : ios(&buffer)
{
    fstream_common::setbuf(memory, length);
    fstream_common::attach(file_descriptor, io_mode);
}
#endif

#if Aiosfs19
fstream::~fstream() {}
#endif















