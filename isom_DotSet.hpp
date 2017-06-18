#ifndef ISOM_DotSet_HPP_INCLUDED
#define ISOM_DotSet_HPP_INCLUDED


#include"isom_dot.hpp"




struct
DotSet
{
  using List = std::vector<Dot>;

  List  dot_list;


  List&  operator *(){return  dot_list;}
  List*  operator->(){return &dot_list;}

  void  relativize(const Point&  base);

  void  render(Renderer&  dst) const;

};




#endif




