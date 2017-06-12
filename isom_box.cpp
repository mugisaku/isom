#include"isom_box.hpp"




void
Box::
build(const Point&  base, int  x_width, int  y_width, int  z_width)
{
  auto  pl = &get_top();

  pl->direction = PlaneDirection::bottom_left;
  pl->base = base+Point(x_width,y_width,z_width);
  pl->x_width = x_width;
  pl->y_width = z_width;
  pl->angle.assign(270,0,0);


  pl = &get_right();

  pl->direction = PlaneDirection::bottom_left;
  pl->base = base+Point(x_width,y_width,z_width);
  pl->x_width = z_width;
  pl->y_width = y_width;
  pl->angle.assign(0,90,0);


  pl = &get_front();

  pl->direction = PlaneDirection::bottom_left;
  pl->base = base+Point(x_width,y_width,z_width);
  pl->x_width = x_width;
  pl->y_width = y_width;

  pl->angle.assign(0,0,0);



  pl = &get_bottom();

  pl->direction = PlaneDirection::top_right;
  pl->base = base;
  pl->x_width = x_width;
  pl->y_width = z_width;
  pl->angle.assign(270,0,0);


  pl = &get_left();

  pl->direction = PlaneDirection::top_right;
  pl->base = base;
  pl->x_width = z_width;
  pl->y_width = y_width;
  pl->angle.assign(0,90,0);


  pl = &get_back();

  pl->direction = PlaneDirection::top_right;
  pl->base = base;
  pl->x_width = x_width;
  pl->y_width = y_width;
  pl->angle.assign(0,0,0);

    for(auto&  pl: planes)
    {
      pl.build();
    }
}




void
Box::
transform(const Transformer&  tr)
{
    for(auto&  pl: planes)
    {
      pl.transform(tr);
    }
}


void
Box::
update()
{
    for(auto&  pl: planes)
    {
      pl.update();
    }
}




void
Box::
render(Renderer&  dst) const
{
    for(auto&  pl: planes)
    {
      pl.render(dst);
    }
}


void
Box::
render_wire(Renderer&  dst) const
{
    for(auto&  pl: planes)
    {
      pl.render_wire(dst);
    }
}





