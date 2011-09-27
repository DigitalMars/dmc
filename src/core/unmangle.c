/* unmangle.c Copyright (c) 1993 by Digital Mars
   
   You may distribute or use this file without restriction except that
   the above copyright notice should remain in the source file if it is
   electronically or otherwise distributed.

   People writing tools to interface with Digital Mars C++ are strongly
   encouraged to use this code.
   
   This file contains a number of functions which allow the un-mangling
   of C++ mangled names as used in Digital Mars C++. The primary function
   is unmangle_ident which takes a character string containing the mangled
   name and returns a pointer to the un-mangled name.

   Written by Walter Bright.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#undef STATIC
#ifdef DEBUG
#define STATIC
#else
#define STATIC static
#endif

#define LOCAL __near __pascal

char *unmangle_pt(char **);
STATIC char * LOCAL str_cat ( char *s1 , char *s2 );
STATIC char * LOCAL str_cat1 ( const char *s1 , char *s2 );
STATIC char * LOCAL str_cat2 ( char *s1 , const char *s2 );
STATIC char * LOCAL str_cat12 ( const char *s1 , const char *s2 );
STATIC char * LOCAL str_bracket ( char cl , char *s , char cr );
STATIC char * LOCAL str_dup ( char *s );
STATIC char * LOCAL um_nestname(char **ps);
STATIC char * LOCAL um_argument_types ( char **ps );
STATIC char * LOCAL um_argument(char **ps);
STATIC char * LOCAL um_data_type ( char **ps , char *id );
STATIC char * LOCAL um_indirect_type ( char **ps );
STATIC char * LOCAL um_type_encoding ( char **ps , char *id );
STATIC char * LOCAL um_function_type(char **ps,char *id,char *);
STATIC char * LOCAL um_calling_convention ( char **ps );
STATIC char * LOCAL um_array_type ( char **ps , char *id );
STATIC void LOCAL um_init ( void );
STATIC void LOCAL um_term ( void );
STATIC char * LOCAL um_zname ( char **ps );
STATIC char * LOCAL um_scope ( char **ps );
STATIC char * LOCAL um_ecsu_name ( char **ps );
STATIC unsigned long LOCAL um_dimension ( char **ps );
STATIC char * LOCAL um_string ( char **ps );
STATIC char * LOCAL id_expand(char *p, int plen);

#define arraysize(a)    (sizeof(a) / sizeof((a)[0]))

static char EMPTY[] = "";        /* empty string        */
static char LPAR [] = "(";
static char RPAR [] = ")";

static struct UM
{
    char *zname[10];
    int znamei;

    char *arg[10];
    int argi;
} um;

/* A (very) small string handling package    */

STATIC void LOCAL ul2a(char *s, unsigned long u)
{
    ultoa(u, s, 10 /*16*/);
}

STATIC char * LOCAL str_cat(char *s1,char *s2)
{  
    char *s;

    if (!s1 || !s2)
        s = NULL;        /* propagate any errors along    */
    else
    {   
        s = (char *) malloc(strlen(s1) + strlen(s2) + 1);
        if (s)
            strcat(strcpy(s,s1),s2);
    }
    free(s1);
    free(s2);
    return s;
}

STATIC char * LOCAL str_cat1(const char *s1,char *s2)
{
    return str_cat(strdup(s1),s2);
}

STATIC char * LOCAL str_cat2(char *s1,const char *s2)
{
    return str_cat(s1,strdup(s2));
}

STATIC char * LOCAL str_cat12(const char *s1,const char *s2)
{
    return str_cat1(s1,strdup(s2));
}

STATIC char * LOCAL str_bracket(char cl,char *s,char cr)
{   
    char buf[2];

    buf[0] = cl;
    buf[1] = 0;
    s = str_cat1(buf,s);
    buf[0] = cr;
    return str_cat2(s,buf);
}

STATIC char * LOCAL str_catc(char *s,char c)
{   
    char buf[2];

    buf[0] = c;
    buf[1] = 0;
    return str_cat2(s,buf);
}

STATIC char * LOCAL str_dup(char *s)
{
    if (s)
        s = strdup(s);
    return s;
}

static char __near *um_table[] = {
    "ctor",    "dtor",
    "new",    "delete",
    "=",    ">>",
    "<<",    "!",
    "==",    "!=",
    "[]",    "",
    "->",    "*",
    "++",    "--",
    "-",    "+",
    "&",    "->*",
    "/",    "%",
    "<",     "<=",
    ">",    ">=",
    ",",    "()",
    "~",    "^",
    "|",    "&&",
    "||",    "*=",
    "+=",    "-=",
    "/=",        /* _0    */
    "%=",
    "<<=",
    ">>=",
    "&=",
    "|=",
    "^=",
    "vftable",
    "vbtable",
    "vcall_thunk",
    "metaclass",
    "guard",
    "lit_string",
    "ult_vbase_dtor",
    "vec_del_dtor",
    "def_ctor_clos",
    "scal_del_dtor",
    "vec_ctor",
    "vec_dtor",
    "vec_vbase_ctor",

    0,0,0,0,0,        /* leave gap for MS expansion    */

    "new[]",
    "delete[]",
    "?_R",            // special operator=()
    /* NCEG operators that only Digital Mars implements    */
    "!<>=",        /* _S    */
    "<>",
    "<>=",
    "!>",
    "!>=",
    "!<",
    "!<=",
    "!<>",        /* _Z    */
};

