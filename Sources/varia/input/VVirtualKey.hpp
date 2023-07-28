#pragma once

typedef int Kinc_Keycode;

struct Virtual_Key
{
    Kinc_Keycode bound_key;
    Boolean requires_shift;
    Boolean requires_control;
    Boolean requires_alt;
};

//NOTE(<zshoals> 07-27-2023): Componentizing this action immediately removed
//  all need for the templating??? WTF?
//  just pass in the virtual action alone and it works I think????