#ifndef STREAMREADER_HH_
#define STREAMREADER_HH_

#include <QThread>
#include "DataStructModel.hh"
#include "Logger.hh"
#include "RecordWriter.hh"

class StreamReader: public QThread
{
public:
  StreamReader(DataStructModel *aModel);
  virtual ~StreamReader();

public slots:

  void run();

protected:
  static ccl::Logger sLogger;
  std::vector<RecordWriter *> _Writers;
  DataStructModel *_DataStructModel;
};

#endif /* STREAMREADER_HH_ */
