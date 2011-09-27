#include <stdlib.h>

_CRTIMP int * __cdecl __p___argc (void)
{
    return &__argc;
}

_CRTIMP char *** __cdecl __p___argv (void)
{
    return &__argv;
}

_CRTIMP wchar_t *** __cdecl __p___wargv (void)
{
    return &__wargv;
}
