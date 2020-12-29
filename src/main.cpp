#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <boost/make_shared.hpp>

#include "al5poly/altypedef.hpp"
#include "al5poly/AssetManager.hpp"
#include "al5poly/IException.hpp"
#include "al5poly/Renderer.hpp"

const int SCREEN_W = 1920;
const int SCREEN_H = 1080;

const char * const boolstr(bool);

void initializeAllegro5(
        al5poly::ALLEGRO_DISPLAY_Ptr &,
        al5poly::ALLEGRO_TIMER_Ptr &,
        al5poly::ALLEGRO_EVENT_QUEUE_Ptr &);

int main(int argc, char ** argv) try
{
    al5poly::ALLEGRO_DISPLAY_Ptr display;
    al5poly::ALLEGRO_TIMER_Ptr timer;
    al5poly::ALLEGRO_EVENT_QUEUE_Ptr eventQueue;

    initializeAllegro5(display, timer, eventQueue);

    al5poly::AssetManager assMan;
    al5poly::Renderer renderer(display);

    al_start_timer(timer.get());

    al_set_target_bitmap(al_get_backbuffer(display.get()));
    al_clear_to_color(al_map_rgb(0, 0, 0));

    while(true)
    {
        ALLEGRO_EVENT event;
        bool tick = false;

        al_wait_for_event(eventQueue.get(), &event);

        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            tick = true;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            int keycode = event.keyboard.keycode;

            if(keycode == ALLEGRO_KEY_ESCAPE)
            {
                break;
            }
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        // Drawing.
        if(tick)
        {
            try
            {
                ALLEGRO_COLOR brown_level2 = assMan.createColor("squirrel-brown", 114, 63, 32);
                ALLEGRO_COLOR brown_level1 = assMan.getColor("squirrel-brown");
                ALLEGRO_COLOR brown_level0 = al_map_rgb(114, 63, 32);

                fprintf(stderr, "brown (level3): %s\n", assMan.printColor("squirrel-brown"));
                fprintf(stderr, "brown (level2): %s\n", assMan.printColor(brown_level2));
                fprintf(stderr, "brown (level1): %s\n", assMan.printColor(brown_level1));
                fprintf(stderr, "brown (level0): %s\n", assMan.printColor(brown_level0));

                al_draw_filled_circle(200, 150, 100, al_map_rgb(255, 0, 0));
                al_draw_filled_circle(200, 150, 100, brown_level0);
                al_draw_filled_circle(400, 150, 100, al_map_rgb(255, 0, 0));
                al_draw_filled_circle(400, 150, 100, brown_level1);
                al_draw_filled_circle(200, 350, 100, al_map_rgb(255, 0, 0));
                al_draw_filled_circle(200, 350, 100, brown_level2);

                renderer.paint(al_map_rgb(255, 255, 255));
            }
            catch(al5poly::IException & ex)
            {
                std::cerr << "Unhandled exception in main loop drawing block: " << ex.getMessage() << std::endl;
            }
        }
    }

    return 0;
}
catch(std::exception & ex)
{
    std::cerr << ex.what() << std::endl;

    return 1;
}
catch(...)
{
    try
    {
        std::exception_ptr p = std::current_exception();
        std::cerr << "An unrecognized unhandled exception of type {" << (p ? p.__cxa_exception_type()->name() : "null") << "} was encountered.\n";
    }
    catch(...)
    {
        std::cerr << "An unrecognized unhandled exception of unknown type was encountered.\n";
    }

    return 1;
}

const char * const boolstr(bool value)
{
    return value ? "true" : "false";
}

void initializeAllegro5(
        al5poly::ALLEGRO_DISPLAY_Ptr & display,
        al5poly::ALLEGRO_TIMER_Ptr & timer,
        al5poly::ALLEGRO_EVENT_QUEUE_Ptr & eventQueue)
{
    const int FPS = 30;

    if(!al_init())
        al5poly::Exception("Failed to initialize Allegro 5!").raise();

    al_set_new_display_flags(ALLEGRO_WINDOWED);

    al5poly::ALLEGRO_DISPLAY_Ptr d(
            al_create_display(SCREEN_W, SCREEN_H),
            al_destroy_display);

    if(!d)
        al5poly::Exception("Failed to create Allegro 5 display!").raise();

    display = d;

    if(!al_install_keyboard())
        al5poly::Exception("Failed to install Allegro 5 keyboard!").raise();

    if(!al_init_image_addon())
        al5poly::Exception("Failed to initialize image addon.").raise();

    if(!al_init_primitives_addon())
        al5poly::Exception("Failed to initialize primitives addon.").raise();

    al5poly::ALLEGRO_TIMER_Ptr t(
            al_create_timer(1.0 / FPS),
            al_destroy_timer);

    if(!t)
        al5poly::Exception("Failed to create Allegro 5 timer!").raise();

    timer = t;

    al5poly::ALLEGRO_EVENT_QUEUE_Ptr eQ(
            al_create_event_queue(),
            al_destroy_event_queue);

    if(!eQ)
        al5poly::Exception("Failed to create Allegro 5 event queue!").raise();

    eventQueue = eQ;

    al_register_event_source(
            eventQueue.get(),
            al_get_display_event_source(display.get()));

    al_register_event_source(
            eventQueue.get(),
            al_get_keyboard_event_source());

    al_register_event_source(
            eventQueue.get(),
            al_get_timer_event_source(timer.get()));
}
