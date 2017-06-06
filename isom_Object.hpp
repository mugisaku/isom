#ifndef ISOM_Object_HPP_INCLUDED
#define ISOM_Object_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_transformer.hpp"
#include"isom_image.hpp"
#include"isom_plane.hpp"
#include"isom_dot.hpp"
#include"isom_DotSet.hpp"
#include"isom_line.hpp"
#include"isom_triangle.hpp"
#include"isom_polygon.hpp"
#include<list>




enum class
ObjectKind
{
  null,
  dot,
  dotset,
  line,
  triangle,
  plane,
  object_list,

};


struct Object;

using ObjectList = std::list<Object>;


union
ObjectData
{
  Dot          dot;
  DotSet    dotset;
  Line        line;
  Polygon  polygon;
  Plane      plane;

  ObjectList  object_list;  

   ObjectData(){}
  ~ObjectData(){}

};


class
Object
{
  ObjectKind  kind;
  ObjectData  data;

public:
  Object();
  Object(Dot&&            dt);
  Object(DotSet&&       dtst);
  Object(Plane&&          pl);
  Object(ObjectList&&  objls);
  Object(const Object&   rhs) noexcept;
  Object(      Object&&  rhs) noexcept;
 ~Object();


  Object&  operator=(const Object&   rhs) noexcept;
  Object&  operator=(      Object&&  rhs) noexcept;

  ObjectData*  operator->(){return &data;}

  void  clear();

  void  transform(const Transformer&  tr);

  void  render(Renderer&  dst) const;

};






#endif




