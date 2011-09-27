// Iostreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1994. All Rights Reserved.

#include <typeinfo.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <iosold\fstream.h>
#ifdef THINK_CPLUS
#include <unix.h>
#endif


// The default buffer size is set to be an "efficient" size
// for a disk block I/O. This value affects the efficiency
// of I/O, and the amount of memory used by the filebuf.

// Default buffer sizes
#if M_UNIX || M_XENIX
const int _dbs = 8192;   /* BSD filesystem */
#elif __INTSIZE == 4
const int _dbs = 0x4000;	/* 386	  */
#else
const int _dbs = 1024;		/* MS-DOS */
#endif

#if Aiosfb1

// The default constructor.
filebuf::filebuf()
    :   file(EOF),
        mode(0),
        do_not_seek(0),
        gptr_(0),
        egptr_(0),
        own_file_descriptor(0)
{}

#endif

#if Aiosfb2

// Constructs a filebuf for the open file attached to the argument
// file descriptor.
filebuf::filebuf(int descriptor, int io_mode)
    :   file(descriptor),
        mode(io_mode),
        do_not_seek(isatty(descriptor)),
        gptr_(0),
        egptr_(0),
        own_file_descriptor(0)
{
// The file may not have been opened with O_APPEND
    if (mode & ios::ate)
        lseek(file, 0L, SEEK_END);
}
#endif

#if Aiosfb3

// Constructs a filebuf for the open file attached to
// "file_descriptor", and sets the buffer to "memory", which is of
// "length" bytes in size. If memory is 0 or length is <= 0,
// it is taken as a request that the file be unbuffered.

filebuf::filebuf(int descriptor, char *memory, int length, int io_mode)
    :   streambuf(memory, length),
        file(descriptor),
        mode(io_mode),
        do_not_seek(isatty(descriptor)),
        gptr_(0),
        egptr_(0),
        own_file_descriptor(0)
{
    buffer_setup();
// The file may not have been opened with O_APPEND
    if (mode & ios::ate)
        lseek(file, 0L, SEEK_END);
}
#endif

#if Aiosfb4

filebuf::~filebuf()
{
    sync();
    if (own_file_descriptor)
        close();
}
#endif

#if Aiosfb5

filebuf *filebuf::attach(int file_descriptor, int io_mode)
{
    if (file != EOF)
        return 0;
    
    file = file_descriptor;
    if (file == EOF)
        return 0;
    mode = io_mode;
    if (isatty(file) || lseek(file, 0L, SEEK_CUR) < 0)
        do_not_seek = 1;
    else {
// The file may not have been opened with O_APPEND
        if (mode & ios::ate)
            lseek(file, 0L, SEEK_END);
    }

    return this;
}
#endif

#if Aiosfb6

// Flushes output, closes the file, and detaches the file from this
// filebuf. Clears the error state unless there is an error flushing
// the output. Will always close the file and detach it from the
// filebuf, even if there are errors.
filebuf *filebuf::close()
{
    int result = sync();

    int rv = (file == EOF)? EOF: ::close(file);
    file = EOF;

    if ( result == EOF || rv)
        return 0;
    else
        return this;
}
#endif

#if Aiosfb7

#include <stdlib.h>

int filebuf::doallocate()
{
#ifdef macintosh
    int bl = _dbs;
#else
    int bl = (mode & ios::translated)? _dbs*2: _dbs;
#endif
    char *buffer = (char *) malloc(bl); //new char[bl];

    if (!buffer)
        return EOF;
    setb(buffer, buffer+bl,1);
    return 1;
}
#endif

#if Aiosfb8

#include <errno.h>

// Opens the file "name", and connects it to this filebuf.
// Mode is a bit-mask containing one or more of the values of
// enum open_mode:
//  ios::in     Open for reading.
//  ios::out    Open for writing.
//  ios::ate    Position to the end-of-file.
//  ios::app    Position to the end-of-file before each write.
//          ios::app implies ios::out .
//  ios::trunc  Truncate the file on open.
//  ios::nocreate   Do not attempt to create the file if it
//          does not exist.
//  ios::noreplace  Cause the open to fail if the file exists.
//  ios::translate  Make CR/LF pairs into newlines on input
//          and vice versa on output
//

