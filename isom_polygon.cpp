#include"isom_polygon.hpp"
#include"isom_line.hpp"




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

      LineContext  mapper( longer_mapper,
                          shorter_mapper,plotter.get_distance());

        for(;;)
        {
          buffer.emplace_back(plotter.get_x(),plotter.get_y(),plotter.get_z(),
                               mapper.get_x(), mapper.get_y(), mapper.get_z());

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

  LineContext  longer_mapper(   top,bottom,longer.get_distance());
  LineContext   upper_mapper(   top,middle, upper.get_distance());
  LineContext   lower_mapper(middle,bottom, lower.get_distance());

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
produce_dotset(const Image*  texture_image, DotSet&  dotset) const
{
    if(normal_vector.z < 0)
    {
      produce_wire_dotset(dotset);

      return;
    }


  buffer.clear();

  prerender(*this);

    if(texture_image)
    {
        for(auto&  v: buffer)
        {
          auto&  color = texture_image->get_color(v.get_u(),v.get_v());

          dotset->emplace_back(Point(v.x,v.y,v.z),color);
        }
    }

  else
    {
        for(auto&  v: buffer)
        {
          Color  color(v.r,v.g,v.b,255);

          dotset->emplace_back(Point(v.x,v.y,v.z),color);
        }
    }
}


void
Polygon::
produce_dotset(const LightSet&  lightset, const Image*  texture_image, DotSet&  dotset) const
{
    if(normal_vector.z < 0)
    {
      produce_wire_dotset(dotset);

      return;
    }


  buffer.clear();

  prerender(*this);

  auto  p = -Vector::dot_product(lightset.directional.transformed_vector,normal_vector);

  p = (1+p)/2;

    if(std::isnan(p))
    {
      p = 0;
    }


  auto&  dlcolor = lightset.directional.color;
  auto&  abcolor =     lightset.ambient.color;

    if(texture_image)
    {
        for(auto&  v: buffer)
        {
          auto&  src_color = texture_image->get_color(v.get_u(),v.get_v());

          int  r = abcolor.r+(dlcolor.r*p);
          int  g = abcolor.g+(dlcolor.g*p);
          int  b = abcolor.b+(dlcolor.b*p);

          r = (r+src_color.r)/2;
          g = (g+src_color.g)/2;
          b = (b+src_color.b)/2;

            if(r > 255){r = 255;}
            if(g > 255){g = 255;}
            if(b > 255){b = 255;}


          Color  color(r,g,b,255);

          dotset->emplace_back(Point(v.x,v.y,v.z),color);
        }
    }

  else
    {
      int  r_base = abcolor.r+(dlcolor.r*p);
      int  g_base = abcolor.g+(dlcolor.g*p);
      int  b_base = abcolor.b+(dlcolor.b*p);

        for(auto&  v: buffer)
        {
          int  r = r_base+v.r;
          int  g = g_base+v.g;
          int  b = b_base+v.b;

            if(r > 255){r = 255;}
            if(g > 255){g = 255;}
            if(b > 255){b = 255;}


          Color  color(r,g,b,255);

          dotset->emplace_back(Point(v.x,v.y,v.z),color);
        }
    }
}




void
Polygon::
produce_wire_dotset(DotSet&  dotset) const
{
  Line(Dot(a,white),Dot(b,white)).produce_dotset(dotset);
  Line(Dot(b,white),Dot(c,white)).produce_dotset(dotset);
  Line(Dot(c,white),Dot(a,white)).produce_dotset(dotset);
}





