#ifndef ISOM_View_HPP_INCLUDED
#define ISOM_View_HPP_INCLUDED


#include"isom_math.hpp"
#include"isom_point.hpp"
#include"isom_transformer.hpp"




struct
View
{
  Angle  angle;

  Point  dst_point;
  Point  src_point;

  Transformer  make_transformer() const;

};




#endif




