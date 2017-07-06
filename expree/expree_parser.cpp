#include"expree_parser.hpp"
#include<cctype>
#include<cstring>




namespace expree{


Parser::
Parser(const std::string&  s)
{
  reset(s);

  start();
}


Parser::
Parser(Parser&  parent, const char*  opening, const char*  closing):
row_count(parent.row_count),
head(parent.head),
current(parent.current),
end(parent.end)
{
  start(closing);

  parent.row_count = row_count;
  parent.head      = head;
  parent.current   = current+1;

  parent.buffer.emplace_back(make_element(opening,closing));
}




bool
Parser::
test_end() const
{
  return(current >= end);
}


void
Parser::
skip_spaces()
{
    while(!test_end())
    {
      auto  c = get_char();

        if(!isspace(c))
        {
          break;
        }


      advance();
    }
}


void
Parser::
newline()
{
  head = current;

  ++row_count;
}


void
Parser::
advance(int  n)
{
    while(n--)
    {
      auto  c = get_char();

        if(test_end())
        {
          break;
        }


      ++current;

        if(c == '\n')
        {
          newline();
        }
    }
}


bool
Parser::
read_number()
{
  auto  c = get_char();

    if(isdigit(c))
    {
      unsigned int  u;

      int  n;

        if(sscanf(current,"%u%n",&u,&n) >= 1)
        {
          current += n;

          buffer.emplace_back(Operand(u));

          return true;
        }

      else
        {
          return false;
        }
    }


  return false;
}


bool
Parser::
read_identifier()
{
  auto  c = get_char();

    if(isalpha(c) || (c == '_'))
    {
      Identifier  id;

      id.string.push_back(c);

      ++current;

        while(!test_end())
        {
          auto  c = get_char();

            if(!isalnum(c) && (c != '_'))
            {
              break;
            }


          id.string.push_back(c);

          ++current;
        }


      buffer.emplace_back(Operand(std::move(id)));

      return true;
    }


  return false;
}


bool
Parser::
read_operator(Operator  o)
{
  const char*  str = o.codes;

  auto  len = std::strlen(str);

    if(std::strncmp(current,str,len) == 0)
    {
      current += len;

      buffer.emplace_back(o);

      return true;
    }


  return false;
}


bool
Parser::
read_block(const char*  opening, const char*  closing)
{
  auto  len = strlen(opening);

    if(std::strncmp(current,opening,len) == 0)
    {
      current += len;

      Parser  child(*this,opening,closing);

      return true;
    }


  return false;
}




void
Parser::
reset(const std::string&  s)
{
  row_count = 0;
  head      = s.data();
  current   = s.data();
  end       = (s.data()+s.size());

  buffer.clear();
}


void
Parser::
start(const char*  closing)
{
  auto  len = closing? std::strlen(closing):0;

    for(;;)
    {
      skip_spaces();

        if(closing)
        {
            if(std::strncmp(current,closing,len) == 0)
            {
              break;
            }


            if(test_end())
            {
              Formatted  f;

              throw Exception(f("%sで閉じられていません",closing));
            }
        }

      else
        if(test_end())
        {
          break;
        }

           if(read_identifier()){}
      else if(read_number()){}
      else if(read_operator(Operator('<','<','='))){}
      else if(read_operator(Operator('<','<'))){}
      else if(read_operator(Operator('<','='))){}
      else if(read_operator(Operator('<'))){}
      else if(read_operator(Operator('>','>','='))){}
      else if(read_operator(Operator('>','>'))){}
      else if(read_operator(Operator('>','='))){}
      else if(read_operator(Operator('>'))){}
      else if(read_operator(Operator('=','='))){}
      else if(read_operator(Operator('='))){}
      else if(read_operator(Operator('!','='))){}
      else if(read_operator(Operator('!'))){}
      else if(read_operator(Operator('|','|'))){}
      else if(read_operator(Operator('|','='))){}
      else if(read_operator(Operator('|'))){}
      else if(read_operator(Operator('&','='))){}
      else if(read_operator(Operator('&','&'))){}
      else if(read_operator(Operator('&'))){}
      else if(read_operator(Operator('^','='))){}
      else if(read_operator(Operator('^'))){}
      else if(read_operator(Operator('+','+'))){}
      else if(read_operator(Operator('+','='))){}
      else if(read_operator(Operator('+'))){}
      else if(read_operator(Operator('-','-'))){}
      else if(read_operator(Operator('-','>','*'))){}
      else if(read_operator(Operator('-','>'))){}
      else if(read_operator(Operator('-','='))){}
      else if(read_operator(Operator('-'))){}
      else if(read_operator(Operator('*','='))){}
      else if(read_operator(Operator('*'))){}
      else if(read_operator(Operator('/','='))){}
      else if(read_operator(Operator('/'))){}
      else if(read_operator(Operator('%','='))){}
      else if(read_operator(Operator('%'))){}
      else if(read_operator(Operator('.','*'))){}
      else if(read_operator(Operator('.'))){}
      else if(read_operator(Operator('~'))){}
      else if(read_operator(Operator(','))){}
      else if(read_block("(",")")){}
      else if(read_block("[","]")){}
      else if(read_block("<",">")){}
      else
        {
          printf("不明な要素: %c\n",get_char());

          break;
        }
    }
}


}


