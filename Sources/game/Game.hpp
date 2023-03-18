#pragma once

struct varia_engine_context_t;

void game_simulate(varia_engine_context_t * engine, double dt);
void game_render(varia_engine_context_t * engine, double dt);