/********************************************
    Accepts a mangled C++ name and returns a simplified
    unmangled name. Returns NULL if no unmangling required or an
    error occurred.

    Note this returns a malloc'd buffer which should be free'd when
    finished with if conversion takes place.
*/

char    *unmangle_ident(char *ident)
{
    //printf("unmangle_ident('%s')\n", ident);

    // See if ident is a compressed name
    char *p;
    for (p = ident; *p; p++)
    {
	if (*p & 0x80)	// it's a compressed name
	{
	    p = id_expand(ident, strlen(ident));
	    if (p)
	    {
		if (*p == '?')
		{
		    ident = p;
		    ident = um_nestname(&ident);
		    if (ident)
		    {	free(p);
			p = ident;
		    }
		}
		return p;
	    }
	    break;
	}
    } 

    if (*ident == '?')        // mangled names always start with '?'
    {
        return um_nestname(&ident);
    }
    else
        return NULL;
}


/**************************************
 */

STATIC char * LOCAL um_nestname(char **ps)
{
    char    *ident = *ps;
    char    *result=NULL;
    char    *s,*cp;
    char    c;
    int        opidx = -1;
    static char opstr[] = "operator ";

    um_init();
    cp = ident;
    if (*cp != '?')        /* mangled names always start with '?'    */
    {    
        s = um_zname(&cp);
        goto ret2;
    }

    cp++;
    if (*cp == '?')            /* if operator_name        */
    {   
	//printf("operator_name\n");
        c = *++cp;
        cp++;
        switch (c)
        {   
            case '0':
            case '1':
                opidx = c - '0';
                s = NULL;
                break;
            case '_':
                opidx = 10 + 26;
                c = *cp++;
                goto L1;
            default:
                opidx = 0;
            L1:
                if (isdigit(c))
                    opidx += c - '0';
                else if (isupper(c))
                    opidx += c - 'A' + 10;
                else
                    goto err;
                if (!um_table[opidx])
                    goto err;
                s = str_dup(um_table[opidx]);
                if (!s)
                    goto err;
                break;
        }
    }
    else                /* zname            */
    {
        s = um_zname(&cp);
        if (!s)
            goto ret;
    }

    /* Handle scopes    */
    while (*cp != '@')
    {   
        char *p;

	//printf("scope '%s'\n", cp);
        p = um_zname(&cp);
        if (!p)
            goto ret;
        if (opidx == 0)            /* if constructor        */
        {   
            s = str_dup(p);
            opidx = -1;
        }
        else if (opidx == 1)        /* if destructor        */
        {   
            s = str_cat12("~",p);
            opidx = -1;
        }
        s = str_cat1("::",s);
        s = str_cat(p,s);
    }
    cp++;

    //printf("um_type_encoding '%s'\n", cp);
    s = um_type_encoding(&cp,s);    /* unmangle type        */
    if (opidx == 11)            /* if user defined conversion    */
        s = str_cat1(opstr,s);
ret2:
    result = s;

err:
ret:
    um_term();
    *ps = cp;
    return result;
}

/*************************************
    Used by unmangle_ident to un-mangle the type safe information.

    Returns:
    NULL    error
    char*    unmangled type safe information
    
    Note this returns a malloced buffer which should be free'd when
    finished with if conversion takes place.
*/

STATIC char * LOCAL um_argument_types(char **ps)
{
    char *s = *ps;
    char *p;

    if (*s == 'X')
    {    
        p = strdup(EMPTY);        /* (void)            */
        s++;
    }
    else if (*s == 'Z')            /* (...)            */
    {    
        p = strdup("...");
        s++;
    }
    else
    {    
        p = NULL;
        for (;;)
        {
            switch (*s)
            {
                case 'Z':
                    p = str_cat2(p,",...");
                    break;
                case 0:
                err:
                    free(p);
                    p = NULL;
                    break;
                case '@':
                    break;
                default:
                    p = p ? str_cat2(p,",") : strdup(EMPTY);
                    p = str_cat(p,um_argument(&s));
                    continue;
            }
            break;
        }
        s++;
    }
    *ps = s;
    return str_bracket('(',p,')');
}

/**************************************
 */

STATIC char * LOCAL um_argument(char **ps)
{   
    char *s = *ps;
    char *t;

    if (isdigit(*s))
    {    
        int i;

        i = *s - '0';
        if (i >= um.argi)
            t = NULL;
        else
            t = str_dup(um.arg[i]);
        s++;
    }
    else
    {    
        char *a;

        a = um_data_type(&s,strdup(EMPTY));
        t = str_dup(a);
        if (a && um.argi < 10 && (s - *ps) > 1)
            um.arg[um.argi++] = a;
        else
            free(a);
    }
    *ps = s;
    return t;
}

