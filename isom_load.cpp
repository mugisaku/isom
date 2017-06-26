#include"isom_load.hpp"
#include"libjson/json_stream.hpp"
#include"libjson/json_FileBuffer.hpp"




namespace{


Vertex
read_vertex(const libjson::Object&  o)
{
  using namespace libjson;

  Vertex  v;

    for(auto&  m: o)
    {
      int*  ptr = nullptr;

      int  n = m.value.to_number();

           if(m.name == "x"){v.x = n;}
      else if(m.name == "y"){v.y = n;}
      else if(m.name == "z"){v.z = n;}
      else if(m.name == "r"){v.r = n;}
      else if(m.name == "g"){v.g = n;}
      else if(m.name == "b"){v.b = n;}
      else if(m.name == "u"){v.r = n;}
      else if(m.name == "v"){v.g = n;}
    }


  return v;
}


void
read_line(const libjson::Object&  o, Object&  obj)
{
  using namespace libjson;

  Vertex  vertexes[2];

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){vertexes[0] = read_vertex(m.value->object);}
          else if(m.name == "b"){vertexes[1] = read_vertex(m.value->object);}
        }
    }


  auto&  a = vertexes[0];
  auto&  b = vertexes[1];

  obj.push(Line(Dot(a,Color(a.r,a.g,a.b,255)),
                Dot(b,Color(b.r,b.g,b.b,255))));
}


void
read_polygon(const libjson::Object&  o, Object&  obj)
{
  using namespace libjson;

  Vertex  vertexes[3];

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){vertexes[0] = read_vertex(m.value->object);}
          else if(m.name == "b"){vertexes[1] = read_vertex(m.value->object);}
          else if(m.name == "c"){vertexes[2] = read_vertex(m.value->object);}
        }
    }


  obj.push(Polygon(0,vertexes[0],
                     vertexes[1],
                     vertexes[2]));
}


void
read_tetragon(const libjson::Object&  o, Object&  obj)
{
  using namespace libjson;

  Vertex  vertexes[4];

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){vertexes[0] = read_vertex(m.value->object);}
          else if(m.name == "b"){vertexes[1] = read_vertex(m.value->object);}
          else if(m.name == "c"){vertexes[2] = read_vertex(m.value->object);}
          else if(m.name == "d"){vertexes[3] = read_vertex(m.value->object);}
        }
    }


  obj.push(Tetragon(0,vertexes[0],
                      vertexes[1],
                      vertexes[2],
                      vertexes[3]));
}


}


void
load_object(const std::string&  s, Object&  obj)
{
  Object  new_obj;

  using namespace libjson;

    try
    {
      FileBuffer  fbuf(std::string(s.data()));

      Value  v(fbuf);

        if(v == ValueKind::array)
        {
            for(auto&  e: v->array)
            {
                if(e == ValueKind::array)
                {
                  auto&  a = e->array;

                    if((a.size() >= 2) &&
                       (a[0] == ValueKind::string) &&
                       (a[1] == ValueKind::object))
                    {
                      auto&  type = a[0]->string;
                      auto&  data = a[1]->object;

                           if(type == "line"    ){read_line(    data,new_obj);}
                      else if(type == "polygon" ){read_polygon( data,new_obj);}
                      else if(type == "tetragon"){read_tetragon(data,new_obj);}
                    }
                }
            }
        }
    }


    catch(Stream&  s)
    {
      s.print();

      return;
    }


  obj = std::move(new_obj);
}




