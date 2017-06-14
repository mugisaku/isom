#ifndef ISOM_ViewController_HPP_INCLUDED
#define ISOM_ViewController_HPP_INCLUDED


#include"isom_math.hpp"
#include"isom_point.hpp"
#include"isom_transformer.hpp"
#include"isom_view.hpp"
#include"isom_renderer.hpp"
#include"isom_mouse.hpp"




struct
ViewController
{
  static constexpr int  frame_size =  80;
  static constexpr int  space_size = 512;
  static constexpr int  square_size = 16;

  static constexpr int  frame_halfsize = frame_size/2;
  static constexpr int  space_halfsize = space_size/2;
  static constexpr int  square_halfsize = square_size/2;

  const char*  title;

  Rect  field_rect;

  int&  src_x_target;
  int&  src_y_target;
  int&  dst_x_target;
  int&  dst_y_target;

  int*  x_target;
  int*  y_target;

  static Rect  make_rect(int  x, int  y);

public:
  ViewController(const char*  title_, int  x, int  y, int&  src_x_target_,
                                                      int&  src_y_target_,
                                                      int&  dst_x_target_,
                                                      int&  dst_y_target_);

  bool  test(const Mouse&  mouse) const;

  void  process(const Mouse&  mouse);

  void  render(Renderer&  dst) const;

};




#endif




