#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_LineContext.hpp"
#include"isom_image.hpp"
#include<SDL.h>
#include<SDL_image.h>
#include<vector>
#include<cstring>


#ifdef EMSCRIPTEN
#include"emscripten.h"
#endif




namespace{


SDL_Window*    window;
SDL_Surface*  surface;

int  bpp;

uint8_t*  row_table[screen::height];


/*
uint32_t
palette[luminance_table_size];
*/


uint32_t
red;


uint32_t
yellow;


bool
needed_to_redraw;


uint32_t
map_rgb(uint8_t  l)
{
  return SDL_MapRGB(surface->format,l,l,l);
}


}




namespace screen{




void
open()
{
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Isom - " __DATE__,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);

  surface = SDL_GetWindowSurface(window);

  bpp = surface->format->BytesPerPixel;


  auto  p = static_cast<uint8_t*>(surface->pixels);

    for(int  y = 0;  y < surface->h;  ++y)
    {
      row_table[y] = p                  ;
                     p += surface->pitch;
    }


  IMG_Init(IMG_INIT_PNG);
}


void
close()
{
  IMG_Quit();

  SDL_DestroyWindow(window);

  SDL_Quit();
}


uint32_t
get_color(uint8_t  r, uint8_t  g, uint8_t  b)
{
  return SDL_MapRGB(surface->format,r,g,b);
}


uint32_t
get_color(uint8_t  r, uint8_t  g, uint8_t  b, uint8_t  a)
{
  return SDL_MapRGBA(surface->format,r,g,b,a);
}


void
lock()
{
  SDL_LockSurface(surface);
}


void
unlock()
{
  SDL_UnlockSurface(surface);
}


void
clear()
{
  SDL_memset(surface->pixels,0,surface->pitch*height);
}




void
put_color(uint32_t  color, int  x, int  y)
{
  auto  dst = row_table[y]+(bpp*x);

    switch(bpp)
    {
  case(1):                              *dst = color;break;
  case(2): *reinterpret_cast<uint16_t*>(dst) = color;break;
  case(4): *reinterpret_cast<uint32_t*>(dst) = color;break;
    }
}


void
put_color_safely(uint32_t  color, int  x, int  y)
{
    if((x >=     0) &&
       (y >=     0) &&
       (x <  width) &&
       (y < height))
    {
      auto  dst = row_table[y]+(bpp*x);

        switch(bpp)
        {
      case(1):                              *dst = color;break;
      case(2): *reinterpret_cast<uint16_t*>(dst) = color;break;
      case(4): *reinterpret_cast<uint32_t*>(dst) = color;break;
        }
    }
}


void
put_renderer(const Renderer&  src, int  x, int  y, int  w, int  h)
{
    if(!w){w = src.get_x_width();}
    if(!h){h = src.get_y_width();}


    for(int  yy = 0;  yy < h;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      auto&  cell = src.get_cell(x+xx,y+yy);

      put_color(get_color(cell.r,cell.g,cell.b),xx,yy);
    }}
}




void
update()
{
  SDL_UpdateWindowSurface(window);


#ifdef EMSCRIPTEN
/*
  emscripten_run_script(
  "var  cv = document.getElementById(\"canvas\");"
  "var  ln = document.getElementById(\"link\");"
  "ln.href = cv.toDataURL();"
  );
*/
#endif
}


void
save_as_bmp()
{
  SDL_SaveBMP(surface,"__isom.bmp");
}


}




