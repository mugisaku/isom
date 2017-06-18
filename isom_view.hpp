#ifndef ISOM_View_HPP_INCLUDED
#define ISOM_View_HPP_INCLUDED


#include"isom_math.hpp"
#include"isom_point.hpp"
#include"isom_transformer.hpp"




struct
View
{
  Point  src_point;
  Point  dst_point;

  Angle  angle;

  void  update();

  Transformer  make_transformer() const;

};




#endif




