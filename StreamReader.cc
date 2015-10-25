#include <iostream>
#include <string>
#include "StreamReader.hh"
#include "SimpleLineConsumer.hh"

StreamReader::StreamReader(DataStructModel *aModel)
  : _DataStructModel(aModel)
{
}

StreamReader::~StreamReader()
{
}

void StreamReader::run()
{
  std::string tInputLine;

//  while(true)
//  {
//    std::getline(std::cin,tInputLine);
//    std::cout << tInputLine << std::endl;
//    sleep(1);
//  }

  std::string tLineBuffer;

  SimpleLineConsumer tReceivedMessageConsumer(
      std::cin,tLineBuffer,"start",true);

  SimpleLineConsumer tEndMessageConsumer(
      std::cin,tLineBuffer,"end",false);

  vector<std::string> tStructLines;

  bool tLookingForStart = true;

  while (std::getline(std::cin,tLineBuffer))
  {
    if (tLookingForStart)
    {
      if (tLineBuffer.compare("start"))
      {
        std::cout << "not starting" << std::endl;
      }
      else
      {
        std::cout << "ready to start" << std::endl;
        tLookingForStart = false;
      }
    }
    else
    {
      if (tLineBuffer.compare("end"))
      {
        std::cout << "pushing back struct line" << std::endl;
        tStructLines.push_back(tLineBuffer);
      }
      else
      {
        std::cout << "reached end of structure" << std::endl;
        _DataStructModel->processLinesIn(tStructLines);
        tStructLines.clear();
        tLookingForStart = true;
      }
    }
  }
}
