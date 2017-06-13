#ifndef ISOM_TextureRenderingContext_HPP_INCLUDED
#define ISOM_TextureRenderingContext_HPP_INCLUDED


#include"isom_LineContext.hpp"
#include"isom_image.hpp"




struct
PointUV
{
  int  u;
  int  v;

  constexpr PointUV(int  u_=0, int  v_=0):
  u(u_), v(v_){}

  void  assign(int  u_, int  v_)
  {
    u = u_;
    v = v_;
  }

};


struct
TextureVertex: public Point, public PointUV
{
  constexpr TextureVertex(int  x_=0, int  y_=0, int  z_=0, int  u_=0, int  v_=0):
  Point(x_,y_,z_), PointUV(u_,v_){}

  constexpr TextureVertex(const Point&  pt, int  u_, int  v_):
  Point(pt), PointUV(u_,v_){}

  void  assign(int  x_, int  y_, int  z_, int  u_, int  v_)
  {
    x = x_;
    y = y_;
    z = z_;
    u = u_;
    v = v_;
  }

  void  assign(const Point&  pt, int  u_, int  v_)
  {
    static_cast<Point&>(*this) = pt;

    u = u_;
    v = v_;
  }

};


class
TextureRenderingContext
{
  static constexpr int  shift_amount = 20;

  const Image*  image;

  LineContext  longer;
  LineContext   upper;
  LineContext   lower;

  LineContext  plotter;

  LineContext  longer_mapper;
  LineContext   upper_mapper;
  LineContext   lower_mapper;

  LineContext  final_mapper;

  int  phase;

  bool  plotting_now;

  void  step_line();

public:
  TextureRenderingContext();
  TextureRenderingContext(const Image&  img, const TextureVertex&  a, const TextureVertex&  b, const TextureVertex&  c);


  void  reset(const Image&  img, const TextureVertex&  a, const TextureVertex&  b, const TextureVertex&  c);


  const Image&  get_image() const{return *image;}

  const LineContext&  get_mapper() const{return final_mapper;}

  const Color&  get_color() const;

  const LineContext&  get_plotter() const{return plotter;}

  bool  is_finished() const
  {
    return !phase;
  }


  void  step();

  void  print() const;

};




#endif




