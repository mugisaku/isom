#ifndef expree_exception_HPP_INCLUDED
#define expree_exception_HPP_INCLUDED


#include<cstdarg>
#include<string>
#include<exception>


#ifndef report
#define report  printf("[report in %s] %d %s\n",__FILE__,__LINE__,__func__);
#endif


namespace expree{




class
Formatted
{
  char  buf[256];

public:
  const char*  operator()(const char*  fmt, ...)
  {
    va_list  ap;

    va_start(ap,fmt);

    vsnprintf(buf,sizeof(buf),fmt,ap);

    va_end(ap);

    return buf;
  }

};


class
Exception: public std::exception
{
  std::string  buf;

public:
  Exception(const char*  s)
  {
    buf += s;
  }

  void  push(const char*  s)
  {
    buf += s;
    buf += '\n';
  }

  const char*  what() const noexcept override{return buf.data();}

};


}


#endif




