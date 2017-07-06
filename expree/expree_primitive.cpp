#include"expree_primitive.hpp"
#include"expree_MemorySpace.hpp"




namespace expree{


Object&
Pointer::
dereference(MemorySpace&  space) const
{
  return space.get_object(data);
}



}




