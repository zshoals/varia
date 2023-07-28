#pragma once

typedef int Kinc_Keycode;

struct Virtual_Key
{
    Kinc_Keycode bound_key;
    //TODO(<zshoals> 07-28-2023): Read below
    // Kinc_Keycode default_key; //Probably a good idea to have this
    Boolean requires_shift;
    Boolean requires_control;
    Boolean requires_alt;
};
