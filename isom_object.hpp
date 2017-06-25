#ifndef ISOM_Object_HPP_INCLUDED
#define ISOM_Object_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_transformer.hpp"
#include"isom_image.hpp"
#include"isom_line.hpp"
#include"isom_polygon.hpp"
#include"isom_tetragon.hpp"




enum class
ObjectKind
{
  null,
  line,
  polygon,
  tetragon,
  object_array,

};


struct Object;

using ObjectArray = std::vector<Object>;


union
ObjectData
{
  Line          line;
  Polygon    polygon;
  Tetragon  tetragon;

  ObjectArray    object_array;

   ObjectData(){}
  ~ObjectData(){}

};


class
Object
{
  uint32_t  id;

  ObjectKind  kind;
  ObjectData  data;

public:
  Object();
  Object(const Line&            ln);
  Object(const Polygon&         po);
  Object(const Tetragon&        te);
  Object(const ObjectArray&  objar);
  Object(const Object&   rhs) noexcept;
  Object(      Object&&  rhs) noexcept;
 ~Object();


  Object&  operator=(const Object&   rhs) noexcept;
  Object&  operator=(      Object&&  rhs) noexcept;

  ObjectData*  operator->(){return &data;}

  uint32_t  get_id() const{return id;}

  void  change_id(uint32_t  id_){id = id_;}

  void  clear();

  void  transform(const Transformer&  tr);

  void  produce_dotset(DotSet&  set) const;


  static void  transform(ObjectArray&  arr, const Transformer&  tr);

  static void  produce_dotset(const ObjectArray&  arr, DotSet&  set);

};






#endif




