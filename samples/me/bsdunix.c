
/* Stuff specific to BSDUNIX */

memset(p,c,n)
register char *p,c;
register unsigned n;
{
	while (n--)
		*p++ = c;
}
