#ifndef SIMPLELINECONSUMER_HH_
#define SIMPLELINECONSUMER_HH_

#include <iostream>

class SimpleLineConsumer
{
public:

  SimpleLineConsumer(std::istream &aStream,std::string &aLineBuffer,
      std::string aMatchRegex,bool aContinueOnMismatch = false);

  virtual ~SimpleLineConsumer();

  bool consume();

  void onMatch();

  void onMismatch();

protected:

  std::istream &_Stream;
  std::string  &_LineBuffer;
  std::string  _MatchString;
  bool         _ContinueOnMismatch;
};

#endif /* SIMPLELINECONSUMER_HH_ */
