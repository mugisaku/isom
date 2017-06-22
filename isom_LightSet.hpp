#ifndef ISOM_LightSet_HPP_INCLUDED
#define ISOM_LightSet_HPP_INCLUDED


#include"isom_image.hpp"
#include"isom_vector.hpp"
#include"isom_transformer.hpp"




struct
LightSet
{
  struct{
    Vector              vector;
    Vector  transformed_vector;

    Color  color;

  } directional;


  struct{
    Color  color;
  } ambient;


  void  transform(const Transformer&  tr)
  {
    directional.transformed_vector = tr(directional.vector);
  }

};




#endif




