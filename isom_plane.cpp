#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"




void
Plane::
update()
{
  points[0].x = base.x;
  points[0].y = base.y;
  points[0].z = base.z;

  points[1].x = base.x;
  points[1].y = base.y+y_width;
  points[1].z = base.z;

  points[2].x = base.x+x_width;
  points[2].y = base.y+y_width;
  points[2].z = base.z;

  points[3].x = base.x+x_width;
  points[3].y = base.y;
  points[3].z = base.z;

  rotate_x();
  rotate_y();
  rotate_z();
}


void
Plane::
rotate_x()
{
  auto  rad = to_radian(x_degree);

  auto  sin_value = std::sin(rad);
  auto  cos_value = std::cos(rad);

    for(auto&  pt: points)
    {
      pt = pt.rotate_x(sin_value,cos_value);
    }
}


void
Plane::
rotate_y()
{
  auto  rad = to_radian(y_degree);

  auto  sin_value = std::sin(rad);
  auto  cos_value = std::cos(rad);

    for(auto&  pt: points)
    {
      pt = pt.rotate_y(sin_value,cos_value);
    }
}


void
Plane::
rotate_z()
{
  auto  rad = to_radian(z_degree);

  auto  sin_value = std::sin(rad);
  auto  cos_value = std::cos(rad);

    for(auto&  pt: points)
    {
      pt = pt.rotate_z(sin_value,cos_value);
    }
}


void
Plane::
render_face(Renderer&  dst) const
{
  dst.render_polygon(points[0],points[1],points[2],Color(0xFF,0xFF,0x00));
  dst.render_polygon(points[0],points[2],points[3],Color(0xFF,0xFF,0x00));
}


void
Plane::
render_wire(Renderer&  dst) const
{
  constexpr Color  wire_color(0xFF,0xFF,0xFF);

  dst.render_line(points[0],points[1],wire_color);
  dst.render_line(points[1],points[2],wire_color);
  dst.render_line(points[2],points[3],wire_color);
  dst.render_line(points[3],points[0],wire_color);
}




