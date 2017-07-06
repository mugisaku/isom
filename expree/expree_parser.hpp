#ifndef expree_Parser_HPP_INCLUDED
#define expree_Parser_HPP_INCLUDED


#include<string>
#include<utility>
#include<vector>
#include"expree_element.hpp"


namespace expree{


class
Parser
{
  int  row_count;

  const char*     head;
  const char*  current;
  const char*      end;

  std::vector<Element>  buffer;

  void  skip_spaces();
  void  advance(int  n=1);
  void  newline();

  int  get_char() const{return *current;}

  bool  read_identifier();
  bool  read_number();
  bool    read_operator(Operator  o);
  bool    read_block(const char*  opening, const char*  closing);

  bool  test_end() const;

public:
  Parser(const std::string&  s);
  Parser(Parser&  parent, const char*  opening, const char*  closing);

  void  reset(const std::string&  s);

  void  start(const char*  closing=nullptr);

  Element  make_element(const char*  opening=nullptr, const char*  closing=nullptr) const;

};


}


#endif




