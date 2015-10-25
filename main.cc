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

  std::string tLineBuffer;

  SimpleLineConsumer tReceivedMessageConsumer(
      std::cin,tLineBuffer,"start",true);

  SimpleLineConsumer tEndMessageConsumer(
      std::cin,tLineBuffer,"end",false);

  vector<std::string> tStructLines;

  bool tLookingForStart = true;

  while (std::getline(std::cin,tLineBuffer))
  {
    if (tLookingForStart)
    {
      if (tLineBuffer.compare("start"))
      {
        std::cout << "not starting" << std::endl;
      }
      else
      {
        std::cout << "ready to start" << std::endl;
        tLookingForStart = false;
      }
    }
    else
    {
      if (tLineBuffer.compare("end"))
      {
        std::cout << "pushing back struct line" << std::endl;
        tStructLines.push_back(tLineBuffer);
      }
      else
      {
        std::cout << "reached end of structure" << std::endl;
        aModel->processLinesIn(tStructLines);
        tStructLines.clear();
        tLookingForStart = true;
      }
    }
  }

#if 0

  SimpleLineConsumer tFieldConsumer(
      std::cin,tLineBuffer,"field",false);

  while (true)
  {
    tReceivedMessageConsumer.consume();
    tFieldConsumer.consume();
    tEndMessageConsumer.consume();
  }

//  StreamReader *tStreamReader = new StreamReader();
//  tStreamReader->start();
#endif

  window->show();
  return app.exec();
}
