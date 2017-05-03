#include"isom_point3.hpp"
#include<cstdio>




void
Point3::
assign(int  x_, int  y_, int  z_)
{
  x = x_;
  y = y_;
  z = z_;
}


Point3
Point3::
operator+(const Point3&  rhs) const
{
  return Point3(x+rhs.x,y+rhs.y,z+rhs.z);
}


Point3
Point3::
operator-(const Point3&  rhs) const
{
  return Point3(x-rhs.x,y-rhs.y,z-rhs.z);
}


Point3
Point3::
operator*(double  rate) const
{
  return Point3(x*rate,y*rate,z*rate);
}


Point3&
Point3::
operator+=(const Point3&  rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;

  return *this;
}


Point3&
Point3::
operator-=(const Point3&  rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;

  return *this;
}


Point3&
Point3::
operator*=(double  rate)
{
  x *= rate;
  y *= rate;
  z *= rate;

  return *this;
}


Point2
Point3::
to_2d() const
{
  int  x2d =  (x  )-(z  )  ;
  int  y2d = -(z/2)-(x/2)+y;

  return Point2(x2d,y2d);
}


void
Point3::
print(const char*  s) const
{
  printf("[print %s point] %4d %4d %4d\n",s,x,y,z);
}




