#ifndef ISOM_Tetragon_HPP_INCLUDED
#define ISOM_Tetragon_HPP_INCLUDED


#include"isom_dot.hpp"
#include"isom_DotSet.hpp"
#include"isom_vector.hpp"
#include"isom_vertex.hpp"
#include"isom_LightSet.hpp"




struct
Tetragon
{
  uint32_t  id;

  Vertex  a;
  Vertex  b;
  Vertex  c;
  Vertex  d;

  const Image*  texture_image;

  constexpr Tetragon(uint32_t  id_=0, const Vertex&  a_=Vertex(),
                                      const Vertex&  b_=Vertex(),
                                      const Vertex&  c_=Vertex(),
                                      const Vertex&  d_=Vertex()):
  id(id_), a(a_), b(b_), c(c_), d(d_), texture_image(nullptr){}


  void  transform(const Transformer&  tr);

  void  produce_vertex_string(VertexString&  s) const;

  void  produce_dotset(                           DotSet&  dotset) const;
  void  produce_dotset(const LightSet&  lightset, DotSet&  dotset) const;

  void  produce_wire_dotset(DotSet&  dotset) const;

};




#endif




