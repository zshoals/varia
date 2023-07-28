#pragma once

typedef int Kinc_Keycode;

struct Virtual_Key
{
    Kinc_Keycode bound_key;
    Boolean requires_shift;
    Boolean requires_control;
    Boolean requires_alt;
};
