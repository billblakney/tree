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
FieldItemData FieldItem::getData()
{
  return _FieldItemData;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::setCheckState(Qt::CheckState aCheckState)
{
  _FieldItemData._CheckState = aCheckState;
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
DEBUG(sLogger,"============================ processLines");
std::cout << "============================ processLines" << std::endl;
  if (aLineIter == aLinesIn.end())
  {
    ERROR(sLogger,"Ran out of lines in FieldItem::processLines");
    return false;
  }

  if ( getData().getNodeType() == FieldItemData::eRoot )
  {
    std::string &tLine = *aLineIter;
    aLineIter++;
    if (!tLine.compare(getData().getMatch()))
    {
      DEBUG(sLogger,"Root node match: " << getData().getMatch());
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
      ERROR(sLogger,"Root didn't match: " << getData().getMatch());
      return false;
    }
  }
  if ( getData().getNodeType() == FieldItemData::eStruct)
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
  else if (getData().getNodeType() == FieldItemData::ePrimitive)
  {
    std::string &tLine = *aLineIter;
    aLineIter++;
    if (!tLine.compare(getData().getMatch()))
    {
      _InLine = tLine;
      DEBUG(sLogger,"primitive node matched: " << getData().getMatch());
    }
    else
    {
      ERROR(sLogger,"Primitive node didn't match: " << getData().getMatch());
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
