#include"isom_box.hpp"




void
Box::
build(const Point&  base, int  x_width, int  y_width, int  z_width)
{
  Transformer  tr;


  auto  pl = &get_top();

  tr.change_angle(90,0,0);
  tr.set_rotation_flag();

  pl->build_x(Point(base.x,base.y-z_width,base.z+y_width),x_width,z_width,false);

  pl->transform(tr);



  pl = &get_right();

  pl->build_z(Point(base.x,base.y,base.z),z_width,y_width,false);


  pl = &get_front();

  pl->build_x(Point(base.x,base.y,base.z+z_width),x_width,y_width,false);



  pl = &get_bottom();

  tr.change_angle(90,0,0);
  tr.set_rotation_flag();

  pl->build_x(Point(base.x,base.y,base.z),x_width,z_width,true);

  pl->transform(tr);


  pl = &get_left();

  pl->build_z(Point(base.x+x_width,base.y+y_width,base.z),z_width,y_width,true);


  pl = &get_back();

  pl->build_x(Point(base.x,base.y+y_width,base.z),x_width,y_width,true);

    for(auto&  pl: planes)
    {
//      pl.build();
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





