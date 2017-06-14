#ifndef ISOM_Angle_HPP_INCLUDED
#define ISOM_Angle_HPP_INCLUDED


#include<cstdio>




struct
Angle
{
  int  x;
  int  y;
  int  z;

  constexpr Angle(int  x_=0, int  y_=0, int  z_=0):
  x(x_), y(y_), z(z_){}

  void  assign(int  x_, int  y_, int  z_)
  {
    x = x_;
    y = y_;
    z = z_;
  }


  Angle  operator+(const Angle&  rhs) const
  {
    return Angle(x+rhs.x,
                 y+rhs.y,
                 z+rhs.z);
  }

  void  print() const
  {
    printf("[angle] X:%4d Y:%4d Z:%4d\n",x%360,y%360,z%360);
  }

};


/*
inline void
print_angle_table()
{
    for(int  i = 0;  i <= 360;  i += 5)
    {
      printf("%10f,//%3d\n",std::sin(to_radian(i)),i);
    }


  printf("-----\n");

    for(int  i = 0;  i <= 360;  i += 5)
    {
      printf("%10f,//%3d\n",std::cos(to_radian(i)),i);
    }
  fflush(stdout);
}
*/


#endif




