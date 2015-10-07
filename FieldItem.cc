#include "FieldItem.hh"

FieldItem::FieldItem(const QList<QVariant> &aData,FieldItem *aParentItem)
{
  _ParentItem = aParentItem;
  _ItemData = aData;
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
