#ifndef ISOM_Polygon_HPP_INCLUDED
#define ISOM_Polygon_HPP_INCLUDED


#include"isom_dot.hpp"
#include"isom_DotSet.hpp"
#include"isom_vector.hpp"
#include"isom_vertex.hpp"




struct
Polygon
{
  Vertex  a;
  Vertex  b;
  Vertex  c;

  Vector  normal_vector;

  const Image*  texture_image;

  constexpr Polygon(const Vertex&  a_=Vertex(),
                    const Vertex&  b_=Vertex(),
                    const Vertex&  c_=Vertex(), const Image*  teximg=nullptr):
  a(a_), b(b_), c(c_), texture_image(teximg){}


  void  update();

  void  transform(const Transformer&  tr);

  void  produce_vertex_string(VertexString&  s) const;

  void  produce_dotset(DotSet&  dotset) const;

};




#endif




