#include"isom_TextureRenderingContext.hpp"




TextureRenderingContext::
TextureRenderingContext(const Image&  img, const Point&  a, const Point&  b, const Point&  c)
{
  reset(img,a,b,c);
}




void
TextureRenderingContext::
reset(const Image&  img, const Point&  a, const Point&  b, const Point&  c)
{
  auto&     top = ::upper(a,::upper(b,c));
  auto&  bottom = ::lower(a,::lower(b,c));

  auto&  middle = ((&a != &top) && (&a != &bottom))? a:
                  ((&b != &top) && (&b != &bottom))? b:c;

  image = &img;

  longer = LineContext(   top,bottom);
   upper = LineContext(   top,middle);
   lower = LineContext(middle,bottom);

  longer_vtx = LineContext(   top.u,   top.v,0,bottom.u,bottom.v,longer.get_distance());
   upper_vtx = LineContext(   top.u,   top.v,0,middle.u,middle.v, upper.get_distance());
   lower_vtx = LineContext(middle.u,middle.v,0,bottom.u,bottom.v, lower.get_distance());

  phase = 1;

  plotting_now = false;
}




const Color&
TextureRenderingContext::
get_color() const
{
  return image->get_color(seeker.get_x(),seeker.get_y());
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
          longer_vtx.step();
        }
    }

  else
    {
      plotter.step();
       seeker.step();
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

       LineContext&  shorter     = ((phase == 1)? upper    :lower    );
       LineContext&  shorter_vtx = ((phase == 1)? upper_vtx:lower_vtx);

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
          shorter_vtx.step();
        }


      plotter = LineContext( longer.get_x(),y, longer.get_z(),
                            shorter.get_x(),y,shorter.get_z());

       seeker = LineContext( longer_vtx.get_x(), longer_vtx.get_y(),0,
                            shorter_vtx.get_x(),shorter_vtx.get_y(),plotter.get_distance());

      plotting_now = true;
    }
}




