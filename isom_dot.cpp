#include"isom_dot.hpp"




void
Dot::
transform(const Transformer&  tr)
{
  point = tr(point);
}




void
Dot::
render(Renderer&  dst) const
{
  dst.put(color,point.x,point.y,point.z);
}





