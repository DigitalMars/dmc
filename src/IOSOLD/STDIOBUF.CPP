#include <iosold\stdiostr.h>
#include <typeinfo.h>

stdiobuf::stdiobuf(FILE *fileptr)
    : fp(fileptr), streambuf(0, 0)
// Set up underlying streambuf unbuffered
{
    gptr_ = egptr_ = 0;
}

stdiobuf::~stdiobuf() {}

int stdiobuf::underflow()
{
// If we are using the pushback reserve, go back to using the
// regular get buffer. There should be characters in it.
    if (gptr_) {
        setg(gptr_, gptr_, egptr_);
        gptr_ = 0;
        if (gptr())
            return *gptr();
    }

    setg(base(),base(),base()+1);
    int c = fgetc(fp);
    if ( c != EOF) {
// Set the get buffer to the character read
        *gptr() = c;
        return *gptr();
    } else
// Get here if there's been an I/O error or EOF while reading.
        return EOF;
}

int stdiobuf::overflow(int c)
{
    if ( c != EOF ) {
        if (fputc(c,fp) == EOF)
            return EOF;
    }
    return 0;
}

int stdiobuf::pbackfail(int c)
{
    if ( gptr_ == 0 ) {
        char *end = pushback_buf+sizeof(pushback_buf);
        gptr_ = gptr();
        egptr_ = egptr();
        setg(pushback_buf, end, end);
    } else
        return EOF;

        gbump(-1);
        *gptr() = c;
        return 0;
}
