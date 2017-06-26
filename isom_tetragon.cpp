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
produce_vertex_string(VertexString&  s) const
{
  Polygon(id,a,b,c,texture_image).produce_vertex_string(s);
  Polygon(id,a,c,d,texture_image).produce_vertex_string(s);
}




void
Tetragon::
produce_dotset(DotSet&  dotset, const LightSet*  lightset) const
{
  Polygon(id,a,b,c,texture_image).produce_dotset(dotset,lightset);
  Polygon(id,a,c,d,texture_image).produce_dotset(dotset,lightset);
}




void
Tetragon::
produce_wire_dotset(DotSet&  dotset) const
{
  Line(Dot(a,white),Dot(b,white)).produce_dotset(dotset);
  Line(Dot(b,white),Dot(c,white)).produce_dotset(dotset);
  Line(Dot(c,white),Dot(d,white)).produce_dotset(dotset);
  Line(Dot(d,white),Dot(a,white)).produce_dotset(dotset);
}


void
Tetragon::
render(Renderer&  renderer, const LightSet*  lightset) const
{
  Polygon(id,a,b,c,texture_image).render(renderer,lightset);
  Polygon(id,a,c,d,texture_image).render(renderer,lightset);
}




