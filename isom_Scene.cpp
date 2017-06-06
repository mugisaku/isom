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




void
Scene::
transform(const Transformer&  tr)
{
    for(auto&  obj: object_list)
    {
      obj.transform(tr);
    }
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




