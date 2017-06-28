#include<SDL.h>
#include<list>
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_object.hpp"
#include"isom_renderer.hpp"
#include"isom_load.hpp"
#include"libjson/json_FileBuffer.hpp"
#include"libjson/json_stream.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;

Renderer  main_renderer(   0,0,screen::width,screen::height);


Image  texture;


Object  standard;
Object    stages;

int  target_index;

std::vector<Object>::const_iterator  target;

Transformer   view_tr;
Transformer  stage_tr;


void
render_main()
{
  main_renderer.clear();

  standard.render(main_renderer,{&stage_tr,&view_tr});

    if(target != stages.get_children().cend())
    {
      Formatted  fmt;

      main_renderer.draw_string(fmt("%2d %s",target_index,target->get_name().data()),white,0,0);

      target->render(main_renderer,{&stage_tr,&view_tr});
    }
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
process_keydown(int  k)
{
    if(k == SDLK_LEFT)
    {
        if(target_index)
        {
          --target      ;
          --target_index;

          needed_to_redraw = true;
        }
    }

  else
    if(k == SDLK_RIGHT)
    {
      int  n = stages.get_children().size();

        if(target_index < (n-1))
        {
          ++target      ;
          ++target_index;

          needed_to_redraw = true;
        }
    }

  else
    if(k == SDLK_SPACE)
    {
      FileBuffer  object_fbuf(FilePath("../object.json"));
      FileBuffer   stage_fbuf(FilePath("../stage.json"));

      Object  o;

        try
        {
          auto  object_o = load_object(         object_fbuf.get_content().data());
                       o =  load_stage(object_o, stage_fbuf.get_content().data());
        }


        catch(libjson::Stream&  s)
        {
          s.print();

          return;
        }


      stages = std::move(o);

      auto&  arr = stages.get_children();

      target = arr.cbegin();

        if(target_index < arr.size())
        {
            for(int  i = 0;  i < target_index;  ++i)
            {
              ++target;
            }
        }

      else
        {
          target_index = 0;
        }


      needed_to_redraw = true;
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
          process_keydown(evt.key.keysym.sym);
          break;
        }
    }


  render();
}


void
make_standard()
{
  int  l = 200;

  standard.push(Line(Dot(Point(-l,   0,   0),black),
                     Dot(Point( 0,   0,   0),red)));
  standard.push(Line(Dot(Point( 0,   0,   0),red),
                     Dot(Point( l,   0,   0),white)));

  standard.push(Line(Dot(Point(   0,-l,   0),black),
                     Dot(Point(   0, 0,   0),green)));
  standard.push(Line(Dot(Point(   0, 0,   0),green),
                     Dot(Point(   0, l,   0),white)));

  standard.push(Line(Dot(Point(   0,   0,-l),black),
                     Dot(Point(   0,   0, 0),blue)));
  standard.push(Line(Dot(Point(   0,   0, 0),blue),
                     Dot(Point(   0,   0, l),white)));
}




}




int
main(int  argc, char**  argv)
{
  screen::open();

  texture.open("texture.png");

  Polygon::texture_image = &texture;

  Renderer::default_lightset.directional.vector = Vector(-1,0,-1);
  Renderer::default_lightset.directional.color  = Color(0x7F,0x7F,0x7F,0xFF);
  Renderer::default_lightset.ambient.color      = Color(0x3F,0x3F,0x3F,0xFF);

  view_tr.change_angle(315,-35,-30);
  view_tr.set_rotation_flag();
  stage_tr.set_rotation_flag();

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


	
