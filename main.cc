#include <cstdio>
#include <unistd.h>
#include <QApplication>
#include "MainWindow.hh"
#include "SimpleLineConsumer.hh"
#include "StreamReader.hh"

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void processCommandLine(int argc,char *argv[])
{
   if( argc < 2 )
   {
      printf("Usage: pp <in_file>\n");
      exit(0);
   }
}

enum ReadState {eNone, eStarted, eEnded};

/*------------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  processCommandLine(argc,argv);

  bool tLaunchGUI = false;

  MainWindow *window = new MainWindow(argc,argv,app,0);

  window->setGeometry(2450,135,625,900);
  window->setupView();

  DataStructModel *aModel = window->getStructDataModel();

#if 0

  SimpleLineConsumer tFieldConsumer(
      std::cin,tLineBuffer,"field",false);

  while (true)
  {
    tReceivedMessageConsumer.consume();
    tFieldConsumer.consume();
    tEndMessageConsumer.consume();
  }

#endif
  StreamReader *tStreamReader = new StreamReader(aModel);
  tStreamReader->start();

  window->show();
  return app.exec();
}
