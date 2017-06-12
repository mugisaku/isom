#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_line.hpp"




Plane::
Plane(const Image*  img, const Rect&  img_rect,
      const Point&  base_, PlaneDirection  dir, int  x_width_,int  y_width_,
      const Point&  center_, const Angle&  angle_):
image(img),
image_rect(img_rect),
base(base_),
direction(dir),
x_width(x_width_),
y_width(y_width_),
center(center_),
angle(angle_)
{
  update();
}




void
Plane::
change_texture(const Image*  img, const Rect&  rect)
{
  image      =  img;
  image_rect = rect;

  update();
}


void
Plane::
build()
{
  bool  xrev = (image_rect.w < 0);
  bool  yrev = (image_rect.h < 0);

  int  w = std::abs(image_rect.w);
  int  h = std::abs(image_rect.h);

  int    left = image_rect.x    ;
  int   right = image_rect.x+w-1;
  int     top = image_rect.y    ;
  int  bottom = image_rect.y+h-1;

    if(xrev){std::swap(left, right);}
    if(yrev){std::swap( top,bottom);}


    switch(direction)
    {
  case(PlaneDirection::top_left):
      polygons[0].a.assign(Point(base.x-x_width,base.y        ,base.z), left,bottom);
      polygons[0].b.assign(Point(base.x-x_width,base.y+y_width,base.z), left,   top);
      polygons[0].c.assign(Point(base.x        ,base.y+y_width,base.z),right,   top);

      polygons[1].a.assign(Point(base.x-x_width,base.y        ,base.z), left,bottom);
      polygons[1].b.assign(Point(base.x        ,base.y+y_width,base.z),   right,top);
      polygons[1].c.assign(Point(base.x        ,base.y        ,base.z),right,bottom);
      break;
  case(PlaneDirection::top_right):
      polygons[0].a.assign(Point(base.x        ,base.y        ,base.z), left,bottom);
      polygons[0].b.assign(Point(base.x        ,base.y+y_width,base.z), left,   top);
      polygons[0].c.assign(Point(base.x+x_width,base.y+y_width,base.z),right,   top);

      polygons[1].a.assign(Point(base.x        ,base.y        ,base.z), left,bottom);
      polygons[1].b.assign(Point(base.x+x_width,base.y+y_width,base.z),   right,top);
      polygons[1].c.assign(Point(base.x+x_width,base.y        ,base.z),right,bottom);
      break;
  case(PlaneDirection::bottom_left):
      polygons[0].a.assign(Point(base.x-x_width,base.y-y_width,base.z), left,bottom);
      polygons[0].b.assign(Point(base.x-x_width,base.y        ,base.z), left,   top);
      polygons[0].c.assign(Point(base.x        ,base.y        ,base.z),right,   top);

      polygons[1].a.assign(Point(base.x-x_width,base.y-y_width,base.z), left,bottom);
      polygons[1].b.assign(Point(base.x        ,base.y        ,base.z),   right,top);
      polygons[1].c.assign(Point(base.x        ,base.y-y_width,base.z),right,bottom);
      break;
  case(PlaneDirection::bottom_right):
      polygons[0].a.assign(Point(base.x        ,base.y-y_width,base.z), left,bottom);
      polygons[0].b.assign(Point(base.x        ,base.y        ,base.z), left,   top);
      polygons[0].c.assign(Point(base.x+x_width,base.y        ,base.z),right,   top);

      polygons[1].a.assign(Point(base.x        ,base.y-y_width,base.z), left,bottom);
      polygons[1].b.assign(Point(base.x+x_width,base.y        ,base.z),   right,top);
      polygons[1].c.assign(Point(base.x+x_width,base.y-y_width,base.z),right,bottom);
      break;
    }


  Transformer  tr;

  tr.change_angle(angle);
  tr.change_center(base+center);

  tr.set_rotation_flag();

    for(auto&  poly: polygons)
    {
      poly.image = image;

      poly.transform(tr);
    }
}


void
Plane::
update()
{
    for(auto&  poly: polygons)
    {
      poly.update();
    }
}


void
Plane::
transform(const Transformer&  tr)
{
    for(auto&  poly: polygons)
    {
      poly.transform(tr);
    }
}


void
Plane::
render(Renderer&  dst) const
{
    for(auto&  poly: polygons)
    {
      poly.render(dst);
    }
}


void
Plane::
render_wire(Renderer&  dst) const
{
  Line  lines[4];

  static_cast<Point&>(lines[0].a) = static_cast<const Point&>(polygons[0].a);
  static_cast<Point&>(lines[0].b) = static_cast<const Point&>(polygons[0].b);
  static_cast<Point&>(lines[1].a) = static_cast<const Point&>(polygons[0].b);
  static_cast<Point&>(lines[1].b) = static_cast<const Point&>(polygons[0].c);
  static_cast<Point&>(lines[2].a) = static_cast<const Point&>(polygons[0].c);
  static_cast<Point&>(lines[2].b) = static_cast<const Point&>(polygons[1].c);
  static_cast<Point&>(lines[3].a) = static_cast<const Point&>(polygons[1].c);
  static_cast<Point&>(lines[3].b) = static_cast<const Point&>(polygons[0].a);

    for(auto&  ln: lines)
    {
      ln.render(dst);
    }
}





