#include<SDL.h>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_plane.hpp"
#include"isom_renderer.hpp"
#include"isom_Scene.hpp"
#include<random>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;


Scene  base_scene;
Scene  c_scene;


constexpr int  sz = 320;


std::default_random_engine  eng;
std::uniform_real_distribution<>  dist(-100,100);


Renderer
renderer(screen::width,screen::height);


Image
texture;


Transformer  tr;


Plane  box_planes[6];




void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();

       
      auto  scene = base_scene;

      scene.transform(tr);

      scene.render(renderer);


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

          auto  o = tr.get_angle();

            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT ):                               {o.x_degree -= step;}break;
          case(SDLK_RIGHT):                               {o.x_degree += step;}break;
          case(SDLK_UP   ): if(shiting){o.z_degree -= step;} else{o.y_degree += step;}break;
          case(SDLK_DOWN ): if(shiting){o.z_degree += step;} else{o.y_degree -= step;}break;
          case(SDLK_SPACE):
//for(int  n = 0;  n < 10;  ++n)
{
Object  o(
Polygon(
Dot(Point(dist(eng),dist(eng),dist(eng)),Color(0xFF,0xFF,0xFF,0xFF)),
Dot(Point(dist(eng),dist(eng),dist(eng)),Color(0xFF,0,0,0xFF)),
Dot(Point(dist(eng),dist(eng),dist(eng)),Color(0xFF,0xFF,0xFF,0xFF))
));


  base_scene.push(std::move(o));
}
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

  texture.open("dice.png");

  Plane  backwall_plane(Point(0,0,0),Point(sz,0,0),Point(sz,sz,0),Point(0,0,0));
  Plane  leftwall_plane(Point(0,0,0),Point(sz,0,0),Point(sz,sz,0),Point(0,0,0));
  Plane  floor_plane(Point(0,0,0),Point(320,0,0),Point(320,0,320),Point(0,0,320));

  std::random_device  rdev;

  eng = std::default_random_engine(rdev());

  tr.change_offset(200,-200,0);

  tr.set_translation_flag();
  tr.set_rotation_flag();

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


	

