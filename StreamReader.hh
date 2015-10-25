#ifndef STREAMREADER_HH_
#define STREAMREADER_HH_

#include <QThread>
#include "DataStructModel.hh"

class StreamReader: public QThread
{
public:
  StreamReader(DataStructModel *aModel);
  virtual ~StreamReader();

public slots:

  void run();

protected:
  DataStructModel *_DataStructModel;
};

#endif /* STREAMREADER_HH_ */
