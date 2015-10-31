#include <iostream>
#include <string>
#include "StreamReader.hh"
#include "SimpleLineConsumer.hh"

ccl::Logger StreamReader::sLogger("StreamReader");

StreamReader::StreamReader(DataStructModel *aModel)
  : _Writers(0),
    _DataStructModel(aModel)
{
  _Writers.push_back(new RecordWriter());
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
        DEBUG(sLogger,"didn't find start match");
      }
      else
      {
        DEBUG(sLogger,"found start match");
        std::cout << "=== start ===" << std::endl;
        tLookingForStart = false;
      }
    }
    else
    {
      if (tLineBuffer.compare("end"))
      {
        DEBUG(sLogger,"pushing back struct line");
        tStructLines.push_back(tLineBuffer);
      }
      else
      {
        DEBUG(sLogger,"reached end of structure");
#define USE_OLD
#ifdef USE_OLD
        if (_DataStructModel->processLinesIn(tStructLines))
        {
          _DataStructModel->printInLines();
        }
#else
        std::vector<RecordWriter *>::iterator tIt;
        for (tIt = _Writers.begin(); tIt != _Writers.end(); tIt++)
        {
          (*tIt)->process(tStructLines);
        }
#endif
        tStructLines.clear();
        std::cout << "=== end ===" << std::endl;
        tLookingForStart = true;
      }
    }
  }
}
