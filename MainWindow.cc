#include <cstdio>
#include <iostream>
#include <vector>
#include <QFileSystemModel>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include "DataStructModel.hh"
#include "../structor/StructorBuilder.hh"
#include "MainWindow.hh"

using namespace std;

extern StructorBuilder *lex_main(char *aHeaderFile);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
MainWindow::MainWindow(int argc,char *argv[],
    QApplication &aApplication,QWidget *aParent)
  : QWidget(aParent),
    _StructorBuilder(0),
    _DataStructModel(0),
    _StructComboBox(0),
    _StructTree(0)
{
  _HFile = argv[1];
  _InitialStruct = argv[2];
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DataStructModel *MainWindow::getStructDataModel()
{
  return _DataStructModel;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MainWindow::setupView()
{
  // Parse the header file.
  _StructorBuilder = lex_main((char *)_HFile.c_str());
//   _StructorBuilder->printSummary();
//   _StructorBuilder->postProcess();

  // Create structure dropdown list.
  std::vector<std::string> tStructNames = _StructorBuilder->getStructNames();
  _StructComboBox = new QComboBox(this);
  _StructComboBox->addItems(convertToQStringList(tStructNames));

  connect(_StructComboBox,SIGNAL(activated(int)),
                       this, SLOT(onStructComboBoxActivated(int)));

  // Create structure tree view.
  _StructTree = new QTreeView(this);
  setTreeViewStruct(_InitialStruct);
  _StructTree->header()->resizeSection(0,325);

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

   QPushButton *tButton = new QPushButton("Set Filter",this);

   //-------------------------
   // Put widgets in the dialog using box layout.
   //-------------------------
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget(_StructComboBox);
   layout->addWidget(_StructTree);
   layout->addWidget(tButton);
#if 0
   layout->addWidget(&btn1);
   layout->addWidget(&btn2);
   layout->addWidget(&btn3);
   //layout->resize(400,120);
#endif

   setLayout(layout);
}

//-------------------------------------------------------------------------------
// Create a struct data model for the specified struct name and load it in the
// tree.
//-------------------------------------------------------------------------------
void MainWindow::setTreeViewStruct(std::string aStructName)
{
  Structure *tStructure = _StructorBuilder->getStructure(aStructName);
  _DataStructModel = new DataStructModel(tStructure,_StructorBuilder);
  _StructTree->setModel(_DataStructModel);
#if 0 //  seems to have no effect
  _StructTree->resizeColumnToContents(0);
  _StructTree->resizeColumnToContents(1);
#endif
}

//-------------------------------------------------------------------------------
// Convert a vector of strings to a QStringList.
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
// Load the newly user-selected structure into the tree view.
//-------------------------------------------------------------------------------
void MainWindow::onStructComboBoxActivated(int index)
{
    QString tString = _StructComboBox->itemText(index);
    setTreeViewStruct(tString.toStdString());
}
