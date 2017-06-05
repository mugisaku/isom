#ifndef ISOM_Rotater_HPP_INCLUDED
#define ISOM_Rotater_HPP_INCLUDED


#include"isom_math.hpp"
#include"isom_point.hpp"




struct
Angle
{
  int  x_degree;
  int  y_degree;
  int  z_degree;

  constexpr Angle(int  x=0, int  y=0, int  z=0):
  x_degree(x), y_degree(y), z_degree(z){}

};


class
Rotater
{
  double  x_sin_value;
  double  x_cos_value;
  double  y_sin_value;
  double  y_cos_value;
  double  z_sin_value;
  double  z_cos_value;

public:
  Rotater(const Angle&  angle);

  Point  operator()(const Point&  target, const Point&  center) const;

};




#endif




