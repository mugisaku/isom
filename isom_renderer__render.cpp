#include"isom_renderer.hpp"




namespace{
LineContext
make_context(const Point&  a, const Point&  b)
{
  return LineContext(to_int(a.x),
                     to_int(a.y),
                     to_int(a.z),
                     to_int(b.x),
                     to_int(b.y),
                     to_int(b.z));
}
}


void
Renderer::
render_line(const Point&  a, const Point&  b, const Color&  color)
{
  LineContext  lc = make_context(a,b);

    for(;;)
    {
      int  x =  lc.get_x();
      int  y = -lc.get_y();

        if(test(x,y))
        {
          auto&  cell = get_cell(x,y);

          auto  z = lc.get_z();

            if(cell.z <= z)
            {
              cell.r = color.r;
              cell.g = color.g;
              cell.b = color.b;

              cell.z = z;
            }
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
render_polygon_line(LineContext&  longer, LineContext&  shorter, const Color&  color)
{
    for(;;)
    {
      int  y = longer.get_y();

        while(shorter.get_y() > y)
        {
            if(shorter.is_finished())
            {
              return;
            }


          shorter.step();
        }


      render_line(Point( longer.get_x(),y, longer.get_z()),
                  Point(shorter.get_x(),y,shorter.get_z()),color);


        if(longer.is_finished())
        {
          break;
        }


      longer.step();
    }
}


void
Renderer::
render_polygon(const Point&  a, const Point&  b, const Point&  c, const Color&  color)
{
  auto&     top = upper(a,upper(b,c));
  auto&  bottom = lower(a,lower(b,c));

  auto&  middle = ((&a != &top) && (&a != &bottom))? a:
                  ((&b != &top) && (&b != &bottom))? b:c;

  LineContext  longer = make_context(top,bottom);
  LineContext   upper = make_context(top,middle);
  LineContext   lower = make_context(middle,bottom);

  render_polygon_line(longer,upper,color);
  render_polygon_line(longer,lower,color);
}




void
Renderer::
render_texture_line(const Image&  img, const Point&  a, const Point&  b, const Point&  va, const Point&  vb)
{
  auto   lc = make_context( a, b);
  auto  vlc = LineContext(va.x,va.y,0,vb.x,vb.y,lc.get_distance());

    for(;;)
    {
      int  x =  lc.get_x();
      int  y = -lc.get_y();

        if(test(x,y))
        {
          auto&  cell = get_cell(x,y);

          auto  z = lc.get_z();

            if(cell.z <= z)
            {
              auto&  color = img.get_color(vlc.get_x(),vlc.get_y());

              cell.r = color.r;
              cell.g = color.g;
              cell.b = color.b;

              cell.z = z;
            }
        }


        if(lc.is_finished())
        {
          break;
        }


       lc.step();
      vlc.step();
    }
}


void
Renderer::
render_texture_line(const Image&  img, LineContext&  longer    , LineContext&  shorter,
                                       LineContext&  longer_vtx, LineContext&  shorter_vtx)
{
    for(;;)
    {
      int  y = longer.get_y();

        while(shorter.get_y() > y)
        {
            if(shorter.is_finished())
            {
              return;
            }


          shorter.step();
          shorter_vtx.step();
        }


      render_texture_line(img,Point( longer.get_x(),y, longer.get_z()),
                              Point(shorter.get_x(),y,shorter.get_z()),
                              Point( longer_vtx.get_x(), longer_vtx.get_y(),0),
                              Point(shorter_vtx.get_x(),shorter_vtx.get_y(),0));

        if(longer.is_finished())
        {
          break;
        }


      longer.step();
      longer_vtx.step();
    }
}


void
Renderer::
render_texture(const Image&  img, const TextureVertex&  a, const TextureVertex&  b, const TextureVertex&  c)
{
  auto&     top = static_cast<const TextureVertex&>(upper(a,upper(b,c)));
  auto&  bottom = static_cast<const TextureVertex&>(lower(a,lower(b,c)));

  auto&  middle = static_cast<const TextureVertex&>(((&a != &top) && (&a != &bottom))? a:
                                                    ((&b != &top) && (&b != &bottom))? b:c);

  LineContext  longer = make_context(   top,bottom);
  LineContext   upper = make_context(   top,middle);
  LineContext   lower = make_context(middle,bottom);

  LineContext  longer_vtx = LineContext(   top.u,   top.v,0,bottom.u,bottom.v,longer.get_distance());
  LineContext   upper_vtx = LineContext(   top.u,   top.v,0,middle.u,middle.v,upper.get_distance());
  LineContext   lower_vtx = LineContext(middle.u,middle.v,0,bottom.u,bottom.v,lower.get_distance());

  render_texture_line(img,longer,upper,longer_vtx,upper_vtx);
  render_texture_line(img,longer,lower,longer_vtx,lower_vtx);
}