filebuf *filebuf::open(const char *name, int io_mode, int protection)
{
    if (file != EOF) {

// Existing file needs close() first
        errno = EINVAL;
        return 0;
    }
        
    int open_mode = 0;
    mode = io_mode;

    if (mode & (ios::out|ios::app)) {
        if (mode & ios::in)
            open_mode |= O_RDWR;
        else
            open_mode |= O_WRONLY;
    }
    else if (mode & ios::in)
        open_mode |= O_RDONLY;
    else {

// Must have one or both of ios::in and ios::out set
        errno = EINVAL;
        return 0;
    }

    if (mode & ios::trunc
    || (mode & ios::out && !(mode & (ios::in|ios::ate|ios::app)) ) )
        open_mode |= O_TRUNC;
    if (!(mode & ios::nocreate))
        open_mode |= O_CREAT;
    if (mode & ios::noreplace)
        open_mode |= O_EXCL;
#ifdef macintosh
    if (mode & ios::ate)
        open_mode |= O_APPEND;
    if ((file = ::open((char *)name, open_mode|O_BINARY)) < 0)
       return 0;
#else
#if M_UNIX || M_XENIX
    if ((file = ::open(name, open_mode, protection)) < 0)
       return 0;
#else
    if ((file = ::open(name, open_mode|O_BINARY, protection)) < 0)
       return 0;
#endif
    if (mode & ios::ate)
       lseek (file, 0, SEEK_END);   
#endif
// ::open does seek to end if ios::ate, seek to
// end is done for each write if ios::app, and after such
// append writes the file pointer is restored to its former
// position. So in app mode, the get and put pointers are
// not tied.

    own_file_descriptor = 1;

    if (isatty(file) || lseek(file, 0, SEEK_CUR) < 0)
        do_not_seek = 1;
    return this;
}
#endif

#if Aiosfb9

// Request that this streambuf use the argument memory buffer.
// A null memory argument, or a zero or negative length argument,
// is taken as a request that this streambuf be unbuffered.
// If the request to set the buffer is honored, return this, otherwise
// return zero.

streambuf *filebuf::setbuf(char *memory, int length)
{
    if (unbuffered() || base())
// If it is already working unbuffered, or has a proper
// buffer, decline the offer.  The one offered may be an
// unsuitable size
        return 0;
    if (streambuf::setbuf(memory, length)) {
        buffer_setup();
        return this;
    } else
        return 0;
}
#endif

#if Aiosfb10

void filebuf::buffer_setup()
{
#ifndef macintosh
// Use both chars of unbuf if translation necessary
    if (unbuffered() && (mode & ios::translated)) 
        setb(base(), base()+2, 0);
#endif

// Zero the other pointers
    setg(0, 0, 0);
    setp(0, 0);
}
#endif

#if Aiosfb11

// set up a lookup table from the C header file
const int filebuf::lseek_consts[3] = { SEEK_SET, SEEK_CUR, SEEK_END };

// Perform a relative seek of the get or put pointers, or both, to
// offset. "which" signifies what pointer is to be affected: ios::in
// for the get pointer, ios::out for the put pointer. In the case of
// a filebuf, the two are usually stuck together.
//
streampos filebuf::seekoff(streamoff offset, ios::relative_to from, int)
{

// Seeking on device which does not support it is an error.
    if ( do_not_seek )
        return streampos(EOF);

    sync(); // eliminate the effects of buffering
    if (offset.stepsize() != 1) return streampos(EOF);
                    // Implementation restriction
    long t = offset.steps();

    if ((t = lseek(file, t, lseek_consts[from])) != -1L)
        return streampos(t,1);
    else
        return streampos(EOF);
}
#endif

#if Aiosfb12

// A utility function for translated mode counts the newline
// characters in the get area so the file pointer can be moved back
// by an appropriate amount (in_avail()+newlines())
int filebuf::newlines()
{
    char *p = gptr();
    for (int n = 0; p < egptr(); ++p)
        if (*p == '\n') ++n;
    return n;
}
#endif

#if Aiosfb13

int filebuf::syncin()
{
    if (!do_not_seek) {

// Waste any characters queued in the get area
#ifdef macintosh
        int nlc = 0;
#else
        int nlc = (mode & ios::translated)? newlines(): 0;
#endif

        long offset = in_avail();

        if (gptr_) {
// gptr_ is set if the pushback reserve has been used.
// Dump the pushback and restore the old get buffer.
// Add the pushback to the offset to seek back.
            setg(gptr_, gptr_, egptr_);
            gptr_ = 0;
#ifndef macintosh
            nlc += (mode & ios::translated) ?newlines(): 0;
#endif
            offset += in_avail();
        }

// This is a weak point. At this stage we have no choice but
// to assume that in translated mode, all newline characters
// in the read buffer were derived from CR/NL pairs in the file.
// This of course may not be the case. There may have been
// isolated newline chars in the file.  This possibility is one
// of the penalties of mimicking Unix in the DOS environment.
        offset += nlc;
        setg(0,0,0);


// Seek back so that it appears that the characters that had
// been queued for read were never read.
        if ( offset > 0 ) {
            long rv = lseek(file, -offset, SEEK_CUR);
            if (rv == -1L) return EOF;
        }
    } else {
        setg(0,0,0);
	return EOF;
    }

return 0;
}
#endif

