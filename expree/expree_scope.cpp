#include"expree_scope.hpp"
#include"expree_MemorySpace.hpp"




namespace expree{


Object&
Scope::
get_object(const std::string&  name)
{
  auto  result = find_object(name);

    if(!result)
    {
      auto  ln = space.make_object();

      ln.name = name;

      link_table.emplace_back(std::move(ln));

      result = &space.get_object(ln.index);
    }


  return *result;
}


Object*
Scope::
find_object(const std::string&  name)
{
    for(auto&  ln: link_table)
    {
        if(ln.name == name)
        {
          return &space.get_object(ln.index);
        }
    }


report;
  return parent? parent->find_object(name):nullptr;
}


Pointer
Scope::
get_pointer(const std::string&  name) const
{
    for(auto&  ln: link_table)
    {
        if(ln.name == name)
        {
          return Pointer(ln.index);
        }
    }


  return parent? parent->get_pointer(name):Pointer();
}


Reference
Scope::
get_reference(const std::string&  name, bool  create_when_found_not)
{
  auto  ptr = get_pointer(name);

    if(ptr)
    {
      return Reference(space,ptr.data);
    }

  else
    if(create_when_found_not)
    {
      auto  ln = space.make_object();

      ln.name = name;

      link_table.emplace_back(std::move(ln));

      return Reference(space,ln.index);
    }



  throw Exception("参照が見つかりませんでした");
}


void
Scope::
print() const
{
  printf("{");

    for(auto&  ln: link_table)
    {
      printf("オブジェクト名:%s, アドレス:0x%04X) = ",ln.name.data(),ln.index);

      space.get_object(ln.index).print();

      printf("\n");
    }


    for(auto&  child: children)
    {
      child.print();
    }


  printf("}");
}


}



