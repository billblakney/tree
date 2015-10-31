#include <iostream>
#include <string>
#include "StreamReader.hh"
#include "SimpleLineConsumer.hh"

ccl::Logger StreamReader::sLogger("StreamReader");

// TODO obsolete
StreamReader::StreamReader(DataStructModel *aModel)
  : _DataStructModel(aModel)
{
}

StreamReader::StreamReader(RecordWriter *aWriter)
{
  _Mutex.lock();
  setRecordWriter(aWriter);
  _Mutex.unlock();
}

StreamReader::~StreamReader()
{
}

// TODO mem leak
void StreamReader::setRecordWriter(RecordWriter *aWriter)
{
  if (_Writers.size() == 0)
  {
    _Writers.push_back(aWriter);
  }
  else
  {
    _Writers[0] = aWriter;
  }
}

void StreamReader::run()
{
  std::string tInputLine;

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
//#define USE_OLD
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
