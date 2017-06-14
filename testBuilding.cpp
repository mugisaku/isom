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
#include"isom_ViewController.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;


constexpr int  sz = 64;


Renderer
renderer(screen::width,screen::height);


Object  obj;


Transformer       tr;
Transformer  view_tr;


Mouse  mouse;


View  view;


ViewController
bottom_view_ctrl("bottom",80,96,
              view.src_point.x,
              view.src_point.z,
              view.dst_point.x,
              view.dst_point.z);

ViewController
front_view_ctrl("front",80,0,
                view.src_point.x,
                view.src_point.y,
                view.dst_point.x,
                view.dst_point.y);

ViewController
left_view_ctrl("left",0,0,
                view.src_point.z,
                view.src_point.y,
                view.dst_point.z,
                view.dst_point.y);




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


       front_view_ctrl.render(renderer);
        left_view_ctrl.render(renderer);
      bottom_view_ctrl.render(renderer);

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
      case(SDL_MOUSEBUTTONDOWN):
        mouse.x = evt.button.x;
        mouse.y = evt.button.y;

             if(evt.button.button == SDL_BUTTON_LEFT ){mouse.left  = 1;}
        else if(evt.button.button == SDL_BUTTON_RIGHT){mouse.right = 1;}

        mouse.modified = 1;
        break;
      case(SDL_MOUSEBUTTONUP):
        mouse.x = evt.button.x;
        mouse.y = evt.button.y;

             if(evt.button.button == SDL_BUTTON_LEFT ){mouse.left  = 0;}
        else if(evt.button.button == SDL_BUTTON_RIGHT){mouse.right = 0;}

        mouse.modified = 1;
        break;
      case(SDL_MOUSEMOTION):
        mouse.x = evt.motion.x;
        mouse.y = evt.motion.y;

        mouse.left  = (evt.motion.state&SDL_BUTTON_LMASK);
        mouse.right = (evt.motion.state&SDL_BUTTON_RMASK);

        mouse.modified = 1;
        break;
      case(SDL_KEYDOWN):
          needed_to_redraw = true;

          constexpr int  step = 15;

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


    if(mouse.modified)
    {
        if(front_view_ctrl.test(mouse))
        {
          front_view_ctrl.process(mouse);

          needed_to_redraw = true;
        }

      else
        if(left_view_ctrl.test(mouse))
        {
          left_view_ctrl.process(mouse);

          needed_to_redraw = true;
        }

      else
        if(bottom_view_ctrl.test(mouse))
        {
          bottom_view_ctrl.process(mouse);

          needed_to_redraw = true;
        }


      mouse.modified = 0;
    }


  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  light.normalize();

  obj = Object(ObjectList());

  Box  box;

  box.build(Point(0,0,0),80,80,40);


  const Image*  ptr = nullptr;

  obj->object_list.emplace_back(std::move(box));
  

  tr.change_offset(sz*2,-sz*2,0);
  tr.change_center(0,0,0);

  tr.set_translation_flag();
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


	

