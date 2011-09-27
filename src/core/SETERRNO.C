#include <scrtl.h>
#include <errno.h>
#include <stdlib.h>
#include <mt.h>

void __pascal __set_errno (int an_errno)
{
#if defined(_MT)
    struct THREAD *my_thread = _getthreaddata ();
    
    /* If getthreaddata fails, then we really cannot set anything */
    if (!my_thread)
    {
        return;
    }
    my_thread->t_errno = an_errno;
    my_thread->t_doserrno = 0;
#else
    errno = an_errno;
    _doserrno = 0;
#endif
}
