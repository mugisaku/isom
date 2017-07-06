#ifndef expree_primitive_HPP_INCLUDED
#define expree_primitive_HPP_INCLUDED


#include<cstdio>
#include<cstdarg>
#include<string>
#include<vector>
#include"expree_exception.hpp"


namespace expree{


struct Object;
struct MemorySpace;




struct
Undefined
{
  constexpr Undefined(){}
};


struct
Null
{
  constexpr Null(){}
};


struct
Void
{
  constexpr Void(){}

};


struct
Integer
{
  int  data;

  constexpr Integer(int  d=0): data(d){}


  constexpr Integer  operator +(const Integer&  rhs) const{return Integer(data+rhs.data);}
  constexpr Integer  operator -(const Integer&  rhs) const{return Integer(data-rhs.data);}
  constexpr Integer  operator *(const Integer&  rhs) const{return Integer(data*rhs.data);}
  constexpr Integer  operator /(const Integer&  rhs) const{return Integer(data/rhs.data);}
  constexpr Integer  operator %(const Integer&  rhs) const{return Integer(data%rhs.data);}
  constexpr Integer  operator<<(const Integer&  rhs) const{return Integer(data<<rhs.data);}
  constexpr Integer  operator>>(const Integer&  rhs) const{return Integer(data>>rhs.data);}

  constexpr Integer  operator |(const Integer&  rhs) const{return Integer(data|rhs.data);}
  constexpr Integer  operator &(const Integer&  rhs) const{return Integer(data&rhs.data);}
  constexpr Integer  operator ^(const Integer&  rhs) const{return Integer(data^rhs.data);}

  constexpr Integer  operator~() const{return Integer(~data);}
  constexpr Integer  operator-() const{return Integer(-data);}

  void  print() const{printf("%d",data);}

};


struct
NullPointer
{
  constexpr NullPointer(){}

};


struct
Pointer
{
  int  data;

  constexpr Pointer(int  d=0): data(d){}


  constexpr Pointer  operator+(const Pointer&  rhs) const{return Pointer(data+rhs.data);}
  constexpr Pointer  operator-(const Pointer&  rhs) const{return Pointer(data-rhs.data);}
  constexpr operator bool() const{return data;}

  Object&  dereference(MemorySpace&  space) const;

  void  print() const{printf("%d",data);}

};


struct
Boolean
{
  bool  data;

  constexpr Boolean(bool  b=false): data(b){}

  constexpr Boolean  operator&&(const Boolean&  rhs) const{return Boolean(data && rhs.data);}
  constexpr Boolean  operator||(const Boolean&  rhs) const{return Boolean(data || rhs.data);}
  constexpr Boolean  operator!() const{return Boolean(!data);}

  void  print() const{printf("%s",data? "true":"false");}

};


constexpr Boolean  operator==(const Pointer&  lhs, const Pointer&  rhs){return Boolean(lhs.data == rhs.data);}
constexpr Boolean  operator!=(const Pointer&  lhs, const Pointer&  rhs){return Boolean(lhs.data != rhs.data);}

constexpr Boolean  operator==(const Integer&  lhs, const Integer&  rhs){return Boolean(lhs.data == rhs.data);}
constexpr Boolean  operator!=(const Integer&  lhs, const Integer&  rhs){return Boolean(lhs.data != rhs.data);}
constexpr Boolean  operator< (const Integer&  lhs, const Integer&  rhs){return Boolean(lhs.data <  rhs.data);}
constexpr Boolean  operator<=(const Integer&  lhs, const Integer&  rhs){return Boolean(lhs.data <= rhs.data);}
constexpr Boolean  operator> (const Integer&  lhs, const Integer&  rhs){return Boolean(lhs.data >  rhs.data);}
constexpr Boolean  operator>=(const Integer&  lhs, const Integer&  rhs){return Boolean(lhs.data >= rhs.data);}

constexpr Boolean  to_boolean(const Integer&  v){return Boolean(v.data);}
constexpr Boolean  to_boolean(const Pointer&  v){return Boolean(v.data);}
constexpr Boolean  to_boolean(const NullPointer&  v){return Boolean();}

constexpr Integer  to_integer(const Pointer&  v){return Integer(v.data);}
constexpr Pointer  to_pointer(const Integer&  v){return Pointer(v.data);}
constexpr Pointer  to_pointer(const NullPointer&  v){return Pointer();}


}


#endif




