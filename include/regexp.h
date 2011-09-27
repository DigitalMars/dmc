
// Regular Expressions
// Copyright (c) 2000-2001 by Digital Mars
// All Rights Reserved
// Written by Walter Bright

#ifndef REGEXP_H
#define REGEXP_H

#include <tchar.h>

/*
	Escape sequences:

	\nnn starts out a 1, 2 or 3 digit octal sequence,
	where n is an octal digit. If nnn is larger than
	0377, then the 3rd digit is not part of the sequence
	and is not consumed.
	For maximal portability, use exactly 3 digits.

	\xXX starts out a 1 or 2 digit hex sequence. X
	is a hex character. If the first character after the \x
	is not a hex character, the value of the sequence is 'x'
	and the XX are not consumed.
	For maximal portability, use exactly 2 digits.

	\uUUUU is a unicode sequence. There are exactly
	4 hex characters after the \u, if any are not, then
	the value of the sequence is 'u', and the UUUU are not
	consumed.

	Character classes:

	[a-b], where a is greater than b, will produce
	an error.
 */

typedef struct regmatch
{
    int rm_so;			// index of start of match
    int rm_eo;			// index past end of match
} regmatch_t;

struct Range;
struct RegBuffer;

struct RegExp
{
    RegExp();
    ~RegExp();

    unsigned re_nsub;		// number of parenthesized subexpression matches
    regmatch_t *pmatch;		// array [re_nsub + 1]

    TCHAR *input;		// the string to search

    // per instance:

    int ref;			// !=0 means don't make our own copy of pattern
    TCHAR *pattern;		// source text of the regular expression

    TCHAR flags[3 + 1];		// source text of the attributes parameter
				// (3 TCHARs max plus terminating 0)
    int errors;

    unsigned attributes;

    #define REAglobal	  1	// has the g attribute
    #define REAignoreCase 2	// has the i attribute
    #define REAmultiline  4	// if treat as multiple lines separated
				// by newlines, or as a single line
    #define REAdotmatchlf 8	// if . matches \n

    int compile(TCHAR *pattern, TCHAR *attributes, int ref);
    int test(TCHAR *string, int startindex = 0);

    TCHAR *replace(TCHAR *format);
    TCHAR *replace2(TCHAR *format);
    static TCHAR *replace3(TCHAR *format, TCHAR *input,
	unsigned re_nsub, regmatch_t *pmatch);
    static TCHAR *replace4(TCHAR *input, regmatch_t *match, TCHAR *replacement);

private:
    TCHAR *src;			// current source pointer
    TCHAR *src_start;		// starting position for match
    TCHAR *p;			// position of parser in pattern
    regmatch_t match;		// match for the entire regular expression
				// (serves as storage for pmatch[0])

    char *program;
    RegBuffer *buf;

    void printProgram(char *prog);
    int trymatch(char *prog, char *progend);
    int parseRegexp();
    int parsePiece();
    int parseAtom();
    int parseRange();
    int escape();
    void error(char *msg);
    void optimize();
    int startchars(Range *r, char *prog, char *progend);
};

#endif

