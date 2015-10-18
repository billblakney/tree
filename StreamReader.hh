#ifndef STREAMREADER_HH_
#define STREAMREADER_HH_

#include <QThread>

class StreamReader: public QThread
{
public:
  StreamReader();
  virtual ~StreamReader();

public slots:

  void run();
};

#endif /* STREAMREADER_HH_ */
