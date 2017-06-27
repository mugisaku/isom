#ifndef ISOM_Tetragon_HPP_INCLUDED
#define ISOM_Tetragon_HPP_INCLUDED


#include"isom_dot.hpp"
#include"isom_vector.hpp"
#include"isom_vertex.hpp"




struct
Tetragon
{
  uint32_t  id;

  Vertex  a;
  Vertex  b;
  Vertex  c;
  Vertex  d;

  constexpr Tetragon(uint32_t  id_=0, const Vertex&  a_=Vertex(),
                                      const Vertex&  b_=Vertex(),
                                      const Vertex&  c_=Vertex(),
                                      const Vertex&  d_=Vertex()):
  id(id_), a(a_), b(b_), c(c_), d(d_){}


  void  transform(const Transformer&  tr);

  void  render(Renderer&  renderer) const;

};




#endif




