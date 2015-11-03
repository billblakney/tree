#ifndef FIELDITEM_HH_
#define FIELDITEM_HH_

#include <iterator>
#include <string>
#include <vector>
#include <QList>
#include <QVariant>
#include "LineConsumer.hh"
#include "Logger.hh"

/**
 * FieldItem represents a node in the data structure tree.
 */
class FieldItem
{
public:

  enum Column {eNameCol, eTypeCol, eMatchCol, ePostfixCol};

  enum NodeType {eNone, eRoot, ePrimitive, eStruct, ePrimitiveArrayPtr,
    eStructArrayPtr};

  FieldItem(NodeType aType,const QList<QVariant> &aData,
      FieldItem *aParentItem = 0,LineConsumer *aLineConsumer = 0);

  virtual ~FieldItem();

  void appendChild(FieldItem *item);

  FieldItem *child(int row);

  int childCount() const;

  int columnCount() const;

  QVariant data(int column) const;

  int row() const;

  FieldItem *parentItem() const;

  NodeType getType() const;

  Qt::CheckState getCheckState();

  void setCheckState(Qt::CheckState aCheckState);

  void setFieldMatch(const QVariant & value);

  void setFieldPostfix(const QVariant & value);

  std::string getFieldName();

  std::string getFieldType();

  std::string getFieldMatch();

  LineConsumer *getLineConsumer();

  bool processLines(std::vector<std::string> &aLinesIn,
      std::vector<std::string>::iterator &aLineIter);

  void printInLines();


protected:

  static ccl::Logger sLogger;

  FieldItem         *_ParentItem;
  NodeType           _Type;
  QList<QVariant>    _ItemData;
  QList<FieldItem*>  _ChildItems;
  Qt::CheckState     _CheckState;
  LineConsumer      *_LineConsumer;
  std::string        _InLine;

};

#endif /* FIELDITEM_HH_ */
