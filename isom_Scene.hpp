#ifndef ISOM_Scene_HPP_INCLUDED
#define ISOM_Scene_HPP_INCLUDED


#include"isom_point.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_image.hpp"
#include"isom_plane.hpp"
#include"isom_Object.hpp"
#include"isom_transformer.hpp"
#include<list>




class
Scene
{
  std::list<Object>  object_list;

public:
  Scene();
  Scene(std::list<Object>&&  ls);

  template<typename  T>
  void  push(T&&  t){object_list.emplace_back(std::move(t));}


  void  transform(const Transformer&  tr);

  void  render(Renderer&  dst) const;

};






#endif




