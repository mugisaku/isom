#include<SDL.h>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_object.hpp"
#include"isom_renderer.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;


constexpr int  sz = 64;


Renderer
renderer(0,0,screen::width,screen::height);


Image  texture;


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


      Object  o = obj;

      o.transform(tr);

//      o.render(renderer);


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

          constexpr int  step = 5;

          bool  flag = false;

          bool  shifting = (SDL_GetModState()&KMOD_SHIFT);

          auto  o = tr.get_angle();

            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT ):                                {o.x -= step;}break;
          case(SDLK_RIGHT):                                {o.x += step;}break;
          case(SDLK_UP   ): if(shifting){o.z -= step;} else{o.y += step;}break;
          case(SDLK_DOWN ): if(shifting){o.z += step;} else{o.y -= step;}break;
          case(SDLK_SPACE):
              break;
          case(SDLK_1): screen::save_as_bmp();break;
            }


          tr.change_angle(o);
        }
    }


  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  texture.open("expandmetal.png");

  texture.set_colorkey(0,0,0);


  obj.push(Polygon(0,Vertex(100, 0, 0),
                     Vertex(180,80, 0),
                     Vertex(100, 0,40)));
  

  tr.change_center(0,0,0);

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


	

