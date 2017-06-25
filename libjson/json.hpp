#ifndef JSON_HPP_INCLUDED
#define JSON_HPP_INCLUDED


#include<string>
#include<vector>
#include"json_FileBuffer.hpp"




namespace libjson{




enum class
ValueKind
{
  undefined,
  null,
  false_,
  true_,
  number,
  string,
  object,
  array,

};


struct Value;
struct ObjectMember;


using Object = std::vector<ObjectMember>;
using Array  = std::vector<Value>;


union
ValueData
{
  double       number;
  std::string  string;
  Object       object;
  Array         array;

   ValueData(){}
  ~ValueData(){}

};


struct Undefined{};
struct Null{};


class
Value
{
  ValueKind  kind;
  ValueData  data;

public:
   Value();
   Value(FilePath&&  fpath);
   Value(const FileBuffer&  fbuf);
   Value(Null  null);
   Value(bool    b);
   Value(int     i);
   Value(double  f);
   Value(std::string&&  s);
   Value(Object&&  o);
   Value(Array&&  a);
   Value(const Value&   rhs) noexcept;
   Value(      Value&&  rhs) noexcept;
  ~Value();

  Value&  operator=(const Value&   rhs) noexcept;
  Value&  operator=(      Value&&  rhs) noexcept;

  const ValueData&  operator *() const;
  const ValueData*  operator->() const;

  bool  operator==(ValueKind  k) const;
  bool  operator!=(ValueKind  k) const;

  void  clear();

  ValueKind  get_kind() const;

  std::string  to_string(unsigned int  indent_n=0) const;
  double       to_number() const;
  bool         to_boolean() const;

};


struct
ObjectMember
{
  std::string   name;

  Value  value;

std::string  to_string(unsigned int  indent_n=0) const
{
  std::string  s;

  s += "\"";

  s += name;

  s += "\":";

  s += value.to_string(indent_n);

  return std::move(s);
}


};


}




#endif




