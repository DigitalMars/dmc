// IOStreams Package
// Steve Teale April 1992
// Copyright Digital Mars Corp 1990-1992. All Rights Reserved.

#include <string.h>
#include <iosold\iostream.h>
#include <typeinfo.h>

#if Aiosios1
const char default_padding_character = ' ';
const long ios::stickywidth = 0x8000;
const long ios::spacing = 0x10000;

ios::ios(streambuf * buffer)
    :   buf(buffer),
        format_state(ios::defaults),
        error_state(0),
        tied_ostream(0),
        padding_character(default_padding_character),
        decimal_precision(_ios_default_decimal_precision),
        field_width(0)
{
    memset(extended_format_words, 0, sizeof(extended_format_words));
}
#endif

#if Aiosios2
ios::~ios()
{
// Do nothing virtual destructor
}
#endif

#if Aiosios3
int ios::width(int new_value)
{
    int old_value = field_width;
    field_width = new_value < 0? 0: new_value;
    unsetf(ios::stickywidth);
    return field_width;
};
#endif

#if Aiosios4
int ios::bitalloc()
{
    static int longbit = (ios::spacing << 1);
    int old_value = longbit;

    longbit <<= 1;
    return old_value;
// returns 0 when all bits have been allocated
}
#endif

#if Aiosios5
// Allocate the next available index into the format words
// array. Returns EOF if no more available.
int ios::xalloc()
{
    static int index = 0;
    if (index >= _ios_n_extended_format_words)
        return EOF;
    int old = index;
    ++index;
    return old;
}
#endif

#if Aiosios6
ios &dec(ios &stream)
{
    stream.setf(ios::dec, ios::basefield);
    return stream;
}
#endif

#if Aiosios7
ios &hex(ios &stream)
{
    stream.setf(ios::hex, ios::basefield);
    return stream;
}
#endif

#if Aiosios8
ios &oct(ios &stream)
{
    stream.setf(ios::oct, ios::basefield);
    return stream;
}
#endif

#if Aiosios9
ios &defaults(ios &stream)
{
    stream.flags(ios::defaults);
    return stream;
}
#endif
