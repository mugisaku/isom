#include"isom_renderer.hpp"
#include"isom_dot.hpp"




LightSet
Renderer::
default_lightset;




Renderer::
Renderer(int  x, int  y, int  w, int  h, const LightSet*  ls, bool  buffering_):
x_width(w),
y_width(h),
lightset(ls),
buffering(buffering_)
{
  table.resize(x_width*y_width);

  change_base_point(x,y);
}




bool
Renderer::
test_2d(int  x, int  y) const
{
  return((x >=       0) &&
         (y >=       0) &&
         (x <  x_width) &&
         (y <  y_width));
}


bool
Renderer::
test_3d(int  x, int  y) const
{
  return((x >=   left) &&
         (y >= bottom) &&
         (x <=  right) &&
         (y <=    top));
}


void
Renderer::
clear()
{
  auto  it = &table[0];

  auto  end = &table[x_width*y_width];

    while(it != end)
    {
      it->z  = INT32_MIN;
      it->id =         0;

      it->r = 0;
      it->g = 0;
      it->b = 0;

      ++it;
    }


  dot_buffer.clear();
}


void
Renderer::
change_base_point(int  x, int  y)
{
  int  w = x_width/2;
  int  h = y_width/2;

  x_base = x;
  y_base = y;
  top    = y+h-1;
  left   = x-w-1;
  right  = x+w-1;
  bottom = y-h-1;
}


Cell&
Renderer::
get_cell(int  x, int  y)
{
  return table[x_width*y+x];
}


const Cell&
Renderer::
get_cell(int  x, int  y) const
{
  return table[x_width*y+x];
}




void
Renderer::
put(const Color&  color, int  x, int  y)
{
    if(test_2d(x,y))
    {
      auto&  cell = get_cell(x,y);

      cell.r = color.r;
      cell.g = color.g;
      cell.b = color.b;
    }
}


void
Renderer::
put(const Color&  color, int  x, int  y, int  z, uint32_t  id_)
{
    if(test_3d(x,y))
    {
      auto&  cell = get_cell(x-left,y_width-(y-bottom)-1);

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


              cell.z  =   z;
              cell.id = id_;
            }
        }
    }
}



void
Renderer::
flush()
{
    for(auto&  dot: dot_buffer)
    {
      put(dot.color,dot.x,dot.y,dot.z);
    }


  dot_buffer.clear();
}




