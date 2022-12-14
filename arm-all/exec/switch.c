#include <exec/execbase.h>
#include <proto/exec.h>

#define DEBUG 0
#include <easyos/debug.h>

#include "arm_exec_internal.h"

easyos_LH0(void, Switch,
    struct ExecBase *, SysBase, 9, Exec)
{
    easyos_LIBFUNC_INIT

    struct Task *this = SysBase->ThisTask;

    /*
        If the state is not TS_RUN then the task is already in a list
    */
    Disable();
    
    if( (this->tc_State != TS_RUN)
         && !(this->tc_Flags & TF_EXCEPT) )
    {
        /*      Its quite possible that they have interrupts Disabled(),
            we should fix that here, otherwise we can't switch. 
            We can't call the dispatcher because we need a signal,
            lets just create one.
            Have to set the dispatch-required flag.
            I use SIGUSR1 (maps to SoftInt) because it has less effect on
            the system clock, and is probably quicker.
        */

        SysBase->AttnResched |= 0x8000;
        __asm__ __volatile__ ("swi #0\n\t");
    }

    Enable();
    
    easyos_LIBFUNC_EXIT
} /* Switch() */
