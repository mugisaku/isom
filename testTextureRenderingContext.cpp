#include<SDL.h>
#include"isom_DotSet.hpp"
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


constexpr int  sz = 128;


Image
texture;


Plane  plane;


Transformer  tr;


DotSet
dotset;

DotSet
nega_dotset;

std::vector<TextureRenderingContext>
ctx_stack;


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();


      dotset.render(renderer);

      renderer.draw_image(texture,nullptr,0,0,0);

      nega_dotset.render(renderer);


      auto  tmplane = plane;

      tmplane.transform(tr);

      tmplane.update();

      tmplane.render_wire(renderer);

      screen::put_renderer(renderer,0,0);


      screen::unlock();
      screen::update();

      needed_to_redraw = false;
    }
}


void
step()
{
  static uint32_t  last;

  auto  now = SDL_GetTicks();

    if(now >= last+40)
    {
      last = now;

        if(ctx_stack.size())
        {
          auto&  ctx = ctx_stack.back();

          int  n = 200;

            while(n--)
            {
              auto&  p = ctx.get_plotter();
              auto&  m = ctx.get_mapper();

                   dotset->emplace_back(Point(p.get_x(), p.get_y(),p.get_z()),ctx.get_color());
              nega_dotset->emplace_back(Point(m.get_x(),-m.get_y(),        1),        Color());

                if(ctx.is_finished())
                {
                  ctx_stack.pop_back();

                  break;
                }

              else
                {
                  ctx.step();
                }
            }


          needed_to_redraw = true;
        }
    }
}


void
process_keydown(int  key)
{
  needed_to_redraw = true;

  constexpr int  step = 15;

  bool  flag = false;

  bool  shifting = (SDL_GetModState()&KMOD_SHIFT);

  static Angle  a;

    switch(key)
    {
  case(SDLK_LEFT ):                                {a.x -= step;}break;
  case(SDLK_RIGHT):                                {a.x += step;}break;
  case(SDLK_UP   ): if(shifting){a.z += step;} else{a.y += step;}break;
  case(SDLK_DOWN ): if(shifting){a.z -= step;} else{a.y -= step;}break;
  case(SDLK_SPACE):
      flag = true;
      break;
  case(SDLK_1): screen::save_as_bmp();break;
    }


  tr.change_angle(a);

    if(flag)
    {
           dotset->clear();
      nega_dotset->clear();


      auto  tmplane = plane;

      tmplane.transform(tr);

      ctx_stack.clear();

      ctx_stack.emplace_back(tmplane.polygons[0].make_rendering_context());
      ctx_stack.emplace_back(tmplane.polygons[1].make_rendering_context());
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
          process_keydown(evt.key.keysym.sym);
          break;
        }
    }


  step();

  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  texture.open("lena_std.png");

  plane.build_x(Point(0,0,0),sz,sz,true);

  plane.image = &texture;

  plane.preset_uv(Rect(0,0,sz,sz),true);


  plane.update();


  tr.set_translation_flag();
  tr.set_rotation_flag();

  tr.change_offset(240,-240,0);

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


	

