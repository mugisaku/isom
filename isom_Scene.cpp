#include"isom_Scene.hpp"




constexpr int  sz = 320;


Scene::
Scene()
{
}




Scene::
Scene(std::list<Object>&&  ls):
object_list(std::move(ls))
{
}




Scene
Scene::
transform(const Transformer&  tr) const
{
  Scene  scn;

    for(auto&  obj: object_list)
    {
      scn.object_list.emplace_back(obj);

      scn.object_list.back().transform(tr);
    }


  return std::move(scn);
}


void
Scene::
render(Renderer&  dst) const
{
    for(auto&  obj: object_list)
    {
      obj.render(dst);
    }
}




