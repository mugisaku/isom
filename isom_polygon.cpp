#include"isom_polygon.hpp"




void
Polygon::
transform(const Transformer&  tr)
{
  static_cast<Point&>(a) = tr(a);
  static_cast<Point&>(b) = tr(b);
  static_cast<Point&>(c) = tr(c);
}




void
Polygon::
update()
{
  Vector  aa(a);
  Vector  bb(b);
  Vector  cc(c);

  Vector  ab(bb-aa);
  Vector  bc(cc-bb);


  normal_vector = ab*bc;

  normal_vector.normalize();
}


namespace{


VertexString
buffer;


void
process(LineContext&   longer, LineContext&   longer_mapper,
        LineContext&  shorter, LineContext&  shorter_mapper)
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
          shorter_mapper.step();
        }


      LineContext  plotter( longer.get_x(),y, longer.get_z(),
                           shorter.get_x(),y,shorter.get_z());

      LineContext  mapper( longer_mapper.get_x(), longer_mapper.get_y(),0,
                          shorter_mapper.get_x(),shorter_mapper.get_y(),0,plotter.get_distance());

        for(;;)
        {
          buffer.emplace_back(plotter.get_x(),plotter.get_y(),plotter.get_z(),
                               mapper.get_u(), mapper.get_v());

            if(plotter.is_finished())
            {
                if(longer.is_finished())
                {
                  return;
                }


                     longer.step();
              longer_mapper.step();

              break;
            }


          plotter.step();
           mapper.step();
        }
    }
}


void
prerender(const Polygon&  poly)
{
  using T = const Vertex&;

  auto&     top = static_cast<T>(::upper(poly.a,::upper(poly.b,poly.c)));
  auto&  bottom = static_cast<T>(::lower(poly.a,::lower(poly.b,poly.c)));

  auto&  middle = static_cast<T>(((&poly.a != &top) && (&poly.a != &bottom))? poly.a:
                                 ((&poly.b != &top) && (&poly.b != &bottom))? poly.b:poly.c);

  LineContext  longer(   top,bottom);
  LineContext   upper(   top,middle);
  LineContext   lower(middle,bottom);

  LineContext  longer_mapper(   top.u,   top.v,0,bottom.u,bottom.v,0,longer.get_distance());
  LineContext   upper_mapper(   top.u,   top.v,0,middle.u,middle.v,0, upper.get_distance());
  LineContext   lower_mapper(middle.u,middle.v,0,bottom.u,bottom.v,0, lower.get_distance());

  process(longer,longer_mapper,upper,upper_mapper);
  process(longer,longer_mapper,lower,lower_mapper);
}


}


void
Polygon::
produce_vertex_string(VertexString&  s) const
{
  buffer.clear();

  prerender(*this);

    for(auto&  v: buffer)
    {
      s.emplace_back(v);
    }
}


void
Polygon::
produce_dotset(DotSet&  dotset) const
{
    if(normal_vector.z < 0)
    {
      return;
    }


  buffer.clear();

  prerender(*this);

  auto  r = Vector::dot_product(light,normal_vector);

    if(texture_image)
    {
      int  l = 128+(127*r);

        for(auto&  v: buffer)
        {
          auto  color = texture_image->get_color(v.u,v.v);

          dotset->emplace_back(Point(v.x,v.y,v.z),color);
        }
    }

  else
    {
      int  l = 128+(127*r);

      Color  color(l,l,l,255);

        for(auto&  v: buffer)
        {
          dotset->emplace_back(Point(v.x,v.y,v.z),color);
        }
    }
}





