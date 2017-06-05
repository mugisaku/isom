#ifndef ISOM_Scene_HPP_INCLUDED
#define ISOM_Scene_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"
#include"isom_plane.hpp"
#include"isom_Object.hpp"
#include<list>




class
Scene
{
  std::list<Object>  object_list;

  Angle   angle;
  Point  center;

public:
  Scene();
  Scene(const Angle  angle_, const Point&  center_, std::list<Object>&&  ls);

  Scene  transform(const Angle  angle_, const Point&  center_) const;

  void  render(Renderer&  dst) const;

};






#endif




