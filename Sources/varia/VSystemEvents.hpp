#pragma once

#include "varia/VShared.hpp"
#include "varia/ds/VDS-ResetQueue.hpp"

enum class E_System_Event_Type
{
    System_Window_Vertical_Sync_Enable,
    System_Window_Vertical_Sync_Disable,
    System_Window_Lost_Focus,
    System_Window_Gained_Focus,
    System_Window_Request_Fullscreen,
    System_Window_Request_Windowed,
    System_Window_Request_Resize,

    System_Window_Attempting_Changes,
    System_Window_Discarding_Changes,
    System_Window_Apply_Changes,
};

using System_Event_Queue = VDS_Reset_Queue<E_System_Event_Type, 128>;
struct System_Events
{
    System_Event_Queue events;
};

void v_system_events_initialize(System_Events * events);
void v_system_events_push(System_Events * events, E_System_Event_Type e);

template<typename FUNC>
void v_system_events_process(System_Events * system_events, FUNC f)
{
    System_Event_Queue * events_location = address_of(system_events->events);

    while (vds_reset_queue_has_elements(events_location))
    {
        E_System_Event_Type e = vds_reset_queue_get_first_element(events_location);
        f(e);
        vds_reset_queue_pop(events_location);
    }

    vds_reset_queue_reset(events_location);
}