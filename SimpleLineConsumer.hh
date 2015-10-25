#ifndef SIMPLELINECONSUMER_HH_
#define SIMPLELINECONSUMER_HH_

#include "LineConsumer.hh"

class SimpleLineConsumer : public LineConsumer
{
public:

  SimpleLineConsumer(std::istream &aStream,std::string &aLineBuffer,
      std::string aMatchRegex,bool aContinueOnMismatch = false);

  virtual ~SimpleLineConsumer();

  // @override
  bool consume();

  void onMatch();

  void onMismatch();

protected:

  std::string  _MatchString;
  bool         _ContinueOnMismatch;
};

#endif /* SIMPLELINECONSUMER_HH_ */
