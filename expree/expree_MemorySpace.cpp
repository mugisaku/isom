#include"expree_MemorySpace.hpp"
#include"expree_object.hpp"




namespace expree{


MemorySpace::
MemorySpace():
global_scope(*this)
{
  object_table.emplace_back(Object());
}




Object&
MemorySpace::
get_object(int  i)
{
  return object_table[i];
}


Link
MemorySpace::
make_object()
{
  Link  ln;

  ln.index = object_table.size();

  object_table.emplace_back(Object());

  return std::move(ln);
}


}




