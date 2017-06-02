#include<SDL.h>
#include<cmath>
#include<climits>
#include<algorithm>
#include<vector>
#include<cstdio>
#include"isom_LineContext.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


constexpr int   cell_size = 24;
constexpr int  table_size = 20;

constexpr int  screen_w = cell_size*table_size;
constexpr int  screen_h = screen_w;


SDL_Window*    window;
SDL_Renderer*  renderer;


bool  needed_to_redraw = true;


struct
Point
{
  int  x;
  int  y;

  constexpr Point(int  x_=0, int  y_=0):
  x(x_), y(y_){}

};


std::vector<Point>
points;


std::vector<Point>
buffer;


LineContext
lc;


void
render_grid()
{
  SDL_SetRenderDrawColor(renderer,0,0,255,255);

    for(int  n = 0;  n < table_size-1;  ++n)
    {
      int  v = cell_size*(1+n)-1;

      SDL_RenderDrawLine(renderer,v,0,v,screen_h);
      SDL_RenderDrawLine(renderer,0,v,screen_w,v);
    }
}


void
render_line()
{
  SDL_SetRenderDrawColor(renderer,0,0,0,255);

  auto  n = points.size();

  SDL_Rect  rect;

    if(n == 1)
    {
      auto&  pt = points[0];

      rect.x = cell_size*pt.x;
      rect.y = cell_size*pt.y;
      rect.w = cell_size;
      rect.h = cell_size;

      SDL_RenderFillRect(renderer,&rect);
    }

  else
    if(n == 2)
    {
      rect.x = cell_size*std::min(lc.get_x0(),lc.get_x1())+1;
      rect.y = cell_size*std::min(lc.get_y0(),lc.get_y1())+1;

      rect.w = cell_size*(lc.get_x_distance()+1)-1;
      rect.h = cell_size*(lc.get_y_distance()+1)-1;

      SDL_RenderDrawRect(renderer,&rect);

      rect.x += 1;
      rect.y += 1;
      rect.w -= 2;
      rect.h -= 2;

      SDL_RenderDrawRect(renderer,&rect);

      rect.w = cell_size;
      rect.h = cell_size;

        for(auto&  pt: buffer)
        {
          rect.x = cell_size*pt.x;
          rect.y = cell_size*pt.y;

          SDL_RenderFillRect(renderer,&rect);
        }
    }
}


void
render()
{
    if(needed_to_redraw)
    {
      SDL_SetRenderDrawColor(renderer,255,255,255,255);

      SDL_RenderClear(renderer);

      render_line();
      render_grid();

      SDL_RenderPresent(renderer);

      needed_to_redraw = false;
    }
}


void
click(const SDL_MouseButtonEvent&  evt)
{
    if(evt.button == SDL_BUTTON_LEFT)
    {
      auto  n = points.size();

        if(n < 2)
        {
          int  x = evt.x/cell_size;
          int  y = evt.y/cell_size;

          points.emplace_back(x,y);

            if(points.size() == 2)
            {
              lc = LineContext(points[0].x,points[0].y,0,
                               points[1].x,points[1].y,0);
            }
        }

      else
        if(points.size() == 2)
        {
          buffer.emplace_back(lc.get_x(),lc.get_y());

            if(!lc.is_finished())
            {
              lc.step();
            }
        }


      needed_to_redraw = true;
    }

  else
    {
      points.clear();
      buffer.clear();

      needed_to_redraw = true;
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
      case(SDL_QUIT):
          SDL_DestroyRenderer(renderer);
          SDL_DestroyWindow(window);
          SDL_Quit();
          quick_exit(EXIT_SUCCESS);
          break;
#endif
      case(SDL_MOUSEBUTTONDOWN):
          click(evt.button);
          break;
        }
    }


  render();
}


}




int
main(int  argc, char**  argv)
{
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("test LineContext",0,0,screen_w,screen_h,0);

  renderer = SDL_CreateRenderer(window,-1,0);

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




