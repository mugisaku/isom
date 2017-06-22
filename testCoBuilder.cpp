#include<SDL.h>
#include<list>
#include"isom_DotSet.hpp"
#include"isom_LineContext.hpp"
#include"isom_screen.hpp"
#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_object.hpp"
#include"isom_renderer.hpp"
#include"libjson/json.hpp"
#include"libjson/json_stream.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


bool  needed_to_redraw = true;

Renderer  main_renderer(   0,0,screen::width,screen::height);


DotSet  standard;


Transformer    view_tr;
Transformer  object_tr;


std::vector<Polygon>
polygon_table;


Vertex
read_vertex(const libjson::Object&  o)
{
  using namespace libjson;

  Vertex  v;

    for(auto&  m: o)
    {
           if(m.name == "x"){v.x = m.value.to_number();}
      else if(m.name == "y"){v.y = m.value.to_number();}
      else if(m.name == "z"){v.z = m.value.to_number();}
      else if(m.name == "r"){v.r = m.value.to_number();}
      else if(m.name == "g"){v.g = m.value.to_number();}
      else if(m.name == "b"){v.b = m.value.to_number();}
      else if(m.name == "u"){v.r = m.value.to_number();}
      else if(m.name == "v"){v.g = m.value.to_number();}
    }


  return v;
}


Polygon
read_polygon(const libjson::Object&  o)
{
  using namespace libjson;

  Color  default_color;

  Polygon  po;

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){po.a = read_vertex(m.value->object);}
          else if(m.name == "b"){po.b = read_vertex(m.value->object);}
          else if(m.name == "c"){po.c = read_vertex(m.value->object);}
        }
    }


  return po;
}


void
load_object()
{
  using namespace libjson;

  std::vector<Polygon>  new_arr;

    try
    {
      Value  v(FilePath("../object.txt"));

        if(v == ValueKind::array)
        {
            for(auto&  e: v->array)
            {
                if(e == ValueKind::object)
                {
                  new_arr.emplace_back(read_polygon(e->object));
                }
            }
        }
    }


    catch(Stream&  s)
    {
      s.print();

      return;
    }


  polygon_table = std::move(new_arr);

  needed_to_redraw = true;
}


void
render_main()
{
  static DotSet  dotset;

  dotset->clear();

  main_renderer.clear();


  standard.render(main_renderer);


    for(auto  o: polygon_table)
    {
      o.transform(object_tr);
      o.transform(  view_tr);

      o.update();

      o.produce_dotset(nullptr,dotset);
    }


  dotset.render(main_renderer);
}


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      render_main();

      screen::put_renderer(main_renderer,0,0);

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
        }
    }


  auto  now = SDL_GetTicks();

  static uint32_t  last;

    if(now >= last+100)
    {
      load_object();

      last = now;

      auto  a = object_tr.get_angle();

      a.y -= 15;

      object_tr.change_angle(a);

      needed_to_redraw = true;
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
  Object::update(arr);

  Object::produce_dotset(arr,standard);
}




}




int
main(int  argc, char**  argv)
{
  screen::open();

  Renderer::default_lightset.directional.vector = Vector(-1,0,-1);
  Renderer::default_lightset.directional.color  = Color(0x7F,0x7F,0x7F,0xFF);
  Renderer::default_lightset.ambient.color      = Color(0x3F,0x3F,0x3F,0xFF);

  view_tr.change_angle(315,-35,-30);
  view_tr.set_rotation_flag();
  object_tr.set_rotation_flag();

  Renderer::default_lightset.transform(view_tr);

  make_standard();

  load_object();

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


	

