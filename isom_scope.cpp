#include"isom_scope.hpp"




void
Scope::
operator+=(const libjson::Value&  v)
{
  using namespace libjson;

    if(v == ValueKind::object)
    {
        for(auto&  m: v->object)
        {
            if(m.value == ValueKind::number)
            {
              table.emplace_back(m.name,m.value->number);
            }
        }
    }
}


int
Scope::
operator[](const std::string&  name) const
{
    for(auto&  c: table)
    {
        if(c.name == name)
        {
          return c.value;
        }
    }


  return parent? (*parent)[name]:0;
}




