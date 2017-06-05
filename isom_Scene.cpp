#include"isom_Scene.hpp"




constexpr int  sz = 320;


Scene::
Scene()
{
}




Scene::
Scene(const Angle  angle_, const Point&  center_, std::list<Object>&&  ls):
object_list(std::move(ls)),
angle(angle_),
center(center_)
{
}




Scene
Scene::
transform(const Angle  angle_, const Point&  center_) const
{
  std::list<Object>  buffer;

  Angle  new_angle = angle;

    for(auto&  obj: object_list)
    {
//      obj.rotate(new_angle);
    }


  return Scene(new_angle,center_,std::move(buffer));
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




