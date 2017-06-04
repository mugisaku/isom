#ifndef ISOM_POINT_HPP_INCLUDED
#define ISOM_POINT_HPP_INCLUDED


#include"isom_math.hpp"


struct
Point
{
  using value_type = double;

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

  void  assign(int  x_, int  y_, int  z_);

  void  print(const char*  s="") const;

  Point  rotate_x(double  sin_value, double  cos_value) const;
  Point  rotate_y(double  sin_value, double  cos_value) const;
  Point  rotate_z(double  sin_value, double  cos_value) const;

};


constexpr const Point&  upper(const Point&  a, const Point&  b){return a.y > b.y? a:b;}
constexpr const Point&  lower(const Point&  a, const Point&  b){return a.y < b.y? a:b;}




#endif




