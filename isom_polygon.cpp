#include"isom_polygon.hpp"
#include"isom_line.hpp"




const Image*
Polygon::
texture_image;


void
Polygon::
transform(const Transformer&  tr)
{
  static_cast<Point&>(a) = tr(a);
  static_cast<Point&>(b) = tr(b);
  static_cast<Point&>(c) = tr(c);
}




namespace{




void
step(Renderer&  renderer, int  x, int  y, int  z, int  r, int  g, int  b, double  lighting)
{
  auto&  dlcolor = renderer.lightset->directional.color;
  auto&  abcolor =     renderer.lightset->ambient.color;

    if(Polygon::texture_image)
    {
      auto&  src_color = Polygon::texture_image->get_color(r,g);

      r = abcolor.r+(dlcolor.r*lighting);
      g = abcolor.g+(dlcolor.g*lighting);
      b = abcolor.b+(dlcolor.b*lighting);

      r = (r+src_color.r)/2;
      g = (g+src_color.g)/2;
      b = (b+src_color.b)/2;

        if(r > 255){r = 255;}
        if(g > 255){g = 255;}
        if(b > 255){b = 255;}


      Color  color(r,g,b,255);

      renderer.put(color,x,y,z,0);
    }

  else
    {
      int  r_base = abcolor.r+(dlcolor.r*lighting);
      int  g_base = abcolor.g+(dlcolor.g*lighting);
      int  b_base = abcolor.b+(dlcolor.b*lighting);

      r += r_base;
      g += g_base;
      b += b_base;

        if(r > 255){r = 255;}
        if(g > 255){g = 255;}
        if(b > 255){b = 255;}


      Color  color(r,g,b,255);

      renderer.put(color,x,y,z,0);
    }
}


void
run(Renderer&  renderer, LineContext&   longer, LineContext&   longer_mapper,
                         LineContext&  shorter, LineContext&  shorter_mapper, double  lighting)
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
          step(renderer,plotter.get_x(),plotter.get_y(),plotter.get_z(),
                         mapper.get_x(), mapper.get_y(), mapper.get_z(),lighting);

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


}


void
Polygon::
render(Renderer&  renderer) const
{
  Vector  aa(a);
  Vector  bb(b);
  Vector  cc(c);

  Vector  ab(bb-aa);
  Vector  bc(cc-bb);

  auto  normal_vector = ab*bc;

  normal_vector.normalize();

    if(normal_vector.z < 0)
    {
//      render_wire(renderer);

      return;
    }


  using T = const Vertex&;

  auto&     top = static_cast<T>(::upper(a,::upper(b,c)));
  auto&  bottom = static_cast<T>(::lower(a,::lower(b,c)));

  auto&  middle = static_cast<T>(((&a != &top) && (&a != &bottom))? a:
                                 ((&b != &top) && (&b != &bottom))? b:c);

  LineContext  longer(   top,bottom);
  LineContext   upper(   top,middle);
  LineContext   lower(middle,bottom);

  LineContext  longer_mapper(   top,bottom,longer.get_distance());
  LineContext   upper_mapper(   top,middle, upper.get_distance());
  LineContext   lower_mapper(middle,bottom, lower.get_distance());


  double lighting = -Vector::dot_product(renderer.lightset->directional.transformed_vector,normal_vector);

  lighting = (1+lighting)/2;

    if(std::isnan(lighting))
    {
      lighting = 0;
    }


  run(renderer,longer,longer_mapper,upper,upper_mapper,lighting);
  run(renderer,longer,longer_mapper,lower,lower_mapper,lighting);
}




void
Polygon::
render_wire(Renderer&  renderer) const
{
  Line(Dot(a,white),Dot(b,white)).render(renderer);
  Line(Dot(b,white),Dot(c,white)).render(renderer);
  Line(Dot(c,white),Dot(a,white)).render(renderer);
}




