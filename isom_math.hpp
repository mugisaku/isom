#ifndef ISOM_MATH_HPP_INCLUDED
#define ISOM_MATH_HPP_INCLUDED


#include<cmath>
#include<climits>
#include<algorithm>
#include<cstdio>
#include<cstdlib>


constexpr double  pi = 3.14159265358979323846;


#ifndef report
#define  report  printf("[%s %4d] %s\n",__FILE__,__LINE__,__func__);
#endif




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


int  get_randint(int  min, int  max);



constexpr int  value_table_size = 360/5+1;

extern const double  sin_value_table[value_table_size];
extern const double  cos_value_table[value_table_size];




#endif




