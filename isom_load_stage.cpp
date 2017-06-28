#include"isom_load.hpp"
#include"isom_scope.hpp"




namespace{


Object
read_object(const Object&  objects, const libjson::Object&  o, const Scope&  parent)
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
                  sscanf(mm.value->string.data()," %d , %d , %d",&offset.x,&offset.y,&offset.z);

                  tr.set_translation_flag();
                }

              else
                if((mm.name == "increment") && (mm.value == ValueKind::string))
                {
                  sscanf(mm.value->string.data()," %d , %d , %d",&increment.x,&increment.y,&increment.z);

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
read_stage(const Object&  objects, const libjson::ObjectMember&  m, const Scope&  parent)
{
  Scope  scope(&parent);

  Object  o;

  o.change_name(m.name.data());

  using namespace libjson;

    if(m.value == ValueKind::object)
    {
        for(auto&  mm: m.value->object)
        {
            if(mm.name == "constants")
            {
              scope += mm.value;
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
  Scope  scope;

  Object  container;

  using namespace libjson;

  FileBuffer  fbuf(std::string(s.data()));

  Value  v(fbuf);

    if(v == ValueKind::object)
    {
        for(auto&  m: v->object)
        {
            if(m.name == "constants")
            {
              scope += m.value;
            }

          else
            if((m.name == "stages") && (m.value == ValueKind::object))
            {
                for(auto&  mm: m.value->object)
                {
                  container.push(read_stage(objects,mm,scope));
                }
            }
        }
    }


  return std::move(container);
}




