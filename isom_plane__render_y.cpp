#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include<cstdlib>




namespace{
Point2
transform(int  x, int  y)
{
  return Point2((x-y*2)/2,y+(x/2));
}
}


void
Plane::
render_y_color(Renderer&  dst) const
{
  auto  dst_pt = (base_point+offset-dst.offset).to_2d();

  const int  w = image_rect.w;
  const int  h = image_rect.h;

  Cell  cell;

  cell.plane = this;

  static_cast<Color&>(cell) = color;

    for(int  y = h;  y >= -h/2;  --y){
    for(int  x = 0;  x <   w*2;  ++x){
      auto  unpt = transform(x,y);

        if((unpt.x >= 0) &&
           (unpt.x <  w) &&
           (unpt.y >= 0) &&
           (unpt.y <  h))
        {
          cell.w_value = (base_point.y+offset.y);

          dst.set_cell(cell,dst_pt.x+x,dst_pt.y+y);
        }
    }}
}


void
Plane::
render_y_image(Renderer&  dst) const
{
  auto  dst_pt = (base_point+offset-dst.offset).to_2d();

  const int  w = image_rect.w;
  const int  h = image_rect.h;

  Cell  cell;

  cell.plane = this;

    for(int  y = h;  y >= -h/2;  --y){
    for(int  x = 0;  x <   w*2;  ++x){
      auto  unpt = transform(x,y);

        if((unpt.x >= 0) &&
           (unpt.x <  w) &&
           (unpt.y >= 0) &&
           (unpt.y <  h))
        {
          static_cast<Color&>(cell) = image->get_color(image_rect.x+unpt.x,
                                                       image_rect.y+image_rect.h-1-unpt.y);

          cell.w_value = (base_point.y+offset.y);

          dst.set_cell(cell,dst_pt.x+x,dst_pt.y+y);
        }
    }}
}




