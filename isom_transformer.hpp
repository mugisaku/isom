#ifndef ISOM_Transformer_HPP_INCLUDED
#define ISOM_Transformer_HPP_INCLUDED


#include"isom_math.hpp"
#include"isom_point.hpp"




struct
Angle
{
  int  x_degree;
  int  y_degree;
  int  z_degree;

  constexpr Angle(int  x=0, int  y=0, int  z=0):
  x_degree(x), y_degree(y), z_degree(z){}

  Angle  operator+(const Angle&  rhs) const
  {
    return Angle(x_degree+rhs.x_degree,
                 y_degree+rhs.y_degree,
                 z_degree+rhs.z_degree);
  }

};




class
Transformer
{
  static constexpr int  translation_flag = 1;
  static constexpr int  scaling_flag     = 2;
  static constexpr int  rotation_flag    = 4;

  int  flags;

  Angle  angle;

  Point  offset;
  Point  center;

  double  x_rate;
  double  y_rate;
  double  z_rate;

  double  x_sin_value;
  double  x_cos_value;
  double  y_sin_value;
  double  y_cos_value;
  double  z_sin_value;
  double  z_cos_value;

public:
  Transformer();

  void  change_angle(int  x, int  y, int  z);

  void  change_scaling_rate(double  x, double  y, double  z);

  void  change_center(double  x, double  y, double  z);
  void  change_offset(double  x, double  y, double  z);

  void  set_translation_flag(){flags |= translation_flag;}
  void  set_scaling_flag()    {flags |=     scaling_flag;}
  void  set_rotation_flag()   {flags |=    rotation_flag;}

  void  unset_translation_flag(){flags &= ~translation_flag;}
  void  unset_scaling_flag()    {flags &=     ~scaling_flag;}
  void  unset_rotation_flag()   {flags &=    ~rotation_flag;}

  bool  test_translation_flag() const{return flags&translation_flag;}
  bool  test_scaling_flag()     const{return flags&    scaling_flag;}
  bool  test_rotation_flag()    const{return flags&   rotation_flag;}

  Point  operator()(const Point&  target) const;

};




#endif




