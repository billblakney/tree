#include <cstdio>
#include <unistd.h>
#include <QApplication>
#include "MainWindow.hh"
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

/*------------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  processCommandLine(argc,argv);

  MainWindow *window = new MainWindow(argc,argv,app,0);

  window->setGeometry(2450,135,625,900);
  window->setupView();

//  DataStructModel *aModel = window

//  StreamReader *tStreamReader = new StreamReader();
//  tStreamReader->start();

  window->show();
  return app.exec();
}
