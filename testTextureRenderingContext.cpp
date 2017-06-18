#include<SDL.h>
#include"isom_DotSet.hpp"
#include"isom_vertex.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_plane.hpp"
#include"isom_renderer.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;


Renderer
renderer(0,0,screen::width,screen::height);


constexpr int  sz = 128;


Image
texture;


Plane  plane;


Transformer  tr;


VertexString
buffer;


int
index;


DotSet       dotset;
DotSet  nega_dotset;
DotSet  wire_dotset;


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();

      wire_dotset->clear();

      dotset.render(renderer);

      renderer.draw_image(texture,nullptr,0,0);

        for(auto&  dot: nega_dotset.dot_list)
        {
          renderer.put(black,dot.x,dot.y);
        }


      auto  tmplane = plane;

      tmplane.transform(tr);

      tmplane.update();

      tmplane.produce_wire_dotset(wire_dotset);

      wire_dotset.render(renderer);

      screen::put_renderer(renderer,0,0);


      screen::unlock();
      screen::update();

      needed_to_redraw = false;
    }
}


void
step()
{
  static uint32_t  last;

  auto  now = SDL_GetTicks();

    if(now >= last+40)
    {
      last = now;

        if(index)
        {
          int  n = 200;

            while(n--)
            {
              auto&  v = buffer[--index];

                   dotset->emplace_back(Point(v.x,v.y,v.z),texture.get_color(v.u,v.v));
              nega_dotset->emplace_back(Point(v.u,v.v,  0),            Color(       ));

                if(!index)
                {
                  break;
                }
            }


          needed_to_redraw = true;
        }
    }
}


void
process_keydown(int  key)
{
  needed_to_redraw = true;

  constexpr int  step = 15;

  bool  flag = false;

  bool  shifting = (SDL_GetModState()&KMOD_SHIFT);

  static Angle  a;

    switch(key)
    {
  case(SDLK_LEFT ):                                {a.x -= step;}break;
  case(SDLK_RIGHT):                                {a.x += step;}break;
  case(SDLK_UP   ): if(shifting){a.z += step;} else{a.y += step;}break;
  case(SDLK_DOWN ): if(shifting){a.z -= step;} else{a.y -= step;}break;
  case(SDLK_SPACE):
      flag = true;
      break;
  case(SDLK_1): screen::save_as_bmp();break;
    }


  tr.change_angle(a);

    if(flag)
    {
      buffer.clear();

           dotset->clear();
      nega_dotset->clear();

      auto  tmplane = plane;

      tmplane.transform(tr);

      tmplane.update();

      tmplane.polygons[0].produce_vertex_string(buffer);
      tmplane.polygons[1].produce_vertex_string(buffer);


      index = buffer.size();
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


  step();

  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  texture.open("lena_std.png");

  plane.build_x(Point(0,0,0),sz,sz,true);

  plane.image = &texture;

  plane.preset_uv(Rect(0,0,sz,sz),true);


  plane.update();


  tr.set_rotation_flag();

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


	

