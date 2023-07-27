#include "VGameloop.hpp"

#include "varia/VShared.hpp"
#include "varia/VGameContext.hpp"
#include "varia/VSystemEventQueue.hpp"
#include "varia/VSimulation.hpp"
#include "varia/VGamestateQuery.hpp"
#include "varia/utility/VMemcpy.hpp"
#include "varia/input/VInput.hpp"

#include "kinc/system.h"
#include "kinc/log.h"


//Game System Imports

static void v_system_callback_focus_lost(void * data)
{
    System_Event_Queue * events = static_cast<System_Event_Queue *>(data);

    System_Event e = ZERO_INIT();
    {
        e.tag = E_System_Event_Type::System_Window_Lost_Focus;
    }

    v_system_event_queue_push(events, e);
}

static void v_system_callback_focus_gained(void * data)
{
    System_Event_Queue * events = static_cast<System_Event_Queue *>(data);

    System_Event e = ZERO_INIT();
    {
        e.tag = E_System_Event_Type::System_Window_Gained_Focus;
    }

    v_system_event_queue_push(events, e);
}

static void v_print_timing_info(Gamestate * gamestate)
{
    // kinc_log(KINC_LOG_LEVEL_INFO, "Player X:    %f", gs->gamedata.player.x);

    kinc_log(KINC_LOG_LEVEL_INFO, 
        ":::::::::::::::::::\n"
        "Cycles:      %d\n"
        "RenderFrms:  %d\n"
        "FixDelta:    %f\n"
        "FixRecip:    %f\n"
        "RenderDelta  %f\n"
        "LastRender:  %f\n"
        "Gametime:    %f\n"
        "VarGametime: %f\n"
        "Update Time: %f\n"
        "Real Time:   %f\n"
        ":::::::::::::::::::\n",
        gamestate->frame_cycles,
        gamestate->rendered_frames,
        gamestate->logic_dt,
        1.0 / gamestate->logic_dt,
        gamestate->render_dt,
        gamestate->previous_rendertime,
        gamestate->logic_cumulative_gameclock,
        gamestate->render_cumulative_gameclock,
        gamestate->total_realtime_fixed_update_time,
        kinc_time()
    );
}

static void v_gameloop_simulate(Gamestate * gs, E_Simulation_Mode mode)
{
    v_simulation_simulate(gs, mode);
}

static void v_gameloop_build_graphics_intermediate_representation
(
    Graphics_Intermediate_Representation * ir_out, 
    Gamestate const * visual_world
)
{
    //TODO(<zshoals> 07-27-2023): Build IR before rendering
    //....obviously

    ir_out->renderables[0] = 68306; //Dummy test stuff ignore me remove me asap
}

static void v_gameloop_render(Graphics_Renderer * gfx, Graphics_Intermediate_Representation const * ir)
{
    v_graphics_renderer_render(gfx, ir);
}

