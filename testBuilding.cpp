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


Renderer
renderer(0,0,screen::width,screen::height);


ObjectArray  standard;
ObjectArray    marker;


struct
Panel
{
  VertexString*  string;

  Point  point;

};



Object*  objptr;


Image  icon;


Transformer  view_tr;


Mouse  mouse;


View  view;


Object*  x_line;
Object*  y_line;


void
render()
{
    if(needed_to_redraw)
    {
      static ObjectArray  arr;

      static DotSet  dotset;

      screen::lock();

      screen::clear();

      renderer.clear();


      arr.clear();

      dotset->clear();

      arr.emplace_back(ObjectArray(standard));

      Object::transform(arr,view_tr);

      Object::update(arr);

      Object::produce_dotset(arr,dotset);

      dotset.render(renderer);

      renderer.draw_image(icon,nullptr,0,0);

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
          break;
        }


    }


    if(mouse.modified)
    {
      static int  x_prev;
      static int  y_prev;

        if(mouse.left)
        {
          needed_to_redraw = true;

          auto  x = renderer.get_x_base();
          auto  y = renderer.get_y_base();

               if(mouse.x < (x_prev-4)){x += 8;}
          else if(mouse.x > (x_prev+4)){x -= 8;}
               if(mouse.y < (y_prev-4)){y -= 8;}
          else if(mouse.y > (y_prev+4)){y += 8;}

          renderer.change_base_point(x,y);

          x_prev = mouse.x;
          y_prev = mouse.y;
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

  icon.open("icon.png");

  light.normalize();

  view.src_point.y =  80;
  view.src_point.z = 200;

  view_tr.change_angle(315,-35,-30);
  view_tr.set_translation_flag();
  view_tr.set_rotation_flag();


  standard.emplace_back(Line(Dot(Point(-400,   0,   0),black),
                             Dot(Point(   0,   0,   0),red)));
  standard.emplace_back(Line(Dot(Point(   0,   0,   0),red),
                             Dot(Point( 400,   0,   0),white)));

  standard.emplace_back(Line(Dot(Point(   0,-400,   0),black),
                             Dot(Point(   0,   0,   0),green)));
  standard.emplace_back(Line(Dot(Point(   0,   0,   0),green),
                             Dot(Point(   0, 400,   0),white)));

  standard.emplace_back(Line(Dot(Point(   0,   0,-400),black),
                             Dot(Point(   0,   0,   0),blue)));
  standard.emplace_back(Line(Dot(Point(   0,   0,   0),blue),
                             Dot(Point(   0,   0, 400),white)));


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


	

