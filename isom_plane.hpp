#ifndef ISOM_PLANE_HPP_INCLUDED
#define ISOM_PLANE_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"
#include"isom_transformer.hpp"




struct
Plane
{
  Point  points[4];

  constexpr Plane(Point  p0=Point(), Point  p1=Point(), Point  p2=Point(), Point  p3=Point()):
  points{p0,p1,p2,p3}{}


  FaceRenderingContext  make_face_rendering_context(int  i, const Color&  color) const;
  TextureRenderingContext  make_texture_rendering_context(int  i, const Image&  image, const Rect&  rect) const;

  void  transform(const Transformer&  tr);

  void  render(Renderer&  dst) const;

  void  render_face(Renderer&  dst, const Color&  color) const;
  void  render_wire(Renderer&  dst) const;
  void  render_texture(Renderer&  dst, const Image&  img, const Rect&  rect) const;

};






#endif




