#include"isom_point2.hpp"
#include<cstdio>




void
Point2::
assign(int  x_, int  y_)
{
  x = x_;
  y = y_;
}


Point2
Point2::
operator+(const Point2&  rhs) const
{
  return Point2(x+rhs.x,y+rhs.y);
}


Point2
Point2::
operator-(const Point2&  rhs) const
{
  return Point2(x-rhs.x,y-rhs.y);
}


Point2
Point2::
operator*(double  rate) const
{
  return Point2(x*rate,y*rate);
}


Point2&
Point2::
operator+=(const Point2&  rhs)
{
  x += rhs.x;
  y += rhs.y;

  return *this;
}


Point2&
Point2::
operator-=(const Point2&  rhs)
{
  x -= rhs.x;
  y -= rhs.y;

  return *this;
}


Point2&
Point2::
operator*=(double  rate)
{
  x *= rate;
  y *= rate;

  return *this;
}


void
Point2::
print(const char*  s) const
{
  printf("[print %s point] %4d %4d\n",s,x,y);
}




