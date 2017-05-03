#ifndef ISOM_POINT3_HPP_INCLUDED
#define ISOM_POINT3_HPP_INCLUDED


#include"isom_point2.hpp"


struct
Point3
{
  int  x;
  int  y;
  int  z;

  constexpr Point3(int  x_=0, int  y_=0, int  z_=0):
  x(x_), y(y_), z(z_){}


  Point3  operator+(const Point3&  rhs) const;
  Point3  operator-(const Point3&  rhs) const;
  Point3  operator*(double  rate) const;

  Point3&  operator+=(const Point3&  rhs);
  Point3&  operator-=(const Point3&  rhs);
  Point3&  operator*=(double  rate);

  Point2  to_2d() const;

  void  assign(int  x_, int  y_, int  z_);

  void  print(const char*  s="") const;

};




#endif