/**************************************
 */

STATIC char * LOCAL um_data_type(char **ps,char *id)
{   
    char *s = *ps;
    char *t;
    char *p;
    char c;
    char c2;
    static char __near *basic[] =
    {    
        "signed char ",
        "char ",
        "unsigned char ",
        "short ",
        "unsigned short ",
        "int ",
        "unsigned ",
        "long ",
        "unsigned long ",
        "__segment ",
        "float ",
        "double ",
        "long double ",
    };

    t = NULL;
    c = *s++;
    switch (c)
    {
        case 'X':
            p = "void ";
            goto L1;
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
            p = basic[c - 'C'];
        L1:
            t = str_cat1(p,id);
            break;
        case '_':
            c = *s++;
            switch (c)
            {    
                case 'J':    
                    // long long
                    p = basic[7];    
                    goto L3;    
                case 'K':    
                    // unsigned long
                    p = basic[8];    
                    goto L3;    
                L3: 
                    t = str_cat12(p,basic[7]);
                    t = str_cat(t,id);
                    break;
#if 1
		// Digital Mars extensions

#if 0		// Don't need this because:
		/* C++98 8.3.2 References [dcl.ref]:
		 * "Cv-qualified references are ill-formed except when the
		 * cv-qualifiers are introduced through the use of a typedef
		 * (7.1.3) or of a template type argument (14.3), in which
		 * case the cv-qualifiers are ignored."
		 */

		case 'L':	// _L : const volatile reference type
		case 'M':	// _M : const reference type
		    t = um_indirect_type(&s);
		    t = str_cat(um_data_type(&s,strdup(EMPTY)),t);
		    t = str_cat2(t,"&const ");
		    if (c == 'L')
			t = str_cat2(t,"volatile ");
		    p = t;
		    goto L1;
#endif

		case 'O':	// _O : const
		    p = "const ";
		    goto L5;
		case 'P':	// _P : volatile
		    p = "volatile ";
		    goto L5;
		case 'Q':	// _Q : const volatile
		    p = "const volatile ";
		    goto L5;
		L5:
	            p = str_cat1(p, um_data_type(&s,strdup(EMPTY)));
		    goto L1;
#endif

		case 'R':	p = "_Imaginary float ";	goto L1;
		case 'S':	p = "_Imaginary double ";	goto L1;
		case 'T':	p = "_Imaginary long double ";	goto L1;
		case 'U':	p = "_Complex float ";		goto L1;
		case 'V':	p = "_Complex double ";		goto L1;
		case 'W':	p = "_Complex long double ";	goto L1;

		case 'N':			// new way
		case 'X':			// 8.1b8 and earlier way
		    p = "bool";
		    goto L1;
		case 'Y':
		    p = "wchar_t";
		    goto L1;
		case 'Z':
		    p = basic[12];
		    goto L1;
                default:    
                    goto L4;    // error
            }
            break;

        case 'P':
        case 'Q':
        case 'R':
        case 'S':
            c2 = *s;
            t = um_indirect_type(&s);
            if (c2 < '6' || c2 > '9')
                t = str_cat2(t,"*");
            if (c == 'Q' || c == 'S')
                t = str_cat2(t,"const ");
            if (c >= 'R')
                t = str_cat2(t,"volatile ");
            t = str_cat(t,id);
            if (c2 < '6' || c2 > '9')
                t = um_data_type(&s,t);
            break;

        /* ecsu_data_type    */
        case 'T':
        case 'U':
        case 'V':
        L2:
            t = um_ecsu_name(&s);
            t = str_cat2(t," ");
            t = str_cat(t,id);
            break;
        case 'W':
            s++;        /* ignore enum_type    */
            goto L2;

        case '?':    /* ecsu_data_indirect_type ecsu_data_type    */
            t = str_cat(um_indirect_type(&s),id);
            t = um_data_type(&s,t);
            break;

        case 'A':    /* reference_type            */
        case 'B':    /* volatile reference_type        */
            t = um_indirect_type(&s);
            t = str_cat(um_data_type(&s,strdup(EMPTY)),t);
            t = str_cat2(t,"&");
            if (c == 'B')
                t = str_cat2(t,"volatile ");
            t = str_cat(t,id);
            break;

        case 'Y':    /* array of                */
            t = um_array_type(&s,id);
            break;

        default:
        L4:
            free(id);
            break;
    } 
    *ps = s;
    return t;
}

/**************************************
 * Handle in parallel:
 *    ecsu_data_indirect_type
 *    data_indirect_type
 *    function_indirect_type
 */

