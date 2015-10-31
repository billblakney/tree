#include <boost/regex.hpp>
#include "SimpleLineMatcher.hh"

SimpleLineMatcher::SimpleLineMatcher()
  : _MatchRegex(".+")
{
}

SimpleLineMatcher::~SimpleLineMatcher()
{
}

bool SimpleLineMatcher::match(std::string &aStr)
{
    boost::match_results<std::string::const_iterator> what;

    if (boost::regex_match(aStr,what,_MatchRegex))
    {
      return true;
    }
    else
    {
      return false;
    }
}
void SimpleLineMatcher::setMatchRegex(std::string aMatchRegex)
{
  boost::regex tMatchRegex(aMatchRegex);
  _MatchRegex = tMatchRegex;
}
