#include"isom_renderer.hpp"
#include"isom_screen.hpp"
#include"isom_point2.hpp"
#include"isom_point3.hpp"
#include"isom_plane.hpp"
#include"isom_image.hpp"
#include<cstdlib>
#include<string>
#include<list>
#include<random>
#include<SDL.h>
#include<SDL_image.h>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif




constexpr int  sz = 64;



Image  t_img;
Image  m_img;


struct
Box
{
  Point3  base_point;

  Plane  x_plane;
  Plane  y_plane;
  Plane  z_plane;
  Plane  shadow_plane;

  double  vector;

  Box(Point3  base):
  base_point(base),
  vector(0),
  x_plane(PlaneKind::x_image,Point3(),t_img,Rect( 0, 0,sz,sz)),
  y_plane(PlaneKind::y_image,Point3(),t_img,Rect( 0,sz,sz,sz)),
  z_plane(PlaneKind::z_image,Point3(),t_img,Rect(sz, 0,sz,sz)),
  shadow_plane(PlaneKind::y_image,Point3(),t_img,Rect(sz,sz,sz,sz))
  {
    x_plane.box = this;
    y_plane.box = this;
    z_plane.box = this;

    x_plane.offset.assign(sz/2,0,sz/2);
    y_plane.offset.assign(-(sz/2),sz,sz/2);
    z_plane.offset.assign(-(sz/2) ,0,sz/2);
    shadow_plane.offset.assign(-(sz/2),0,sz/2);

    correct();
  }

  void  correct()
  {
    x_plane.base_point = base_point;
    y_plane.base_point = base_point;
    z_plane.base_point = base_point;

    shadow_plane.base_point.x = base_point.x;
    shadow_plane.base_point.z = base_point.z;
  }

  void  render(Renderer&  dst) const
  {
    x_plane.render(dst);
    y_plane.render(dst);
    z_plane.render(dst);
    shadow_plane.render(dst);
  }

};




namespace{


struct
RNG
{
  std::default_random_engine        eng;
  std::uniform_int_distribution<>  dist;

  RNG():
  eng(std::random_device()()),
  dist(0,4){}

  int  get(){return dist(eng);}

} rng;


Renderer
renderer(screen::width,screen::height);

Plane*  current_plane;


int  phase;

uint32_t  count;




Plane   plane(PlaneKind::z_image,Point3(0,0,0),m_img,Rect(0,0,sz,sz));


std::list<Box>
box_list;


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

        for(auto&  box: box_list)
        {
          box.render(renderer);
        }


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
      auto&  cell = renderer.get_cell(evt.x,screen::height-evt.y);

      current_plane = const_cast<Plane*>(cell.plane);

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

        if(current_plane->box)
        {
          auto&  box = *current_plane->box;

            if(current_plane == &box.x_plane)
            {
              box.base_point.z -= tmp_pt.x;
              box.base_point.y += tmp_pt.y;
            }

          else
            if(current_plane == &box.y_plane)
            {
              box.base_point.z -= tmp_pt.x;
              box.base_point.x -= tmp_pt.y;
            }

          else
            {
              box.base_point.x += tmp_pt.x;
              box.base_point.y += tmp_pt.y;
            }


          box.correct();
        }

      else
        {
          current_plane->base_point.x += tmp_pt.x;
          current_plane->base_point.y += tmp_pt.y;
        }


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
step_to_north()
{
  plane.base_point.z -= 2;

    if(plane.kind != PlaneKind::x_image)
    {
      plane.offset.x =      0;
      plane.offset.z = (sz/2);

      plane.kind = PlaneKind::x_image;
    }


  auto&  w = plane.image_rect.w;

    if(w < 0)
    {
      w = -w;
    }


  needed_to_redraw = true;
}


void
step_to_south()
{
  plane.base_point.z += 2;

    if(plane.kind != PlaneKind::x_image)
    {
      plane.offset.x =      0;
      plane.offset.z = (sz/2);

      plane.kind = PlaneKind::x_image;
    }


  auto&  w = plane.image_rect.w;

    if(w >= 0)
    {
      w = -w;
    }


  needed_to_redraw = true;
}


void
step_to_west()
{
  plane.base_point.x -= 2;

    if(plane.kind != PlaneKind::z_image)
    {
      plane.offset.x = -(sz/2);
      plane.offset.z = 0;

      plane.kind = PlaneKind::z_image;
    }


  auto&  w = plane.image_rect.w;

    if(w >= 0)
    {
      w = -w;
    }


  needed_to_redraw = true;
}


void
step_to_east()
{
  plane.base_point.x += 2;

    if(plane.kind != PlaneKind::z_image)
    {
      plane.offset.x = -(sz/2);
      plane.offset.z = 0;

      plane.kind = PlaneKind::z_image;
    }


  auto&  w = plane.image_rect.w;

    if(w < 0)
    {
      w = -w;
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
          load(evt.drop.file);
          break;
      case(SDL_WINDOWEVENT):
            if(evt.window.event == SDL_WINDOWEVENT_EXPOSED)
            {
            }
          break;
      case(SDL_KEYDOWN):
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


auto  now = SDL_GetTicks();
static uint32_t  last_time;
if(now > (last_time+160))
{
last_time = now;

  if(++phase >= 4)
  {
    phase = 0;
  }

static const int  table[] = {0,1,0,2};

plane.image_rect.x = sz*table[phase];

  needed_to_redraw = true;
}


    for(auto&  box: box_list)
    {
        if(box.base_point.y)
        {
          box.vector -= 1.0;

          needed_to_redraw = true;
        }


      box.base_point.y += box.vector;

        if(box.base_point.y <= 0)
        {
          box.base_point.y = 0;

          box.vector /=  2;
          box.vector *= -1;
        }


      box.correct();
    }


  static void  (*step)() = step_to_north;

    if(count)
    {
      --count;

      step();

      needed_to_redraw = true;
    }


    if(!count)
    {
      count = 4;

        switch(rng.get())
        {
      case(0): step = step_to_north;break;
      case(1): step = step_to_south;break;
      case(2): step = step_to_west;break;
      case(3): step = step_to_east;break;
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

  t_img.open("test.png");
  m_img.open("motion.png");

  box_list.emplace_back(Point3(-64,200,-200));
  box_list.emplace_back(Point3(  0,200,-200));

  step_to_north();

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




