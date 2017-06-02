#include<SDL.h>
#include<cmath>
#include<climits>
#include<algorithm>
#include<vector>
#include"isom_LineContext.hpp"


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif


namespace{


constexpr int  screen_w = 320;
constexpr int  screen_h = 320;


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


const Point&
upper(const Point&  a, const Point&  b)
{
  return((a.y <= b.y)? a:b);
}


const Point&
lower(const Point&  a, const Point&  b)
{
  return((a.y >= b.y)? a:b);
}


void
draw_hline(int  x0, int  x1, int  y)
{
  int  x     = std::min(x0,x1);
  int  x_end = std::max(x0,x1);

    while(x < x_end)
    {
      SDL_RenderDrawPoint(renderer,x++,y);
    }
}


struct
RenderContext
{
  LineContext  longer;
  LineContext   upper;
  LineContext   lower;

  LineContext*  shorter;

  RenderContext(                                                 ): shorter(nullptr){             }
  RenderContext(const Point&  a, const Point&  b, const Point&  c): shorter(nullptr){reset(a,b,c);}

  void  reset(const Point&  a, const Point&  b, const Point&  c)
  {
    const Point&     top = ::upper(a,::upper(b,c));
    const Point&  bottom = ::lower(a,::lower(b,c));
    const Point&  middle = ((&a != &top) && (&a != &bottom))? a:
                           ((&b != &top) && (&b != &bottom))? b:c;

    longer = LineContext(   top.x,   top.y,bottom.x,bottom.y);
     upper = LineContext(   top.x,   top.y,middle.x,middle.y);
     lower = LineContext(middle.x,middle.y,bottom.x,bottom.y);

    shorter = &upper;
  }

  bool  is_finished() const{return !shorter;}

  void  step()
  {
    auto  y = shorter->get_y();

      while(longer.get_y() != y)
      {
          if(longer.is_finished())
          {
            shorter = nullptr;

            return;
          }


        longer.step();
      }


   draw_hline(shorter->get_x(),longer.get_x(),y);

      if(shorter->is_finished())
      {
        shorter = (shorter == &upper)? &lower:nullptr;

        return;
      }


    shorter->step();
  }

};


std::vector<Point>
points;


void
render()
{
    if(needed_to_redraw)
    {
      SDL_SetRenderDrawColor(renderer,0,0,0,255);

      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer,255,255,255,255);


      auto  n = points.size();

        if(n == 1)
        {
          auto&  pt = points[0];

          SDL_RenderDrawPoint(renderer,pt.x,pt.y);
        }

      else
        if(n == 2)
        {
          auto  c = LineContext(points[0].x,points[0].y,points[1].x,points[1].y);

            while(!c.is_finished())
            {
              SDL_RenderDrawPoint(renderer,c.get_x(),c.get_y());

              c.step();
            }
        }

      else
        if(n == 3)
        {
          static RenderContext  r;

          r.reset(points[0],
                  points[1],
                  points[2]);

            while(!r.is_finished())
            {
              r.step();
            }
        }


      SDL_RenderPresent(renderer);

      needed_to_redraw = false;
    }
}


void
click(const SDL_MouseButtonEvent&  evt)
{
    if(evt.button == SDL_BUTTON_LEFT)
    {
        if(points.size() < 3)
        {
          points.emplace_back(evt.x,evt.y);
        }

      else
        {
          auto&  pt = points[2];

          pt.x = evt.x;
          pt.y = evt.y;
        }


      needed_to_redraw = true;
    }

  else
    {
      points.clear();

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

  window = SDL_CreateWindow("test FillTriangle",0,0,screen_w,screen_h,0);

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




