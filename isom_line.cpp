#include"isom_line.hpp"




void
Line::
transform(const Transformer&  tr)
{
  a.transform(tr);
  b.transform(tr);
}




void
Line::
render(Renderer&  dst) const
{
  LineContext  lc(a.point,b.point);

  constexpr int  shift_amount = 16;

  auto&  r0 = a.color.r;
  auto&  g0 = a.color.g;
  auto&  b0 = a.color.b;
  auto&  r1 = b.color.r;
  auto&  g1 = b.color.g;
  auto&  b1 = b.color.b;


  int  r_ = r0<<shift_amount;
  int  g_ = g0<<shift_amount;
  int  b_ = b0<<shift_amount;

  auto  d = lc.get_distance();

  int  amount_of_add_to_r = (std::abs(r1-r0)<<shift_amount)/d;
  int  amount_of_add_to_g = (std::abs(g1-g0)<<shift_amount)/d;
  int  amount_of_add_to_b = (std::abs(b1-b0)<<shift_amount)/d;

    if(r0 > r1){amount_of_add_to_r *= -1;}
    if(g0 > g1){amount_of_add_to_g *= -1;}
    if(b0 > b1){amount_of_add_to_b *= -1;}


    for(;;)
    {
      Color  color(r_>>shift_amount,
                   g_>>shift_amount,
                   b_>>shift_amount);

      dst.put(color,lc.get_x(),lc.get_y(),lc.get_z());

        if(lc.is_finished())
        {
          break;
        }


      r_ += amount_of_add_to_r;
      g_ += amount_of_add_to_g;
      b_ += amount_of_add_to_b;

      lc.step();
    }
}