STATIC char * LOCAL um_indirect_type(char **ps)
{   
    char *s = *ps;
    char *t = NULL;
    char *p;
    char c;
    int i;
    static char __near *ptr[4] = {"","far ","huge ","based " };

    c = *s++;
    if (isupper(c))
        i = c - 'A';
    else if (isdigit(c))
        i = c - '0' + 26;
    else
        goto ret;        /* error    */

    switch (c)
    {    
        case 'M':
        case 'N':
        case 'O':
        case 'P':
            goto ret;        /* not supported    */
        case '6':
        case '7':
            t = strdup((c & 1) ? "far *" : "*");
            goto L2;
        case '8':
        case '9':
            t = um_scope(&s);
            t = str_cat2(t,"::*");
        L2:
            t = str_bracket('(',t,')');
            t = um_function_type(&s,t,NULL);
            break;
        default:
            if (i >= 'Q' - 'A')
            {    
                t = um_scope(&s);
                t = str_cat2(t,"::");
            }
            else
                t = strdup(EMPTY);
            t = str_cat2(t,ptr[(i >> 2) & 3]);
            if (i & 1)
                t = str_cat2(t,"const ");
            if (i & 2)
                t = str_cat2(t,"volatile ");
            break;
    }

ret:
    *ps = s;
    return t;
}

/**************************************
 */

STATIC char * LOCAL um_type_encoding(char **ps,char *id)
{   
    char *s = *ps;
    char *t = NULL;
    char *r;
    char c;
    char *pthis = NULL;

#define U    0x100        /* _ prefix    */
#define D    0x200        /* _$ prefix    */

    c = *s++;
L1:
    switch (c)
    {    
        case '_':
            c = U | *s++;
            goto L1;
        case '$':
            c = D | *s++;
            goto L1;
        case U | '$':
            c = U | D | *s++;
            goto L1;

        default:        /* error    */
            break;

        case 'A':
        case 'B':
        case 'E':
        case 'F':
        case 'I':
        case 'J':
        case 'M':
        case 'N':
        case 'Q':
        case 'R':
        case 'U':
        case 'V':
            pthis = um_indirect_type(&s);
            goto L2;

        case 'C':
        case 'D':
        case 'K':
        case 'L':
        case 'S':
        case 'T':
        case 'Y':        /* near function    */
            goto L2;
        case 'Z':        /* far function        */
            id = str_cat1("far ",id);
        L2:
            t = um_function_type(&s,id,pthis);
            break;

        case '0':
        case '1':
        case '2':
        case '3':        /* external_data_type    */
            t = um_data_type(&s,id);
            t = str_cat(um_indirect_type(&s),t);    /* storage_convention    */
            break;

        case '6':        /* vftable_type        */
        case '7':        /* vbtable_type        */
            t = str_cat(um_indirect_type(&s),id);    /* storage_convention    */
            free(um_scope(&s));
            break;
    }
    *ps = s;
    return t;

#undef U
#undef D
}

STATIC char * LOCAL um_function_type(char **ps,char *id,char *pthis)
{   
    char *s = *ps;
    char *t;
    char *r;

    t = str_cat(um_calling_convention(&s),id);
    r = s;
    if (*r == '@')
        s++;
    else
        free(um_data_type(&s,strdup(EMPTY)));
    t = str_cat(t,um_argument_types(&s));
    if (pthis)
        t = str_cat(t,pthis);
    free(um_argument_types(&s));    /* ignore throw_type    */
    if (*r != '@')
        t = um_data_type(&r,t);
    *ps = s;
    return t;
}

STATIC char * LOCAL um_calling_convention(char **ps)
{   
    char *s = *ps;
    static char __near * cc[] =
    {    
        "cdecl ",
        "pascal ",
        "syscall ",
        "stdcall ",
        "fastcall ",
        "interrupt ",
    };
    int i;
    char *t;

    i = *s++ - 'A';
    if (i < 0 || i > ('K' - 'A'))
        t = NULL;
    else
    {
        t = strdup(cc[i >> 1]);
        if (i & 1)
            t = str_cat2(t,"saveregs ");
    }
    *ps = s;
    return t;
}

/*****************************
 * Unmangle a PT name.
 */

