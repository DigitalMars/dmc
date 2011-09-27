// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <ctype.h>
#include <limits.h>
#include <iosold\iostream.h>
#include <typeinfo.h>

#if Aiosis1
// Construct an istream associated with buffer.
istream::istream(streambuf * buffer) : ios(buffer), read_count(0) {}
#endif

#if Aiosis2
// Construct an istream associated with buffer.
istream::istream() : ios(0), read_count(0) {}
#endif

#if Aiosis3
istream::~istream()
{
// There is a default constructor which sets a zero buffer
// pointer so must check for this.
    if (rdbuf())
        rdbuf()->sync();
}
#endif

#if Aiosis4
// Extract up to "length" characters, and store them in "data". Stop extraction
// at the delimiter character without extracting the delimiter.
istream &istream::get(char *data, int length, char delimiter)
{
    read_count = 0;
    int c = 0;

    if ( ipfx(1) ) {
        for ( --length; length--
        && (c = rdbuf()->sgetc()) != delimiter;) {
            if ( c == EOF ) {
                clear(eofbit|rdstate());
                break;
            }
            *data++ = c;
            rdbuf()->stossc();
            read_count++;
        }
    }
    *data = '\0';
    if (!read_count && c == EOF)
        clear(failbit|rdstate());
    return *this;
}
#endif

#if Aiosis5
istream & istream::get(unsigned char *data, int length, char delimiter)
{
    return get((char *) data, length, delimiter);
}
#endif

#if Aiosis6
istream &istream::get(char &c)
{
    int t = get();
    if (t == EOF)
        clear(eofbit|failbit);
    else
        c = t;
    return *this;
}
#endif

#if Aiosis7
istream &istream::get(signed char &c)
{
    int t = get();
    if (t == EOF)
        clear(eofbit|failbit);
    else
        c = t;
    return *this;
}
#endif

#if Aiosis8
istream &istream::get(unsigned char &c)
{
    int t = get();
    if (t == EOF)
        clear(eofbit|failbit);
    else
        c = t;
    return *this;
}
#endif

#if Aiosis9
// Extract characters, and stuff them into the argument streambuf.
// Stop extraction at the delimiter character without extracting the delimiter.
// Otherwise, stop extraction if EOF is encountered or the put to the
// streambuf fails.
istream &istream::get(streambuf &destination, char delimiter)
{
    read_count = 0;

    if ( ipfx(1) ) {
        int c;

        while ( (c = rdbuf()->sgetc()) != delimiter ) {
            if ( c == EOF ) {
                clear(read_count? (eofbit|rdstate()):
                                        (eofbit|failbit));
                return *this;
            }

// Make sure that read count reflects what was actually transferred
            if ( destination.sputc(c) == EOF )
                break;

            rdbuf()->stossc();
            read_count++;
        }
    }
    return *this;
}
#endif

#if Aiosis10
int istream::get()
{
    int t;

    if ( ipfx(1) ) {
        if ((t = rdbuf()->sbumpc()) != EOF ) {
            read_count = 1;
            return t;
        }
        clear(eofbit|failbit);
    }
    read_count = 0;
    return EOF;
}
#endif

#if Aiosis11
// Extract up to "length" characters, and store them in "data". Stop extraction
// after the delimiter character, extracting the delimiter, but not storing it.
// The get count reflects the total number of characters extracted.
istream &istream::getline(char *data, int length, char delimiter)
{
    read_count = 0;
    int c;

    if ( ipfx(1) ) {
        for (--length; length--;) {
            c = rdbuf()->sgetc();
            if ( c == EOF ) {
                clear(eofbit|rdstate());
                break;
            }
            read_count++;
            rdbuf()->stossc();
            if (c == delimiter)
                break;
            *data++ = c;
        }
    }
    *data = '\0';
    if (!read_count && c == EOF)
        clear(failbit|rdstate());
    return *this;
}
#endif

#if Aiosis12
istream &istream::getline(unsigned char *data, int length, char delimiter)
{
    return getline((char *) data, length, delimiter);
}
#endif

#if Aiosis13
// Throw away up to "length" characters. Stop after a character is equal
// to "delimiter".
istream &istream::ignore(int length, int delimiter)
{

    if ( length && ipfx(1) ) {
        register int c;

        read_count = 0;

        while ( length-- && (c = rdbuf()->sgetc()) != EOF ) 
            {
            rdbuf()->stossc();
            read_count++;
            if ( c == delimiter )
                break;
            }

        if ( c == EOF )
            clear(eofbit|rdstate());
    }
    return *this;
}
#endif

#if Aiosis14
// Extract into a character array until a white-space character is seen.
// If width() is non-zero, it is taken as
// the size of the array, and no more than width-1 characters will be
// extracted. A terminating null is always stored, even if nothing else
// is done because of the error state.
istream &istream::operator>>(char *data)
{
    int got_a_character = 0, length = width();
    length = length? length-1: INT_MAX-1;
    if (!(flags() & ios::stickywidth))
        width(0);   // reset width if not sticky

    if ( ipfx(0) ) {
        int c = 0;

        while (length--
                && (c = rdbuf()->sgetc()) != EOF
                && !isspace(c)) {
            *data++ = c;
            rdbuf()->stossc();
            got_a_character = 1;
        }
        if ( c == EOF )
            clear(eofbit|rdstate());
    }

// Set fail if zero characters extracted
    if (!got_a_character)
        clear(failbit|rdstate());

    *data = '\0';

    return *this;
}
#endif

