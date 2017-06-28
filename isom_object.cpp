#include"isom_object.hpp"
#include<new>




Object::Object(): id(0){}


Object::
~Object()
{
  clear();
}




void
Object::
clear()
{
  id = 0;

  lines.clear();
  polygons.clear();
  tetragons.clear();
  children.clear();
}




Line&
Object::
push(Line&&  ln)
{
  lines.emplace_back(std::move(ln));

  return lines.back();
}


Polygon&
Object::
push(Polygon&&  po)
{
  polygons.emplace_back(std::move(po));

  return polygons.back();
}


Tetragon&
Object::
push(Tetragon&&  te)
{
  tetragons.emplace_back(std::move(te));

  return tetragons.back();
}


Object&
Object::
push(Object&&  ob)
{
  children.emplace_back(std::move(ob));

  return children.back();
}




const Object*
Object::
find_child_by_name(const std::string&  name_) const
{
    for(auto&  child: children)
    {
        if(child.name == name_)
        {
          return &child;
        }


      auto  result = child.find_child_by_name(name_);

        if(result)
        {
          return result;
        }
    }


  return nullptr;
}


void
Object::
transform(const Transformer&  tr)
{
    for(auto&  line: lines)
    {
      line.transform(tr);
    }


    for(auto&  polygon: polygons)
    {
      polygon.transform(tr);
    }


    for(auto&  tetragon: tetragons)
    {
      tetragon.transform(tr);
    }


    for(auto&  child: children)
    {
      child.transform(tr);
    }
}


void
Object::
render(Renderer&  renderer, std::initializer_list<const Transformer*>  trls) const
{
    for(auto  line: lines)
    {
        for(auto  tr: trls)
        {
          line.transform(*tr);
        }


      line.render(renderer);
    }


    for(auto  polygon: polygons)
    {
        for(auto  tr: trls)
        {
          polygon.transform(*tr);
        }


      polygon.render(renderer);
    }


    for(auto  tetragon: tetragons)
    {
        for(auto  tr: trls)
        {
          tetragon.transform(*tr);
        }


      tetragon.render(renderer);
    }


    for(auto&  child: children)
    {
      child.render(renderer,trls);
    }
}




