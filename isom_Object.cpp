#include"isom_Object.hpp"




Object::Object(): kind(ObjectKind::null){}
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
  case(ObjectKind::plane):
      break;
    }
}


Object&
Object::
operator=(Object&&  rhs) noexcept
{
  clear();

  std::swap(kind,rhs.kind);

    switch(kind)
    {
  case(ObjectKind::plane):
      break;
    }
}




void
Object::
clear()
{
    switch(kind)
    {
  case(ObjectKind::plane):
      break;
    }


  kind = ObjectKind::null;
}




void
Object::
render(Renderer&  dst) const
{
    switch(kind)
    {
  case(ObjectKind::plane):
      break;
    }
}




