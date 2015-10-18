#include <iostream>
#include "StreamReader.hh"

StreamReader::StreamReader()
{
}

StreamReader::~StreamReader()
{
}

void StreamReader::run()
{
  while(true)
  {
    std::cout << "still alive" << std::endl;
    sleep(1);
  }
}
