#include<SDL.h>
#include"isom_DotSet.hpp"
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
renderer(screen::width,screen::height);


constexpr int  sz = 128;


Image
texture;


Plane  base_plane(Point(0,sz,0),Point(sz,sz,0),Point(sz,0,0),Point(0,0,0));
Plane  plane;


Transformer  tr;


DotSet
dotset;

DotSet
nega_dotset;

std::vector<TextureRenderingContext>
ctx_stack;


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();


      dotset.render(renderer);

      plane = base_plane;

      plane.transform(tr);

      plane.render_wire(renderer);

      renderer.render_image(texture,nullptr,0,0,0);

      nega_dotset.render(renderer);

      screen::put_renderer(renderer,0,0);


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
          needed_to_redraw = true;

          constexpr double  step = 5;

          bool  flag = false;

          bool  shiting = (SDL_GetModState()&KMOD_SHIFT);

          static Angle  a;

            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT ):                                      {a.x_degree -= step;}break;
          case(SDLK_RIGHT):                                      {a.x_degree += step;}break;
          case(SDLK_UP   ): if(shiting){a.z_degree += step;} else{a.y_degree += step;}break;
          case(SDLK_DOWN ): if(shiting){a.z_degree -= step;} else{a.y_degree -= step;}break;
          case(SDLK_SPACE):
              flag = true;
              break;
          case(SDLK_1): screen::save_as_bmp();break;
            }


          tr.change_angle(a.x_degree,a.y_degree,a.z_degree);

            if(flag)
            {
                   dotset->clear();
              nega_dotset->clear();

              ctx_stack.clear();

              constexpr Rect  rect(0,0,sz,sz);

              ctx_stack.emplace_back(plane.make_texture_rendering_context(0,texture,rect));
              ctx_stack.emplace_back(plane.make_texture_rendering_context(1,texture,rect));
            }
        }
    }


static uint32_t  last;

auto  now = SDL_GetTicks();

  if(now >= last+40)
  {
    last = now;

      if(ctx_stack.size())
      {
        auto&  ctx = ctx_stack.back();

        int  n = 200;

          while(n--)
          {
            auto&  p = ctx.get_plotter();
            auto&  m = ctx.get_mapper();

                 dotset->emplace_back(Point(p.get_x(), p.get_y(),p.get_z()),ctx.get_color());
            nega_dotset->emplace_back(Point(m.get_u(),-m.get_v(),        1),        Color());

              if(ctx.is_finished())
              {
                ctx_stack.pop_back();

                break;
              }

            else
              {
                ctx.step();
              }
          }


        needed_to_redraw = true;
      }
  }


  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  texture.open("lena_std.png");

  tr.set_translation_flag();
  tr.set_rotation_flag();

  tr.change_offset(200,-400,0);

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


	

