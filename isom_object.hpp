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
#include<initializer_list>




class
Object
{
  uint32_t  id;

  std::string  name;

  std::vector<Line>          lines;
  std::vector<Polygon>    polygons;
  std::vector<Tetragon>  tetragons;

  std::vector<Object>  children;

public:
  Object();
 ~Object();


  uint32_t  get_id() const{return id;}

  void  change_id(uint32_t  id_){id = id_;}

  void  clear();

  Line&      push(Line&&      ln);
  Polygon&   push(Polygon&&   po);
  Tetragon&  push(Tetragon&&  te);
  Object&    push(Object&&    ob);

  void  transform(const Transformer&  tr);

  void  produce_dotset(DotSet&  set, const LightSet*  lightset=nullptr) const;

  void  render(Renderer&  renderer, std::initializer_list<const Transformer*>  trls, const LightSet*  lightset=nullptr) const;

};






#endif




