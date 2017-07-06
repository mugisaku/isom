#ifndef expree_MemorySpace_HPP_INCLUDED
#define expree_MemorySpace_HPP_INCLUDED


#include<vector>
#include<string>
#include"expree_scope.hpp"


namespace expree{


struct Object;


struct
Link
{
  int  index;

  std::string  name;

};


class
MemorySpace
{
  std::vector<Object>  object_table;

  Scope  global_scope;

public:
  MemorySpace();

  Object&  get_object(int  i);

  Scope&  get_global_scope(){return global_scope;}

  Link  make_object();

};


}


#endif




