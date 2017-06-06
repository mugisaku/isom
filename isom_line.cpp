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
render(Renderer&  dst) const
{
  LineContext  lc(a.point,b.point);

  Color  color;

    for(;;)
    {
      dst.put(color,lc.get_x(),lc.get_y(),lc.get_z());

        if(lc.is_finished())
        {
          break;
        }


      lc.step();
    }
}





