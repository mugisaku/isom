#include"isom_renderer.hpp"




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
      it->w_value = INT32_MIN;
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

        if(dst.w_value <= src.w_value)
        {
          dst.plane = src.plane;

            if(src.a)
            {
                if(src.a == 255)
                {
                  dst.w_value = src.w_value;

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


const Cell&
Renderer::
get_cell(int  x, int  y) const
{
  return table[width*y+x];
}




