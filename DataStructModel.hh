#ifndef DATASTRUCTMODEL_HH_
#define DATASTRUCTMODEL_HH_

#include <QAbstractItemModel>
#include <QFont>
#include <QModelIndex>
#include <QVariant>
#include "FieldItem.hh"
#include "Structure.hh"
#include "StructorBuilder.hh"

class DataStructModel : public QAbstractItemModel
{
public:
  DataStructModel(Structure *aStructure);
  virtual ~DataStructModel();

  DataStructModel(
      Structure *aStructure,StructorBuilder *aStructBuilder);

  void buildTree(FieldItem *rootItem,Structure *aStructure,
      StructorBuilder *aStructBuilder,int aLevel = 0);

  QList<QVariant> buildDataList(
    const std::string aName,const std::string aType,std::string aMatch = "");

  bool processLinesIn(std::vector<std::string> &aLinesIn);

  std::string getDotString(StructorBuilder *aStructBuilder,
    std::string aName,std::string aPrefix);

  std::string getMatchString(FieldItem *aFieldItem);

  QVariant data(const QModelIndex &index,int role) const;

  bool setData(const QModelIndex &index,const QVariant &value,
      int role = Qt::EditRole);

  Qt::ItemFlags flags(const QModelIndex &index) const;

  QVariant headerData(int section,Qt::Orientation orientation,
      int role = Qt::DisplayRole) const;

  QModelIndex index(int row,int col,
      const QModelIndex &parent = QModelIndex()) const;

  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent = QModelIndex()) const;

  int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:

  static QFont kArrayFont;

  FieldItem *_RootItem;
  FieldItem *_TopNodeItem;
  void setupModelData() {}
  void setChildrenCheckStates(
      const QModelIndex &aParentIndex,Qt::CheckState aCheckState);
  void updateParentCheckState(
      const QModelIndex &aChildIndex,Qt::CheckState aNewState);
};

#endif /* DATASTRUCTMODEL_HH_ */
