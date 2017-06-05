#include"isom_rotater.hpp"




constexpr int
to_index(int  degree)
{
  auto  d = degree%360;

  return((d < 0)? d+360:d)/5;
}




Rotater::
Rotater(const Angle&  angle)
{
  auto  xi = to_index(angle.x_degree);
  auto  yi = to_index(angle.y_degree);
  auto  zi = to_index(angle.z_degree);

  x_sin_value = sin_value_table[xi];
  x_cos_value = cos_value_table[xi];
  y_sin_value = sin_value_table[yi];
  y_cos_value = cos_value_table[yi];
  z_sin_value = sin_value_table[zi];
  z_cos_value = cos_value_table[zi];
}





Point
Rotater::
operator()(const Point&  target, const Point&  center) const
{
  double  x0 = target.x-center.x;
  double  y0 = target.y-center.y;
  double  z0 = target.z-center.z;


  double  x1 = (                            x0);
  double  y1 = ( x_cos_value*y0+x_sin_value*z0);
  double  z1 = (-x_sin_value*y0+x_cos_value*z0);


  double  x2 = (y_cos_value*x1+-y_sin_value*z1);
  double  y2 = (                            y1);
  double  z2 = (y_sin_value*x1+ y_cos_value*z1);


  double  tx = ( z_cos_value*x2+z_sin_value*y2)+center.x;
  double  ty = (-z_sin_value*x2+z_cos_value*y2)+center.y;
  double  tz = (                            z2)+center.z;

  return Point(tx,ty,tz);
}




