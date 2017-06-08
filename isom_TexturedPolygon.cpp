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


void
TexturedPolygon::
render(Renderer&  dst) const
{
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
}





