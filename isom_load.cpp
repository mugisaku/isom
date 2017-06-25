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
           if(m.name == "x"){v.x = m.value.to_number();}
      else if(m.name == "y"){v.y = m.value.to_number();}
      else if(m.name == "z"){v.z = m.value.to_number();}
      else if(m.name == "r"){v.r = m.value.to_number();}
      else if(m.name == "g"){v.g = m.value.to_number();}
      else if(m.name == "b"){v.b = m.value.to_number();}
      else if(m.name == "u"){v.r = m.value.to_number();}
      else if(m.name == "v"){v.g = m.value.to_number();}
    }


  return v;
}


Polygon
read_polygon(const libjson::Object&  o, ObjectArray&  arr)
{
  using namespace libjson;

  Color  default_color;

  Vertex  vertexes[8];

  int  n = 0;

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){vertexes[0] = read_vertex(m.value->object);}
          else if(m.name == "b"){vertexes[1] = read_vertex(m.value->object);}
          else if(m.name == "c"){vertexes[2] = read_vertex(m.value->object);  n = 3;}
          else if(m.name == "d"){vertexes[3] = read_vertex(m.value->object);  n = 4;}
          else if(m.name == "e"){vertexes[4] = read_vertex(m.value->object);}
          else if(m.name == "f"){vertexes[5] = read_vertex(m.value->object);}
          else if(m.name == "g"){vertexes[6] = read_vertex(m.value->object);}
          else if(m.name == "h"){vertexes[7] = read_vertex(m.value->object);  n = 8;}
        }
    }


    if(n == 3)
    {
      arr.emplace_back(Polygon(0,vertexes[0],
                                 vertexes[1],
                                 vertexes[2]));
    }

  else
    if(n >= 4)
    {
      arr.emplace_back(Tetragon(0,vertexes[0],
                                  vertexes[1],
                                  vertexes[2],
                                  vertexes[3]));

        if(n >= 8)
        {
          arr.emplace_back(Tetragon(0,vertexes[4],
                                      vertexes[7],
                                      vertexes[6],
                                      vertexes[5]));

          arr.emplace_back(Tetragon(0,vertexes[0],
                                      vertexes[4],
                                      vertexes[5],
                                      vertexes[1]));

          arr.emplace_back(Tetragon(0,vertexes[3],
                                      vertexes[2],
                                      vertexes[6],
                                      vertexes[7]));

          arr.emplace_back(Tetragon(0,vertexes[0],
                                      vertexes[3],
                                      vertexes[7],
                                      vertexes[4]));

          arr.emplace_back(Tetragon(0,vertexes[1],
                                      vertexes[5],
                                      vertexes[6],
                                      vertexes[2]));
        }
    }
}


}


void
load_object(const std::string&  s, ObjectArray&  arr)
{
  using namespace libjson;

  ObjectArray  new_arr;

    try
    {
      FileBuffer  fbuf(std::string(s.data()));

      Value  v(fbuf);

        if(v == ValueKind::array)
        {
            for(auto&  e: v->array)
            {
                if(e == ValueKind::object)
                {
                  read_polygon(e->object,new_arr);
                }
            }
        }
    }


    catch(Stream&  s)
    {
      s.print();

      return;
    }


  arr = std::move(new_arr);
}




