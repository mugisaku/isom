#include"isom_transformer.hpp"




constexpr int
to_index(int  degree)
{
  auto  d = degree%360;

  return((d < 0)? d+360:d)/5;
}




Transformer::
Transformer():
flags(0),
x_rate(1),
y_rate(1),
z_rate(1)
{
  x_sin_value = sin_value_table[0];
  x_cos_value = cos_value_table[0];
  y_sin_value = sin_value_table[0];
  y_cos_value = cos_value_table[0];
  z_sin_value = sin_value_table[0];
  z_cos_value = cos_value_table[0];
}




void
Transformer::
change_angle(int  x, int  y, int  z)
{
  auto  xi = to_index(x);
  auto  yi = to_index(y);
  auto  zi = to_index(z);

  angle.x_degree = x;
  angle.y_degree = y;
  angle.z_degree = z;

  x_sin_value = sin_value_table[xi];
  x_cos_value = cos_value_table[xi];
  y_sin_value = sin_value_table[yi];
  y_cos_value = cos_value_table[yi];
  z_sin_value = sin_value_table[zi];
  z_cos_value = cos_value_table[zi];
}




void
Transformer::
change_scaling_rate(double  x, double  y, double  z)
{
  x_rate = x;
  y_rate = y;
  z_rate = z;
}


void
Transformer::
change_center(int  x, int  y, int  z)
{
  center.assign(x,y,z);
}


void
Transformer::
change_offset(int  x, int  y, int  z)
{
  offset.assign(x,y,z);
}




Point
Transformer::
operator()(const Point&  target) const
{
  auto  pt = target;

    if(test_scaling_flag())
    {
      pt.x *= x_rate;
      pt.y *= y_rate;
      pt.z *= z_rate;
    }


    if(test_rotation_flag())
    {
      double  x0 = pt.x-center.x;
      double  y0 = pt.y-center.y;
      double  z0 = pt.z-center.z;


      double  x1 = (                            x0);
      double  y1 = ( x_cos_value*y0+x_sin_value*z0);
      double  z1 = (-x_sin_value*y0+x_cos_value*z0);


      double  x2 = (y_cos_value*x1+-y_sin_value*z1);
      double  y2 = (                            y1);
      double  z2 = (y_sin_value*x1+ y_cos_value*z1);


      pt.x = ( z_cos_value*x2+z_sin_value*y2)+center.x;
      pt.y = (-z_sin_value*x2+z_cos_value*y2)+center.y;
      pt.z = (                            z2)+center.z;
    }


    if(test_translation_flag())
    {
      pt += offset;
    }


  return pt;
}




