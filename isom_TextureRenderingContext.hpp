#ifndef ISOM_TextureRenderingContext_HPP_INCLUDED
#define ISOM_TextureRenderingContext_HPP_INCLUDED


#include"isom_LineContext.hpp"
#include"isom_image.hpp"




class
TextureRenderingContext
{
  const Image*  image;

  LineContext  longer;
  LineContext   upper;
  LineContext   lower;

  LineContext  longer_vtx;
  LineContext   upper_vtx;
  LineContext   lower_vtx;

  LineContext  plotter;
  LineContext   seeker;

  int  phase;

  bool  plotting_now;

  void  step_line();

public:
  TextureRenderingContext(const Image&  img, const Point&  a, const Point&  b, const Point&  c);


  void  reset(const Image&  img, const Point&  a, const Point&  b, const Point&  c);


  const Image&  get_image() const{return *image;}

  const Color&  get_color() const;

  const LineContext&  get_plotter() const{return plotter;}
  const LineContext&  get_seeker()  const{return  seeker;}

  bool  is_finished() const
  {
    return !phase;
  }


  void  step();

  void  print() const;

};




#endif




