#include <iostream>
#include "FieldItem.hh"
#include "SimpleLineMatcher.hh"

ccl::Logger FieldItem::sLogger("FieldItem");

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItem::FieldItem(FieldItemData aData,FieldItem *aParentItem)
  : _ParentItem(aParentItem)
{
  _FieldItemData = aData;

  QList<QVariant> tList;

  _ItemData.append(QVariant(aData.getName().c_str()));
  _ItemData.append(QVariant(aData.getType().c_str()));
  _ItemData.append(QVariant(aData.getMatch().c_str()));
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
  _FieldItemData.setCheckState(aCheckState);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::setFieldMatch(const QVariant &aValue)
{
  _ItemData[eMatchCol] = QVariant(QString(aValue.toString()));
  _FieldItemData.setMatch(aValue.toString().toStdString());
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void FieldItem::setFieldPostfix(const QVariant &aValue)
{
  _ItemData[ePostfixCol] = aValue;
  _FieldItemData.setPostfix(aValue.toString().toStdString());
}

static SimpleLineMatcher tMatcher; //TODO

//-------------------------------------------------------------------------------
// note: root doesn't consume any lines for wsdlfilter
//-------------------------------------------------------------------------------
bool FieldItem::processRootLines(
    std::vector<std::string> &aLinesIn,
    std::vector<std::string>::iterator &aLineIter,
    std::vector<std::string> &aLinesOut)
{
  DEBUG(sLogger,"Proceeding with root");
  for (int tIdx = 0; tIdx < childCount(); tIdx++)
  {
    bool tResult = child(tIdx)->processLines(aLinesIn,aLineIter,aLinesOut);
    if (tResult == false)
    {
      ERROR(sLogger,"Processing child failed");
      return false;
    }
  }
#if 0
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
#endif
  return true;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool FieldItem::processPrimitiveLines(
    std::vector<std::string> &aLinesIn,
    std::vector<std::string>::iterator &aLineIter,
    std::vector<std::string> &aLinesOut)
{
  std::string &tLine = *aLineIter;
  aLineIter++;

  tMatcher.setMatchRegex(getData().getMatch());

  //TODO rm    if (!tLine.compare(getData().getMatch()))
  //TODO make matcher a FieldItem field?
  if (tMatcher.match(tLine))
  {
    _InLine = tLine;
    if (getData().getCheckState() == Qt::Checked)
    {
      aLinesOut.push_back(tLine);
    }
    DEBUG(sLogger,"primitive node matched: " << getData().getMatch());
  }
  else
  {
    ERROR(sLogger,"Primitive node <" << tLine << "> didn't match: <"
        << getData().getMatch() << ">");
    return false;
  }
  return true;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool FieldItem::processPrimitiveArrayLines(
    std::vector<std::string> &aLinesIn,
    std::vector<std::string>::iterator &aLineIter,
    std::vector<std::string> &aLinesOut)
{
  return true;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool FieldItem::processStructLines(
    std::vector<std::string> &aLinesIn,
    std::vector<std::string>::iterator &aLineIter,
    std::vector<std::string> &aLinesOut,
      bool aSkipStructName)
{
  DEBUG(sLogger,"Looking for struct with match: " << getData().getMatch());
  if (!aSkipStructName)
  {
    std::string &tLine = *aLineIter;
    aLineIter++;

    tMatcher.setMatchRegex(getData().getMatch());
    if (tMatcher.match(tLine))
    {
      _InLine = tLine;
      //    _InLine = ""; //TODO decide on this
      DEBUG(sLogger,"struct node matched: " << getData().getMatch());
    }
    else
    {
      ERROR(sLogger,"struct node <" << tLine << "> didn't match: <"
          << getData().getMatch() << ">");
      return false;
    }
  }

  for (int tIdx = 0; tIdx < childCount(); tIdx++)
  {
    bool tResult = child(tIdx)->processLines(aLinesIn,aLineIter,aLinesOut);
    if (tResult == false)
    {
      ERROR(sLogger,"Processing child failed");
      return false;
    }
  }
  return true;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool FieldItem::processStructArrayLines(
    std::vector<std::string> &aLinesIn,
    std::vector<std::string>::iterator &aLineIter,
    std::vector<std::string> &aLinesOut)
{
  DEBUG(sLogger,"Looking for struct array with match: " << getData().getMatch());
  std::string &tLine = *aLineIter;
  aLineIter++;

  tMatcher.setMatchRegex(getData().getMatch());
  if (tMatcher.match(tLine))
  {
    _InLine = tLine;
    //    _InLine = ""; //TODO decide on this
    DEBUG(sLogger,"struct array node matched: " << getData().getMatch());
  }
  else
  {
    ERROR(sLogger,"struct array node <" << tLine << "> didn't match: <"
        << getData().getMatch() << ">");
    return false;
  }

  // Now get "array of len: N".
  tLine = *aLineIter;
  aLineIter++;

  tMatcher.setMatchRegex(".*array of len: (\\d+)"); //TODO exact match of tabs?
  if (tMatcher.match(tLine))
  {
    _InLine = tLine;
    //    _InLine = ""; //TODO decide on this
    DEBUG(sLogger,"array of len matched: " << tLine);
  }
  else
  {
    ERROR(sLogger,"array of len didn't match: <" << tLine);
    return false;
  }

  int tArrayLen = 2;
  for (int tIdx = 0; tIdx < tArrayLen; tIdx++)
  {
    processStructLines(aLinesIn,aLineIter,aLinesOut,true);
  }
return false;
}

//-------------------------------------------------------------------------------
//  enum NodeType {eNone, eRoot, ePrimitive, eStruct, ePrimitiveArrayPtr,
//    eStructArrayPtr};
//-------------------------------------------------------------------------------
bool FieldItem::processLines(
    std::vector<std::string> &aLinesIn,
    std::vector<std::string>::iterator &aLineIter,
    std::vector<std::string> &aLinesOut)
{

  if (aLineIter == aLinesIn.end())
  {
    ERROR(sLogger,"Ran out of lines in FieldItem::processLines");
    return false;
  }

  if ( getData().getNodeType() == FieldItemData::eRoot )
  {
    processRootLines(aLinesIn,aLineIter,aLinesOut);
  }

  if ( getData().getNodeType() == FieldItemData::eStruct)
  {
    processStructLines(aLinesIn,aLineIter,aLinesOut);
  }
  else if (getData().getNodeType() == FieldItemData::ePrimitive)
  {
    processPrimitiveLines(aLinesIn,aLineIter,aLinesOut);
  }
  else if (getData().getNodeType() == FieldItemData::eStructArrayPtr)
  {
    processStructArrayLines(aLinesIn,aLineIter,aLinesOut);
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
