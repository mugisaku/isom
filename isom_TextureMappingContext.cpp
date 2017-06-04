#include"isom_TextureMappingContext.hpp"




TextureMappingContext::
TextureMappingContext(int  u0_, int  v0_, int  u1_, int  v1_, int  count_)
{
  reset(u0_,v0_,u1_,v1_,count_);
}




void
TextureMappingContext::
reset(int  u0_, int  v0_, int  u1_, int  v1_, int  count_)
{
  constexpr int  base = (1<<shift_amount)/2;

  count = count_;

  u = base+(u0_<<shift_amount);
  v = base+(v0_<<shift_amount);

    if(count)
    {
      amount_of_add_to_u = ((std::abs(u1_-u0_)<<shift_amount))/count_;
      amount_of_add_to_v = ((std::abs(v1_-v0_)<<shift_amount))/count_;

        if(u0_ > u1_){amount_of_add_to_u *= -1;}
        if(v0_ > v1_){amount_of_add_to_v *= -1;}
    }
}




void
TextureMappingContext::
step()
{
    if(count)
    {
      u += amount_of_add_to_u;
      v += amount_of_add_to_v;

      --count;
    }
}




