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


constexpr int  sz = 48;


Renderer  main_renderer(0,0,screen::width,screen::height);
Renderer   sub_renderer(0,0,sz*2,sz*2);


DotSet       standard;
DotSet  mini_standard;

Polygon  sample_polygon(Vertex( 0, 0,0),
                        Vertex(sz, 0,0),
                        Vertex( 0,sz,0));

DotSet  marker;


struct
PanelSource
{
  Polygon  polygon;

};


struct
Panel
{
  VertexString*  string;

  Point  point;

};



Object*  objptr;


Image  icon;


Transformer    view_tr;
Transformer  sample_tr;


Mouse  mouse;


View  view;


Object*  x_line;
Object*  y_line;


void
render_main()
{
  static ObjectArray  arr;
  static DotSet  dotset;

  dotset->clear();

  main_renderer.clear();


  standard.render(main_renderer);


  arr.clear();

  Object::update(arr);

  Object::produce_dotset(arr,dotset);

  dotset.render(main_renderer);
}


void
render_sub()
{
  static DotSet  dotset;

  dotset->clear();

  sub_renderer.clear();


  auto  p = sample_polygon;

  p.transform(sample_tr);
  p.transform(  view_tr);

  p.update();

  p.produce_dotset(dotset);

  mini_standard.render(sub_renderer);
  dotset.render(       sub_renderer);
}


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      directional_light::transformed_vector = view_tr(directional_light::vector);

      render_main();
      render_sub();


      screen::put_renderer(main_renderer,0,0);
      screen::put_renderer( sub_renderer,0,0);

      screen::unlock();
      screen::update();


      needed_to_redraw = false;
    }
}


void
process_key(SDL_Keycode  k)
{
  bool  shifting = SDL_GetModState()&KMOD_SHIFT;

  auto  a = sample_tr.get_angle();

  constexpr int  step = 5;

    switch(k)
    {
  case(SDLK_UP   ): if(shifting){a.z -= step;} else{a.y += step;}break;
  case(SDLK_DOWN ): if(shifting){a.z += step;} else{a.y -= step;}break;
  case(SDLK_LEFT ): a.x -= step;break;
  case(SDLK_RIGHT): a.x += step;break;
    }


  sample_tr.change_angle(a);

  needed_to_redraw = true;
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
          process_key(evt.key.keysym.sym);
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

          auto  x = main_renderer.get_x_base();
          auto  y = main_renderer.get_y_base();

               if(mouse.x < (x_prev-4)){x += 8;}
          else if(mouse.x > (x_prev+4)){x -= 8;}
               if(mouse.y < (y_prev-4)){y -= 8;}
          else if(mouse.y > (y_prev+4)){y += 8;}

          main_renderer.change_base_point(x,y);

          x_prev = mouse.x;
          y_prev = mouse.y;
        }


      mouse.modified = 0;
    }


  render();
}


void
make_standard()
{
  ObjectArray  arr;

  arr.emplace_back(Line(Dot(Point(-400,   0,   0),black),
                        Dot(Point(   0,   0,   0),red)));
  arr.emplace_back(Line(Dot(Point(   0,   0,   0),red),
                        Dot(Point( 400,   0,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,-400,   0),black),
                        Dot(Point(   0,   0,   0),green)));
  arr.emplace_back(Line(Dot(Point(   0,   0,   0),green),
                        Dot(Point(   0, 400,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,   0,-400),black),
                        Dot(Point(   0,   0,   0),blue)));
  arr.emplace_back(Line(Dot(Point(   0,   0,   0),blue),
                        Dot(Point(   0,   0, 400),white)));

  Object::transform(arr,view_tr);
  Object::update(arr);

  Object::produce_dotset(arr,standard);


  arr.clear();

  arr.emplace_back(Line(Dot(Point(-64,   0,   0),black),
                        Dot(Point(  0,   0,   0),red)));
  arr.emplace_back(Line(Dot(Point(  0,   0,   0),red),
                        Dot(Point( 64,   0,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,-64,   0),black),
                        Dot(Point(   0,  0,   0),green)));
  arr.emplace_back(Line(Dot(Point(   0,  0,   0),green),
                        Dot(Point(   0, 64,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,   0,-64),black),
                        Dot(Point(   0,   0,  0),blue)));
  arr.emplace_back(Line(Dot(Point(   0,   0,  0),blue),
                        Dot(Point(   0,   0, 64),white)));

  Object::transform(arr,view_tr);
  Object::update(arr);

  Object::produce_dotset(arr,mini_standard);
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  icon.open("icon.png");

  view.src_point.y =  80;
  view.src_point.z = 200;

  view_tr.change_angle(315,-35,-30);
  view_tr.set_translation_flag();
  view_tr.set_rotation_flag();
  sample_tr.set_rotation_flag();


  make_standard();


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


	

