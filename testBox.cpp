#include<SDL.h>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_plane.hpp"
#include"isom_object.hpp"
#include"isom_renderer.hpp"
#include"isom_view.hpp"
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


Image  a_texture;
Image  b_texture;


Object  obj;


Transformer       tr;
Transformer  view_tr;

View  view;


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();


      Object  o = obj;

      o.transform(view_tr);

      o.update();

      o.render(renderer);


      renderer.draw_rect(Rect(0,0,80,80),Color(255,255,255,255));
      renderer.draw_line(Color(255,255,255,255),40, 0,40,80,2);
      renderer.draw_line(Color(255,255,255,255), 0,40,80,40,2);

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

          auto&  o = view.src_point;

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


          view_tr = view.make_transformer();
        }
    }


  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  a_texture.open("expandmetal.png");
  b_texture.open("lena_std.png");

  a_texture.set_colorkey(0,0,0);

  std::random_device  rdev;

  eng = std::default_random_engine(rdev());

light.normalize();
  obj = Object(ObjectList());

  Box  box;

  box.build(Point(0,0,0),80,80,40);


  const Image*  ptr = nullptr;

  obj->object_list.emplace_back(std::move(box));

  Polygon  poly(Dot(Point(  0,0,  0),Color()),
                Dot(Point(100,0,  0),Color()),
                Dot(Point(  0,0,100),Color()));

//  obj->object_list.emplace_back(std::move(poly));
  

  tr.change_offset(sz*2,-sz*2,0);
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


	

