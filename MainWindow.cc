#include <cstdio>
#include <iostream>
#include <vector>
#include <QFileSystemModel>
#include <QAbstractItemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include "DataStructModel.hh"
#include "../structor/StructorBuilder.hh"
#include "MainWindow.hh"

using namespace std;

extern StructorBuilder *lex_main(char *aHeaderFile);


MainWindow::MainWindow(int argc,char *argv[],
    QApplication &aApplication,QWidget *aParent)
  : QWidget(aParent),
    _StructorBuilder(0),
    _StructComboBox(0),
    _StructTree(0)
{
  _HFile = argv[1];
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupView()
{
  _StructorBuilder = lex_main((char *)_HFile.c_str());
//   _StructorBuilder->printSummary();
//   _StructorBuilder->postProcess();

  // Create structure dropdown list.
  std::vector<std::string> tStructNames = _StructorBuilder->getStructNames();
  _StructComboBox = new QComboBox(this);
  _StructComboBox->addItems(convertToQStringList(tStructNames));

  connect(_StructComboBox,SIGNAL(activated(int)),
                       this, SLOT(onStructComboBoxActivated(int)));

  _StructTree = new QTreeView(this);

  // Create structure tree view.
//  std::string tCarReportType("OUTER_T");
  std::string tCarReportType("CONTACT_ATTRIBUTES_REPORT_T");
  setTreeViewStruct(tCarReportType);

// TODO works form 4.8 on
#ifdef EXPAND_ALL
   _StructTree->expandAll();
#else
   _StructTree->expand(_StructTree->model()->index(0,0,QModelIndex()));
#endif

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
   layout->addWidget(_StructComboBox);
   layout->addWidget(_StructTree);
#if 0
   layout->addWidget(&btn1);
   layout->addWidget(&btn2);
   layout->addWidget(&btn3);
   //layout->resize(400,120);
#endif

   setLayout(layout);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void MainWindow::setTreeViewStruct(std::string aStructName)
{
  Structure *tStructure = _StructorBuilder->getStructure(aStructName);
  DataStructModel *tDataStructModel = new DataStructModel(tStructure,_StructorBuilder);
  _StructTree->setModel(tDataStructModel);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QStringList MainWindow::convertToQStringList(std::vector<std::string> aStrings)
{
  QStringList tList;
  std::vector<std::string>::iterator tIt;
  for( tIt = aStrings.begin(); tIt != aStrings.end(); tIt++ )
  {
    tList.push_back(tIt->c_str());
  }
  return tList;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void MainWindow::onStructComboBoxActivated(int index)
{
    QString tString = _StructComboBox->itemText(index);
    setTreeViewStruct(tString.toStdString());
}
