#include<SDL.h>
#include<list>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_object.hpp"
#include"isom_renderer.hpp"
#include"isom_load.hpp"
#include"libjson/json_FileBuffer.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;

Renderer  main_renderer(   0,0,screen::width,screen::height);


ObjectArray  standard;


Transformer    view_tr;
Transformer  object_tr;


ObjectArray
objects;


void
render_main()
{
  static DotSet  dotset;

  dotset->clear();

  main_renderer.clear();

    for(auto  o: standard)
    {
      o.transform(object_tr);
      o.transform(  view_tr);

      o.produce_dotset(dotset);
    }


    for(auto  o: objects)
    {
      o.transform(object_tr);
      o.transform(  view_tr);

      o.produce_dotset(dotset);
    }


  dotset.render(main_renderer);
}


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      render_main();

      screen::put_renderer(main_renderer,0,0);

      screen::unlock();
      screen::update();


      needed_to_redraw = false;
    }
}


void
main_loop()
{
  static SDL_Event  evt;

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
#ifndef EMSCRIPTEN
      case(SDL_DROPFILE):
          break;
      case(SDL_WINDOWEVENT):
            if(evt.window.event == SDL_WINDOWEVENT_EXPOSED)
            {
              needed_to_redraw = true;
            }
          break;
      case(SDL_QUIT):
          screen::close();
          quick_exit(EXIT_SUCCESS);
          break;
#endif
      case(SDL_KEYDOWN):
            if(evt.key.keysym.sym == SDLK_SPACE)
            {
              FileBuffer  fbuf(FilePath("../object.txt"));

              load_object(fbuf.get_content().data(),objects);


              auto  a = object_tr.get_angle();

              a.y = 0;

              object_tr.change_angle(a);


              needed_to_redraw = true;
            }
          break;
        }
    }


  auto  now = SDL_GetTicks();

  static uint32_t  last;

    if(now >= last+100)
    {
      last = now;

      auto  a = object_tr.get_angle();

      a.y -= 15;

      object_tr.change_angle(a);

      needed_to_redraw = true;
    }


  render();
}


void
make_standard()
{
  int  l = 200;

  standard.emplace_back(Line(Dot(Point(-l,   0,   0),black),
                             Dot(Point( 0,   0,   0),red)));
  standard.emplace_back(Line(Dot(Point( 0,   0,   0),red),
                             Dot(Point( l,   0,   0),white)));

  standard.emplace_back(Line(Dot(Point(   0,-l,   0),black),
                             Dot(Point(   0, 0,   0),green)));
  standard.emplace_back(Line(Dot(Point(   0, 0,   0),green),
                             Dot(Point(   0, l,   0),white)));

  standard.emplace_back(Line(Dot(Point(   0,   0,-l),black),
                             Dot(Point(   0,   0, 0),blue)));
  standard.emplace_back(Line(Dot(Point(   0,   0, 0),blue),
                             Dot(Point(   0,   0, l),white)));
}




}




int
main(int  argc, char**  argv)
{
  screen::open();

  Renderer::default_lightset.directional.vector = Vector(-1,0,-1);
  Renderer::default_lightset.directional.color  = Color(0x7F,0x7F,0x7F,0xFF);
  Renderer::default_lightset.ambient.color      = Color(0x3F,0x3F,0x3F,0xFF);

  view_tr.change_angle(315,-35,-30);
  view_tr.set_rotation_flag();
  object_tr.set_rotation_flag();

  Renderer::default_lightset.transform(view_tr);

  make_standard();

  render();



#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,-1,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  return 0;
}


	