char *unmangle_pt(char **ps)
{
    /*    mangling ::= '$' template_name { type | expr }
    type ::= "T" mangled type
    expr ::= integer | string | address | float | double | long_double
    value ::= integer | string | address | float | double | long_double
    integer ::= "I" dimension
    string ::= "S" string
    address ::= "R" zname
    float ::= "F" hex_digits
    double ::= "D" hex_digits
    long_double ::= "L" hex_digits
     */
    char *s = *ps;
    char *t = NULL;
    char c;
    char *id;
    char first;
    char newmangle = 0;

    //printf("unmangle_pt()\n");
    c = *s;
    if (c == '?')
    {    
        newmangle = 1;
        c = *++s;
    }
    if (c != '$')
        return NULL;
    s++;

    um_init();

    t = um_zname(&s);

    // This handles scoped template names. Note that it only works
    // if the scoped name start is lower case.
    while ('a' <= *s && *s <= 'z')
    {
	char *p;
	p = um_zname(&s);
	if (!p)
	    return NULL;
        t = str_cat1("::",t);
        t = str_cat(p,t);
    }

    t = str_catc(t,'<');
    first = 1;
    while (1)
    {    
        unsigned long ul;
        char buf[sizeof(long) * 3 + 1 + 14];    /* 14 is for the %g format */
        char *p;
        int sz;
        long double ld;
        int i;
        char *a;

        if (newmangle)
        {
            switch (*s)
            {    
                case '$':
                    s++;
                    break;
                case '@':
                case 0:
                    goto done;
                default:
                    a = um_argument(&s);    // type
                    if (!a)
                        goto done;
                    goto L2;
            }
        }
        switch (*s++)
        {
            case 'T':                /* type        */
                a = um_argument(&s);
                break;
            case 'I':
            case '0':
                ul = um_dimension(&s);
		ul2a(buf, ul);
                a = strdup(buf);
                break;
#if 0 // Floating parameters no longer allowed
            case 'F': 
                sz = sizeof(float);    
                goto L1;
            case 'D': 
                sz = sizeof(double);    
                goto L1;
            case 'L': 
                sz = sizeof(long double); 
                goto L1;
            L1:
                p = (char *) &ld;
                for (i = 0; i < sz; i++)
                {   
                    unsigned char x;
                    char c;

                    c = *s++;
                    x = (c > '9') ? c - 'A' + 10 : c - '0';
                    c = *s++;
                    x |= ((c > '9') ? c - 'A' + 10 : c - '0') << 4;
                    *p++ = x;
                }
                switch (sz)
                {    
                    case sizeof(float):
                        ld = *(float *)&ld;
                        break;
                    case sizeof(double):
                        ld = *(double *)&ld;
                        break;
                }
                sprintf(buf,"%g",ld);
                a = strdup(buf);
                break;
#endif
            case 'S':
                p = um_string(&s);
                a = str_bracket('"',p,'"');
                break;
            case 'R':
                a = um_zname(&s);
                break;
            case '1':
                a = str_cat1("&",um_nestname(&s));
                break;
            case '@':
                s--;
                goto done;
            default:            /* assume end of PT    */
                goto done;
        }
        L2:    ;
        if (!first)
            t = str_catc(t,',');
        t = str_cat(t,a);
        first = 0;
    }
done:
    t = str_catc(t,'>');
    *ps = s;
    um_term();
    return t;
}

/****************************
 */

STATIC char * LOCAL um_array_type(char **ps,char *id)
{
    char *p;
    unsigned long ndims;

    p = strdup(EMPTY);
    ndims = um_dimension(ps);
    for (; ndims; ndims--)
    {    
        char buffer[1 + sizeof(unsigned long) * 3 + 1 + 1];
        unsigned long dim;

	if (**ps == 'X')		// DMC extension: VLA's
	{
	    p = str_catc(p, '*');
	    (*ps)++;
	}
	else
	{   char *s;

	    dim = um_dimension(ps);
	    ul2a(buffer, dim);
	    s = str_bracket('[', strdup(buffer), ']');
	    p = str_cat2(p,s);
	}
    }
    if (strchr(id,' '))
        id = str_bracket('(',id,')');
    p = str_cat1(um_data_type(ps,id),p);
    return p;
}

STATIC void LOCAL um_init()
{
    um.znamei = 0;
    um.argi = 0;
}

STATIC void LOCAL um_term()
{
    while (um.znamei)
        free(um.zname[--um.znamei]);
    while (um.argi)
        free(um.arg[--um.argi]);
}

STATIC char * LOCAL um_zname(char **ps)
{
    char *s = *ps;
    char *p;
    size_t len;

    if (!s)
        return NULL;
    if (isdigit(*s))
    {    
        int i;

        i = *s - '0';
        if (i >= um.znamei)
            return NULL;
        p = str_dup(um.zname[i]);
    }
    else
    {
        if (*s == '$' || (s[0] == '?' && s[1] == '$'))
        {   
            struct UM umsave = um;

            p = unmangle_pt(&s);
	    //printf("unmangle_pt = '%s', '%s'\n", p, s);
            um = umsave;
            if (p)
                goto L1;
        }
        while (*s != '@')
        {
            if (!*s)
                return NULL;
            s++;
        }
        len = s - *ps;
        p = (char *) calloc(len + 1,1);
        if (p)
        {   
            memcpy(p,*ps,len);
        L1:
            if (um.znamei < 10)
            {   
                char *z;

                z = str_dup(p);
                if (!z)
                {   
                    free(p);
                    p = NULL;
                }
                else
                    um.zname[um.znamei++] = z;
            }
        }
    }
    *ps = s + 1;
    return p;
}

STATIC char * LOCAL um_scope(char **ps)
{   
    char *s = *ps;
    char *t = NULL;

    while (*s != '@')
    {    
        if (!*s)                /* error    */
        {   
            free(t);
            t = NULL;
            break;
        }
        if (t)
        {   
            t = str_cat2(t,"::");
            t = str_cat(t,um_zname(&s));
        }
        else
            t = um_zname(&s);
        if (!t)
            break;
    }
    *ps = s + 1;
    return t;
}

STATIC char * LOCAL um_ecsu_name(char **ps)
{
    return um_scope(ps);
}

