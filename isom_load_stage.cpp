#include"isom_load.hpp"




namespace{


Point
scan_string_as_point(const std::string&  s, expree::Scope&  scope)
{
  char  buf[256];

  int  x;
  int  y;
  int  z;

  int  n;

  const char*  p = s.data();

  sscanf(p," %256[^,],%n ",buf,&n);

  p += n;

  x = load_integer(scope,std::string(buf));

  sscanf(p," %256[^,],%n ",buf,&n);

  p += n;

  y = load_integer(scope,std::string(buf));

  sscanf(p," %256[^,],%n ",buf,&n);

  p += n;

  z = load_integer(scope,std::string(buf));

  return Point(x,y,z);
}


Object
read_object(const Object&  objects, const libjson::Object&  o, expree::Scope&  parent)
{
  Object  container;

  using namespace libjson;

    for(auto&  m: o)
    {
      auto  result = objects.find_child_by_name(m.name);

        if(result && (m.value == ValueKind::object))
        {
          Point     offset;
          Point  increment;

          Transformer  tr;

          int  count = 1;

            for(auto&  mm: m.value->object)
            {
                if((mm.name == "offset") && (mm.value == ValueKind::string))
                {
                  offset = scan_string_as_point(mm.value->string,parent);

                  tr.set_translation_flag();
                }

              else
                if((mm.name == "angle") && (mm.value == ValueKind::string))
                {
                  auto  angle = scan_string_as_point(mm.value->string,parent);

                  tr.change_angle(angle.x,angle.y,angle.z);

                  tr.set_rotation_flag();
                }

              else
                if((mm.name == "increment") && (mm.value == ValueKind::string))
                {
                  increment = scan_string_as_point(mm.value->string,parent);

                  tr.set_translation_flag();
                }

              else
                if((mm.name == "count") && (mm.value == ValueKind::number))
                {
                  count = mm.value->number;
                }
            }


            while(count--)
            {
              auto  tmp = *result;

              tr.change_offset(offset);

              tmp.transform(tr);

              offset += increment;

              container.push(std::move(tmp));
            }
        }
    }


  return std::move(container);
}


Object
read_stage(const Object&  objects, const libjson::ObjectMember&  m, expree::Scope&  parent)
{
  expree::Scope  scope(parent);

  Object  o;

  o.change_name(m.name.data());

  using namespace libjson;

    if(m.value == ValueKind::object)
    {
        for(auto&  mm: m.value->object)
        {
            if(mm.name == "expressions" && (mm.value == ValueKind::array))
            {
              read_expressions(mm.value->array,scope);
            }

          else
            if((mm.name == "objects") && (mm.value == ValueKind::object))
            {
                for(auto&  mmm: mm.value->object)
                {
                  o.push(read_object(objects,mmm.value->object,scope));
                }
            }
        }
    }


  return std::move(o);
}


}


Object
load_stage(const Object&  objects, const std::string&  s)
{
  expree::MemorySpace  memsp;

  auto&  scope = memsp.get_global_scope();

  Object  container;

  using namespace libjson;

  FileBuffer  fbuf(std::string(s.data()));

  Value  v(fbuf);

    if(v == ValueKind::object)
    {
        for(auto&  m: v->object)
        {
            if(m.name == "expressions" && (m.value == ValueKind::array))
            {
              read_expressions(m.value->array,scope);
            }

          else
            if((m.name == "stages") && (m.value == ValueKind::object))
            {
                for(auto&  mm: m.value->object)
                {
                    if(mm.value == ValueKind::object)
                    {
                      container.push(read_stage(objects,mm,scope));
                    }

                  else
                    if(mm.name == "expressions" && (mm.value == ValueKind::array))
                    {
                      read_expressions(mm.value->array,scope);
                    }
                }
            }
        }
    }


  return std::move(container);
}




