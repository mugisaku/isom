#ifndef EXPREE_Scope_HPP_INCLUDED
#define EXPREE_Scope_HPP_INCLUDED


#include<string>
#include<list>
#include"expree_primitive.hpp"
#include"expree_object.hpp"


namespace expree{


struct MemorySpace;
struct Link;


struct
Scope
{
  MemorySpace&  space;

  Scope*  const parent=nullptr;

  std::list<Link>  link_table;

  std::list<Scope>  children;

  Scope(MemorySpace&   space_): space(space_       ), parent( nullptr){}
  Scope(Scope&        parent_): space(parent_.space), parent(&parent_){}

  Object&   get_object(const std::string&  name);
  Object*  find_object(const std::string&  name);

  Pointer      get_pointer(const std::string&  name                                   ) const;
  Reference  get_reference(const std::string&  name, bool  create_when_found_not=false)      ;

  void  print() const;

};


}


#endif




