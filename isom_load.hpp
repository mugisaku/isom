#ifndef ISOM_Load_HPP_INCLUDED
#define ISOM_Load_HPP_INCLUDED


#include"isom_object.hpp"
#include<string>
#include"libjson/json.hpp"
#include"expree_parser.hpp"
#include"expree_scope.hpp"
#include"expree_MemorySpace.hpp"


void  read_expressions(const libjson::Array&  arr, expree::Scope&  scope);

int  load_integer(expree::Scope&  scope, const std::string&  s);

Object  load_object(const std::string&  s);
Object   load_stage(const Object&  objects, const std::string&  s);


#endif




