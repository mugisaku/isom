#include"isom_point.hpp"
#include<cstdio>




void
Point::
assign(value_type  x_, value_type  y_, value_type  z_)
{
  x = x_;
  y = y_;
  z = z_;
}


Point
Point::
operator+(const Point&  rhs) const
{
  return Point(x+rhs.x,y+rhs.y,z+rhs.z);
}


Point
Point::
operator-(const Point&  rhs) const
{
  return Point(x-rhs.x,y-rhs.y,z-rhs.z);
}


Point
Point::
operator*(double  rate) const
{
  return Point(x*rate,y*rate,z*rate);
}


Point&
Point::
operator+=(const Point&  rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;

  return *this;
}


Point&
Point::
operator-=(const Point&  rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;

  return *this;
}


Point&
Point::
operator*=(double  rate)
{
  x *= rate;
  y *= rate;
  z *= rate;

  return *this;
}


Point
Point::
to_isometric_angle() const
{
  value_type  tx =  (x  )-(z  )  ;
  value_type  ty = -(z/2)-(x/2)+y;

  return Point(tx,ty,z);
}


void
Point::
print(const char*  s) const
{
  printf("[print %s point] %4f %4f %4f\n",s,x,y,z);
}




