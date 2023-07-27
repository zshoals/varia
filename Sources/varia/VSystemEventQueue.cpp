#include "varia/VSystemEventQueue.hpp"

#include "varia/VShared.hpp"

Boolean v_system_event_queue_has_events(System_Event_Queue const * events)
{
    return (events->item_count > 0);
}

System_Event v_system_event_queue_get_first_event(System_Event_Queue const * events)
{
    return events->queue[events->current_head];
}

void v_system_event_queue_push(System_Event_Queue * events, System_Event new_event)
{
    VARIA_ASSERT(events->item_count <= SYSTEM_EVENT_QUEUE_MAX_SIZE, "Event queue exceeded max size.");
    events->queue[events->item_count] = new_event;
    events->item_count += 1;
}

void v_system_event_queue_pop(System_Event_Queue * events)
{
    VARIA_ASSERT(events->current_head <= SYSTEM_EVENT_QUEUE_MAX_SIZE, "Event queue head overrun; didn't reset queue?");
    events->current_head += 1;
}

void v_system_event_queue_clear(System_Event_Queue * events)
{
    events->current_head = 0;
    events->item_count = 0;
}