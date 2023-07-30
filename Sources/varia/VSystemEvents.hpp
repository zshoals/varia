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

struct System_Events
{
    #define SYSTEM_EVENT_QUEUE_MAX_SIZE 128
    VDS_Reset_Queue_Storage<E_System_Event_Type, SYSTEM_EVENT_QUEUE_MAX_SIZE> events;
};

void v_system_events_initialize(System_Events * events);
void v_system_events_push(System_Events * events, E_System_Event_Type e);

template<typename FUNC>
void v_system_events_process(System_Events * system_events, FUNC f)
{
    VDS_Reset_Queue<E_System_Event_Type> events = vds_reset_queue_make_interface(address_of(system_events->events));
    VDS_Reset_Queue<E_System_Event_Type> * events_location = address_of(events);

    while (vds_reset_queue_has_elements(events_location))
    {
        E_System_Event_Type e = vds_reset_queue_get_first_element(events_location);
        f(e);
        vds_reset_queue_pop(events_location);
    }

    vds_reset_queue_reset(events_location);
}