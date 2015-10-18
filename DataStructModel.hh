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
  DataStructModel();
  DataStructModel(Structure *aStructure);
  virtual ~DataStructModel();

  DataStructModel(
      Structure *aStructure,StructorBuilder *aStructBuilder);

  void buildTree(FieldItem *rootItem,Structure *aStructure,
      StructorBuilder *aStructBuilder,int aLevel = 0);

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

  FieldItem *rootItem;
  void setupModelData() {}
  void setChildrenCheckStates(
      const QModelIndex &aParentIndex,Qt::CheckState aCheckState);
  void updateParentCheckState(
      const QModelIndex &aChildIndex,Qt::CheckState aNewState);
};

#endif /* DATASTRUCTMODEL_HH_ */
