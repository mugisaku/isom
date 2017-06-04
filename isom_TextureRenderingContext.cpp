#include"isom_TextureRenderingContext.hpp"




TextureRenderingContext::
TextureRenderingContext(const Image&  img, const TextureVertex&  a, const TextureVertex&  b, const TextureVertex&  c)
{
  reset(img,a,b,c);
}




void
TextureRenderingContext::
reset(const Image&  img, const TextureVertex&  a, const TextureVertex&  b, const TextureVertex&  c)
{
  using T = const TextureVertex&;

  auto&     top = static_cast<T>(::upper(a,::upper(b,c)));
  auto&  bottom = static_cast<T>(::lower(a,::lower(b,c)));

  auto&  middle = static_cast<T>(((&a != &top) && (&a != &bottom))? a:
                                 ((&b != &top) && (&b != &bottom))? b:c);

  image = &img;

  longer = LineContext(   top,bottom);
   upper = LineContext(   top,middle);
   lower = LineContext(middle,bottom);

  longer_mapper.reset(   top.u,   top.v,bottom.u,bottom.v,longer.get_distance());
   upper_mapper.reset(   top.u,   top.v,middle.u,middle.v, upper.get_distance());
   lower_mapper.reset(middle.u,middle.v,bottom.u,bottom.v, lower.get_distance());


  phase = 1;

  plotting_now = false;
}




const Color&
TextureRenderingContext::
get_color() const
{
  return image->get_color(final_mapper.get_u(),
                          final_mapper.get_v());
}


void
TextureRenderingContext::
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
          longer_mapper.step();
        }
    }

  else
    {
           plotter.step();
      final_mapper.step();
    }
}


void
TextureRenderingContext::
step()
{
    if(plotting_now)
    {
      step_line();
    }

  else
    {
      int  y = longer.get_y();

       auto&  shorter        = ((phase == 1)? upper       :lower       );
       auto&  shorter_mapper = ((phase == 1)? upper_mapper:lower_mapper);

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
          shorter_mapper.step();
        }


      plotter.reset( longer.get_x(),y, longer.get_z(),
                    shorter.get_x(),y,shorter.get_z());

      final_mapper.reset( longer_mapper.get_u(), longer_mapper.get_v(),
                         shorter_mapper.get_u(),shorter_mapper.get_v(),plotter.get_distance());

      plotting_now = true;
    }
}




