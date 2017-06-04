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


int  x_degree;
int  y_degree;
int  z_degree;

Renderer
renderer(screen::width,screen::height);


constexpr int  sz = 64;

Image
texture;


Plane  box_planes[6];


DotSet
dotset;

std::vector<TextureRenderingContext>
ctx_stack;


void
make_box()
{
  box_planes[0] = Plane({Point(0,0,0),Point(sz,0,0),Point(sz,sz,0),Point(0,sz,0)});
  box_planes[0].change_degrees(0,0,0);

  box_planes[1] = Plane({Point(0,0,sz),Point(sz,0,sz),Point(sz,sz,sz),Point(0,sz,sz)});
  box_planes[1].change_degrees(0,0,0);

  box_planes[2] = Plane({Point(0,0,0),Point(0,0,sz),Point(0,sz,sz),Point(0,sz,0)});
  box_planes[2].change_degrees(0,0,0);

  box_planes[3] = Plane({Point(sz,0,0),Point(sz,0,sz),Point(sz,sz,sz),Point(sz,sz,0)});
  box_planes[3].change_degrees(0,0,0);

  box_planes[4] = Plane({Point(0,0,0),Point(sz,0,0),Point(sz,0,sz),Point(0,0,sz)});
  box_planes[4].change_degrees(0,0,0);

  box_planes[5] = Plane({Point(0,sz,0),Point(sz,sz,0),Point(sz,sz,sz),Point(0,sz,sz)});
  box_planes[5].change_degrees(0,0,0);

    for(auto&  pl: box_planes)
    {
      pl.x_degree += x_degree;
      pl.y_degree += y_degree;
      pl.z_degree += z_degree;

      pl.update();
    }
}


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();

      renderer.render_dotset(dotset);

    for(auto&  pl: box_planes)
    {
      constexpr Color  color(0x7F,0x7F,0x7F);

//      pl.render_face(renderer,color);
      pl.render_texture(renderer,texture);

      pl.render_wire(renderer);
    }




if(0)
{
/*
  printf("X: %3d Y: %3d Z: %3d\n",
         (x_degree%360),
         (y_degree%360),
         (z_degree%360));
*/
}

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

            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT ):                                 {x_degree -= step;}break;
          case(SDLK_RIGHT):                                 {x_degree += step;}break;
          case(SDLK_UP   ): if(shiting){z_degree += step;} else{y_degree += step;}break;
          case(SDLK_DOWN ): if(shiting){z_degree -= step;} else{y_degree -= step;}break;
          case(SDLK_SPACE):
/*
              x_degree = 315;
              y_degree =  35;
              z_degree =  30;
*/
flag=true;
              break;
          case(SDLK_1): screen::save_as_bmp();break;
            }


          make_box();
if(flag)
{
ctx_stack.clear();
for(auto&  pl: box_planes)
{
  Color  color(rand(),rand(),rand());

  ctx_stack.emplace_back(pl.make_texture_rendering_context(0,texture));
  ctx_stack.emplace_back(pl.make_texture_rendering_context(1,texture));
}
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

        int  n = 32;

          while(n--)
          {
            auto&  p = ctx.get_plotter();

            dotset.emplace_back(p.get_x(),p.get_y(),p.get_z(),ctx.get_color());

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

  texture.open("dice.png");

  renderer.x_offset = 240;
  renderer.y_offset = 240;
make_box();
  render();
/*
for(int  i = 0;  i <= 360;  i += 5)
{
printf("%10f,//%3d\n",std::sin(to_radian(i)),i);
}


printf("-----\n");

for(int  i = 0;  i <= 360;  i += 5)
{
printf("%10f,//%3d\n",std::cos(to_radian(i)),i);
}
fflush(stdout);
*/


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


	

