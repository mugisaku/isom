#ifndef ISOM_SCREEN_HPP_INCLUDED
#define ISOM_SCREEN_HPP_INCLUDED


#include<cstdint>
#include<cstdio>


struct Renderer;
struct Image;
struct Rect;


namespace screen{


constexpr int  width  = 540;
constexpr int  height = 540;


constexpr uint32_t  white = 0xFFFFFFFF;
constexpr uint32_t  black = 0;


void   open();
void  close();

uint32_t  get_color(uint8_t  r, uint8_t  g, uint8_t  b);
uint32_t  get_color(uint8_t  r, uint8_t  g, uint8_t  b, uint8_t  a);


void    lock();
void  unlock();

void  clear();

void   put_color(       uint32_t  color, int  x, int  y);
void   put_color_safely(uint32_t  color, int  x, int  y);
void   put_string(const char*      s, uint32_t  color, int  x, int  y);
void   put_string(const char16_t*  s, uint32_t  color, int  x, int  y);
void   put_renderer(const Renderer&  src, int  x, int  y, int  w=0, int  h=0);
void   put_image(const Image&  src, const Rect*  src_rect, int  dst_x, int  dst_y);

void   draw_rectangle(uint32_t  color, int  x, int  y, int  w, int  h);
void   fill_rectangle(uint32_t  color, int  x, int  y, int  w, int  h);

void  update();

void  save_as_bmp();


}




#endif




