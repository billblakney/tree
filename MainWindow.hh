#ifndef MAINWINDOW_HH_
#define MAINWINDOW_HH_

#include <string>
#include <QComboBox>
#include <QStringList>
#include <QTreeView>
#include <QWidget>
#include "StructorBuilder.hh"
#include "DataStructModel.hh"
#include "SimpleRecordWriter.hh"
#include "StreamReader.hh"

class MainWindow: public QWidget
{
  Q_OBJECT
public:

  MainWindow(int argc,char *argv[],
      QApplication &aApplication,QWidget *aParent);

  virtual ~MainWindow();

	void setupView();

  DataStructModel *getStructDataModel();

public slots:

  void onStructComboBoxActivated(int index);
  void onSetFilterClicked(bool);

protected:

	std::string _HFile;

	std::string _InitialStruct;

	StructorBuilder *_StructorBuilder;

	DataStructModel *_DataStructModel;

	QComboBox       *_StructComboBox;
	QTreeView       *_StructTree;

  SimpleRecordWriter *_Writer;
  StreamReader *_StreamReader;

	void setTreeViewStruct(std::string aStructName);

	QStringList convertToQStringList(std::vector<std::string> aStrings);

	std::string getMatchString();
};

#endif /* MAINWINDOW_HH_ */
