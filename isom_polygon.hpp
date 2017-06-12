#ifndef ISOM_Polygon_HPP_INCLUDED
#define ISOM_Polygon_HPP_INCLUDED


#include"isom_dot.hpp"
#include"isom_vector.hpp"




struct
Polygon
{
  Dot  a;
  Dot  b;
  Dot  c;

  Vector  normal_vector;

  constexpr Polygon(const Dot&  a_, const Dot&  b_, const Dot&  c_):
  a(a_), b(b_), c(c_){}


  void  update();

  void  transform(const Transformer&  tr);
  void  render(Renderer&  dst) const;

};




#endif




