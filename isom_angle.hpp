#ifndef ISOM_Angle_HPP_INCLUDED
#define ISOM_Angle_HPP_INCLUDED


#include<cstdio>




struct
Angle
{
  int  x_degree;
  int  y_degree;
  int  z_degree;

  constexpr Angle(int  x=0, int  y=0, int  z=0):
  x_degree(x), y_degree(y), z_degree(z){}

  void  assign(int  x, int  y, int  z)
  {
    x_degree = x;
    y_degree = y;
    z_degree = z;
  }


  Angle  operator+(const Angle&  rhs) const
  {
    return Angle(x_degree+rhs.x_degree,
                 y_degree+rhs.y_degree,
                 z_degree+rhs.z_degree);
  }

  void  print() const
  {
    printf("[angle] X:%4d Y:%4d Z:%4d\n",x_degree%360,y_degree%360,z_degree%360);
  }

};




#endif




