#include"isom_polygon.hpp"
#include"isom_FaceRenderingContext.hpp"




void
Polygon::
transform(const Transformer&  tr)
{
  a.transform(tr);
  b.transform(tr);
  c.transform(tr);
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


void
Polygon::
render(Renderer&  dst) const
{
  auto  r = Vector::dot_product(light,normal_vector);

    if(r > 0)
    {
      auto  f = 255*r;

      Color  color(f,0,0,255);

      FaceRenderingContext  ctx(Dot(a,color),Dot(b,color),Dot(c,color));

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





