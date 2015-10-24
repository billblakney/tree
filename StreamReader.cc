#include <iostream>
#include <string>
#include "StreamReader.hh"

StreamReader::StreamReader()
{
}

StreamReader::~StreamReader()
{
}

void StreamReader::run()
{
  std::string tInputLine;

  while(true)
  {
    std::getline(std::cin,tInputLine);
    std::cout << tInputLine << std::endl;
    sleep(1);
  }
}
