#ifndef ISOM_POINT_HPP_INCLUDED
#define ISOM_POINT_HPP_INCLUDED


#include"isom_math.hpp"
#include"isom_angle.hpp"




struct
Point
{
  using value_type = int32_t;

  value_type  x;
  value_type  y;
  value_type  z;

  constexpr Point(value_type  x_=0, value_type  y_=0, value_type  z_=0):
  x(x_), y(y_), z(z_){}


  Point  operator+(const Point&  rhs) const;
  Point  operator-(const Point&  rhs) const;
  Point  operator*(double  rate) const;

  Point&  operator+=(const Point&  rhs);
  Point&  operator-=(const Point&  rhs);
  Point&  operator*=(double  rate);

  void  assign(value_type  x_, value_type  y_, value_type  z_);

  void  print(const char*  s="") const;

  Point  to_isometric_angle() const;

  static Angle  get_angle(const Point&  base, const Point&  target);

};


constexpr const Point&  upper(const Point&  a, const Point&  b){return a.y > b.y? a:b;}
constexpr const Point&  lower(const Point&  a, const Point&  b){return a.y < b.y? a:b;}




#endif