STATIC unsigned long LOCAL um_dimension(char **ps)
{   
    char *s = *ps;
    char c;
    unsigned long ul;

    c = *s++;
    if (isdigit(c))
        ul = c - '0' + 1;
    else
    {
        ul = 0;
        while (c != '@')
        {
            c -= 'A';
            if (c & ~0x0F)
                break;            // error
            ul <<= 4;
            ul |= c;
            c = *s++;
        }
    }

    *ps = s;
    return ul;
}

STATIC char * LOCAL um_string(char **ps)
{   
    char *s = *ps;
    char *p;
    char c;

    p = strdup(EMPTY);
    while (1)
    {
        c = *s++;
        switch (c)
        {
            case 0:            // string is not recognized
            err:
                free(p);
                p = NULL;
                goto ret;
            case '@':            // end of string
                goto ret;
            case '?':
                c = *s++;
                if (c == '$')
                {   
                    char x;

                    c = *s++ - 'A';
                    if (c & ~0x0F)
                        goto err;
                    x = c << 4;
                    c = *s++ - 'A';
                    if (c & ~0x0F)
                        goto err;
                    x |= c;
                    p = str_catc(p,x);
                }
                else if (isdigit(c))
                {   
                    static char special_char[10] = ",/\\:. \n\t'-";

                    p = str_catc(p,special_char[c - '0']);
                }
                else if (isalpha(c))
                    p = str_catc(p,0x80 + c);
                else
                    goto err;
                break;
            default:
                if (isalnum(c))
                    p = str_catc(p,c);
                else
                    goto err;
                break;
        }
    }
ret:
    *ps = s;
    return p;
}


// Needed to unmangle the VC++ exception types
// as VC mangles their typeinfo
char *__unmangle_vc_exception_type(const char *p) 
{
    char *ps=(char*)p;
    char *retval;
    int len;

    um_init();
    retval= um_data_type(&ps,str_dup(EMPTY));
    um_term();

    // strip off trailing spaces
    len=strlen(retval);
    if (len>0 && retval[len-1]==' ') 
        retval[len-1]=0;

    return retval;
}
  

/*********************************
 * Expand compressed identifier.
 * Returns:
 *	malloc'd expanded identifier
 *	NULL on error
 */

STATIC char * LOCAL id_expand(char *p, int plen)
{
    char *id;
    char *idbuf;
    char tmp[2048];

    int i;
    int idi;
    int idmax;

    //printf("id_expand()\n");
    idi = 0;
    idmax = sizeof(tmp);
    idbuf = tmp;

    for (i = 0; i < plen; i++)
    {
	unsigned char c;
	int off;
	int len;

	if (idi + 2 >= idmax)
	{
	    idmax += 1024;
	    id = (char *)alloca(idmax);
	    if (!id)
		goto Lerr;
	    memcpy(id, idbuf, idi);
	    idbuf = id;
	}

	c = p[i];
	if (!(c & 0x80))
	{   idbuf[idi++] = c;
	    continue;
	}
	if ((c & 0xC0) == 0xC0)
	{
	    len = (c & 7) + 1;
	    off = ((c >> 3) & 7) + 1;
	}
	else
	{
	    len = (p[i + 1] & 0x7F) | ((c & 0x38) << 4);
	    off = (p[i + 2] & 0x7F) | ((c & 7) << 7);
	    i += 2;
	}

	if (off < len || idi < off)
	    goto Lerr;
	if (idi + len + 1 >= idmax)
	{
	    idmax += len + 1024;
	    id = (char *)alloca(idmax);
	    if (!id)
		goto Lerr;
	    memcpy(id, idbuf, idi);
	    idbuf = id;
	}
	//printf("len = %2d, -off = %d\n", len, off);
	memcpy(idbuf + idi, idbuf + idi - off, len);
	idi += len;
    }
    idbuf[idi] = 0;
    return strdup(idbuf);

Lerr:
    //printf("id_expand() : error\n");
    return NULL;
}


#if 0

/* Some samples to try */

