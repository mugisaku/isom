#ifndef ISOM_PLANE_HPP_INCLUDED
#define ISOM_PLANE_HPP_INCLUDED


#include"isom_point3.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"


enum class
PlaneKind
{
  image,
  color,
  x_image,
  x_color,
  y_image,
  y_color,
  z_image,
  z_color,

};


struct
Plane
{
  PlaneKind  kind;

  Point3  base_point;

  const Image*  image;

  Rect  image_rect;

  Color  color;

  constexpr Plane(PlaneKind  k, Point3  base_, int  w, int  h, Color  color_):
  kind(k),
  base_point(base_),
  image(nullptr),
  image_rect(0,0,w,h),
  color(color_){}

  constexpr Plane(PlaneKind  k, Point3  base_, const Image&  img, Rect  img_rect):
  kind(k),
  base_point(base_),
  image(&img),
  image_rect(img_rect){}

  void  render(Renderer&  dst) const;

  void  render_color(Renderer&  dst) const;
  void  render_image(Renderer&  dst) const;

  void  render_x_color(Renderer&  dst) const;
  void  render_x_image(Renderer&  dst) const;

  void  render_y_color(Renderer&  dst) const;
  void  render_y_image(Renderer&  dst) const;
 
  void  render_z_color(Renderer&  dst) const;
  void  render_z_image(Renderer&  dst) const;

  void  print() const{}

};




#endif




