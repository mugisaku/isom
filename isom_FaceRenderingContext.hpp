#ifndef ISOM_FaceRenderingContext_HPP_INCLUDED
#define ISOM_FaceRenderingContext_HPP_INCLUDED


#include"isom_LineContext.hpp"
#include"isom_dot.hpp"




class
FaceRenderingContext
{
  LineContext  longer;
  LineContext   upper;
  LineContext   lower;

  LineContext  longer_color;
  LineContext   upper_color;
  LineContext   lower_color;
  LineContext   final_color;

  LineContext  plotter;

  int  phase;

  bool  plotting_now;
  
  void  step_line();

public:
  FaceRenderingContext(const Dot&  a, const Dot&  b, const Dot&  c);

  void  reset(const Dot&  a, const Dot&  b, const Dot&  c);

  Color  get_color() const;

  const LineContext&  get_plotter() const{return plotter;}

  bool  is_finished() const
  {
    return !phase;
  }


  void  step();

  void  print() const;

};




#endif




