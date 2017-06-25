#include"json_FileBuffer.hpp"
#include<cstdio>




FileBuffer::
FileBuffer(FilePath&&  path_):
path(std::move(path_))
{
  auto  f = fopen(path->data(),"rb");

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          content.push_back(c);
        }


      fclose(f);
    }
}


FileBuffer::
FileBuffer(std::string&&  content_):
content(std::move(content_))
{
}




