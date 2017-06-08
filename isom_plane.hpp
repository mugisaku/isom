#ifndef ISOM_PLANE_HPP_INCLUDED
#define ISOM_PLANE_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"
#include"isom_transformer.hpp"
#include"isom_TexturedPolygon.hpp"




enum class
PlaneDirection
{
  top_left,
  top_right,
  bottom_left,
  bottom_right,

};


struct
Plane
{
  const Image*  image;

  Rect  image_rect;

  Point  base;

  PlaneDirection  direction;

  int  x_width;
  int  y_width;

  Point  center;

  Angle  angle;

  TexturedPolygon  polygons[2];


  Plane(): image(nullptr){}
  Plane(const Image*  img, const Rect&  img_rect,
        const Point&  base_, PlaneDirection  dir, int  x_width_,int  y_width_,
        const Point&  center_, const Angle&  angle_);


  void  update();

  void  transform(const Transformer&  tr);

  void  render(Renderer&  dst) const;
  void  render_wire(Renderer&  dst) const;

};






#endif




