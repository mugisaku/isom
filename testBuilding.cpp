#include<SDL.h>
#include<list>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_object.hpp"
#include"isom_renderer.hpp"
#include"isom_mouse.hpp"
#include"libjson/json.hpp"
#include"libjson/json_stream.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;


constexpr int  sz = 48;

constexpr int  model_renderer_x_pos = sz*2;


Renderer  main_renderer(   0,0,screen::width,screen::height);
Renderer    model_renderer(0,0,sz*2,sz*2);
Renderer   sample_renderer(0,0,sz*2,sz*2);


Point  cursor_point;


DotSet       standard;
DotSet  mini_standard;

Polygon  sample_polygon(0,Vertex(Point( 0, 0,0),white),
                          Vertex(Point(sz, 0,0),white),
                          Vertex(Point( 0,sz,0),white));

constexpr Color  model_color(0x7F,0x7F,0x7F,0xFF);


Polygon
model_polygons[4] =
{
  Polygon(1,Vertex(Point(  0,  0,0),model_color),
            Vertex(Point( sz,  0,0),model_color),
            Vertex(Point(  0, sz,0),model_color)),
  Polygon(2,Vertex(Point(  0,  0,0),model_color),
            Vertex(Point(  0, sz,0),model_color),
            Vertex(Point(-sz,  0,0),model_color)),
  Polygon(3,Vertex(Point(  0,  0,0),model_color),
            Vertex(Point(-sz,  0,0),model_color),
            Vertex(Point(  0,-sz,0),model_color)),
  Polygon(4,Vertex(Point(  0,  0,0),model_color),
            Vertex(Point(  0,-sz,0),model_color),
            Vertex(Point( sz,  0,0),model_color)),
};


DotSet  marker;


Image  texture;



Object*  objptr;


Image  icon;


Transformer   model_tr;
Transformer    view_tr;
Transformer  sample_tr;


Mouse  mouse;


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

  Object::produce_dotset(arr,dotset);

  dotset.render(main_renderer);


  Formatted  fmt;

  main_renderer.draw_ascii(fmt("X: %4d",cursor_point.x),white,0,screen::height-24);
  main_renderer.draw_ascii(fmt("Y: %4d",cursor_point.x),white,0,screen::height-16);
  main_renderer.draw_ascii(fmt("Z: %4d",cursor_point.x),white,0,screen::height- 8);
}


void
render_sample()
{
  static DotSet  dotset;

  sample_renderer.clear();


  mini_standard.render(sample_renderer);

  dotset->clear();

  auto  po = sample_polygon;

  po.transform(sample_tr);
  po.transform(  view_tr);

  po.produce_dotset(dotset);

  dotset.render(sample_renderer,po.id);


  const auto  rect = Rect(0,0,sample_renderer.get_x_width(),
                              sample_renderer.get_y_width());

  sample_renderer.draw_rect(rect,white);
}


void
render_model()
{
  static DotSet  dotset;

  model_renderer.clear();


  mini_standard.render(model_renderer);

    for(auto  bo: model_polygons)
    {
      dotset->clear();

      bo.transform(model_tr);
      bo.transform( view_tr);

      bo.produce_dotset(dotset);

      dotset.render(model_renderer,bo.id);

      bo.produce_wire_dotset(dotset);
    }


  const auto  rect = Rect(0,0,model_renderer.get_x_width(),
                              model_renderer.get_y_width());

  model_renderer.draw_rect(rect,white);
}


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();


      render_main();
      render_model();
      render_sample();


      screen::put_renderer(  main_renderer,0,0);
      screen::put_renderer( model_renderer,model_renderer_x_pos,0);
      screen::put_renderer(sample_renderer,0,0);

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

  constexpr int  step = 15;

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
refresh_model_polygons(uint32_t  id)
{
    for(auto&  po: model_polygons)
    {
      int  l = (po.id == id)? 0xFF:0x7F;

      po.a.r = l;
      po.a.g = l;
      po.a.b = l;
      po.b.r = l;
      po.b.g = l;
      po.b.b = l;
      po.c.r = l;
      po.c.g = l;
      po.c.b = l;
    }


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

        if((mouse.x >= (model_renderer_x_pos                             )) &&
           (mouse.x <  (model_renderer_x_pos+model_renderer.get_x_width())) &&
           (mouse.y <  (                     model_renderer.get_y_width())))
        {
          auto&  cell = model_renderer.get_cell(mouse.x-model_renderer_x_pos,mouse.y);

            if(cell.id)
            {
              static uint32_t  prev_id;

                if(cell.id != prev_id)
                {
                  prev_id = cell.id;

                  refresh_model_polygons(cell.id);
                }


                if(mouse.left)
                {
                  auto&  model = model_polygons[cell.id-1];

                  static_cast<Point&>(sample_polygon.a) = model.a;
                  static_cast<Point&>(sample_polygon.b) = model.b;
                  static_cast<Point&>(sample_polygon.c) = model.c;

                  needed_to_redraw = true;
                }
            }
        }

      else
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
  int  l = 200;

  ObjectArray  arr;

  arr.emplace_back(Line(Dot(Point(-l,   0,   0),black),
                        Dot(Point( 0,   0,   0),red)));
  arr.emplace_back(Line(Dot(Point( 0,   0,   0),red),
                        Dot(Point( l,   0,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,-l,   0),black),
                        Dot(Point(   0, 0,   0),green)));
  arr.emplace_back(Line(Dot(Point(   0, 0,   0),green),
                        Dot(Point(   0, l,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,   0,-l),black),
                        Dot(Point(   0,   0, 0),blue)));
  arr.emplace_back(Line(Dot(Point(   0,   0, 0),blue),
                        Dot(Point(   0,   0, l),white)));

  Object::transform(arr,view_tr);

  Object::produce_dotset(arr,standard);


  arr.clear();

  l = 80;

  arr.emplace_back(Line(Dot(Point(-l,   0,   0),black),
                        Dot(Point( 0,   0,   0),red)));
  arr.emplace_back(Line(Dot(Point( 0,   0,   0),red),
                        Dot(Point( l,   0,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,-l,   0),black),
                        Dot(Point(   0, 0,   0),green)));
  arr.emplace_back(Line(Dot(Point(   0, 0,   0),green),
                        Dot(Point(   0, l,   0),white)));

  arr.emplace_back(Line(Dot(Point(   0,   0,-l),black),
                        Dot(Point(   0,   0, 0),blue)));
  arr.emplace_back(Line(Dot(Point(   0,   0, 0),blue),
                        Dot(Point(   0,   0, l),white)));

  Object::transform(arr,view_tr);

  Object::produce_dotset(arr,mini_standard);
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  texture.open("expandmetal.png");

  icon.open("icon.png");

  Renderer::default_lightset.directional.vector = Vector(-1,0,-1);
  Renderer::default_lightset.directional.color  = Color(0x7F,0x7F,0x7F,0xFF);
  Renderer::default_lightset.ambient.color      = Color(0x3F,0x3F,0x3F,0xFF);

  view_tr.change_angle(315,-35,-30);
  view_tr.set_translation_flag();
  view_tr.set_rotation_flag();
  sample_tr.set_rotation_flag();
  model_tr.set_rotation_flag();

  Renderer::default_lightset.transform(view_tr);

  make_standard();

  libjson::Value  v(FilePath("../object.txt"));

  printf("%s\n",v.to_string().data());

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


	

