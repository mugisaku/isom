#ifndef ISOM_TextureMappingContext_HPP_INCLUDED
#define ISOM_TextureMappingContext_HPP_INCLUDED


#include"isom_LineContext.hpp"




class
TextureMappingContext
{
  static constexpr int  shift_amount = 20;

  int  u;
  int  v;

  int  count;

  int  amount_of_add_to_u;
  int  amount_of_add_to_v;

public:
  TextureMappingContext(int  u0_=0, int  v0_=0, int  u1_=0, int  v1_=0, int  count_=0);


  int  get_u() const{return u>>shift_amount;}
  int  get_v() const{return v>>shift_amount;}

  void  reset(int  u0_, int  v0_, int  u1_, int  v1_, int  count_);

  bool  is_finished() const
  {
    return !count;
  }


  void  step();

};




#endif




