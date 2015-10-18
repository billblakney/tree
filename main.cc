#include <iostream>
#include <QApplication>
#include <QFileSystemModel>
#include <QAbstractItemModel>
#include <QTreeView>
#include <QPushButton>
#include <QVBoxLayout>
#include <set>
#include "DataStructModel.hh"

using namespace std;

#define DO_STRUCTOR_STUFF
#ifdef DO_STRUCTOR_STUFF
#include <cstdio>
#include "../structor/StructorBuilder.hh"

extern StructorBuilder &lex_main(char *aHeaderFile);

static DataStructModel *tDataStructModel;

void doStructorStuff(int argc,char **argv)
{
   if( argc < 2 )
      printf("Usage: pp <in_file>\n");

   StructorBuilder &sb = lex_main(argv[1]);
//   sb.printSummary();
//   sb.postProcess();

  std::string tCarReportType("CONTACT_ATTRIBUTES_REPORT_T");

Structure *tStructure = sb.getStructure(tCarReportType);

tDataStructModel = new DataStructModel(
    tStructure,&sb);
}
#endif

/*------------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

doStructorStuff(argc,argv);

  QWidget *window = new QWidget;
//  window->setMinimumSize(400,800);
  window->setGeometry(2555,135,400,800);

//   QFileSystemModel *tModel = new QFileSystemModel;
//   QAbstractItemModel *tModel = new DataStructModel();
   QTreeView *tTree = new QTreeView(window);
//   tTree->setModel(tModel);
   tTree->setModel(tDataStructModel);

#if 0
   //-------------------------
   // btn1 uses default layers (all)
   //-------------------------
   const QString str1("Button1                                                                ");
   DefaultIconButton btn1(str1);
   btn1.resize(300, 30);

   //-------------------------
   // btn2 uses custom set of layers
   //-------------------------
   set<swftsParams::layerIdentifierType_t> layers;
   layers.insert(swftsParams::eLayerIdentifier_Base);
   layers.insert(swftsParams::eLayerIdentifier_BaseOutline);
   layers.insert(swftsParams::eLayerIdentifier_PlatformId);
   layers.insert(swftsParams::eLayerIdentifier_BearingRateIndicator);

   const QString str2("Button2                                                                ");
   DefaultIconButton btn2(str2,layers,5);
   btn2.resize(300, 30);

   //-------------------------
   // btn3 cycles through all classification types
   //-------------------------
   const QString str3("Button3");
   CycleIconButton btn3(str3);
   btn3.resize(300, 30);
   btn3.numCycles = BaseIconButton::numClassifications;
#endif

   //-------------------------
   // Put widgets in the dialog using box layout.
   //-------------------------
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget(tTree);
#if 0
   layout->addWidget(&btn1);
   layout->addWidget(&btn2);
   layout->addWidget(&btn3);
   //layout->resize(400,120);
#endif

   window->setLayout(layout);

   window->show();
   return app.exec();
}
