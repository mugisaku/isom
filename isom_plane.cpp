#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"




void
Plane::
render(Renderer&  dst) const
{
    switch(kind)
    {
  case(PlaneKind::color): render_color(dst);break;
  case(PlaneKind::image): render_image(dst);break;
  case(PlaneKind::x_color): render_x_color(dst);break;
  case(PlaneKind::x_image): render_x_image(dst);break;
  case(PlaneKind::y_color): render_y_color(dst);break;
  case(PlaneKind::y_image): render_y_image(dst);break;
  case(PlaneKind::z_color): render_z_color(dst);break;
  case(PlaneKind::z_image): render_z_image(dst);break;
  default:;
    }
}


void
Plane::
render_color(Renderer&  dst) const
{
  auto  dst_pt = (base_point+offset-dst.offset).to_2d();

  const int  w = image_rect.w;
  const int  h = image_rect.h;

  Cell  cell;

  cell.plane = this;

  static_cast<Color&>(cell) = color;

    for(int  y = h;  y >= 0;  --y){
    for(int  x = 0;  x <  w;  ++x){
        if((x >= 0) &&
           (x <  w) &&
           (y >= 0) &&
           (y <  h))
        {
          cell.w_value = (y)+(base_point.y+offset.y);

          dst.set_cell(cell,dst_pt.x+x,dst_pt.y+y);
        }
    }}
}


void
Plane::
render_image(Renderer&  dst) const
{
  auto  dst_pt = (base_point+offset-dst.offset).to_2d();

  const int  w = image_rect.w;
  const int  h = image_rect.h;

  Cell  cell;

  cell.plane = this;

    for(int  y = h;  y >= 0;  --y){
    for(int  x = 0;  x <  w;  ++x){
        if((x >= 0) &&
           (x <  w) &&
           (y >= 0) &&
           (y <  h))
        {
          static_cast<Color&>(cell) = image->get_color(image_rect.x+x,
                                                       image_rect.y+image_rect.h-1-y);

          cell.w_value = (y)+(base_point.y+offset.y);

          dst.set_cell(cell,dst_pt.x+x,dst_pt.y+y);
        }
    }}
}




