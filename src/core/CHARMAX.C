/*_ charmax.c  */


/*
    By default, the data returned from localeconv is filled
    with values of CHAR_MAX. When -J is not used, this value
    is 127. However, if the user compiles with -J, then they
    should get a value of 255 returned. To accomodate for this,
    in locale.h, a comdef is declared for __charmax. An external
    reference to __charmax is in setlc.c and lconv.c. If the comdef 
    is pulled in, then the linker will never pull in this definition, 
    thus __charmax, will be 0. This will inform the code to fixup the
    data tables to fill in values of 255 for CHAR_MAX values.

    If this gets pulled in, then the lconv structure returned
    from localeconv will not get initialized with charmax values
    of 255. 
*/
int __charmax = 1;


