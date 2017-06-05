#ifndef ISOM_Object_HPP_INCLUDED
#define ISOM_Object_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"
#include"isom_plane.hpp"
#include<list>




enum class
ObjectKind
{
  null,
  point,
  line,
  polygon,
  plane,

};


union
ObjectData
{
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
  Object(const Object&   rhs) noexcept;
  Object(      Object&&  rhs) noexcept;
 ~Object();


  Object&  operator=(const Object&   rhs) noexcept;
  Object&  operator=(      Object&&  rhs) noexcept;

  void  clear();

  void  rotate(const Angle&  angle);

  void  render(Renderer&  dst) const;

};






#endif




