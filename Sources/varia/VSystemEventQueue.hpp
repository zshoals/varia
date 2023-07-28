#pragma once

#include "varia/VShared.hpp"
#include "varia/input/VEventMoveRight.hpp"

enum class E_System_Event_Type
{
    Gameplay_Move_Right_Pressed,
    Gameplay_Move_Right_Released,
    Gameplay_Move_Left_Pressed,
    Gameplay_Move_Left_Released,

    System_Window_Vertical_Sync_Enable,
    System_Window_Vertical_Sync_Disable,
    System_Window_Lost_Focus,
    System_Window_Gained_Focus,
    System_Window_Request_Fullscreen,
    System_Window_Request_Windowed,
    System_Window_Request_Resize,
};

struct System_Event
{
    E_System_Event_Type tag;
    union
    {
        Event_Move_Right_Pressed_Data move_right_pressed_data;
        Event_Move_Right_Released_Data move_right_released_data;
    };
};


constexpr Integer_64 SYSTEM_EVENT_QUEUE_MAX_SIZE = 128;
struct System_Event_Queue
{
    System_Event queue[SYSTEM_EVENT_QUEUE_MAX_SIZE];
    Integer_64 item_count;
    Integer_64 current_head;
};

Boolean v_system_event_queue_has_events(System_Event_Queue const * events);
System_Event v_system_event_queue_get_first_event(System_Event_Queue const * events);
void v_system_event_queue_push(System_Event_Queue * events, System_Event new_event);
void v_system_event_queue_pop(System_Event_Queue * events);
void v_system_event_queue_clear(System_Event_Queue * events);
