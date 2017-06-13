#include"isom_renderer.hpp"
#include"isom_FaceRenderingContext.hpp"




Vector
light(0,0,1);




Renderer::
Renderer(int  w, int  h)
{
  resize(w,h);
}




void
Renderer::
resize(int  w, int  h)
{
  width  = w;
  height = h;

  table.resize(w*h);
}


void
Renderer::
clear()
{
  auto  it = &table[0];

  auto  end = &table[width*height];

    while(it != end)
    {
      it->z     = INT32_MIN;
      it->plane = nullptr;

      it->r = 0;
      it->g = 0;
      it->b = 0;

      ++it;
    }
}




int  Renderer::get_width() const{return width;}
int  Renderer::get_height() const{return height;}


void
Renderer::
set_cell(const Cell&  src, int  x, int  y)
{
    if((x >=     0) &&
       (y >=     0) &&
       (x <  width) &&
       (y < height))
    {
      auto&  dst = table[width*y+x];

        if(dst.z <= src.z)
        {
          dst.plane = src.plane;

            if(src.a)
            {
                if(src.a == 255)
                {
                  dst.z = src.z;

                  dst.r = src.r;
                  dst.g = src.g;
                  dst.b = src.b;
                }

              else
                {
                  dst.r = (((src.r-dst.r)*src.a)>>8)+dst.r;
                  dst.g = (((src.g-dst.g)*src.a)>>8)+dst.g;
                  dst.b = (((src.b-dst.b)*src.a)>>8)+dst.b;
                }
            }
        }
    }
}


Cell&
Renderer::
get_cell(int  x, int  y)
{
  return table[width*y+x];
}


const Cell&
Renderer::
get_cell(int  x, int  y) const
{
  return table[width*y+x];
}




void
Renderer::
put(const Color&  color, int  x, int  y)
{
    if(test(x,y))
    {
      auto&  cell = get_cell(x,y);

      cell.r = color.r;
      cell.g = color.g;
      cell.b = color.b;
    }
}


void
Renderer::
put(const Color&  color, int  x, int  y, int  z)
{
  y = -y;

    if(test(x,y))
    {
      auto&  cell = get_cell(x,y);

        if(cell.z <= z)
        {
            if(color.a)
            {
              auto  a = color.a;

                if(a == 255)
                {
                  cell.r = color.r;
                  cell.g = color.g;
                  cell.b = color.b;
                }

              else
                if(a == 127)
                {
                  cell.r = (color.r+cell.r)/2;
                  cell.g = (color.g+cell.g)/2;
                  cell.b = (color.b+cell.b)/2;
                }

              else
                {
                  cell.r = ((color.r-cell.r)*a>>8)+cell.r;
                  cell.g = ((color.g-cell.g)*a>>8)+cell.g;
                  cell.b = ((color.b-cell.b)*a>>8)+cell.b;
                }


              cell.z = z;
            }
        }
    }
}


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
draw_image(const Image&  src, const Rect*  src_rect, int  src_z, int  dst_x, int  dst_y)
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

      put(color,dst_x+xx,-(dst_y+yy),src_z);
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


namespace{
const Color
red(255,0,0,255);
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




