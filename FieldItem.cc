#include <iostream>
#include "FieldItem.hh"

ccl::Logger FieldItem::sLogger("FieldItem");

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItem::FieldItem(FieldItemData aData,FieldItem *aParentItem)
  : _ParentItem(aParentItem)
{
  _FieldItemData = aData;

  QList<QVariant> tList;

  _ItemData.append(QVariant(aData._Name.c_str()));
  _ItemData.append(QVariant(aData._Type.c_str()));
  _ItemData.append(QVariant(aData._Match.c_str()));
  _ItemData.append(QVariant(QString("newline (\"\\n\")")));

  _NodeType = aData._NodeType;
  _CheckState = Qt::Unchecked;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItem::~FieldItem()
{
  qDeleteAll(_ChildItems);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::appendChild(FieldItem *item)
{
  _ChildItems.append(item);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItem *FieldItem::child(int row)
{
  return _ChildItems.value(row);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int FieldItem::childCount() const
{
  return _ChildItems.count();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int FieldItem::row() const
{
  if( _ParentItem )
  {
    return _ParentItem->_ChildItems.indexOf(const_cast<FieldItem *>(this));
  }
  return 0;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int FieldItem::columnCount() const
{
  return _ItemData.count();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QVariant FieldItem::data(int column) const
{
  return _ItemData.value(column);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItem *FieldItem::parentItem() const
{
  return _ParentItem;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItemData::NodeType FieldItem::getNodeType() const
{
  return _NodeType;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Qt::CheckState FieldItem::getCheckState()
{
  return _CheckState;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string FieldItem::getFieldName()
{
  return _FieldItemData._Name;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string FieldItem::getFieldType()
{
  return _FieldItemData._Type;
}

//-------------------------------------------------------------------------------
// TODO?
//-------------------------------------------------------------------------------
std::string FieldItem::getFieldMatch()
{
  if (_NodeType != FieldItemData::eStruct)
  {
    return _FieldItemData._Match;
  }
  return std::string("");
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::setCheckState(Qt::CheckState aCheckState)
{
  _CheckState = aCheckState;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::setFieldMatch(const QVariant &aValue)
{
  _ItemData[eMatchCol] = QVariant(QString(aValue.toString()));
  _FieldItemData._Match = aValue.toString().toStdString();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::setFieldPostfix(const QVariant &aValue)
{
  _ItemData[ePostfixCol] = aValue;
  _FieldItemData._Postfix = aValue.toString().toStdString();
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

  if ( _NodeType == FieldItemData::eRoot )
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
  if ( _NodeType == FieldItemData::eStruct)
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
  else if (_NodeType == FieldItemData::ePrimitive)
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