void v_gameloop_entrypoint(void * data)
{
    Game_Context * context = static_cast<Game_Context *>(data);
    Gamestate * logic_world = address_of(context->logic_world);

    //Update World Data From Key/Mouse Inputs
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    {
        Input_Virtual_Action_State * input = address_of(logic_world->input);
        logic_world->display_time_multiplier = input->move_right_action.data.movement_multiplier;
    }

    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //Process the System Event Loop
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        System_Event_Queue * events = address_of(context->system_events);

        while (v_system_event_queue_has_events(events))
        {
            System_Event e = v_system_event_queue_get_first_event(events);

            switch (e.tag)
            {
                default:
                {
                    VARIA_UNREACHABLE("Unhandled system event.");
                    break;
                }
                case E_System_Event_Type::Gameplay_Move_Right:
                {
                    break;
                }
                case E_System_Event_Type::Gameplay_Move_Left:
                {
                    break;
                }
                case E_System_Event_Type::System_Window_Vertical_Sync_Enable:
                {
                    break;
                }
                case E_System_Event_Type::System_Window_Lost_Focus:
                {
                    kinc_log(KINC_LOG_LEVEL_INFO, "Lost Focus");
                    break;
                }
                case E_System_Event_Type::System_Window_Gained_Focus:
                {
                    kinc_log(KINC_LOG_LEVEL_INFO, "Gained focus");
                    break;
                }
                case E_System_Event_Type::System_Window_Request_Fullscreen:
                {
                    break;
                }
                case E_System_Event_Type::System_Window_Request_Windowed:
                {
                    break;
                }
                case E_System_Event_Type::System_Window_Request_Resize:
                {
                    break;
                }
            }

            v_system_event_queue_pop(events);
        }

        v_system_event_queue_clear(events);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Process important commands (Kinc state configuration updates, for example)
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        if (logic_world->window_requires_reapplication)
        {
            //TODO(<zshoals> 07-27-2023): Stuff?
        }

        if (logic_world->framebuffer_requires_reapplication)
        {
            //TODO(<zshoals> 07-27-2023): Stuff?
        }
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Update Current Realtime Parameters
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    Float_64 realtime = kinc_time();
    Float_64 realtime_advance = kinc_time() - logic_world->previous_realtime;
    logic_world->previous_realtime = realtime;

    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



    //Logic Step
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        logic_world->logic_accumulator += realtime_advance;

        //Max frametime checks / program exit if we get into a nasty death spiral situation
        //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        if 
        (
            logic_world->enable_excessive_frametime_exit &&
            logic_world->recent_frametime_overrun_count >= logic_world->max_frametime_overrun_threshold
        )
        {
            VARIA_UNREACHABLE("Repeatedly exceeding the maximum permissible frametime;"
                "likely in a logic death spiral. Aborting program.");

            kinc_log(KINC_LOG_LEVEL_ERROR, "Repeatedly exceeding the maximum permissible frametime;"
                "likely in a logic death spiral. Aborting program.");
            kinc_stop();
        }
        else if (logic_world->logic_accumulator > logic_world->max_frametime)
        {
            logic_world->recent_frametime_overrun_count += 1;
            logic_world->logic_accumulator = logic_world->max_frametime;
        }
        else
        {
            logic_world->recent_frametime_overrun_count = 0;
        }
        //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        


        //Primary fixed update loop
        //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

        logic_world->total_realtime_fixed_update_time = kinc_time();
        while (logic_world->logic_accumulator >= logic_world->fixed_timestep_interval)
        {
            logic_world->logic_accumulator -= logic_world->fixed_timestep_interval;
            logic_world->logic_cumulative_gameclock += logic_world->fixed_timestep_interval;

            v_gameloop_simulate(logic_world, E_Simulation_Mode::Fixed_Step);

            logic_world->previous_logictime = kinc_time();
        }
        logic_world->total_realtime_fixed_update_time = 
            kinc_time() - logic_world->total_realtime_fixed_update_time;

        //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::




    //Render Step
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        logic_world->render_accumulator += realtime_advance;

        //NOTE(<zshoals> 07-19-2023): Extrapolated time, how far ahead the render time is
        //  from the last recent world step
        logic_world->render_dt = kinc_time() - logic_world->previous_logictime;

        //Frame limiting check
        if //vertical sync off, framelimiter on, haven't reached the accumulator threshold yet
        ( 
            !(logic_world->framebuffer.vertical_sync) &&
            logic_world->enable_framerate_limit &&
            logic_world->render_accumulator <= logic_world->fps_limit
        )
        {
            VARIA_NO_OPERATION();
        }
        else
        {
            Gamestate * visual_world = address_of(context->visual_world);
            //Create an "on demand" gamestate which will not be part of the fixed-step
            //  logic chain, this gamestate is for visual purposes only
            memcpy(visual_world, logic_world, sizeof(*logic_world));

            v_gameloop_simulate(visual_world, E_Simulation_Mode::Extrapolate); 
            Graphics_Intermediate_Representation * ir_out = address_of(context->ir_storage);
            v_gameloop_build_graphics_intermediate_representation(ir_out, visual_world);
            v_gameloop_render(address_of(context->gfx), ir_out);

            logic_world->render_cumulative_gameclock += kinc_time() - logic_world->previous_rendertime;
            logic_world->previous_rendertime = kinc_time();

            logic_world->render_accumulator = 0.0;

            logic_world->rendered_frames += 1;
        }
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    logic_world->frame_cycles += 1;
}

void v_gameloop_initialize(kinc_window_options_t wo, kinc_framebuffer_options_t fbo)
{
	static Game_Context game = ZERO_INIT();
	{
		//[Timing Defaults]
		game.logic_world.fixed_timestep_interval = 1.0 / 480.0;
		game.logic_world.max_frametime = 1.0 / 16.0;
		game.logic_world.max_frametime_overrun_threshold = 10;

		//[Gamestate Defaults]
		game.logic_world.logic_dt = game.logic_world.fixed_timestep_interval;
		game.logic_world.logic_timescale = 1.0;

		//[Gameloop Configuration]
		game.logic_world.enable_excessive_frametime_exit = true;
		game.logic_world.enable_framerate_limit = true;
		game.logic_world.fps_limit = 1.0 / 144.0;

		//[Kinc Settings]
		game.logic_world.window = wo;
		game.logic_world.framebuffer = fbo;
	}
	
    kinc_init
    (
        game.logic_world.window.title, 
        game.logic_world.window.width, 
        game.logic_world.window.height,
        address_of(game.logic_world.window),
        address_of(game.logic_world.framebuffer)
    );

    //Initialize Input
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        Input_Virtual_Action_State * input = address_of(game.logic_world.input);

        kinc_keyboard_set_key_down_callback(&v_input_keydown_callback, input);
        kinc_keyboard_set_key_up_callback(&v_input_keyup_callback, input);
        //TODO(<zshoals> 07-27-2023): Mouse stuff?

        Virtual_Action<Action_Move_Right_Data> * move_right = address_of(input->move_right_action);
        {
            move_right->bound_key = KINC_KEY_R;
            move_right->requires_shift = false;
            move_right->requires_control = false;
            move_right->requires_alt = false;
            move_right->on_keydown = &v_action_move_right_keydown;
            move_right->on_keyup = &v_action_move_right_keyup;
        }

        //Reset all parameters to their default up state
        //TODO(<zshoals> 07-27-2023): This probably needs to happen on alt tab as well
        //  aka window focus loss
        v_input_trigger_all_keyup_actions(input);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Set Kinc's Environment Callbacks
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        System_Event_Queue * events = address_of(game.system_events);
        //Window focus gained/loss, etc.
        kinc_set_foreground_callback(&v_system_callback_focus_gained, events);
        kinc_set_background_callback(&v_system_callback_focus_lost, events);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	kinc_set_update_callback(&v_gameloop_entrypoint, &game);
	kinc_start();
}