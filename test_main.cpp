#include<SDL.h>
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


constexpr int  sz = 40;

Image
texture;


Plane  pl(Point(0,0,0),sz,sz);


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();

pl.render_wire(renderer);
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
          break;
      case(SDL_QUIT):
          screen::close();
          quick_exit(EXIT_SUCCESS);
          break;
#endif
      case(SDL_KEYDOWN):
          needed_to_redraw = true;

          constexpr double  step = 5;

          bool  shiting = (SDL_GetModState()&KMOD_SHIFT);

            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT ):                                 {pl.x_degree -= step;}break;
          case(SDLK_RIGHT):                                 {pl.x_degree += step;}break;
          case(SDLK_UP   ): if(shiting){pl.z_degree += step;} else{pl.y_degree += step;}break;
          case(SDLK_DOWN ): if(shiting){pl.z_degree -= step;} else{pl.y_degree -= step;}break;
          case(SDLK_SPACE):
/*

              x_degree = 315;
              y_degree =  35;
              z_degree =  30;
*/
              break;
          case(SDLK_1): screen::save_as_bmp();break;
            }


          pl.update();
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




