#ifndef ISOM_Polygon_HPP_INCLUDED
#define ISOM_Polygon_HPP_INCLUDED


#include"isom_dot.hpp"
#include"isom_vector.hpp"
#include"isom_vertex.hpp"
#include"isom_LightSet.hpp"




struct
Polygon
{
  uint32_t  id;

  Vertex  a;
  Vertex  b;
  Vertex  c;

  static const Image*  texture_image;

  constexpr Polygon(uint32_t  id_=0, const Vertex&  a_=Vertex(),
                                     const Vertex&  b_=Vertex(),
                                     const Vertex&  c_=Vertex()):
  id(id_), a(a_), b(b_), c(c_){}


  void  transform(const Transformer&  tr);

  void  render_wire(Renderer&  renderer) const;
  void  render(     Renderer&  renderer) const;

};




#endif




