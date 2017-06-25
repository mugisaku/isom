#ifndef JSON_FileBuffer_HPP_INCLUDED
#define JSON_FileBuffer_HPP_INCLUDED


#include<string>




class
FilePath
{
  std::string  string;

public:
  FilePath(const char*  s=""): string(s){}

  const std::string&  operator*()  const{return  string;}
  const std::string*  operator->() const{return &string;}

};


class
FileBuffer
{
  FilePath  path;

  std::string  content;

public:
  explicit FileBuffer(FilePath&&  path_);
  explicit FileBuffer(std::string&&  content_);

  const std::string&     get_path() const{return *path;}
  const std::string&  get_content() const{return content;}

};




#endif




