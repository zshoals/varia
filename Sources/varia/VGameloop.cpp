#include "VGameloop.hpp"

#include "varia/VShared.hpp"
#include "varia/VGameContext.hpp"
#include "varia/VSimulation.hpp"
#include "varia/VGamestateQuery.hpp"
#include "varia/utility/VMemcpy.hpp"
#include "varia/input/VInput.hpp"
#include "varia/VSystemCallbacks.hpp"

#include "kinc/display.h"
#include "kinc/system.h"
#include "kinc/log.h"
#include "kinc/threads/thread.h"



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

    //Process the Input Event Loop
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        VDS_Reset_Queue<E_Gameplay_Event> events = vds_reset_queue_make_interface(address_of(context->input.events));
        VDS_Reset_Queue<E_Gameplay_Event> * events_location = address_of(events);

        while (vds_reset_queue_has_elements(events_location))
        {
            E_Gameplay_Event e = vds_reset_queue_get_first_element(events_location);

            switch (e)
            {
                default:
                {
                    VARIA_UNREACHABLE("Unhandled system event.");
                    break;
                }
                case E_Gameplay_Event::No_Action:
                {
                    break;
                }
                case E_Gameplay_Event::Move_Right_Pressed:
                {
                    Float_64 data = kinc_time();
                    kinc_log(KINC_LOG_LEVEL_INFO, "Right Press detected! %f!", data);
                    break;
                }
                case E_Gameplay_Event::Move_Right_Released:
                {
                    Float_64 data = kinc_time();
                    kinc_log(KINC_LOG_LEVEL_INFO, "RIGHT RELEASE FUCK YOU detected! %f!", data);
                    break;
                }
                case E_Gameplay_Event::Move_Left_Pressed:
                {
                    Float_64 data = kinc_time();
                    kinc_log(KINC_LOG_LEVEL_INFO, "Left Press detected! %f!", data);
                    break;
                }
                case E_Gameplay_Event::Move_Left_Released:
                {
                    Float_64 data = kinc_time();
                    kinc_log(KINC_LOG_LEVEL_INFO, "LEFT RELEASE FUCK YOU detected! %f!", data);
                    break;
                }
            }

            vds_reset_queue_pop(events_location);
        }

        vds_reset_queue_reset(events_location);
    }
    
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



    //Process the System Event Loop
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        VDS_Reset_Queue<E_System_Event_Type> events = vds_reset_queue_make_interface(address_of(context->system_events));
        VDS_Reset_Queue<E_System_Event_Type> * events_location = address_of(events);

        while (vds_reset_queue_has_elements(events_location))
        {
            E_System_Event_Type e = vds_reset_queue_get_first_element(events_location);

            switch (e)
            {
                default:
                {
                    VARIA_UNREACHABLE("Unhandled system event.");
                    break;
                }
                //Kinc State Events
                case E_System_Event_Type::System_Window_Vertical_Sync_Enable:
                {
                    break;
                }
                case E_System_Event_Type::System_Window_Vertical_Sync_Disable:
                {
                    break;
                }
                case E_System_Event_Type::System_Window_Lost_Focus:
                {
                    //TODO(<zshoals> 07-28-2023): This doesn't completely solve the problem;
                    //  Kinc seems to ignore the next keypress, leading to a double release
                    //  however this at least seems to stop any alt-tab exploit
                    //  Also, keypresses generated here won't be handled until the next frame
                    v_input_trigger_all_keyup_actions(address_of(context->input));
                    logic_world->enable_sleep_in_loop = true;

                    kinc_log(KINC_LOG_LEVEL_INFO, "Lost Focus; all keys released, sleeping loop enabled");

                    break;
                }
                case E_System_Event_Type::System_Window_Gained_Focus:
                {
                    kinc_log(KINC_LOG_LEVEL_INFO, "Gained focus, sleeping loop disabled");
                    logic_world->enable_sleep_in_loop = false;

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

            vds_reset_queue_pop(events_location);
        }

        vds_reset_queue_reset(events_location);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Process important commands (Kinc state configuration updates, for example)
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        //NOTE(<zshoals> 07-28-2023): This might be an expensive operation that isn't needed
        //  a lot of these "system query" type operations are weirdly expensive
        // logic_world->kinc_primary_display_index = kinc_primary_display();

        if (logic_world->window_requires_reapplication)
        {
            //TODO(<zshoals> 07-27-2023): Stuff?

        }

        if (logic_world->framebuffer_requires_reapplication)
        {
            //TODO(<zshoals> 07-27-2023): Stuff?
            kinc_window_change_framebuffer(logic_world->kinc_primary_display_index, address_of(logic_world->framebuffer));
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
        


        //Fixed update loop
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

    if (logic_world->enable_sleep_in_loop)
    {
        kinc_thread_sleep(1);
    }
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
        game.logic_world.kinc_primary_display_index = kinc_primary_display();
	}
	
    kinc_init
    (
        game.logic_world.window.title, 
        game.logic_world.window.width, 
        game.logic_world.window.height,
        address_of(game.logic_world.window),
        address_of(game.logic_world.framebuffer)
    );
    kinc_threads_init();

    //Initialize Input
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        Input_Virtual_Action_State * input = address_of(game.input);
        kinc_keyboard_set_key_down_callback(&v_input_keydown_callback, input);
        kinc_keyboard_set_key_up_callback(&v_input_keyup_callback, input);
        //TODO(<zshoals> 07-27-2023): Mouse stuff?

        //Move Right Action
        {
            Input_Virtual_Key & key = input->virtual_keys[(Integer_64)E_Gameplay_Actions::Move_Right];
            key.bound_key = KINC_KEY_D;
            key.requires_shift = false;
            key.requires_control = false;
            key.requires_alt = false;
            key.event_pressed = E_Gameplay_Event::Move_Right_Pressed;
            key.event_released = E_Gameplay_Event::Move_Right_Released;
        }
        //Move Left Action
        {
            Input_Virtual_Key & key = input->virtual_keys[(Integer_64)E_Gameplay_Actions::Move_Left];
            key.bound_key = KINC_KEY_A;
            key.requires_shift = false;
            key.requires_control = false;
            key.requires_alt = false;
            key.event_pressed = E_Gameplay_Event::Move_Left_Pressed;
            key.event_released = E_Gameplay_Event::Move_Left_Released;
        }


        //Reset all parameters to their default up state
        v_input_trigger_all_keyup_actions(input);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


    //Set Kinc's Environment Callbacks
    //BEGIN:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    {
        VDS_Reset_Queue<E_System_Event_Type> events = vds_reset_queue_make_interface(address_of(game.system_events));
        VDS_Reset_Queue<E_System_Event_Type> * events_location = address_of(events);

        kinc_set_foreground_callback(&v_system_callback_focus_gained, events_location);
        kinc_set_background_callback(&v_system_callback_focus_lost, events_location);
    }
    //END:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	kinc_set_update_callback(&v_gameloop_entrypoint, &game);
	kinc_start();
}