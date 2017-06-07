#include"isom_FaceRenderingContext.hpp"




FaceRenderingContext::
FaceRenderingContext(const Dot&  a, const Dot&  b, const Dot&  c)
{
  reset(a,b,c);
}




void
FaceRenderingContext::
reset(const Dot&  a, const Dot&  b, const Dot&  c)
{
  auto&     top = static_cast<const Dot&>(::upper(a,::upper(b,c)));
  auto&  bottom = static_cast<const Dot&>(::lower(a,::lower(b,c)));

  auto&  middle = static_cast<const Dot&>(((&a != &top) && (&a != &bottom))? a:
                                          ((&b != &top) && (&b != &bottom))? b:c);

  longer = LineContext(top,bottom);
   upper = LineContext(top,middle);
   lower = LineContext(middle,bottom);

  longer_color = LineContext(   top.color,bottom.color,longer.get_distance());
   upper_color = LineContext(   top.color,middle.color, upper.get_distance());
   lower_color = LineContext(middle.color,bottom.color, lower.get_distance());

  phase = 1;

  plotting_now = false;
}




Color
FaceRenderingContext::
get_color() const
{
  return Color(final_color.get_r(),
               final_color.get_g(),
               final_color.get_b(),255);
}


void
FaceRenderingContext::
step_line()
{
    if(plotter.is_finished())
    {
      plotting_now = false;

        if(longer.is_finished())
        {
          phase = 0;
        }

      else
        {
                longer.step();
          longer_color.step();
        }
    }

  else
    {
          plotter.step();
      final_color.step();
    }
}




void
FaceRenderingContext::
step()
{
    if(plotting_now)
    {
      step_line();
    }

  else
    {
      int  y = longer.get_y();

      LineContext&  shorter       = ((phase == 1)? upper      :lower      );
      LineContext&  shorter_color = ((phase == 1)? upper_color:lower_color);

        while(shorter.get_y() > y)
        {
            if(shorter.is_finished())
            {
                if(phase == 1)
                {
                  ++phase;
                }

              else
                {
                  phase = 0;
                }


              return;
            }


                shorter.step();
          shorter_color.step();
        }


      plotter = LineContext( longer.get_x(),y, longer.get_z(),
                            shorter.get_x(),y,shorter.get_z());

      final_color = LineContext( longer_color.get_r(), longer_color.get_g(), longer_color.get_b(),
                                shorter_color.get_r(),shorter_color.get_g(),shorter_color.get_b(),plotter.get_distance());

      plotting_now = true;
    }
}



