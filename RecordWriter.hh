#ifndef RECORDWRITER_HH_
#define RECORDWRITER_HH_

#include <string>
#include <vector>
#include <LineMatcher.hh>

class RecordWriter
{
public:
  RecordWriter();
  virtual ~RecordWriter();
  virtual void process(std::vector<std::string> &aRecordStrings);
  void setLineMatcher(LineMatcher *aMatcher);
protected:
  LineMatcher *_LineMatcher;
};

#endif /* RECORDWRITER_HH_ */
