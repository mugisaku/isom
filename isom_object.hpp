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
#include"isom_polygon.hpp"
#include"isom_TexturedPolygon.hpp"
#include"isom_box.hpp"
#include<list>




enum class
ObjectKind
{
  null,
  dot,
  dotset,
  line,
  polygon,
  textured_polygon,
  plane,
  box,
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
  Plane      plane;

  Polygon                   polygon;
  TexturedPolygon  textured_polygon;

  Box  box;

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
  Object(Dot&&                dt);
  Object(DotSet&&           dtst);
  Object(Line&&               ln);
  Object(Plane&&              pl);
  Object(Polygon&&            po);
  Object(TexturedPolygon&&  txpo);
  Object(Box&&               box);
  Object(ObjectList&&      objls);
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




