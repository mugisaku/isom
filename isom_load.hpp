#ifndef ISOM_Load_HPP_INCLUDED
#define ISOM_Load_HPP_INCLUDED


#include"isom_object.hpp"
#include<string>


Object  load_object(const std::string&  s);
Object   load_stage(const Object&  objects, const std::string&  s);


#endif




