#ifndef SIMPLELINEMATCHER_HH_
#define SIMPLELINEMATCHER_HH_

#include "LineMatcher.hh"

class SimpleLineMatcher: public LineMatcher
{
public:
  SimpleLineMatcher();
  virtual ~SimpleLineMatcher();
  virtual bool match(std::string &aStr) =0;
  void setMatchRegex(std::string aMatchRegex);
protected:
  boost::regex _MatchRegex;
};

#endif /* SIMPLELINEMATCHER_HH_ */
