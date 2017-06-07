#ifndef ISOM_Polygon_HPP_INCLUDED
#define ISOM_Polygon_HPP_INCLUDED


#include"isom_dot.hpp"




struct
Polygon
{
  Dot  a;
  Dot  b;
  Dot  c;

  constexpr Polygon(const Dot&  a_, const Dot&  b_, const Dot&  c_):
  a(a_), b(b_), c(c_){}

  void  transform(const Transformer&  tr);
  void  render(Renderer&  dst) const;

};




#endif




