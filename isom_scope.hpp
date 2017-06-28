#ifndef ISOM_Scope_HPP_INCLUDED
#define ISOM_Scope_HPP_INCLUDED


#include<string>
#include<vector>
#include"libjson/json.hpp"
#include"libjson/json_stream.hpp"
#include"libjson/json_FileBuffer.hpp"


struct
Constant
{
  std::string  name;

  int  value;

  Constant(const std::string&  name_, int  v):
  name(name_), value(v){}

};


struct
Scope
{
  const Scope*  const parent=nullptr;

  std::vector<Constant>  table;

  Scope(const Scope*  parent_=nullptr): parent(parent_){}

  void  operator+=(const libjson::Value&  v);
  int  operator[](const std::string&  name) const;

};


#endif




