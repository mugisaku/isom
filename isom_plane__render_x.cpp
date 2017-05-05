#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include<cstdlib>




namespace{
Point2
transform(int  x, int  y)
{
  return Point2(x,y-(x/2));
}
}


void
Plane::
render_x_color(Renderer&  dst) const
{
  auto  dst_pt = (base_point+offset-dst.offset).to_2d();

  const int  w = image_rect.w  ;
  const int  h = image_rect.h*2;

  Cell  cell;

  cell.plane = this;

  static_cast<Color&>(cell) = color;

    for(int  y = h;  y >= 0;  --y){
    for(int  x = 0;  x <  w;  ++x){
      auto  unpt = transform(x,y);

        if((unpt.x >=            0) &&
           (unpt.x <  image_rect.w) &&
           (unpt.y >=            0) &&
           (unpt.y <  image_rect.h))
        {
          cell.w_value = (y-(x/2))+(base_point.y+offset.y);

          dst.set_cell(cell,dst_pt.x+x,dst_pt.y+y);
        }
    }}
}


void
Plane::
render_x_image(Renderer&  dst) const
{
  auto  dst_pt = (base_point+offset-dst.offset).to_2d();

  const bool  reversing = (image_rect.w < 0);

  const int  w = std::abs(image_rect.w  );
  const int  h =         (image_rect.h*2);

  Cell  cell;

  cell.plane = this;

    for(int  y = h;  y >= 0;  --y){
    for(int  x = 0;  x <  w;  ++x){
      auto  unpt = transform(x,y);

        if((unpt.x >=            0) &&
           (unpt.x <             w) &&
           (unpt.y >=            0) &&
           (unpt.y <  image_rect.h))
        {
            if(reversing)
            {
              static_cast<Color&>(cell) = image->get_color(image_rect.x+w-1-unpt.x,
                                                           image_rect.y+image_rect.h-1-unpt.y);
            }

          else
            {
              static_cast<Color&>(cell) = image->get_color(image_rect.x+unpt.x,
                                                           image_rect.y+image_rect.h-1-unpt.y);
            }


          cell.w_value = (y-(x/2))+(base_point.y+offset.y);

          dst.set_cell(cell,dst_pt.x+x,dst_pt.y+y);
        }
    }}
}




