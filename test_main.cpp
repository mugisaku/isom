#include<SDL.h>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_plane.hpp"
#include"isom_object.hpp"
#include"isom_renderer.hpp"
#include<random>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;


constexpr int  sz = 64;


std::default_random_engine  eng;
std::uniform_real_distribution<>  dist(-100,100);


Renderer
renderer(screen::width,screen::height);


Image
texture;


Object  obj;


Transformer  tr;


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();

       
      auto  o = obj;

      o.transform(tr);

      o.render(renderer);


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

          bool  shifting = (SDL_GetModState()&KMOD_SHIFT);

          auto  o = tr.get_angle();

            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT ):                                       {o.x_degree -= step;}break;
          case(SDLK_RIGHT):                                       {o.x_degree += step;}break;
          case(SDLK_UP   ): if(shifting){o.z_degree -= step;} else{o.y_degree += step;}break;
          case(SDLK_DOWN ): if(shifting){o.z_degree += step;} else{o.y_degree -= step;}break;
          case(SDLK_SPACE):
              break;
          case(SDLK_1): screen::save_as_bmp();break;
            }


          tr.change_angle(o);
        }
    }


  render();
}


void
push(ObjectList&  ls, PlaneDirection  d, const Angle&  angle)
{
  ls.emplace_back(Plane());


  auto&  pl = ls.back()->plane;

  pl.image = &texture;
  pl.image_rect = Rect(0,0,sz,sz);
  pl.direction = d;
  pl.x_width = sz;
  pl.y_width = sz;

  pl.angle = angle;
  pl.center.assign(0,0,0);

  pl.update();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  texture.open("expandmetal.png");

  texture.set_colorkey(0,0,0);

  std::random_device  rdev;

  eng = std::default_random_engine(rdev());


  ObjectList  ls;

  push(ls,PlaneDirection::top_left,Angle(90,0,0));
  push(ls,PlaneDirection::bottom_left,Angle(0,0,0));
  push(ls,PlaneDirection::bottom_right,Angle(0,-90,0));

  obj = Object(std::move(ls));

  tr.change_offset(sz*2,-sz,0);
  tr.change_center(0,0,0);

  tr.set_translation_flag();
  tr.set_rotation_flag();

  render();


if(0)
{
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
}


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


	

