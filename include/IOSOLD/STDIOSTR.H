#ifndef __STDIOSTR_H
#define __STDIOSTR_H

#include <stdio.h>
#include <iosold\iostream.h>

#pragma pack(__DEFALIGN)
class stdiobuf : public streambuf {
public:
    stdiobuf(FILE*);
    FILE* stdiofile() { return fp; }
    ~stdiobuf();

    int  underflow();
#if __SC__ > 0x214
    int  overflow(int=EOF);
#else
    int overflow(int);
#endif
    int  pbackfail(int);

private:
    FILE*   fp;
    char *gptr_;
    char *egptr_;
                // Save old gptr() & egptr() while using the
                // pushback buffer.
    char pushback_buf[4];
    int fillbuf();
};

class stdiostream : public ios {
public:
    stdiostream(FILE*);
    ~stdiostream();
    stdiobuf* rdbuf() { return &buffer; }

private:
    stdiobuf buffer;
};

#pragma pack()
#endif
