#include"isom_object.hpp"
#include<new>




Object::Object(                         ): id(0), kind(ObjectKind::null        ){}
Object::Object(const Line&            ln): id(0), kind(ObjectKind::line        ){new(&data) Line(ln);}
Object::Object(const Polygon&         po): id(0), kind(ObjectKind::polygon     ){new(&data) Polygon(po);}
Object::Object(const Tetragon&        te): id(0), kind(ObjectKind::tetragon    ){new(&data) Tetragon(te);}
Object::Object(const ObjectArray&  objar): id(0), kind(ObjectKind::object_array){new(&data) ObjectArray(objar);}
Object::Object(const Object&   rhs) noexcept: id(0), kind(ObjectKind::null){*this = rhs;}
Object::Object(      Object&&  rhs) noexcept: id(0), kind(ObjectKind::null){*this = std::move(rhs);}


Object::
~Object()
{
  clear();
}




Object&
Object::
operator=(const Object&   rhs) noexcept
{
  clear();

  id   = rhs.id;
  kind = rhs.kind;

    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::line):
      new(&data) Line(rhs.data.line);
      break;
  case(ObjectKind::polygon):
      new(&data) Polygon(rhs.data.polygon);
      break;
  case(ObjectKind::tetragon):
      new(&data) Tetragon(rhs.data.tetragon);
      break;
  case(ObjectKind::object_array):
      new(&data) ObjectArray(rhs.data.object_array);
      break;
  default:;
    }


  return *this;
}


Object&
Object::
operator=(Object&&  rhs) noexcept
{
  clear();

  id = id;

  std::swap(kind,rhs.kind);

    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::line):
      new(&data) Line(std::move(rhs.data.line));
      break;
  case(ObjectKind::polygon):
      new(&data) Polygon(std::move(rhs.data.polygon));
      break;
  case(ObjectKind::tetragon):
      new(&data) Tetragon(std::move(rhs.data.tetragon));
      break;
  case(ObjectKind::object_array):
      new(&data) ObjectArray(std::move(rhs.data.object_array));
      break;
  default:;
    }


  return *this;
}




void
Object::
clear()
{
    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::line):
      data.line.~Line();
      break;
  case(ObjectKind::polygon):
      data.polygon.~Polygon();
      break;
  case(ObjectKind::tetragon):
      data.tetragon.~Tetragon();
      break;
  case(ObjectKind::object_array):
      data.object_array.~ObjectArray();
      break;
  default:;
    }


  kind = ObjectKind::null;
}




void
Object::
transform(const Transformer&  tr)
{
    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::line):
      data.line.transform(tr);
      break;
  case(ObjectKind::polygon):
      data.polygon.transform(tr);
      break;
  case(ObjectKind::tetragon):
      data.tetragon.transform(tr);
      break;
  case(ObjectKind::object_array):
      transform(data.object_array,tr);
      break;
  default:;
    }
}


void
Object::
produce_dotset(DotSet&  set) const
{
    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::line):
      data.line.produce_dotset(set);
      break;
  case(ObjectKind::polygon):
      data.polygon.produce_dotset(set);
      break;
  case(ObjectKind::tetragon):
      data.tetragon.produce_dotset(set);
      break;
  case(ObjectKind::object_array):
      produce_dotset(data.object_array,set);
      break;
  default:;
    }
}




void
Object::
transform(ObjectArray&  arr, const Transformer&  tr)
{
    for(auto&  obj: arr)
    {
      obj.transform(tr);
    }
}


void
Object::
produce_dotset(const ObjectArray&  arr, DotSet&  set)
{
    for(auto&  obj: arr)
    {
      obj.produce_dotset(set);
    }
}





