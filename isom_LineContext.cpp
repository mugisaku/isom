#include"isom_LineContext.hpp"




LineContext::
LineContext(value_type  x0_, value_type  y0_, value_type  z0_,
            value_type  x1_, value_type  y1_, value_type  z1_)
{
  reset(x0_,y0_,z0_,x1_,y1_,z1_);
}


LineContext::
LineContext(value_type  x0_, value_type  y0_, value_type  z0_,
            value_type  x1_, value_type  y1_, value_type  z1_, value_type  dist)
{
  reset(x0_,y0_,z0_,x1_,y1_,z1_,dist);
}


LineContext::
LineContext(const Point&  a, const Point&  b)
{
  reset(to_int(a.x),
        to_int(a.y),
        to_int(a.z),
        to_int(b.x),
        to_int(b.y),
        to_int(b.z));
}


LineContext::
LineContext(const Point&  a, const Point&  b, value_type  dist)
{
  reset(to_int(a.x),
        to_int(a.y),
        to_int(a.z),
        to_int(b.x),
        to_int(b.y),
        to_int(b.z),dist);
}


LineContext::
LineContext(const Color&  a, const Color&  b, value_type  dist)
{
  reset(a.r,
        a.g,
        a.b,
        b.r,
        b.g,
        b.b,dist);
}




void
LineContext::
reset(value_type  x0_, value_type  y0_, value_type  z0_,
      value_type  x1_, value_type  y1_, value_type  z1_)
{
  x = (x0_<<shift_amount);
  y = (y0_<<shift_amount);
  z = (z0_<<shift_amount);

  x0 = (x0_);
  y0 = (y0_);
  z0 = (z0_);

  x1 = (x1_);
  y1 = (y1_);
  z1 = (z1_);

  x_distance = std::abs(x1_-x0_);
  y_distance = std::abs(y1_-y0_);
  z_distance = std::abs(z1_-z0_);

  distance = std::max(x_distance,std::max(y_distance,z_distance));

    if(distance)
    {
      x += basehalf;
      y += basehalf;
      z += basehalf;

        if(distance == x_distance)
        {
          amount_of_add_to_x = base;
          amount_of_add_to_y = (y_distance<<shift_amount)/distance;
          amount_of_add_to_z = (z_distance<<shift_amount)/distance;
        }

      else
        if(distance == y_distance)
        {
          amount_of_add_to_x = (x_distance<<shift_amount)/distance;
          amount_of_add_to_y = base;
          amount_of_add_to_z = (z_distance<<shift_amount)/distance;
        }

      else
        {
          amount_of_add_to_x = (x_distance<<shift_amount)/distance;
          amount_of_add_to_y = (y_distance<<shift_amount)/distance;
          amount_of_add_to_z = base;
        }


        if(x0 > x1){amount_of_add_to_x *= -1;}
        if(y0 > y1){amount_of_add_to_y *= -1;}
        if(z0 > z1){amount_of_add_to_z *= -1;}
    }
}


void
LineContext::
reset(value_type  x0_, value_type  y0_, value_type  z0_,
      value_type  x1_, value_type  y1_, value_type  z1_, value_type  dist)
{
  x = (x0_<<shift_amount);
  y = (y0_<<shift_amount);
  z = (z0_<<shift_amount);

  x0 = (x0_);
  y0 = (y0_);
  z0 = (z0_);

  x1 = (x1_);
  y1 = (y1_);
  z1 = (z1_);

  x_distance = std::abs(x1_-x0_);
  y_distance = std::abs(y1_-y0_);
  z_distance = std::abs(z1_-z0_);

  distance = dist;

    if(distance)
    {
      x += basehalf;
      y += basehalf;
      z += basehalf;

      amount_of_add_to_x = (x_distance<<shift_amount)/distance;
      amount_of_add_to_y = (y_distance<<shift_amount)/distance;
      amount_of_add_to_z = (z_distance<<shift_amount)/distance;

        if(x0 > x1){amount_of_add_to_x *= -1;}
        if(y0 > y1){amount_of_add_to_y *= -1;}
        if(z0 > z1){amount_of_add_to_z *= -1;}
    }
}




void
LineContext::
step()
{
    if(distance)
    {
      x += amount_of_add_to_x;
      y += amount_of_add_to_y;
      z += amount_of_add_to_z;

      --distance;
    }
}


void
LineContext::
print() const
{
  printf("X %4d += %4d\n"  ,x,amount_of_add_to_x);
  printf("Y %4d += %4d\n\n",y,amount_of_add_to_y);
}





