#ifndef ISOM_LineContext_HPP_INCLUDED
#define ISOM_LineContext_HPP_INCLUDED


#include<cstdint>
#include<cstdio>
#include<algorithm>
#include"isom_point.hpp"
#include"isom_image.hpp"




class
LineContext
{
  using value_type = int32_t;

  static constexpr value_type  shift_amount = 22;

  static constexpr int  base     = (1<<shift_amount);
  static constexpr int  basehalf = base/2;


  value_type  x_distance;
  value_type  y_distance;
  value_type  z_distance;
  value_type    distance;

  value_type  amount_of_add_to_x;
  value_type  amount_of_add_to_y;
  value_type  amount_of_add_to_z;

  value_type  x0;
  value_type  y0;
  value_type  z0;
  value_type  x1;
  value_type  y1;
  value_type  z1;

  value_type  x;
  value_type  y;
  value_type  z;

public:

  LineContext(value_type  x0_=0,
              value_type  y0_=0,
              value_type  z0_=0,
              value_type  x1_=0,
              value_type  y1_=0,
              value_type  z1_=0);

  LineContext(value_type  x0_,
              value_type  y0_,
              value_type  z0_,
              value_type  x1_,
              value_type  y1_,
              value_type  z1_, value_type  dist);

  LineContext(const Point&  a, const Point&  b                  );
  LineContext(const Point&  a, const Point&  b, value_type  dist);
  LineContext(const Color&  a, const Color&  b, value_type  dist);


  void  reset(value_type  x0_,
              value_type  y0_,
              value_type  z0_,
              value_type  x1_,
              value_type  y1_,
              value_type  z1_);

  void  reset(value_type  x0_,
              value_type  y0_,
              value_type  z0_,
              value_type  x1_,
              value_type  y1_,
              value_type  z1_, value_type  dist);

  value_type  get_x() const{return x>>shift_amount;}
  value_type  get_y() const{return y>>shift_amount;}
  value_type  get_z() const{return z>>shift_amount;}

  value_type  get_u() const{return get_x();}
  value_type  get_v() const{return get_y();}

  value_type  get_r() const{return get_x();}
  value_type  get_g() const{return get_y();}
  value_type  get_b() const{return get_z();}

  value_type  get_x0() const{return x0;}
  value_type  get_y0() const{return y0;}
  value_type  get_z0() const{return z0;}
  value_type  get_x1() const{return x1;}
  value_type  get_y1() const{return y1;}
  value_type  get_z1() const{return z1;}

  value_type  get_x_distance() const{return x_distance;}
  value_type  get_y_distance() const{return y_distance;}
  value_type  get_z_distance() const{return z_distance;}
  value_type  get_distance()   const{return   distance;}

  bool  is_finished() const
  {
    return !distance;
  }

  void  step();

  void  print() const;

};




#endif




