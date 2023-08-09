#include "varia/VSimulation.hpp"
#include "varia/VGamestate.hpp"

void v_simulation_simulate(Gamestate * gs, E_Simulating simulating, E_Simulation_Mode mode)
{
    if (simulating == E_Simulating::Pause)
    {
        return;
    }

    //TODO(<zshoals> 07-27-2023): Everything
}