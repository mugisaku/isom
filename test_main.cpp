#include<SDL.h>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_plane.hpp"
#include"isom_renderer.hpp"
#include"isom_Scene.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;


Scene         base_scene;
Scene  transformed_scene;


constexpr int  sz = 320;


Renderer
renderer(screen::width,screen::height);


Image
texture;


Point  offset(200,-200,0);


Plane  box_planes[6];




void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();

       

      Rotater  r(Angle(315,35,30),Point(0,0,0));

      transformed_scene = base_scene.transform(r);

      transformed_scene.render(renderer);


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
          case(SDLK_LEFT ):                                 {offset.x -= step;}break;
          case(SDLK_RIGHT):                                 {offset.x += step;}break;
          case(SDLK_UP   ): if(shiting){offset.z -= step;} else{offset.y += step;}break;
          case(SDLK_DOWN ): if(shiting){offset.z += step;} else{offset.y -= step;}break;
          case(SDLK_SPACE):
              break;
          case(SDLK_1): screen::save_as_bmp();break;
            }
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

  Plane  backwall_plane(Point(0,0,0),Point(sz,0,0),Point(sz,sz,0),Point(0,0,0));
  Plane  leftwall_plane(Point(0,0,0),Point(sz,0,0),Point(sz,sz,0),Point(0,0,0));
  Plane  floor_plane(Point(0,0,0),Point(320,0,0),Point(320,0,320),Point(0,0,320));

  base_scene.push(std::move(backwall_plane));
  base_scene.push(std::move(leftwall_plane));
  base_scene.push(std::move(   floor_plane));

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


	

