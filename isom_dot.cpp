#include"isom_dot.hpp"




void
Dot::
transform(const Transformer&  tr)
{
  static_cast<Point&>(*this) = tr(*this);
}




void
Dot::
render(Renderer&  dst, uint32_t  id) const
{
  dst.put(color,x,y,z,id);
}





