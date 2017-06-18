#ifndef ISOM_PLANE_HPP_INCLUDED
#define ISOM_PLANE_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"
#include"isom_transformer.hpp"
#include"isom_polygon.hpp"




struct
Plane
{
  const Image*  image;

  Polygon  polygons[2];


  Plane(): image(nullptr){}


  void  build_x(const Point&  base, int  w, int  h, bool  clockwise);
  void  build_y(const Point&  base, int  w, int  h, bool  clockwise);
  void  build_z(const Point&  base, int  w, int  h, bool  clockwise);

  void  preset_uv(const Rect&  rect, bool  clockwise);


  void  update();

  void  transform(const Transformer&  tr);

  void  produce_face_dotset(DotSet&  set) const;
  void  produce_wire_dotset(DotSet&  set) const;

};






#endif




