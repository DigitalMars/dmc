#include <ctype.h>
#include <string.h>
#include <fltpnt.h>
#include <math.h>
#include <iosold\iostream.h>
#ifdef macintosh
#include <stdlib.h>
#endif
#include <typeinfo.h>

static char *double_ok = "+-.0123456789AEFINSTYe";

static int double_tt[] = {
2,3,4,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,0,0,0,6,7,0,0,0,0,
0,0,4,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,0,0,0,6,7,0,0,0,0,
0,0,4,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,0,0,0,6,7,0,0,0,0,
0,0,0,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,0,0,0,0,0,0,0,0,0,
0,0,-9,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,0,11,0,0,0,0,0,0,12,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,
0,0,0,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,0,11,0,0,0,0,0,0,12,
0,0,0,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,0,11,0,0,0,0,0,0,12,
0,0,-9,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,0,11,0,0,0,0,0,0,12,
16,17,0,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,0,0,0,0,0,0,0,0,0,
16,17,0,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-19,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-20,0,0,0,0,
0,0,0,-15,-15,-15,-15,-15,-15,-15,-15,-15,-15,0,11,0,0,0,0,0,0,12,
0,0,0,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,0,0,0,0,0,0,0,0,0,
0,0,0,-18,-18,-18,-18,-18,-18,-18,-18,-18,-18,0,0,0,0,0,0,0,0,0,
0,0,0,-21,-21,-21,-21,-21,-21,-21,-21,-21,-21,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-23,0,0,0,
0,0,0,-24,-24,-24,-24,-24,-24,-24,-24,-24,-24,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-28,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int fhf(int n, char c, int *fpt)
{
	if (!n)
		*fpt = FP_NORMAL;
	switch (c) {
	case 'A':
		*fpt = FP_NANQ;
		return 0;
	case 'S':
		*fpt = FP_NANS;
		return 0;
	case 'I':
		*fpt = FP_INFINITE;
		return 0;
	default:
	    return 0;
	}
}

static void fxf(double *t, const char *s, int* ti)
{
	switch (*ti) {
	case FP_NANQ:
		*t = NAN;
		return;
	case FP_NANS:
		*t = NANS;
		return;
	case FP_INFINITE:
		*t = (*s == '-')? -INFINITY: INFINITY;
		return;
	default:
		*t = atof(s);
		return;
	}
}

istream &istream::operator>>(float &f)
{
	double d;
    int fpt;
    extract(&d,double_ok,double_tt,(translate_function_t) fxf,
				(helper_function_t) (fhf),&fpt);
	f = d;
	return *this;
}
	

istream &istream::operator>>(double &a)
{
    int fpt;
    extract(&a,double_ok,double_tt,(translate_function_t) fxf,
				(helper_function_t)(fhf),&fpt);
	return *this;
}

#if macintosh
istream &istream::operator>>(long double &a)
{
    int fpt;
    extract(&a,double_ok,double_tt,(translate_function_t) fxf,
				(helper_function_t) fhf,&fpt);
	return *this;
}
#endif
