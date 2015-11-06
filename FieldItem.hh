#ifndef FIELDITEM_HH_
#define FIELDITEM_HH_

#include <iterator>
#include <string>
#include <vector>
#include <QList>
#include <QVariant>
#include "FieldItemData.hh"
#include "Logger.hh"

/**
 * FieldItem represents a node in the data structure tree.
 */
class FieldItem
{
public:

  enum Column {eNameCol, eTypeCol, eMatchCol, ePostfixCol};

  FieldItem(FieldItemData aData,FieldItem *aParentItem = 0);

  virtual ~FieldItem();

  //===========================================================================
  // tree related methods
  //===========================================================================

  void appendChild(FieldItem *item);

  FieldItem *child(int row);

  int childCount() const;

  int columnCount() const;

  QVariant data(int column) const;

  int row() const;

  FieldItem *parentItem() const;

  //===========================================================================
  // data related methods
  //===========================================================================

  FieldItemData getData();

  FieldItemData::NodeType getNodeType() const;

  Qt::CheckState getCheckState();

  void setCheckState(Qt::CheckState aCheckState);

  void setFieldMatch(const QVariant & value);

  void setFieldPostfix(const QVariant & value);

  bool processLines(std::vector<std::string> &aLinesIn,
      std::vector<std::string>::iterator &aLineIter);

  void printInLines();


protected:

  static ccl::Logger sLogger;

  FieldItem         *_ParentItem;
  FieldItemData::NodeType           _NodeType;
  QList<QVariant>    _ItemData;
  QList<FieldItem*>  _ChildItems;
  Qt::CheckState     _CheckState;
  std::string        _InLine;

  FieldItemData      _FieldItemData;

};

#endif /* FIELDITEM_HH_ */
