#include"expree_object.hpp"
#include"expree_scope.hpp"
#include"expree_MemorySpace.hpp"




namespace expree{


Integer
Object::
to_integer(bool  strong)  const
{
  auto  o = remove_reference(*this);

    if(strong)
    {
           if(o.is_integer()){return o->integer;}
      else if(o.is_pointer()){return expree::to_integer(o->pointer);}
    }

  else
    {
      if(o.is_integer()){return o->integer;}
    }


  throw Exception("整数に変換できません");
}


Pointer
Object::
to_pointer(bool  strong)  const
{
    if(is_reference())
    {
      return data.reference.access().to_pointer(strong);;
    }


    if(strong)
    {
           if(is_integer()){return expree::to_pointer(data.integer);}
      else if(is_pointer()){return data.pointer;}
    }

  else
    {
        if(is_pointer()){return data.pointer;}
    }


  throw Exception("ポインタに変換できません");
}


Boolean
Object::
to_boolean(bool  strong)  const
{
    if(is_reference())
    {
      return data.reference.access().to_boolean(strong);
    }


       if(is_integer()){return expree::to_boolean(data.integer);}
  else if(is_pointer()){return expree::to_boolean(data.pointer);}
  else if(is_boolean()){return data.boolean;}


  throw Exception("真偽値に変換できません");
}


Object
Object::
ari_add(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()+rhs.to_integer());
}


Object
Object::
ari_sub(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()-rhs.to_integer());
}


Object
Object::
ari_mul(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()*rhs.to_integer());
}


Object
Object::
ari_div(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()/rhs.to_integer());
}


Object
Object::
ari_rem(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()%rhs.to_integer());
}


Object
Object::
bit_shl(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()<<rhs.to_integer());
}


Object
Object::
bit_shr(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()>>rhs.to_integer());
}


Object
Object::
bit_and(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()&rhs.to_integer());
}


Object
Object::
bit_or(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()|rhs.to_integer());
}


Object
Object::
bit_xor(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer()^rhs.to_integer());
}


Object
Object::
bit_not(const Object&  o)
{
  return Object(~o.to_integer());
}


Object
Object::
cmp_eq(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer() == rhs.to_integer());
}


Object
Object::
cmp_neq(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer() != rhs.to_integer());
}


Object
Object::
cmp_lt(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer() < rhs.to_integer());
}


Object
Object::
cmp_lteq(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer() <= rhs.to_integer());
  return Object();
}


Object
Object::
cmp_gt(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer() > rhs.to_integer());
}


Object
Object::
cmp_gteq(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_integer() >= rhs.to_integer());
}


Object
Object::
log_not(const Object&  o)
{
  return Object(!o.to_boolean());
}


Object
Object::
log_and(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_boolean() && rhs.to_boolean());
}


Object
Object::
log_or(const Object&  lhs, const Object&  rhs)
{
  return Object(lhs.to_boolean() || rhs.to_boolean());
}


Object
Object::
neg(const Object&  o)
{
  return Object(-o.to_integer());
}


Object
Object::
ld(Scope&  scope, const Object&  o)
{
    if(o.is_pointer())
    {
      return Object(Reference(scope.space,o->pointer));
    }


  throw Exception("ポインタでないオブジェクトを、デリファレンスしようとしました");
}


Object
Object::
st(const Object&  lhs, const Object&  rhs)
{
    if(lhs.is_reference())
    {
      auto&  dst = lhs->reference.access();

      dst = remove_reference(rhs);

      return dst;
    }


  throw Exception("参照でないオブジェクトに、代入しようとしました");
}


Object
Object::
get_address(Scope&  scope, const Object&  o)
{
    if(o.is_reference())
    {
      return Object(o->reference.get_pointer());
    }


  throw Exception("参照でないオブジェクトのアドレスを取得しようとしました");
}


Object
Object::
remove_reference(const Object&  o)
{
    if(o.is_reference())
    {
      return o->reference.access();
    }


  return o;
}


}


