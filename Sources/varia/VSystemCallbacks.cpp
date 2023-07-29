#include "varia/VSystemCallbacks.hpp"

#include "varia/VShared.hpp"
#include "varia/ds/VDS-ResetQueue.hpp"
#include "varia/VSystemEvents.hpp"

void v_system_callback_focus_lost(void * data)
{
    VDS_Reset_Queue<E_System_Event_Type> * events = static_cast<VDS_Reset_Queue<E_System_Event_Type> *>(data);

    E_System_Event_Type e = E_System_Event_Type::System_Window_Lost_Focus;
    vds_reset_queue_push(events, e);
}

void v_system_callback_focus_gained(void * data)
{
    VDS_Reset_Queue<E_System_Event_Type> * events = static_cast<VDS_Reset_Queue<E_System_Event_Type> *>(data);

    E_System_Event_Type e = E_System_Event_Type::System_Window_Gained_Focus;
    vds_reset_queue_push(events, e);
}