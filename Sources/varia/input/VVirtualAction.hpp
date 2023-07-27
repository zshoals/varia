#pragma once

typedef int Kinc_Keycode;

template <typename DATATYPE>
struct Virtual_Action
{
    Kinc_Keycode bound_key;
    Boolean requires_shift;
    Boolean requires_control;
    Boolean requires_alt;
    void (*on_keydown)(DATATYPE * data);
    void (*on_keyup)(DATATYPE * data);
    DATATYPE data;
};
