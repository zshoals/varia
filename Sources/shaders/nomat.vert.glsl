#version 450

in vec2 pos;

//x = 1920, y = 1080
in vec2 resolution;

in vec2 camera_position;
in vec2 camera_viewport;
in float camera_zoomfactor;
in float camera_rotation;

in float layer;

void main()
{

    pos += -camera_position;
    
    float cos_angle = cos(camera_rotation);
    float sin_angle = sin(camera_rotation);
    pos.x = (pos.x * cos_angle) - (pos.y * sin_angle);
    pos.y = (pos.x * sin_angle) + (pos.y * cos_angle);

    vec2 adjusted_pos = pos / camera_viewport * vec2(camera_zoomfactor);
    vec2 screen_ratio = vec2(1.0) / resolution;
    adjusted_pos *= screen_ratio;

    float max_layers = 4096.0;
    gl_position = vec4(adjusted_pos, (layer / max_layers), 1.0);
}