#ifndef ISOM_FaceRenderingContext_HPP_INCLUDED
#define ISOM_FacerRenderingContext_HPP_INCLUDED


#include"isom_LineContext.hpp"
#include"isom_image.hpp"




class
FaceRenderingContext
{
  Color  color;

  LineContext  longer;
  LineContext   upper;
  LineContext   lower;

  LineContext  longer_vtx;
  LineContext   upper_vtx;
  LineContext   lower_vtx;

  LineContext  plotter;

  int  phase;

  bool  plotting_now;
  
  void  step_line();

public:
  FaceRenderingContext(const Color&  color, const Point&  a, const Point&  b, const Point&  c);

  void  reset(const Color&  color, const Point&  a, const Point&  b, const Point&  c);

  const Color&  get_color() const{return color;}

  const LineContext&  get_plotter() const{return plotter;}

  bool  is_finished() const
  {
    return !phase;
  }


  void  step();

  void  print() const;

};




#endif




