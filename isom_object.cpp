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
produce_dotset(DotSet&  set, const LightSet*  lightset) const
{
    for(auto&  line: lines)
    {
      line.produce_dotset(set);
    }


    for(auto&  polygon: polygons)
    {
      polygon.produce_dotset(set,lightset);
    }


    for(auto&  tetragon: tetragons)
    {
      tetragon.produce_dotset(set,lightset);
    }


    for(auto&  child: children)
    {
      child.produce_dotset(set,lightset);
    }
}


void
Object::
render(Renderer&  renderer, std::initializer_list<const Transformer*>  trls, const LightSet*  lightset) const
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


      polygon.render(renderer,lightset);
    }


    for(auto  tetragon: tetragons)
    {
        for(auto  tr: trls)
        {
          tetragon.transform(*tr);
        }


      tetragon.render(renderer,lightset);
    }


    for(auto&  child: children)
    {
      child.render(renderer,trls,lightset);
    }
}




