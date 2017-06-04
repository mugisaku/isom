#include"isom_FaceRenderingContext.hpp"




FaceRenderingContext::
FaceRenderingContext(const Color&  color, const Point&  a, const Point&  b, const Point&  c)
{
  reset(color,a,b,c);
}




void
FaceRenderingContext::
reset(const Color&  color_, const Point&  a, const Point&  b, const Point&  c)
{
  auto&     top = ::upper(a,::upper(b,c));
  auto&  bottom = ::lower(a,::lower(b,c));

  auto&  middle = ((&a != &top) && (&a != &bottom))? a:
                  ((&b != &top) && (&b != &bottom))? b:c;

  color = (color_);

  longer = LineContext(top,bottom);
   upper = LineContext(top,middle);
   lower = LineContext(middle,bottom);

  phase = 1;

  plotting_now = false;
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
        }
    }

  else
    {
      plotter.step();
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

      LineContext&  shorter = ((phase == 1)? upper:lower);

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
        }


      plotter = LineContext( longer.get_x(),y, longer.get_z(),
                            shorter.get_x(),y,shorter.get_z());

      plotting_now = true;
    }
}



