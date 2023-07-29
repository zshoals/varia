#pragma once

enum class E_System_Event_Type
{
    System_Window_Vertical_Sync_Enable,
    System_Window_Vertical_Sync_Disable,
    System_Window_Lost_Focus,
    System_Window_Gained_Focus,
    System_Window_Request_Fullscreen,
    System_Window_Request_Windowed,
    System_Window_Request_Resize,
};
