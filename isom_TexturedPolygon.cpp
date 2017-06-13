#include"isom_TexturedPolygon.hpp"




void
TexturedPolygon::
transform(const Transformer&  tr)
{
  static_cast<Point&>(a) = tr(a);
  static_cast<Point&>(b) = tr(b);
  static_cast<Point&>(c) = tr(c);
}




TextureRenderingContext
TexturedPolygon::
make_rendering_context() const
{
  return TextureRenderingContext(*image,a,b,c);
}


FaceRenderingContext
TexturedPolygon::
make_face_rendering_context() const
{
  auto  r = Vector::dot_product(light,normal_vector);

  int  l = 128+(127*r);


  return FaceRenderingContext(Dot(a,Color(l,0,0,255)),
                              Dot(b,Color(l,0,0,255)),
                              Dot(c,Color(l,0,0,255)));
}


void
TexturedPolygon::
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


void
TexturedPolygon::
render(Renderer&  dst) const
{
    if(normal_vector.z < 0)
    {
//      return;
    }


    if(image)
    {
      auto  ctx = make_rendering_context();

        for(;;)
        {
          auto&  p = ctx.get_plotter();

          dst.put(ctx.get_color(),p.get_x(),p.get_y(),p.get_z());

            if(ctx.is_finished())
            {
              break;
            }


          ctx.step();
        }
    }

  else
    {
      auto  ctx = make_face_rendering_context();

        for(;;)
        {
          auto&  p = ctx.get_plotter();

          dst.put(ctx.get_color(),p.get_x(),p.get_y(),p.get_z());

            if(ctx.is_finished())
            {
              break;
            }


          ctx.step();
        }
    }
}





