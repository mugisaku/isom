#include"isom_plane.hpp"
#include"isom_image.hpp"
#include"isom_screen.hpp"
#include"isom_renderer.hpp"
#include"isom_line.hpp"




void
Plane::
build_x(const Point&  base, int  w, int  h, bool  clockwise)
{
    if(clockwise)
    {
      polygons[0].a.Point::assign(base.x  ,base.y  ,base.z);
      polygons[0].b.Point::assign(base.x+w,base.y  ,base.z);
      polygons[0].c.Point::assign(base.x+w,base.y-h,base.z);
      polygons[1].a.Point::assign(base.x  ,base.y  ,base.z);
      polygons[1].b.Point::assign(base.x+w,base.y-h,base.z);
      polygons[1].c.Point::assign(base.x  ,base.y-h,base.z);
    }

  else
    {
      polygons[0].a.Point::assign(base.x  ,base.y  ,base.z);
      polygons[0].b.Point::assign(base.x+w,base.y  ,base.z);
      polygons[0].c.Point::assign(base.x+w,base.y+h,base.z);
      polygons[1].a.Point::assign(base.x  ,base.y  ,base.z);
      polygons[1].b.Point::assign(base.x+w,base.y+h,base.z);
      polygons[1].c.Point::assign(base.x  ,base.y+h,base.z);
    }


  update();
}


void
Plane::
build_y(const Point&  base, int  w, int  h, bool  clockwise)
{
    if(clockwise)
    {
      polygons[0].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[0].b.Point::assign(base.x,base.y+w,base.z  );
      polygons[0].c.Point::assign(base.x,base.y+w,base.z-h);
      polygons[1].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[1].b.Point::assign(base.x,base.y+w,base.z-h);
      polygons[1].c.Point::assign(base.x,base.y  ,base.z-h);
    }

  else
    {
      polygons[0].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[0].b.Point::assign(base.x,base.y+w,base.z  );
      polygons[0].c.Point::assign(base.x,base.y+w,base.z+h);
      polygons[1].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[1].b.Point::assign(base.x,base.y+w,base.z+h);
      polygons[1].c.Point::assign(base.x,base.y  ,base.z+h);
    }


  update();
}


void
Plane::
build_z(const Point&  base, int  w, int  h, bool  clockwise)
{
    if(clockwise)
    {
      polygons[0].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[0].b.Point::assign(base.x,base.y  ,base.z+w);
      polygons[0].c.Point::assign(base.x,base.y-h,base.z+w);
      polygons[1].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[1].b.Point::assign(base.x,base.y-h,base.z+w);
      polygons[1].c.Point::assign(base.x,base.y-h,base.z  );
    }

  else
    {
      polygons[0].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[0].b.Point::assign(base.x,base.y  ,base.z+w);
      polygons[0].c.Point::assign(base.x,base.y+h,base.z+w);
      polygons[1].a.Point::assign(base.x,base.y  ,base.z  );
      polygons[1].b.Point::assign(base.x,base.y+h,base.z+w);
      polygons[1].c.Point::assign(base.x,base.y+h,base.z  );
    }


  update();
}




void
Plane::
preset_uv(const Rect&  rect, bool  clockwise)
{
  bool  x_swap = (rect.w < 0);
  bool  y_swap = (rect.h < 0);

  int  w = std::abs(rect.w);
  int  h = std::abs(rect.h);

  int    left = rect.x    ;
  int   right = rect.x+w-1;
  int     top = rect.y    ;
  int  bottom = rect.y+h-1;

    if(x_swap){std::swap(left, right);}
    if(y_swap){std::swap( top,bottom);}


    if(clockwise)
    {
      polygons[0].a.set_uv( left,   top);
      polygons[0].b.set_uv(right,   top);
      polygons[0].c.set_uv(right,bottom);
      polygons[1].a.set_uv( left,   top);
      polygons[1].b.set_uv(right,bottom);
      polygons[1].c.set_uv( left,bottom);
    }

  else
    {
      polygons[0].a.set_uv( left,bottom);
      polygons[0].b.set_uv(right,bottom);
      polygons[0].c.set_uv(right,   top);
      polygons[1].a.set_uv( left,bottom);
      polygons[1].b.set_uv(right,   top);
      polygons[1].c.set_uv( left,   top);
    }
}


void
Plane::
update()
{
    for(auto&  poly: polygons)
    {
      poly.texture_image = image;

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
produce_face_dotset(DotSet&  set) const
{
    for(auto&  poly: polygons)
    {
      poly.produce_dotset(set);
    }
}


void
Plane::
produce_wire_dotset(DotSet&  set) const
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
      ln.produce_dotset(set);
    }
}





