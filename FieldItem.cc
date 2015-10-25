#include <iostream>
#include "FieldItem.hh"

ccl::Logger FieldItem::sLogger("FieldItem");

FieldItem::FieldItem(
    NodeType aType,const QList<QVariant> &aData,FieldItem *aParentItem,
        LineConsumer *aLineConsumer)
  : _ParentItem(aParentItem),
    _Type(aType),
    _ItemData(aData),
    _CheckState(Qt::Unchecked),
    _LineConsumer(aLineConsumer),
    _InLine("")
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
    ERROR(sLogger,"Ran out of lines in FieldItem::processLines");
    return false;
  }

  if ( _Type == eRoot )
  {
    std::string &tLine = *aLineIter;
    aLineIter++;
    if (!tLine.compare(getFieldMatch()))
    {
      DEBUG(sLogger,"Root node match: " << getFieldMatch());
      _InLine = tLine;
      for (int tIdx = 0; tIdx < childCount(); tIdx++)
      {
        bool tResult = child(tIdx)->processLines(aLinesIn,aLineIter);
        if (tResult == false)
        {
          ERROR(sLogger,"Processing child failed");
          return false;
        }
      }
    }
    else
    {
      ERROR(sLogger,"Root didn't match: " << getFieldMatch());
      return false;
    }
  }
  if ( _Type == eStruct)
  {
    _InLine = "";
    for (int tIdx = 0; tIdx < childCount(); tIdx++)
    {
      bool tResult = child(tIdx)->processLines(aLinesIn,aLineIter);
      if (tResult == false)
      {
        ERROR(sLogger,"Processing child failed");
        return false;
      }
    }
  }
  else if (_Type == ePrimitive)
  {
    std::string &tLine = *aLineIter;
    aLineIter++;
    if (!tLine.compare(getFieldMatch()))
    {
      _InLine = tLine;
      DEBUG(sLogger,"primitive node matched: " << getFieldMatch());
    }
    else
    {
      ERROR(sLogger,"Primitive node didn't match: " << getFieldMatch());
      return false;
    }
  }

  return true;;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::printInLines()
{
  if (_InLine.size())
  {
    std::cout << _InLine << std::endl;
  }
  for (int tIdx = 0; tIdx < childCount(); tIdx++)
  {
    child(tIdx)->printInLines();
  }
}
