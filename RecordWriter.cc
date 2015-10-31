#include <iostream>
#include <iterator>
#include "RecordWriter.hh"

RecordWriter::RecordWriter()
  : _LineMatcher(0)
{
}

RecordWriter::~RecordWriter()
{
}

void RecordWriter::process(std::vector<std::string> &aRecordStrings)
{
  std::vector<std::string>::iterator tIt;
  for (tIt = aRecordStrings.begin(); tIt != aRecordStrings.end(); tIt++)
  {
    if (_LineMatcher && _LineMatcher->match(*tIt))
    {
      std::cout << *tIt << std::endl;
    }
    else
    {
      std::cout << *tIt << std::endl;
    }
  }
}

void RecordWriter::setLineMatcher(LineMatcher *aMatcher)
{
  _LineMatcher = aMatcher;
}
