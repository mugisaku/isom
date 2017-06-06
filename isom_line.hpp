#ifndef ISOM_Line_HPP_INCLUDED
#define ISOM_Line_HPP_INCLUDED


#include"isom_dot.hpp"



struct
Line
{
  Dot  a;
  Dot  b;

  void  transform(const Transformer&  tr);

  void  render(Renderer&  dst) const;

};




#endif




