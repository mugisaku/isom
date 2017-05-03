#include"isom_renderer.hpp"
#include"isom_screen.hpp"
#include"isom_point2.hpp"
#include"isom_point3.hpp"
#include"isom_plane.hpp"
#include"isom_image.hpp"
#include<cstdlib>
#include<string>
#include<SDL.h>
#include<SDL_image.h>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


Renderer
renderer(screen::width,screen::height);

Plane*  current_plane;

Image  img;


constexpr int  sz = 64;

Plane   plane(PlaneKind::image  ,Point3( 0, 0,0),img,Rect(0,0,sz,sz));
Plane  xplane(PlaneKind::x_color,Point3(sz, 0,0),sz,sz,Color(0xFF,0,0,0x7F));
Plane  yplane(PlaneKind::y_image,Point3( 0,sz,0),img,Rect(0,sz,sz,sz));
Plane  zplane(PlaneKind::z_image,Point3( 0, 0,-sz/2),img,Rect(sz,0,sz,sz));


bool
needed_to_redraw = true;


Point2
previous_point;


void
render()
{
    if(needed_to_redraw)
    {
      screen::lock();

      screen::clear();

      renderer.clear();

      plane.render(renderer);
      yplane.render(renderer);
      zplane.render(renderer);
      xplane.render(renderer);

      screen::put_renderer(renderer,0,0);

        if(current_plane)
        {
          char  buf[90];

          snprintf(buf,sizeof(buf),"X:%4d",current_plane->base_point.x);

          screen::put_string(buf,screen::white,0,0);

          snprintf(buf,sizeof(buf),"Y:%4d",current_plane->base_point.y);

          screen::put_string(buf,screen::white,0,16);

          snprintf(buf,sizeof(buf),"Z:%4d",current_plane->base_point.z);

          screen::put_string(buf,screen::white,0,32);
        }


      screen::unlock();

      screen::update();

      needed_to_redraw = false;
    }
}


void
process_button(const SDL_MouseButtonEvent&  evt)
{
    if(evt.button == SDL_BUTTON_LEFT)
    {
      current_plane = const_cast<Plane*>(renderer.get_cell(evt.x,screen::height-evt.y).plane);

      previous_point.assign(-evt.x,-(screen::height-evt.y));
    }

  else
    if(evt.button == SDL_BUTTON_RIGHT)
    {
    }
}


void
process_motion(const SDL_MouseMotionEvent&  evt)
{
    if(current_plane)
    {
      Point2  cur_pt(-evt.x,-(screen::height-evt.y));

      auto  tmp_pt(previous_point-cur_pt);

      current_plane->base_point.x += tmp_pt.x;
      current_plane->base_point.y += tmp_pt.y;

      previous_point = cur_pt;

      needed_to_redraw = true;
    }
}


#ifndef EMSCRIPTEN
void
load(char*  path)
{
  auto  f = fopen(path,"rb");

    if(f)
    {
      std::string  s;

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f) || ferror(f))
            {
              break;
            }


          s.push_back(c);
        }


      fclose(f);
    }


  SDL_free(path);
}
#endif


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
          load(evt.drop.file);
          break;
      case(SDL_WINDOWEVENT):
            if(evt.window.event == SDL_WINDOWEVENT_EXPOSED)
            {
            }
          break;
      case(SDL_KEYDOWN):
//if(0)
{
            switch(evt.key.keysym.sym)
            {
          case(SDLK_LEFT ):  ;  needed_to_redraw = true;break;
          case(SDLK_RIGHT):  ;  needed_to_redraw = true;break;
          case(SDLK_UP   ):  ;  needed_to_redraw = true;break;
          case(SDLK_DOWN ):  ;  needed_to_redraw = true;break;
          case(SDLK_SPACE): screen::save_as_bmp();break;
            }
}
if(0)
{
            switch(evt.key.keysym.sym)
            {
          case(SDLK_UP   ):  ++renderer.w_max;  needed_to_redraw = true;break;
          case(SDLK_DOWN ):  --renderer.w_max;  needed_to_redraw = true;break;
            }
}
          break;
      case(SDL_QUIT):
          fflush(stdout);
          screen::close();
          quick_exit(EXIT_SUCCESS);
          break;
#endif
      case(SDL_MOUSEBUTTONDOWN):
          process_button(evt.button);
          break;
      case(SDL_MOUSEBUTTONUP):
          current_plane = nullptr;
          break;
      case(SDL_MOUSEMOTION):
          process_motion(evt.motion);
          break;
        }
    }


  render();
}


}




int
main(int  argc, char**  argv)
{
  screen::open();

  renderer.offset.z = 160;

  renderer.w_min = INT32_MIN;
  renderer.w_max = INT32_MAX;

  img.open("test.png");

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




