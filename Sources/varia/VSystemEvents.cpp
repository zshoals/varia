#include "varia/VSystemEvents.hpp"

#include "kinc/system.h"

static void v_system_callback_focus_lost(void * data)
{
    System_Events * system_events = static_cast<System_Events *>(data);
    v_system_events_push(system_events, E_System_Event_Type::System_Window_Lost_Focus);
}

static void v_system_callback_focus_gained(void * data)
{
    System_Events * system_events = static_cast<System_Events *>(data);
    v_system_events_push(system_events, E_System_Event_Type::System_Window_Gained_Focus);
}



void v_system_events_initialize(System_Events * events)
{
    //TODO(<zshoals> 07-30-2023): This is bad; we're setting global state
    //  via a non-static/non-global object...
    //  We could reset all of these callbacks every time we process the event loop, that way we
    //  could "properly" support multiple system event loops...not like that's ever going to be something
    //  we need to do...
    kinc_set_background_callback(&v_system_callback_focus_lost, events);
    kinc_set_foreground_callback(&v_system_callback_focus_gained, events);
}

void v_system_events_push(System_Events * system_events, E_System_Event_Type e)
{
    VDS_Reset_Queue<E_System_Event_Type> events = vds_reset_queue_make_interface(address_of(system_events->events));
    vds_reset_queue_push(address_of(events), e);
}