#ifndef MAINWINDOW_HH_
#define MAINWINDOW_HH_

#include <string>
#include <QComboBox>
#include <QStringList>
#include <QTreeView>
#include <QWidget>
#include "StructorBuilder.hh"

class MainWindow: public QWidget
{
  Q_OBJECT
public:
  MainWindow(int argc,char *argv[],
      QApplication &aApplication,QWidget *aParent);
  virtual ~MainWindow();
	void setupView();
public slots:
  void onStructComboBoxActivated(int index);
protected:
	std::string _HFile;
	StructorBuilder *_StructorBuilder;
	QComboBox       *_StructComboBox;
	QTreeView       *_StructTree;

	void setTreeViewStruct(std::string aStructName);
	QStringList convertToQStringList(std::vector<std::string> aStrings);
};

#endif /* MAINWINDOW_HH_ */
