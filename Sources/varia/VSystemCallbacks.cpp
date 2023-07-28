#include "varia/VSystemCallbacks.hpp"
#include "varia/VSystemEventQueue.hpp"

void v_system_callback_focus_lost(void * data)
{
    System_Event_Queue * events = static_cast<System_Event_Queue *>(data);

    System_Event e = ZERO_INIT();
    {
        e.tag = E_System_Event_Type::System_Window_Lost_Focus;
    }

    v_system_event_queue_push(events, e);
}

void v_system_callback_focus_gained(void * data)
{
    System_Event_Queue * events = static_cast<System_Event_Queue *>(data);

    System_Event e = ZERO_INIT();
    {
        e.tag = E_System_Event_Type::System_Window_Gained_Focus;
    }

    v_system_event_queue_push(events, e);
}