#ifndef ISOM_MATH_HPP_INCLUDED
#define ISOM_MATH_HPP_INCLUDED


#include<cmath>
#include<climits>
#include<algorithm>


constexpr double  pi = 3.14159265358979323846;


constexpr
int
to_int(double  d)
{
  return static_cast<int>(std::round(d));
}


constexpr
double
to_radian(int  degree)
{
  return degree*pi/180;
}


constexpr int
to_degree(double  rad)
{
  return rad*180/pi;
}




#endif




