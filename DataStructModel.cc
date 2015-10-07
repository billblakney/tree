/*
 * DataStructModel.cc
 *
 *  Created on: Oct 6, 2015
 *      Author: bill
 */

#include "DataStructModel.hh"

DataStructModel::DataStructModel()
{
  QList<QVariant> rootData;
  QVariant tRootVariant("Root");
  rootData.append(tRootVariant);
  rootItem = new FieldItem(rootData);

  QList<QVariant> data1;
  QVariant data1Variant("data1");
  data1.append(data1Variant);
  FieldItem *data1Item = new FieldItem(data1,rootItem);
  rootItem->appendChild(data1Item);

  QList<QVariant> data2;
  QVariant data2Variant("data2");
  data2.append(data2Variant);
  FieldItem *data2Item = new FieldItem(data2,rootItem);
  rootItem->appendChild(data2Item);

  QList<QVariant> data3;
  QVariant data3Variant("data1a");
  data3.append(data3Variant);
  FieldItem *data3Item = new FieldItem(data3,data1Item);
  data1Item->appendChild(data3Item);

}

DataStructModel::~DataStructModel()
{
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QVariant DataStructModel::data(const QModelIndex &index,int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  FieldItem *item = static_cast<FieldItem*>(index.internalPointer());

  return item->data(index.column());
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Qt::ItemFlags DataStructModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return 0;
  }

  return QAbstractItemModel::flags(index);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QVariant DataStructModel::headerData(int section,Qt::Orientation orientation,
      int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    return rootItem->data(section);
  }
  return QVariant();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QModelIndex DataStructModel::index(int row,int col,
      const QModelIndex &parent) const
{
  if (!hasIndex(row,col,parent))
  {
    return QModelIndex();
  }

  FieldItem *parentItem;

  if (!parent.isValid())
  {
    parentItem = rootItem;
  }
  else
  {
    parentItem = static_cast<FieldItem*>(parent.internalPointer());
  }

  FieldItem *childItem = parentItem->child(row);
  if (childItem)
  {
    return createIndex(row,col,childItem);
  }
  else
  {
    return QModelIndex();
  }
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QModelIndex DataStructModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return QModelIndex();
  }

  FieldItem *childItem = static_cast<FieldItem *>(index.internalPointer());
  FieldItem *parentItem = childItem->parentItem();

  if (parentItem == rootItem)
  {
    return QModelIndex();
  }

  return createIndex(parentItem->row(),0,parentItem);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int DataStructModel::rowCount(const QModelIndex &parent) const
{
  FieldItem *parentItem;
  if (parent.column() > 0)
  {
    return 0;
  }

  if (!parent.isValid())
  {
    parentItem = rootItem;
  }
  else
  {
    parentItem = static_cast<FieldItem*>(parent.internalPointer());
  }

  return parentItem->childCount();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int DataStructModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return static_cast<FieldItem*>(parent.internalPointer())->columnCount();
  }
  else
  {
    return rootItem->columnCount();
  }
}

