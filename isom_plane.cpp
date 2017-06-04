#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"




void
Plane::
change_degrees(int  x, int  y, int  z)
{
  x_degree = x;
  y_degree = y;
  z_degree = z;
}


void
Plane::
update()
{
    for(int  i = 0;  i < 4;  ++i)
    {
      transformed_points[i] = points[i];
    }


  rotate_x();
  rotate_y();
  rotate_z();
}


constexpr int
to_index(int  degree)
{
  auto  d = degree%360;

  return((d < 0)? d+360:d)/5;
}


void
Plane::
rotate_x()
{
  auto  d = to_index(x_degree);

  auto  sin_value = sin_value_table[d];
  auto  cos_value = cos_value_table[d];

    for(auto&  pt: transformed_points)
    {
      pt = pt.rotate_x(sin_value,cos_value);
    }
}


void
Plane::
rotate_y()
{
  auto  d = to_index(y_degree);

  auto  sin_value = sin_value_table[d];
  auto  cos_value = cos_value_table[d];

    for(auto&  pt: transformed_points)
    {
      pt = pt.rotate_y(sin_value,cos_value);
    }
}


void
Plane::
rotate_z()
{
  auto  d = to_index(z_degree);

  auto  sin_value = sin_value_table[d];
  auto  cos_value = cos_value_table[d];

    for(auto&  pt: transformed_points)
    {
      pt = pt.rotate_z(sin_value,cos_value);
    }
}




FaceRenderingContext
Plane::
make_face_rendering_context(int  i, const Color&  color) const
{
  auto&  pts = transformed_points;

    if(i == 0){return FaceRenderingContext(color,pts[0],pts[1],pts[2]);}
  else        {return FaceRenderingContext(color,pts[0],pts[2],pts[3]);}
}


TextureRenderingContext
Plane::
make_texture_rendering_context(int  i, const Image&  image) const
{
  auto&  pts = transformed_points;

    if(i == 0){return TextureRenderingContext(image,pts[0],pts[1],pts[2]);}
  else        {return TextureRenderingContext(image,pts[0],pts[2],pts[3]);}
}


void
Plane::
render_face(Renderer&  dst, const Color&  color) const
{
  auto&  pts = transformed_points;


  FaceRenderingContext  frctx(color,pts[0],pts[1],pts[2]);

  dst.render_face(frctx);

  frctx.reset(color,pts[0],pts[2],pts[3]);

  dst.render_face(frctx);
}


void
Plane::
render_wire(Renderer&  dst) const
{
  constexpr Color  wire_color(0xFF,0xFF,0xFF);

  auto&  pts = transformed_points;

  dst.render_line(pts[0],pts[1],wire_color);
  dst.render_line(pts[1],pts[2],wire_color);
  dst.render_line(pts[2],pts[3],wire_color);
  dst.render_line(pts[3],pts[0],wire_color);
}


void
Plane::
render_texture(Renderer&  dst, const Image&  img) const
{
  auto&  pts = transformed_points;

  using T = Point;

  TextureRenderingContext  trctx(img,T(pts[0],0,0),T(pts[1],64, 0),T(pts[2], 0,64));

  dst.render_texture(trctx);

  trctx.reset(img,T(pts[0],0,0),T(pts[2],64,64),T(pts[3],64, 0));

  dst.render_texture(trctx);
}




