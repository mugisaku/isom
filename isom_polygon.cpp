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
render(Renderer&  dst) const
{
  FaceRenderingContext  ctx(a,b,c);

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





