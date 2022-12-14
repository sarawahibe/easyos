#include "battclock_intern.h"

/*****************************************************************************
    NAME */
#include <proto/battclock.h>

        easyos_LH1(void, WriteBattClock,

/*  SYNOPSIS */
        easyos_LHA(ULONG, time, D0),

/*  LOCATION */
        APTR *, BattClockBase, 3, Battclock)

/*  FUNCTION
        Set the system's battery backed up clock to the time specified. The
        value should be the number of seconds since 00:00:00 on 1.1.1978.
    INPUTS
        time - The number of seconds elapsed since 00:00:00 1.1.1978
    RESULT
        The clock will be set.
    NOTES
        This may not do anything on some systems where the battery backed
        up clock either doesn't exist, or may not be writable.
    EXAMPLE
    BUGS
    SEE ALSO
        ReadBattClock(), ResetBattClock()
    INTERNALS
*****************************************************************************/
{
    easyos_LIBFUNC_INIT

    return;

    easyos_LIBFUNC_EXIT
} /* WriteBattClock */
Footer
