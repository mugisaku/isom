#ifndef ISOM_Vector_HPP_INCLUDED
#define ISOM_Vector_HPP_INCLUDED


#include"isom_math.hpp"
#include"isom_point.hpp"




struct
Vector
{
  using  value_type = double;

  value_type  x;
  value_type  y;
  value_type  z;

  constexpr Vector(value_type  x_=0, value_type  y_=0, value_type  z_=0):
  x(x_), y(y_), z(z_){}


  constexpr Vector(const Point&  pt):
  x(pt.x), y(pt.y), z(pt.z){}


  constexpr Vector  operator-(const Vector&  rhs) const
  {
    return Vector(x-rhs.x,y-rhs.y,z-rhs.z);
  }


  constexpr Vector  operator*(const Vector&  rhs) const
  {
    return Vector((y*rhs.z)-(z*rhs.y),
                  (z*rhs.x)-(x*rhs.z),
                  (x*rhs.y)-(y*rhs.x));
  }


  Vector&  normalize()
  {
    auto  length = std::sqrt((x*x)+(y*y)+(z*z));

    x /= length;
    y /= length;
    z /= length;


    return *this;
  }


  static constexpr value_type  dot_product(const Vector&  a, const Vector&  b)
  {
    return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
  }


  static constexpr value_type  cross_product_2d(const Vector&  a, const Vector&  b)
  {
    return (a.x*b.y)-(b.x*a.y);
  }


  static constexpr Vector  cross_product_3d(const Vector&  a, const Vector&  b)
  {
    return a*b;
  }


};


#endif




