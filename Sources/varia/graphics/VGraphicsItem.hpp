#pragma once

struct Graphics_Renderable
{
    float x;
    float y;
    //NOTE(<zshoals> 08-09-2023): Layer is duplicated to avoid having to somehow get 
    //  this value from Graphics_Item_Sortable later
    float layer;
    float w;
    float h;
    //Note: before submitting the graphics item, we must resolve the uv coordinates
    float u_min;
    float v_min;
    float u_max;
    float v_max;
    float r;
    float g;
    float b;
    float a;
};

struct Graphics_Renderable_Sortable
{
    float layer;
    Graphics_Renderable const * item;
};
