#include"isom_renderer.hpp"




void
Renderer::
draw_line(const Color&  color, int  x0, int  y0, int  x1, int  y1, int  interval)
{
  LineContext  lc(x0,y0,0,x1,y1,0);

  int  n = interval;

    for(;;)
    {
        if(n)
        {
          --n;
        }


        if(!n)
        {
          put(color,lc.get_x(),lc.get_y());

          n = interval;
        }


        if(lc.is_finished())
        {
          break;
        }


      lc.step();
    }
}


void
Renderer::
draw_image(const Image&  src, const Rect*  src_rect, int  dst_x, int  dst_y)
{
  Rect  tmp_rect;

    if(!src_rect)
    {
      tmp_rect.x = 0;
      tmp_rect.y = 0;
      tmp_rect.w = src.get_width();
      tmp_rect.h = src.get_height();

      src_rect = &tmp_rect;
    }


  auto&  rect = *src_rect;

    for(int  yy = 0;  yy < rect.h;  ++yy){
    for(int  xx = 0;  xx < rect.w;  ++xx){
      auto&  color = src.get_color(rect.x+xx,rect.y+yy);

      put(color,dst_x+xx,(dst_y+yy));
    }}
}




void
Renderer::
draw_rect(const Rect&  rect, const Color&  color)
{
  int  x = rect.x;
  int  y = rect.y;
  int  h = rect.h;

    for(int  n = 0;  n < rect.w;  ++n)
    {
      put(color,x+n,y    );
      put(color,x+n,y+h-1);
    }


  y += 1;
  h -= 2;

    while(h--)
    {
      put(color,x         ,y  );
      put(color,x+rect.w-1,y++);
    }
}


void
Renderer::
fill_rect(const Rect&  rect, const Color&  color)
{
    for(int  y = 0;  y < rect.h;  ++y){
    for(int  x = 0;  x < rect.w;  ++x){
      put(color,rect.x+x,rect.y+y);
    }}
}




void
Renderer::
draw_glyph(const Glyph*  gl, const Color&  color, int  x, int  y)
{
    if(!gl)
    {
      return;
    }


  const uint16_t*  p = gl->data;

  x += 2;
  y += 2;

    for(int  yy = 0;  yy < Glyph::size;  ++yy)
    {
      auto  v = *p++;

        for(int  xx = 0;  xx < Glyph::size;  ++xx)
        {

            if(v&0x8000)
            {
              put(color,x+xx,y+yy);
            }


          v <<= 1;
        }
    }
}


void
Renderer::
draw_string(const char*  s, const Color&  color, int  x, int  y)
{
  bool  strong_flag = false;

    while(*s)
    {
      auto  c = *s++;

        if(c == '*')
        {
          strong_flag = !strong_flag;
        }

      else
        {
          draw_glyph(get_glyph(c),strong_flag? red:color,x,y);

          x += 16;
        }
    }
}


void
Renderer::
draw_string(const char16_t*  s, const Color&  color, int  x, int  y)
{
  bool  strong_flag = false;

    while(*s)
    {
      auto  c = *s++;

        if(c == '*')
        {
          strong_flag = !strong_flag;
        }

      else
        {
          draw_glyph(get_glyph(c),strong_flag? red:color,x,y);

          x += 16;
        }
    }
}