#if Aiosis15
// Extract a single character.
istream &istream::operator>>(char &c)
{
    int t;

    if (ipfx(0)) {
        if ((t = rdbuf()->sbumpc()) == EOF)
            clear(eofbit|failbit);
        else 
            c = t;
    }
    return *this;
}
#endif

#if Aiosis16
// Extract characters and stuff them into the argument streambuf.
// Stop on EOF, or if the put to the streambuf fails.
istream &istream::operator>>(streambuf *destination)
{
    int got_a_character = 0;

    if ( ipfx(1) ) {
        int c;

        while ( (c = rdbuf()->sgetc()) != EOF ) {

// Stop if the receiving streambuf is full
            if ( destination->sputc(c) == EOF )
                return *this;

            rdbuf()->stossc();
        }
        clear(eofbit|rdstate());
    }
    return *this;
}
#endif

#if Aiosis17
int istream::ipfx(int need)
{
    if ( !good() )
        {
        clear(failbit | rdstate());
        return 0;
        }

// If there is a tied ostream, flush it.
// The draft ansi spec says if need <= characters than are
// currently available in the input stream, don't bother, but
// this doesn't make sense, since it also states that ipfx is
// called with argument 0 or 1. So this implementation flushes
// it regardless.
    if (tie())
        tie()->flush();

    if (!need && (flags() & skipws)) {
        int c;

// Ellis and Stroustrup 2.1 says that white-space is:
// Blanks, horizontal and vertical tabs, newlines, formfeeds,
// and comments. Comments don't apply here. They don't mention
// carriage-return, but they are not being character-set
// specific.
        while ((c = rdbuf()->sgetc()) != EOF) {
            if (isspace(c)) {
                rdbuf()->stossc();
                continue;
            }
            else {
                return 1;
            };
        }
// EOF, so stream no longer useable, but not a fail!
        clear(eofbit|rdstate());
        return 0;
    }
    return 1;
}
#endif

#if Aiosis18
istream &ws(istream &stream)
{
    stream.ipfx(0);
    return stream;
}
#endif

#if Aiosis19
// Peek at the next character in the input stream without extracting it.
// Saves one from having to use putback().
int istream::peek()
{
    if ( ipfx(1) ) {
        int result = rdbuf()->sgetc();

        if (result == EOF)
            clear(eofbit|rdstate());

        return result;
    }
    else
        return EOF;
}
#endif

#if Aiosis20
// Push the last character read back onto the input stream.
istream &istream::putback(char c)
{
    if (good()) {
        if ( rdbuf()->sputbackc(c) == EOF )
            clear(failbit|rdstate());
    }
    return *this;
}
#endif

#if Aiosis21
// Read "length" characters into "data". If less than "length" can be
// read, get what I can, set eofbit, and the length actually read
// is available from gcount().
istream &istream::read(void *data, int length)
{
    if (ipfx(1)) {
        read_count = rdbuf()->sgetn(data, length);
        if ( read_count < length )
             clear(eofbit|rdstate());
    }
    else
        read_count = 0;

    return *this;
}
#endif

#if Aiosis22
// Absolute-seek the input character stream to "position".
istream &istream::seekg(streampos position)
{
    int rv = rdbuf()->seekoff(streamoff(position), ios::beg, ios::in);
    if (rv == EOF)
        clear(eofbit|failbit|rdstate());
    return *this;
}
#endif

#if Aiosis23
// Relative-seek the input character stream to "position".
istream &istream::seekg(streamoff offset, ios::relative_to rt)
{
    int rv = rdbuf()->seekoff(offset, rt, ios::in);
    if (rv == EOF)
        clear(eofbit|failbit|rdstate());
    return *this;
}
#endif

#if Aiosis24
// Establish consistency between the internal data structure and the
// external character source. This usually means that
// characters that were buffered for input are thrown away, and
// the file pointer is decremented so that it appears that the
// buffered characters were never read.
int istream::sync()
{
    if ( rdbuf()->sync() == EOF ) {
        clear(failbit|rdstate());
        return EOF;
    }
    return 0;
}
#endif

#if Aiosis25
// Return the current file position
streampos istream::tellg()
{
    return rdbuf()->seekoff(0, ios::cur, ios::in);
}
#endif

#if Aiosisdfa
#include <string.h>

istream &istream::extract(void *t, const char *ok,
       int *tt, translate_function_t tf, helper_function_t hf, void *tds)
{
    if (!ipfx())
        return *this;
    int alphabet = strlen(ok), pc, c = rdbuf()->sgetc(), err = 0, state = 0,
        back2 = 0, back1 = 0, last = 0, accept = 0, n = 0;
	char buf[256], *p = buf;
    for (;;) {
        if (c == EOF) {
            err = ios::eofbit;
            break;
        }
        const char *q = strchr(ok,c);
        if (!q) {             // char not in Type's alphabet
            back2 = back1;
            back1 = last;
            last = 0;
            break;
        }
        state = tt[state*alphabet+(q-ok)];
                                    // get next state from Type's transition table
        if (state < 0) {
            accept = 1;
            state = -state;
        }
        --state;
        back2 = back1;
        back1 = last;
        last = accept;
        accept = 0;
        if (state < 0) break;       // character out of context
        pc = c;                     // remember previous char for putback
        hf(n,c,tds);
		*p++ = c;
        c = rdbuf()->snextc();
        ++n;
    }
    if (!(back2 || back1 || last)) {
        err |= ios::failbit;
        if (n)
            rdbuf()->sputbackc(pc);
    } else {
        if (last)
            rdbuf()->stossc();
        else if (!back1) {
			--p;
            rdbuf()->sputbackc(pc);
		}
		*p = 0;
        tf(t,buf,tds);
    }
    clear(err);
    return *this;
}
#endif
