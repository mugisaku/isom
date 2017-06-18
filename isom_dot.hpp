#ifndef ISOM_Dot_HPP_INCLUDED
#define ISOM_Dot_HPP_INCLUDED


#include"isom_image.hpp"
#include"isom_point.hpp"
#include"isom_transformer.hpp"
#include"isom_renderer.hpp"




struct
Dot: public Point
{
  Color  color;

  constexpr Dot(const Point&  point_, const Color&  color_):
  Point(point_),
  color(color_){}

  void  transform(const Transformer&  tr);

  void  render(Renderer&  dst, uint32_t  id=0) const;

};




#endif




