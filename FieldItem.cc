#include <iostream>
#include "FieldItem.hh"

FieldItem::FieldItem(
    NodeType aType,const QList<QVariant> &aData,FieldItem *aParentItem,
        LineConsumer *aLineConsumer)
  : _ParentItem(aParentItem),
    _Type(aType),
    _ItemData(aData),
    _CheckState(Qt::Unchecked),
    _LineConsumer(aLineConsumer)
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
// TODO?
//-------------------------------------------------------------------------------
std::string FieldItem::getFieldMatch()
{
  if (_Type != eStruct)
  {
    return _ItemData.at(eMatchCol).toString().toStdString();
  }
  return std::string("");
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
LineConsumer *FieldItem::getLineConsumer()
{
  return _LineConsumer;
}

//-------------------------------------------------------------------------------
//  enum NodeType {eNone, eRoot, ePrimitive, eStruct, ePrimitiveArrayPtr,
//    eStructArrayPtr};
//-------------------------------------------------------------------------------
bool FieldItem::processLines(std::vector<std::string> &aLinesIn,
      std::vector<std::string>::iterator &aLineIter)
{
  if (aLineIter == aLinesIn.end())
  {
    std::cout << "ran out of lines in FieldItem::processLines" << std::endl;
    return false;
  }

  if ( _Type == eRoot )
  {
    if (!(aLineIter++)->compare(getFieldMatch()))
    {
      std::cout << "root|struct node match: " << getFieldMatch() << std::endl;
      for (int tIdx = 0; tIdx < childCount(); tIdx++)
      {
        bool tResult = child(tIdx)->processLines(aLinesIn,aLineIter);
        if (tResult == false)
        {
          std::cout << "ERROR: processing child failed" << std::endl;
          return false;
        }
      }
    }
    else
    {
      std::cout << "ERROR: struct|root didn't match: " << getFieldMatch() << std::endl;
      return false;
    }
  }
  if ( _Type == eStruct)
  {
    for (int tIdx = 0; tIdx < childCount(); tIdx++)
    {
      bool tResult = child(tIdx)->processLines(aLinesIn,aLineIter);
      if (tResult == false)
      {
        std::cout << "ERROR: processing child failed" << std::endl;
        return false;
      }
    }
  }
  else if (_Type == ePrimitive)
  {
    if (!(aLineIter++)->compare(getFieldMatch()))
    {
      std::cout << "primitive node matched: " << getFieldMatch() << std::endl;
    }
    else
    {
      std::cout << "ERROR: primitive node didn't match: " << getFieldMatch() << std::endl;
      return false;
    }
  }

  return true;;
}
