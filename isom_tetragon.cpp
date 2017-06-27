#include"isom_tetragon.hpp"
#include"isom_polygon.hpp"
#include"isom_line.hpp"




void
Tetragon::
transform(const Transformer&  tr)
{
  static_cast<Point&>(a) = tr(a);
  static_cast<Point&>(b) = tr(b);
  static_cast<Point&>(c) = tr(c);
  static_cast<Point&>(d) = tr(d);
}




void
Tetragon::
render(Renderer&  renderer) const
{
  Polygon(id,a,b,c).render(renderer);
  Polygon(id,a,c,d).render(renderer);
}




