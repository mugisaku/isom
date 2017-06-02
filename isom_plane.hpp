#ifndef ISOM_PLANE_HPP_INCLUDED
#define ISOM_PLANE_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"




struct
Plane
{
  Point  base;

  int  x_width;
  int  y_width;

  int  x_degree;
  int  y_degree;
  int  z_degree;

  Point  points[4];

  int  image_x;
  int  image_y;

  constexpr Plane(const Point&  base_, int  x_width_, int  y_width_):
  base(base_),
  x_width(x_width_),
  y_width(y_width_),
  x_degree(0),
  y_degree(0),
  z_degree(0),
  image_x(0),
  image_y(0){update();}


  void  update();
  void  rotate_x();
  void  rotate_y();
  void  rotate_z();

  void  render_face(Renderer&  dst) const;
  void  render_wire(Renderer&  dst) const;

};






#endif




