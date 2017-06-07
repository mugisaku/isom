#include"isom_renderer.hpp"
#include"isom_FaceRenderingContext.hpp"




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
put(const Color&  color, int  x, int  y, int  z)
{
  y = -y;

    if(test(x,y))
    {
      auto&  cell = get_cell(x,y);

        if(cell.z <= z)
        {
          cell.r = color.r;
          cell.g = color.g;
          cell.b = color.b;

          cell.z = z;
        }
    }
}


void
Renderer::
put(const LineContext&  lc, const Color&  color)
{
  put(color,lc.get_x(),lc.get_y(),lc.get_z());
}


void
Renderer::
render_line(const Point&  p0, const Point&  p1, const Color&  color)
{
  LineContext  lc(p0,p1);

    for(;;)
    {
      put(lc,color);

        if(lc.is_finished())
        {
          break;
        }


      lc.step();
    }
}


void
Renderer::
render_face(FaceRenderingContext&  ctx)
{
    for(;;)
    {
      auto&  p = ctx.get_plotter();

//      put(ctx.get_color(),p.get_x(),p.get_y(),p.get_z());

        if(ctx.is_finished())
        {
          break;
        }


      ctx.step();
    }
}


void
Renderer::
render_texture(TextureRenderingContext&  ctx)
{
    for(;;)
    {
      auto&  p = ctx.get_plotter();

      put(ctx.get_color(),p.get_x(),p.get_y(),p.get_z());

        if(ctx.is_finished())
        {
          break;
        }


      ctx.step();
    }
}


void
Renderer::
render_image(const Image&  src, const Rect*  src_rect, int  src_z, int  dst_x, int  dst_y)
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




