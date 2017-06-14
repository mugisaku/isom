#include"isom_ViewController.hpp"




ViewController::
ViewController(const char*  title_, int  x, int  y, int&  src_x_target_,
                                                    int&  src_y_target_,
                                                    int&  dst_x_target_,
                                                    int&  dst_y_target_):
title(title_),
field_rect(x,y+16,frame_size,frame_size),
src_x_target(src_x_target_),
src_y_target(src_y_target_),
dst_x_target(dst_x_target_),
dst_y_target(dst_y_target_),
x_target(nullptr),
y_target(nullptr)
{
  
}




bool
ViewController::
test(const Mouse&  mouse) const
{
  return field_rect.test(mouse.x,mouse.y);
}


void
ViewController::
process(const Mouse&  mouse)
{
  int  x0 = field_rect.x+( src_x_target+space_halfsize)/(space_size/frame_size);
  int  y0 = field_rect.y+(-src_y_target+space_halfsize)/(space_size/frame_size);
  int  x1 = field_rect.x+( dst_x_target+space_halfsize)/(space_size/frame_size);
  int  y1 = field_rect.y+(-dst_y_target+space_halfsize)/(space_size/frame_size);

    if(x_target)
    {
        if(mouse.left)
        {
          *x_target =  (mouse.x-field_rect.x-frame_halfsize)*(space_size/frame_size);
          *y_target = -(mouse.y-field_rect.y-frame_halfsize)*(space_size/frame_size);
        }

      else
        {
          x_target = nullptr;
          y_target = nullptr;
        }
    }

  else
    if(mouse.left)
    {
        if(make_rect(x0,y0).test(mouse.x,mouse.y))
        {
          x_target = &src_x_target;
          y_target = &src_y_target;
        }

      else
        if(make_rect(x1,y1).test(mouse.x,mouse.y))
        {
          x_target = &dst_x_target;
          y_target = &dst_y_target;
        }
    }
}




Rect
ViewController::
make_rect(int  x, int  y)
{
  return Rect(x-square_halfsize,
              y-square_halfsize,square_size,square_size);
}


void
ViewController::
render(Renderer&  dst) const
{
  dst.draw_string(title,white,field_rect.x,field_rect.y-16);

  dst.draw_rect(field_rect,white);

  dst.draw_line(gray,field_rect.x+frame_halfsize,
                     field_rect.y               ,
                     field_rect.x+frame_halfsize,
                     field_rect.y+frame_size    ,2);
  dst.draw_line(gray,field_rect.x               ,
                     field_rect.y+frame_halfsize,
                     field_rect.x+frame_size    ,
                     field_rect.y+frame_halfsize,2);

  int  x0 = field_rect.x+( src_x_target+space_halfsize)/(space_size/frame_size);
  int  y0 = field_rect.y+(-src_y_target+space_halfsize)/(space_size/frame_size);
  int  x1 = field_rect.x+( dst_x_target+space_halfsize)/(space_size/frame_size);
  int  y1 = field_rect.y+(-dst_y_target+space_halfsize)/(space_size/frame_size);

  dst.draw_line(red,x0,
                    y0,
                    x1,
                    y1);

  dst.fill_rect(make_rect(x0,y0),   red);
  dst.fill_rect(make_rect(x1,y1),yellow);
}




