#ifndef expree_object_HPP_INCLUDED
#define expree_object_HPP_INCLUDED


#include"expree_primitive.hpp"
#include"expree_operator.hpp"
#include<string>


namespace expree{


struct MemorySpace;
struct Scope;


enum class
ObjectKind
{
  null,
  undefined,
  void_,
  integer,
  boolean,
  pointer,
  reference,

};


struct
Reference
{
  MemorySpace&  space;

  Pointer  pointer;

  constexpr Reference(MemorySpace&  sp, Pointer  ptr): space(sp), pointer(ptr){}

  Pointer  get_pointer() const{return pointer;}

  Object&  access() const{return pointer.dereference(space);}

};


union
ObjectData
{
  Integer  integer;
  Boolean  boolean;
  Pointer  pointer;
  Reference  reference;

   ObjectData(){}
  ~ObjectData(){}

};


class
Object
{
  ObjectKind  kind;
  ObjectData  data;

public:
  Object();
  Object(const Null&  n);
  Object(const Void&  v);
  Object(const Pointer&  p);
  Object(const Integer&  i);
  Object(const Boolean&  b);
  Object(const Reference&  r);
  Object(const Object&   rhs) noexcept;
  Object(      Object&&  rhs) noexcept;
 ~Object();


  Object&  operator=(const Object&   rhs) noexcept;
  Object&  operator=(      Object&&  rhs) noexcept;

  const ObjectData*  operator->() const{return &data;}

  bool  operator==(ObjectKind  k) const{return(kind == k);}
  bool  operator!=(ObjectKind  k) const{return(kind != k);}

  ObjectKind  get_kind() const{return kind;}

  bool  is_integer() const{return(kind == ObjectKind::integer);}
  bool  is_pointer() const{return(kind == ObjectKind::pointer);}
  bool  is_boolean() const{return(kind == ObjectKind::boolean);}
  bool  is_reference() const{return(kind == ObjectKind::reference);}

  Integer  to_integer(bool  strong=false) const;
  Pointer  to_pointer(bool  strong=false) const;
  Boolean  to_boolean(bool  strong=false) const;

  static Object  ari_add(const Object&  lhs, const Object&  rhs);
  static Object  ari_sub(const Object&  lhs, const Object&  rhs);
  static Object  ari_mul(const Object&  lhs, const Object&  rhs);
  static Object  ari_div(const Object&  lhs, const Object&  rhs);
  static Object  ari_rem(const Object&  lhs, const Object&  rhs);
  static Object  bit_shl(const Object&  lhs, const Object&  rhs);
  static Object  bit_shr(const Object&  lhs, const Object&  rhs);
  static Object  bit_and(const Object&  lhs, const Object&  rhs);
  static Object  bit_or(const Object&  lhs, const Object&  rhs);
  static Object  bit_xor(const Object&  lhs, const Object&  rhs);
  static Object  bit_not(const Object&  o);
  static Object  cmp_eq(const Object&  lhs, const Object&  rhs);
  static Object  cmp_neq(const Object&  lhs, const Object&  rhs);
  static Object  cmp_lt(const Object&  lhs, const Object&  rhs);
  static Object  cmp_lteq(const Object&  lhs, const Object&  rhs);
  static Object  cmp_gt(const Object&  lhs, const Object&  rhs);
  static Object  cmp_gteq(const Object&  lhs, const Object&  rhs);
  static Object  log_not(const Object&  o);
  static Object  log_and(const Object&  lhs, const Object&  rhs);
  static Object  log_or(const Object&  lhs, const Object&  rhs);
  static Object  neg(const Object&  o);

  static Object  ld(Scope&  scope, const Object&  o);
  static Object  st(const Object&  lhs, const Object&  rhs);

  static Object  get_address(Scope&  scope, const Object&  o);

  static Object  remove_reference(const Object&  o);

  void  clear();

  void  print() const;

};


}


#endif




