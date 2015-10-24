#include <iostream>
#include "FieldItem.hh"

FieldItem::FieldItem(
    NodeType aType,const QList<QVariant> &aData,FieldItem *aParentItem)
  : _ParentItem(aParentItem),
    _Type(aType),
    _ItemData(aData),
    _CheckState(Qt::Unchecked)
{
}

FieldItem::~FieldItem()
{
  qDeleteAll(_ChildItems);
}

void FieldItem::appendChild(FieldItem *item)
{
  _ChildItems.append(item);
}

FieldItem *FieldItem::child(int row)
{
  return _ChildItems.value(row);
}

int FieldItem::childCount() const
{
  return _ChildItems.count();
}

int FieldItem::row() const
{
  if( _ParentItem )
  {
    return _ParentItem->_ChildItems.indexOf(const_cast<FieldItem *>(this));
  }
  return 0;
}

int FieldItem::columnCount() const
{
  return _ItemData.count();
}

QVariant FieldItem::data(int column) const
{
  return _ItemData.value(column);
}

FieldItem *FieldItem::parentItem() const
{
  return _ParentItem;
}

FieldItem::NodeType FieldItem::getType() const
{
  return _Type;
}

Qt::CheckState FieldItem::getCheckState()
{
  return _CheckState;
}

void FieldItem::setCheckState(Qt::CheckState aCheckState)
{
  _CheckState = aCheckState;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string FieldItem::getFieldName()
{
  return _ItemData.at(eNameCol).toString().toStdString();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string FieldItem::getFieldType()
{
  return _ItemData.at(eTypeCol).toString().toStdString();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string FieldItem::getFieldMatch()
{
  return _ItemData.at(eMatchCol).toString().toStdString();
}
