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




namespace{


struct
Required
{
  const Polygon*  target;

  const LightSet*  lightset;

  DotSet*  dotset;

  Renderer*  renderer;

  double  lighting;

};


using Callback = void(*)(const Required&  req, int  x, int  y, int  z, int  r, int  g, int  b);


void
process(LineContext&   longer, LineContext&   longer_mapper,
        LineContext&  shorter, LineContext&  shorter_mapper, Callback  cb, const Required&  req)
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
          cb(req,plotter.get_x(),plotter.get_y(),plotter.get_z(),
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
render_internal(const Required&  req, int  x, int  y, int  z, int  r, int  g, int  b)
{
  auto&  dlcolor = req.lightset->directional.color;
  auto&  abcolor =     req.lightset->ambient.color;

    if(req.target->texture_image)
    {
      auto&  src_color = req.target->texture_image->get_color(r,g);

      r = abcolor.r+(dlcolor.r*req.lighting);
      g = abcolor.g+(dlcolor.g*req.lighting);
      b = abcolor.b+(dlcolor.b*req.lighting);

      r = (r+src_color.r)/2;
      g = (g+src_color.g)/2;
      b = (b+src_color.b)/2;

        if(r > 255){r = 255;}
        if(g > 255){g = 255;}
        if(b > 255){b = 255;}


      Color  color(r,g,b,255);

        if(req.dotset)
        {
          (*req.dotset)->emplace_back(Point(x,y,z),color);
        }

      else
        if(req.renderer)
        {
          req.renderer->put(color,x,y,z,0);
        }
    }

  else
    {
      int  r_base = abcolor.r+(dlcolor.r*req.lighting);
      int  g_base = abcolor.g+(dlcolor.g*req.lighting);
      int  b_base = abcolor.b+(dlcolor.b*req.lighting);

      r += r_base;
      g += g_base;
      b += b_base;

        if(r > 255){r = 255;}
        if(g > 255){g = 255;}
        if(b > 255){b = 255;}


      Color  color(r,g,b,255);

        if(req.dotset)
        {
          (*req.dotset)->emplace_back(Point(x,y,z),color);
        }

      else
        if(req.renderer)
        {
          req.renderer->put(color,x,y,z,0);
        }
    }
}


}


void
Polygon::
produce_vertex_string(VertexString&  s) const
{
}




void
Polygon::
render(const Polygon&  self, DotSet*  dotset, Renderer*  renderer, const LightSet*  lightset)
{
  Vector  aa(self.a);
  Vector  bb(self.b);
  Vector  cc(self.c);

  Vector  ab(bb-aa);
  Vector  bc(cc-bb);

  auto  normal_vector = ab*bc;

  normal_vector.normalize();

    if(normal_vector.z < 0)
    {
           if(dotset  ){self.produce_wire_dotset(*dotset);}
      else if(renderer){self.render_wire(*renderer);}

      return;
    }


  using T = const Vertex&;

  auto&     top = static_cast<T>(::upper(self.a,::upper(self.b,self.c)));
  auto&  bottom = static_cast<T>(::lower(self.a,::lower(self.b,self.c)));

  auto&  middle = static_cast<T>(((&self.a != &top) && (&self.a != &bottom))? self.a:
                                 ((&self.b != &top) && (&self.b != &bottom))? self.b:self.c);

  LineContext  longer(   top,bottom);
  LineContext   upper(   top,middle);
  LineContext   lower(middle,bottom);

  LineContext  longer_mapper(   top,bottom,longer.get_distance());
  LineContext   upper_mapper(   top,middle, upper.get_distance());
  LineContext   lower_mapper(middle,bottom, lower.get_distance());

  Required  r;

  r.target = &self;
  r.dotset = dotset;
  r.renderer = renderer;
  r.lightset = lightset;

    if(lightset)
    {
      r.lighting = -Vector::dot_product(lightset->directional.transformed_vector,normal_vector);

      r.lighting = (1+r.lighting)/2;

        if(std::isnan(r.lighting))
        {
          r.lighting = 0;
        }
    }


  process(longer,longer_mapper,upper,upper_mapper,render_internal,r);
  process(longer,longer_mapper,lower,lower_mapper,render_internal,r);
}



void
Polygon::
produce_dotset(DotSet&  dotset, const LightSet*  lightset) const
{
  render(*this,&dotset,nullptr,lightset);
}




void
Polygon::
produce_wire_dotset(DotSet&  dotset) const
{
  Line(Dot(a,white),Dot(b,white)).produce_dotset(dotset);
  Line(Dot(b,white),Dot(c,white)).produce_dotset(dotset);
  Line(Dot(c,white),Dot(a,white)).produce_dotset(dotset);
}


void
Polygon::
render_wire(Renderer&  renderer) const
{
  Line(Dot(a,white),Dot(b,white)).render(renderer);
  Line(Dot(b,white),Dot(c,white)).render(renderer);
  Line(Dot(c,white),Dot(a,white)).render(renderer);
}


void
Polygon::
render(Renderer&  renderer, const LightSet*  lightset) const
{
  render(*this,nullptr,&renderer,lightset);
}




