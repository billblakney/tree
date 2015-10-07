#ifndef DATASTRUCTMODEL_HH_
#define DATASTRUCTMODEL_HH_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "FieldItem.hh"

class DataStructModel : public QAbstractItemModel
{
public:
  DataStructModel();
  virtual ~DataStructModel();

  QVariant data(const QModelIndex &index,int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section,Qt::Orientation orientation,
      int role = Qt::DisplayRole) const;
  QModelIndex index(int row,int col,
      const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
  FieldItem *rootItem;
  void setupModelData() {}
};

#endif /* DATASTRUCTMODEL_HH_ */
