#include"isom_line.hpp"




void
Line::
transform(const Transformer&  tr)
{
  a.transform(tr);
  b.transform(tr);
}




void
Line::
render(Renderer&  renderer) const
{
  LineContext        lc(a,b);
  LineContext  color_lc(a.color.r,a.color.g,a.color.b,
                        b.color.r,b.color.g,b.color.b,lc.get_distance());

    for(;;)
    {
      Color  color(color_lc.get_x(),
                   color_lc.get_y(),
                   color_lc.get_z(),255);

      renderer.put(color,lc.get_x(),lc.get_y(),lc.get_z(),0);

        if(lc.is_finished())
        {
          break;
        }


            lc.step();
      color_lc.step();
    }
}





