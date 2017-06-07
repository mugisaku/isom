#include"isom_dot.hpp"




void
Dot::
transform(const Transformer&  tr)
{
  static_cast<Point&>(*this) = tr(*this);
}




void
Dot::
render(Renderer&  dst) const
{
  dst.put(color,x,y,z);
}