#if Aiosfb14

int filebuf::syncout()
{
    if (!(mode & ios::out)) {
        setp(0,0);
        return EOF;
    }
// In append-mode, always seek to the end of the file to write
    long cp, rv;
    if (mode & ios::app) {
        cp = lseek(file, 0L, SEEK_CUR);
        if (cp == -1L)
            return EOF;
        rv = lseek(file, 0L, SEEK_END);
        if (rv == -1L)
            return EOF;
    }
    if (flushbuf() == EOF) return EOF;
    if (mode & ios::app) {
        rv = lseek(file, cp, SEEK_SET);
        if (rv == -1L)
            return EOF;
    }
    setp(0,0);		// disable putting
    return 0;
}
#endif

#if Aiosfb15

// Make the external character stream and the streambuf consistent
// with each other. This means:
// 1. If there are characters in the get area, back up the file pointer
//    by the number of characters which remain unread in the get area.
//    This is only done if the device supports seeks.
// 2. If there are characters in the put area, write them to the
//    file. This leaves the file pointer positioned immediately after
//    the characters just written.
// 3. Zero out both get and put pointer sets (setg(0,0,0), setp(0,0))
// 4. Return EOF if there's an error, otherwise return some other
//    value.

int filebuf::sync()
{
    if ( file == EOF )
        return EOF;
    int rv;
    if (gptr())
        rv = syncin();
    if (rv == EOF) return EOF;
    if (out_waiting())
        rv = syncout();
    return rv;
}
#endif

#if Aiosfb16

// Get more bytes for reading.  See also the comment before filebuf::overflow.
int filebuf::underflow()
{
    if (!(mode & ios::in))
        return EOF;
// If we are using the pushback reserve, go back to using the
// regular get buffer. There should always be characters in it.
    if (gptr_) {
        setg(gptr_, gptr_, egptr_);
        gptr_ = 0;
        return (unsigned char) *gptr();
    }

// If there is no buffer, try to allocate one
    if (!base()) {
        if ((allocate()) == EOF) return EOF;
        setp(0,0);  // No puts without overflow
    } else {
// The file pointer needs to be adjusted if any characters have
// been put since last time the get buffer was filled.
        if (out_waiting()) {
            if (syncout() == EOF)
                return EOF;
        }
    }
// Now go for some more of the file
    int result = fillbuf();
    if ( result != EOF) {

// Set the get buffer to the characters just read
        setg(base(), base(), base()+result);
        return (unsigned char) *gptr();
    } else {

// Get here if there's been an I/O error or EOF while reading.
        setg(0, 0, 0);
        return EOF;
    }
}
#endif

#if Aiosfb17

// Try to fill the buffer from the file. In the DOS case this
// requires reading a buffer full, then stripping out CR/LF pairs
// and substituting LF, and returning the number of translated
// characters. fillbuf() uses the entire buffer area, so any chars
// in the put area need to have been dealt with first.
int filebuf::fillbuf()
{
    int trans = (mode & ios::translated)? 1: 0;
    char *p = base(), *q;
    int required = blen()-trans;

    int t = ::read(file,p,required);
    if (t <= 0) return EOF;   // no more characters

    if (trans) {

#ifndef macintosh
        q = p+(t-1);
        if (*q++ =='\r') {
// If there is an isolated CR see if it's part of a pair
            if (::read(file,q,1) == 1) {
                if (*q == '\r') {
                    lseek(file,-1L,SEEK_CUR);
// If its another CR then step the file back.  This should only
// happen infrequently
                } else
// usually it will be part of a pair or some other character
                    ++t;
            }
        }
#endif

    }

// If not translated that is all
    else
        return t;

// Otherwise do the translation
    q = p;
    for (; t--;) {
#ifdef macintosh
        if (*q == 0x0d)
            *q = 0x0a;
        q++;
#else
        if (t > 0 && *((short *) p) == 0x0a0d) {
            ++p;    // skip the '\r'
            --t;
        }
        *q++ = *p++;
#endif
    }
    return q-base();  // the translated length
}
#endif

#if Aiosfb18

