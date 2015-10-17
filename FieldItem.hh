#ifndef FIELDITEM_HH_
#define FIELDITEM_HH_

#include <QList>
#include <QVariant>

/**
 * FieldItem represents a node in the data structure tree.
 */
class FieldItem
{
public:
  FieldItem(const QList<QVariant> &aData,FieldItem *aParentItem = 0);
  virtual ~FieldItem();

  void appendChild(FieldItem *item);

  FieldItem *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  FieldItem *parentItem() const;

private:
  QList<FieldItem*> _ChildItems;
  QList<QVariant> _ItemData;
  FieldItem *_ParentItem;

};

#endif /* FIELDITEM_HH_ */
