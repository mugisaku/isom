#ifndef ISOM_DotSet_HPP_INCLUDED
#define ISOM_DotSet_HPP_INCLUDED


#include"isom_image.hpp"




struct
Dot: public Color
{
  int  x;
  int  y;
  int  z;

  constexpr Dot(int  x_, int  y_, int  z_, const Color&  color):
  x(x_), y(y_), z(z_),
  Color(color){}

};


using DotSet = std::vector<Dot>;




#endif




