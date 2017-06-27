#include"isom_load.hpp"
#include"libjson/json_stream.hpp"
#include"libjson/json_FileBuffer.hpp"




namespace{


struct
Constant
{
  std::string  name;

  int  value;

  Constant(const std::string&  name_, int  v):
  name(name_), value(v){}

};


struct
Scope
{
  const Scope*  const parent=nullptr;

  std::vector<Constant>  table;

  Scope(const Scope*  parent_=nullptr): parent(parent_){}

  void  operator+=(const libjson::Value&  v)
  {
    using namespace libjson;

      if(v == ValueKind::object)
      {
          for(auto&  m: v->object)
          {
              if(m.value == ValueKind::number)
              {
                table.emplace_back(m.name,m.value->number);
              }
          }
      }
  }

  int  operator[](const std::string&  name) const
  {
      for(auto&  c: table)
      {
          if(c.name == name)
          {
            return c.value;
          }
      }


    return parent? (*parent)[name]:0;
  }

};


Vertex
read_vertex(const libjson::Object&  o, const Scope&  parent)
{
  using namespace libjson;

  Vertex  v;

    for(auto&  m: o)
    {
      int  n = (m.value == ValueKind::number)? m.value->number:
               (m.value == ValueKind::string)? parent[m.value->string]:0;

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


Line
read_line(const libjson::Object&  o, const Scope&  parent)
{
  using namespace libjson;

  Vertex  vertexes[2];

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){vertexes[0] = read_vertex(m.value->object,parent);}
          else if(m.name == "b"){vertexes[1] = read_vertex(m.value->object,parent);}
        }
    }


  auto&  a = vertexes[0];
  auto&  b = vertexes[1];

  return Line(Dot(a,Color(a.r,a.g,a.b,255)),
              Dot(b,Color(b.r,b.g,b.b,255)));
}


Polygon
read_polygon(const libjson::Object&  o, const Scope&  parent)
{
  using namespace libjson;

  Vertex  vertexes[3];

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){vertexes[0] = read_vertex(m.value->object,parent);}
          else if(m.name == "b"){vertexes[1] = read_vertex(m.value->object,parent);}
          else if(m.name == "c"){vertexes[2] = read_vertex(m.value->object,parent);}
        }
    }


  return Polygon(0,vertexes[0],
                   vertexes[1],
                   vertexes[2]);
}


Tetragon
read_tetragon(const libjson::Object&  o, const Scope&  parent)
{
  using namespace libjson;

  Vertex  vertexes[4];

    for(auto&  m: o)
    {
        if(m.value == ValueKind::object)
        {
               if(m.name == "a"){vertexes[0] = read_vertex(m.value->object,parent);}
          else if(m.name == "b"){vertexes[1] = read_vertex(m.value->object,parent);}
          else if(m.name == "c"){vertexes[2] = read_vertex(m.value->object,parent);}
          else if(m.name == "d"){vertexes[3] = read_vertex(m.value->object,parent);}
        }
    }


  return Tetragon(0,vertexes[0],
                    vertexes[1],
                    vertexes[2],
                    vertexes[3]);
}


Object
read_root(const libjson::Array&  arr, const Scope&  parent)
{
  Scope  scope(&parent);

  Object  root;

  using namespace libjson;

    for(auto&  e: arr)
    {
      scope += e;

        if(e == ValueKind::array)
        {
          auto&  a = e->array;

            if((a.size() >= 2) &&
               (a[0] == ValueKind::string) &&
               (a[1] == ValueKind::object))
            {
              auto&  type = a[0]->string;
              auto&  data = a[1]->object;

                   if(type == "line"    ){root.push(read_line(    data,scope));}
              else if(type == "polygon" ){root.push(read_polygon( data,scope));}
              else if(type == "tetragon"){root.push(read_tetragon(data,scope));}
            }
        }
    }


  return std::move(root);
}


}


Object
load_object(const std::string&  s)
{
  Scope  scope;

  Object  container;

  using namespace libjson;

  FileBuffer  fbuf(std::string(s.data()));

  Value  v(fbuf);

    if(v == ValueKind::array)
    {
        for(auto&  e: v->array)
        {
          scope += e;

            if(e == ValueKind::array)
            {
              auto&  a = e->array;

                if((a.size() >= 2) &&
                   (a[0] == ValueKind::string) &&
                   (a[1] == ValueKind::array ))
                {
                  auto&  name = a[0]->string;
                  auto&  data = a[1]->array ;

                  auto  o = read_root(data,scope);

                  o.change_name(name.data());

                  container.push(std::move(o));
                }
            }
        }
    }


  return std::move(container);
}




