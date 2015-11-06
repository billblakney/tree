#include <iostream>
#include <string>
#include "StreamReader.hh"
#include "SimpleLineMatcher.hh"

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

#if 0 //TODO rm
  SimpleLineConsumer tReceivedMessageConsumer(
      std::cin,tLineBuffer,".*RECEIVED MESSAGE.*",true);

  SimpleLineConsumer tEndMessageConsumer(
      std::cin,tLineBuffer,".*=====END MESSAGE=====.*",false);
#endif

  SimpleLineMatcher tBeginMessageMatcher(".*===RECEIVED MESSAGE===.*");
  SimpleLineMatcher tEndMessageMatcher(".*===END MESSAGE===.*");

  vector<std::string> tStructLines;

  bool tLookingForStart = true;

  while (std::getline(std::cin,tLineBuffer))
  {
    if (tLookingForStart)
    {
      if (tBeginMessageMatcher.match(tLineBuffer) )
      {
        DEBUG(sLogger,"found start match");
        std::cout << "=== start ===" << std::endl;
        tLookingForStart = false;
      }
      else
      {
        DEBUG(sLogger,"didn't find start match");
      }
    }
    else
    {
      if (!tEndMessageMatcher.match(tLineBuffer) )
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
