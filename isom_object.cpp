#include"isom_object.hpp"
#include<new>




Object::Object(                       ): kind(ObjectKind::null            ){}
Object::Object(Dot&&                dt): kind(ObjectKind::dot             ){new(&data) Dot(std::move(dt));}
Object::Object(DotSet&&           dtst): kind(ObjectKind::dotset          ){new(&data) DotSet(std::move(dtst));}
Object::Object(Line&&               ln): kind(ObjectKind::line            ){new(&data) Line(std::move(ln));}
Object::Object(Plane&&              pl): kind(ObjectKind::plane           ){new(&data) Plane(std::move(pl));}
Object::Object(Polygon&&            po): kind(ObjectKind::polygon         ){new(&data) Polygon(std::move(po));}
Object::Object(TexturedPolygon&&  txpo): kind(ObjectKind::textured_polygon){new(&data) TexturedPolygon(std::move(txpo));}
Object::Object(ObjectList&&      objls): kind(ObjectKind::object_list     ){new(&data) ObjectList(std::move(objls));}
Object::Object(const Object&   rhs) noexcept: kind(ObjectKind::null){*this = rhs;}
Object::Object(      Object&&  rhs) noexcept: kind(ObjectKind::null){*this = std::move(rhs);}


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

  kind = rhs.kind;

    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::dot):
      new(&data) Dot(rhs.data.dot);
      break;
  case(ObjectKind::dotset):
      new(&data) DotSet(rhs.data.dotset);
      break;
  case(ObjectKind::line):
      new(&data) Line(rhs.data.line);
      break;
  case(ObjectKind::plane):
      new(&data) Plane(rhs.data.plane);
      break;
  case(ObjectKind::polygon):
      new(&data) Polygon(rhs.data.polygon);
      break;
  case(ObjectKind::textured_polygon):
      new(&data) TexturedPolygon(rhs.data.textured_polygon);
      break;
  case(ObjectKind::object_list):
      new(&data) ObjectList(rhs.data.object_list);
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

  std::swap(kind,rhs.kind);

    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::dot):
      new(&data) Dot(std::move(rhs.data.dot));
      break;
  case(ObjectKind::dotset):
      new(&data) DotSet(std::move(rhs.data.dotset));
      break;
  case(ObjectKind::line):
      new(&data) Line(std::move(rhs.data.line));
      break;
  case(ObjectKind::plane):
      new(&data) Plane(std::move(rhs.data.plane));
      break;
  case(ObjectKind::polygon):
      new(&data) Polygon(std::move(rhs.data.polygon));
      break;
  case(ObjectKind::textured_polygon):
      new(&data) TexturedPolygon(std::move(rhs.data.textured_polygon));
      break;
  case(ObjectKind::object_list):
      new(&data) ObjectList(std::move(rhs.data.object_list));
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
  case(ObjectKind::dot):
      data.dot.~Dot();
      break;
  case(ObjectKind::dotset):
      data.dotset.~DotSet();
      break;
  case(ObjectKind::line):
      data.line.~Line();
      break;
  case(ObjectKind::plane):
      data.plane.~Plane();
      break;
  case(ObjectKind::polygon):
      data.polygon.~Polygon();
      break;
  case(ObjectKind::textured_polygon):
      data.textured_polygon.~TexturedPolygon();
      break;
  case(ObjectKind::object_list):
      data.object_list.~list();
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
  case(ObjectKind::dot):
      data.dot.transform(tr);
      break;
  case(ObjectKind::dotset):
      data.dotset.transform(tr);
      break;
  case(ObjectKind::line):
      data.line.transform(tr);
      break;
  case(ObjectKind::plane):
      data.plane.transform(tr);
      break;
  case(ObjectKind::polygon):
      data.polygon.transform(tr);
      break;
  case(ObjectKind::textured_polygon):
      data.textured_polygon.transform(tr);
      break;
  case(ObjectKind::object_list):
        for(auto&  obj: data.object_list)
        {
          obj.transform(tr);
        }
      break;
  default:;
    }
}


void
Object::
render(Renderer&  dst) const
{
    switch(kind)
    {
  case(ObjectKind::null):
      break;
  case(ObjectKind::dot):
      data.dot.render(dst);
      break;
  case(ObjectKind::dotset):
      data.dotset.render(dst);
      break;
  case(ObjectKind::line):
      data.line.render(dst);
      break;
  case(ObjectKind::plane):
      data.plane.render(dst);
      break;
  case(ObjectKind::polygon):
      data.polygon.render(dst);
      break;
  case(ObjectKind::textured_polygon):
      data.textured_polygon.render(dst);
      break;
  case(ObjectKind::object_list):
        for(auto&  obj: data.object_list)
        {
          obj.render(dst);
        }
      break;
  default:;
    }
}