// Flush bytes in the put area to the file.
//
// Note that a filebuf is always in one of two states, either getting
// or putting.  The 'other' set of pointers are zeroed to force an
// overflow if getting changes to putting, or an underflow if putting
// changes to getting.  This is the mechanism that essentially 'ties'
// the get and put pointers together in a filebuf, since there is
// no mechanism for this in the underlying streambuf.
//
// It follows that code which switches frequently between getting
// and putting will be inefficient.
int filebuf::overflow(int c)
{
    if (!(mode & ios::out))
        return EOF;
// If there is no buffer, try allocate one
    if (!base()) {
        if (allocate() == EOF) return EOF;
        setg(0,0,0);    // No gets without underflow
    } else {
// Otherwise, flush any characters buffered for write. This
// throws away the contents of the get buffer as if they
// were never fetched from the file, it also zeroes get and
// put area pointers.
        if (in_avail()) {
            if (syncin() == EOF)
                return EOF;
        }
        if (out_waiting()) {
            if (syncout() == EOF)
                return EOF;
        }
    }
// Set up the whole buffer for put
    if (!unbuffered()) {
        int bl = blen();
#ifdef macintosh
        setp(base(), base()+bl);
#else
        setp(base(), base()+((mode & ios::translated)? bl/2: bl));
#endif
    }

// Now if not unbuffered, the overflow character can just be
// entered into the put buffer, otherwise it must be written
// directly or with translation if required.
    if ( c != EOF ) {
        if ( pptr() < epptr() ) {
            *pptr() = c;
            pbump(1);
        }  else {
            char b = c; // for portability
            long cp, rv;
            if (mode & ios::app) {
                cp = lseek(file, 0L, SEEK_CUR);
                if (cp == -1L)
                    return EOF;
                rv = lseek(file, 0L, SEEK_END);
                if (rv == -1L)
                    return EOF;
            }
            if ((mode & ios::translated) && c == '\n') {
#ifdef macintosh
                if(::write(file,"\r",1) != 1)
#else
                if(::write(file,"\r\n",2) != 2)
#endif
                    return EOF;
            } else {
                if (::write(file, &b, 1) != 1)
                    return EOF;
            }
            if (mode & ios::app) {
                rv = lseek(file, cp, SEEK_SET);
                if (rv == -1L)
                    return EOF;
            }
        }
    }
    return 0;
}
#endif

#if Aiosfb19

// Empty the buffer to the file. In the translated case this means first
// translating all newline characters to CR/LF pairs, then writing
// this extended length to the file.
int filebuf::flushbuf()
{
	int n;
	char *q;

// The put buffer always lies at the beginning of the buffer area.
// In translating, it gets copied to the other end, thus obliterating
// anything in the get buffer, which had therefore better have been
// dealt with first! This should always be the case since filebuf
// is one track minded!
	if (mode & ios::translated) {
#ifdef macintosh
		q = pbase();
		n = pptr() - q;
		for (; n; n--, q++)
		    if (*q == '\n')
			#if THINK_CPLUS
				*q = '\r'
			#endif
				;
		q = pbase();
		n = pptr() - q;
#else
		char *p = pptr()-1;
		q = ebuf();
		for (; p >= pbase();) {
			*--q = *p--;
			if (*q == '\n')
				*--q = '\r';
		}
		n = ebuf()-q;
#endif
	} else {
		q = pbase();
		n = pptr() - q;
	}
	if (::write(file,q,n) < n) return EOF;
	return 0;
}
#endif

#if Aiosfb20

// This is called by sputbackc() when the get pointer is equal to
// the base of the get area, and there is thus no space to put back
// characters. It may try to re-arrange the buffer so that it is
// possible to put back characters, and then put back c. If it
// succeeds it should return c, otherwise it should return EOF.

int filebuf::pbackfail(int c)
{
    if (c == EOF)           // can't put back EOF
        return EOF;
    if (gptr() <= eback()) {  // genuine fail
        if (!gptr()) {          // no get area set up
            if (!base()) {
                if ((allocate()) == EOF) return EOF;
                setp(0,0);  // One thing at a time
            } else {
                if (out_waiting()) {
                    if (syncout() == EOF)
                        return EOF;
                }
            }
// set up minimal get area
            setg(base(),base(),base()+1);
            *gptr() = c;
            return 0;
        }
        if (gptr_)          // already using the reserve
            return EOF;
// switch to putback reserve buffer
        char *end = pushback_buf+sizeof(pushback_buf);
        gptr_ = gptr();
        egptr_ = egptr();
        setg(pushback_buf, end, end);
    }
    gbump(-1);
    *gptr() = c;
    return 0;
}
#endif

