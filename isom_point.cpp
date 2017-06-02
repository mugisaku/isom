#include"isom_point.hpp"
#include<cstdio>




void
Point::
assign(int  x_, int  y_, int  z_)
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


/*
Point2
Point::
to_2d() const
{
  int  x2d =  (x  )-(z  )  ;
  int  y2d = -(z/2)-(x/2)+y;

  return Point2(x2d,y2d);
}
*/

Point
Point::
rotate_x(double  sin_value, double  cos_value) const
{
  value_type  tx = (                       x);
  value_type  ty = ( cos_value*y+sin_value*z);
  value_type  tz = (-sin_value*y+cos_value*z);

  return Point(tx,ty,tz);
}


Point
Point::
rotate_y(double  sin_value, double  cos_value) const
{
  value_type  tx = (cos_value*x+-sin_value*z);
  value_type  ty = (                       y);
  value_type  tz = (sin_value*x+ cos_value*z);

  return Point(tx,ty,tz);
}


Point
Point::
rotate_z(double  sin_value, double  cos_value) const
{
  value_type  tx = ( cos_value*x+sin_value*y);
  value_type  ty = (-sin_value*x+cos_value*y);
  value_type  tz = (                       z);

  return Point(tx,ty,tz);
}


void
Point::
print(const char*  s) const
{
  printf("[print %s point] %4f %4f %4f\n",s,x,y,z);
}