void main()
{
    static char __near * name[] =
    {
#if 0
        "?_AfxVBCreateHsz@@ZCPEPM1DIPFD@Z",
        "?messageMap@CWinApp@@1?AUAFX_MSGMAP@@A",
        "?wndTop@CWnd@@2?BV1@B",
        "?messageMap@CFrameWnd@@1?AUAFX_MSGMAP@@A",
        "?messageMap@CToolBar@@1?AUAFX_MSGMAP@@A",
        "?FromHandle@CWnd@@TCPEV1@PBUHWND__@@@Z",
        "?v1@@3QAY0P@HA",
        "?v2@@3QEY1CP@2HE",
        "?v3@@3QBY100HB",
        "?v4@@3HA",
        "?v5@@3PAHA",
        "?v6@@3PAPBHA",
        "?v7@@3PAPAPBHA",
        "?v8@@3PBHB",
        "?v9@@3QAHA",
        "?v10@@3HB",
        "?func@@YA?AUabc@@XZ",
        "?r1@@3AAHA",
        "?p1@@3PAPAPAPEHA",
        "?p2@@3PAHA",
        "?p3@@3PAPAPAPEHA",
        "?p4@@3PEHE",
        "?p5@@3PAPEHA",
        "?p6@@3PEPAHE",
        "?x@@3HE",
        "??_7abc@@6B@",
        "??0abc@@QAC@XZ",
        "??1abc@@QAC@XZ",
        "?func1@abc@@QACHH@Z",
        "?func2@abc@@QCCHH@Z",
        "?func3@abc@@SAHHH@Z",
        "?func4@abc@@UACHPAPAH0@Z",
        "?foopm1@@YAXP8abc@@CHH@Z@Z",
        "?foopm2@@YAXPQabc@@H@Z",
        "?test@@YCXPAV$ABC@TNICP@@@@Z",
        "?test@@YCXPAV$ABC@TNICP@@@0@Z",
        "??1CWinApp@@VEC@XZ",
        "??1CDialog@@VEC@XZ",
        "??0CDialog@@REC@IPEVCWnd@@@Z",
        "?OnIdle@CWinApp@@VECHJ@Z",
        "?FromHandle@CWnd@@TCPEV1@PBUHWND__@@@Z",
        "?SetDialogBkColor@CWinApp@@JECXKK@Z",
        "?WinHelp@CWinApp@@VECXKI@Z",
        "?OnDDECommand@CWinApp@@VECHPED@Z",
        "?DoWaitCursor@CWinApp@@VECXH@Z",
        "?ProcessWndProcException@CWinApp@@VECJPEVCException@@PFUtagMSG@@@Z",
        "?ProcessMessageFilter@CWinApp@@VECHHPEUtagMSG@@@Z",
        "?DoMessageBox@CWinApp@@VECHPFDII@Z",
        "?ExitInstance@CWinApp@@VECHXZ",
        "?SaveAllModified@CWinApp@@VECHXZ",
        "?PreTranslateMessage@CWinApp@@VECHPEUtagMSG@@@Z",
        "?Run@CWinApp@@VECHXZ",
        "?InitApplication@CWinApp@@VECHXZ",
        "?AddToRecentFileList@CWinApp@@VECXPFD@Z",
        "?OpenDocumentFile@CWinApp@@VECPEVCDocument@@PFD@Z",
        "?OnCmdMsg@CCmdTarget@@VECHIHPEXPEUAFX_CMDHANDLERINFO@@@Z",
        "?Dump@CObject@@VFCXAAVCDumpContext@@@Z",
        "?AssertValid@CObject@@VFCXXZ",
        "?GetRuntimeClass@CWinApp@@VFCPEUCRuntimeClass@@XZ",
        "??0CWinApp@@REC@PFD@Z",
        "??1CString@@REC@XZ",
        "?SetPaneText@CStatusBar@@RECHHPFDH@Z",
        "??4CString@@RECABV0@ABV0@@Z",
        "??H@ZC?AVCString@@ABV0@PFD@Z",
        "??H@ZC?AVCString@@PFDABV0@@Z",
        "?GetBufferSetLength@CString@@RECPEDH@Z",
        "??0CString@@REC@XZ",
        "?SetButtonInfo@CToolBar@@RECXHIIH@Z",
        "?SetButtons@CToolBar@@RECHPFIH@Z",
        "?SetPaneInfo@CStatusBar@@RECXHIIH@Z",
        "?GetPaneInfo@CStatusBar@@RFCXHAAI0AAH@Z",
        "?SetIndicators@CStatusBar@@RECHPFIH@Z",
        "?Create@CStatusBar@@RECHPEVCWnd@@KI@Z",
        "?Attach@CGdiObject@@RECHPBX@Z",
        "?Create@CComboBox@@RECHKABUtagRECT@@PEVCWnd@@I@Z",
        "?LoadBitmap@CToolBar@@RECHPFD@Z",
        "?Create@CToolBar@@RECHPEVCWnd@@KI@Z",
        "?Create@CDialogBar@@RECHPEVCWnd@@PFDII@Z",
        "?OnCreate@CFrameWnd@@JECHPEUtagCREATESTRUCT@@@Z",
        "?AfxRegisterWndClass@@ZCPFDIPBUHICON__@@PBUHBRUSH__@@0@Z",
        "_afxCurrentResourceHandle",
        "_afxCurrentWinApp",
        "?OnUpdateFrameTitle@CFrameWnd@@VECXH@Z",
        "?GetMessageBar@CFrameWnd@@VECPEVCWnd@@XZ",
        "?OnCreateClient@CFrameWnd@@NECHPEUtagCREATESTRUCT@@PEUCCreateContext@@@Z",
        "?OnSetPreviewMode@CFrameWnd@@VECXHPEUCPrintPreviewState@@@Z",
        "?ActivateFrame@CFrameWnd@@VECXH@Z",
        "?RecalcLayout@CFrameWnd@@VECXXZ",
        "?GetActiveDocument@CFrameWnd@@VECPEVCDocument@@XZ",
        "?LoadFrame@CFrameWnd@@VECHIKPEVCWnd@@PEUCCreateContext@@@Z",
        "?OnChildNotify@CWnd@@NECHIIJPEJ@Z",
        "?PostNcDestroy@CFrameWnd@@NECXXZ",
        "?DefWindowProc@CWnd@@NECJIIJ@Z",
        "?WindowProc@CWnd@@NECJIIJ@Z",
        "?PreTranslateMessage@CFrameWnd@@NECHPEUtagMSG@@@Z",
        "?GetSuperWndProcAddr@CWnd@@NECPEP7CJPBUHWND__@@IIJ@ZXZ",
        "?OnCommand@CFrameWnd@@NECHIJ@Z",
        "?GetScrollBarCtrl@CWnd@@VFCPEVCScrollBar@@H@Z",
        "?CalcWindowRect@CWnd@@VECXPEUtagRECT@@@Z",
        "?DestroyWindow@CWnd@@VECHXZ",
        "?Create@CWnd@@VECHPFD0KABUtagRECT@@PEV1@IPEUCCreateContext@@@Z",
        "?OnCmdMsg@CFrameWnd@@VECHIHPEXPEUAFX_CMDHANDLERINFO@@@Z",
        "?GetRuntimeClass@CFrameWnd@@VFCPEUCRuntimeClass@@XZ",
        "??1CWnd@@VEC@XZ",
        "??_7CFrameWnd@@6B@",
        "??1CStatusBar@@VEC@XZ",
        "??1CToolBar@@VEC@XZ",
        "??1CComboBox@@VEC@XZ",
        "??1CGdiObject@@VEC@XZ",
        "??_7CFont@@6B@",
        "??1CDialogBar@@VEC@XZ",
        "??0CDialogBar@@REC@XZ",
        "??_7CGdiObject@@6B@",
        "??_7CComboBox@@6B@",
        "??_7CWnd@@6B@",
        "??_7CCmdTarget@@6B@",
        "??_7CObject@@6B@",
        "??0CToolBar@@REC@XZ",
        "??0CStatusBar@@REC@XZ",
        "??0CFrameWnd@@REC@XZ",
        "?HitTest@CToolBar@@NECHVCPoint@@@Z",
        "?GetItemRect@CToolBar@@VFCXHPEUtagRECT@@@Z",
        "?OnUpdateCmdUI@CToolBar@@NECXPEVCFrameWnd@@H@Z",
        "?DoPaint@CToolBar@@NECXPEVCDC@@@Z",
        "?PostNcDestroy@CControlBar@@NECXXZ",
        "?WindowProc@CControlBar@@NECJIIJ@Z",
        "?PreTranslateMessage@CControlBar@@NECHPEUtagMSG@@@Z",
        "?OnCommand@CWnd@@NECHIJ@Z",
        "?PreCreateWindow@CWnd@@VECHAAUtagCREATESTRUCT@@@Z",
        "?GetMessageMap@CToolBar@@NFCPEUAFX_MSGMAP@@XZ",
        "?GetRuntimeClass@CToolBar@@VFCPEUCRuntimeClass@@XZ",
        "?OnLButtonUp@CToolBar@@JECXIVCPoint@@@Z",
        "??1CDC@@VEC@XZ",
        "?Detach@CDC@@RECPBUHDC__@@XZ",
        "?Attach@CDC@@RECHPBUHDC__@@@Z",
        "??0CDC@@REC@XZ",
        "?OnMouseMove@CToolBar@@JECXIVCPoint@@@Z",
        "?OnLButtonDown@CToolBar@@JECXIVCPoint@@@Z",
        "??_7CBrush@@6B@",
        "?EndDrawButton@CToolBar@@JECXAAUDrawState@1@@Z",
        "?DrawButton@CToolBar@@JECHPBUHDC__@@HHHI@Z",
        "?GetButtonInfo@CToolBar@@RFCXHAAI0AAH@Z",
        "?PrepareDrawButton@CToolBar@@JECHAAUDrawState@1@@Z",
        "?DoPaint@CControlBar@@NECXPEVCDC@@@Z",
        "?CreateEx@CWnd@@JECHKPFD0KHHHHPBUHWND__@@PBUHMENU__@@PED@Z",
        "?SetSizes@CToolBar@@RECXUtagSIZE@@0@Z",
        "?lib_int86@@YAHHPATREGS@@H@Z",
        "?lib_int86@@YAHHPATREGS@@0@Z",
	"??$f@ÙL@UáCCCö@VñFFÉÛä@DUB1234567890€ŠŠ€””€‰Š@@0ÑDÑHÑ€ƒ×E€ßãÉ€„ÚD‰ÅÉÉ‹È“@QAEXƒã˜€ƒ‰‹ËþZ",
        "?$TST@H",
#endif
	"??0sentry@?$basic_ostream@std@DV?$char_traits@std@D@1@@std@@QAE@AAV12@@Z",
	"??Bsentry@?$basic_ostream@std@DV?$char_traits@std@D@1@@std@@QBE_NXZ",
    };
    char *p;
    int i;

    for (i = 0; i < sizeof(name)/sizeof(name[0]); i++)
    {
        p = unmangle_ident(name[i]);
        printf("%s\t->\t'%s'\n",name[i],p);
    }
}

#endif
