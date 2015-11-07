#include <iostream>
#include <iterator>
#include <sstream>
#include <boost/regex.hpp>
#include <QVariant>
#include "DataStructModel.hh"

ccl::Logger DataStructModel::sLogger("DataStructModel");

QFont DataStructModel::kArrayFont;

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
DataStructModel::DataStructModel(
    Structure *aStructure,StructorBuilder *aStructBuilder)
  : _RootItem(0),
    _TopNodeItem(0)
{
  kArrayFont.setItalic(true);

  // root item
  FieldItemData tRootFieldItemData(FieldItemData::eRoot,aStructure->_Name,aStructure->_Name);
  _RootItem = new FieldItem(tRootFieldItemData);

  // top node item
  FieldItemData tTopFieldItemData(FieldItemData::eRoot,"struct",aStructure->_Name);
  _TopNodeItem = new FieldItem(tTopFieldItemData,_RootItem);

  _RootItem->appendChild(_TopNodeItem);

  buildTree(_TopNodeItem,aStructure,aStructBuilder);

//DEBUG(sLogger,"CTOR getDotString: " << getDotString(aStructBuilder,aStructure->_Name,"aStruct"));
//DEBUG(sLogger,"CTOR getMatchString: " << getMatchString(_TopNodeItem));
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
DataStructModel::~DataStructModel()
{
}

//TODO deep enough? replace generically?
static std::string blanks[] = {
    "",
    "  ",
    "    ",
    "      ",
    "        ",
    "          ",
    "            ",
    "              ",
    "                ",
    "                  ",
};

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string DataStructModel::buildMatchForField(const Field &aField,int aIndentLevel)
{
  std::string tMatch = "^";
  for (int tIdx = 0; tIdx < aIndentLevel; tIdx++)
  {
    tMatch += "\t";
  }
  tMatch += aField._Name + ":";
  return tMatch;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string DataStructModel::buildMatchForPrimitiveArrayField(const Field &aField,int aIndentLevel)
{
  return buildMatchForField(aField,aIndentLevel);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string DataStructModel::buildMatchForPrimitiveField(const Field &aField,int aIndentLevel)
{
  return buildMatchForField(aField,aIndentLevel);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string DataStructModel::buildMatchForStructArrayField(const Field &aField,int aIndentLevel)
{
  return buildMatchForField(aField,aIndentLevel);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string DataStructModel::buildMatchForStructField(const Field &aField,int aIndentLevel)
{
  return buildMatchForField(aField,aIndentLevel);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::string DataStructModel::getFirstFieldMatch()
{
  std::cout << "top children: " << _TopNodeItem->childCount() << std::endl;
  return _TopNodeItem->child(0)->getData().getMatch();
}

//-------------------------------------------------------------------------------
// Prints a description of the data structure in a hierarchical fashion.
//  enum NodeType {eNone, eRoot, ePrimitive, eStruct, ePrimitiveArrayPtr, eStructArrayPtr};
//-------------------------------------------------------------------------------
void DataStructModel::buildTree(FieldItem *aParentItem,
    Structure *aStructure,StructorBuilder *aStructBuilder,int aLevel)
{
  boost::regex array_size_regex("^([a-zA-Z]+)_size$");

  // If level is zero, this is the root node.
  // Print the root node name at indent level, and increment the level to
  // indent child fields.
  if (aLevel == 0)
  {
    DEBUG(sLogger,"root node: " << aStructure->_Name);
    aLevel++;
  }

  // Print all of the fields. For fields that are structs themselves,
  // this method will be called recursively.
  vector<Field>::iterator tIter;
  for (tIter = aStructure->_Fields.begin(); tIter != aStructure->_Fields.end(); tIter++)
  {
    boost::match_results<std::string::const_iterator> what;

    // For array size line,
    // print the array type and name.
    if (boost::regex_match(tIter->_Name,what,array_size_regex))
    {
//      std::string tName = what[1];
      DEBUG(sLogger,blanks[aLevel] << "array size entry: "
          << tIter->_Type << ":" << tIter->_Name);
    }
    else if (tIter->_IsPointer)
    {
      // For primitive pointer,
      // print type and name.
      if (aStructBuilder->isPrimitive(tIter->_Type))
      {
        std::string tMatch = buildMatchForPrimitiveArrayField(*tIter,aLevel);
        FieldItemData tData(FieldItemData::ePrimitiveArrayPtr,
            tIter->_Name,tIter->_Type,tMatch);
        FieldItem *dataItem = new FieldItem(tData,aParentItem);
        aParentItem->appendChild(dataItem);

//        DEBUG(sLogger,blanks[aLevel]);
        DEBUG(sLogger,blanks[aLevel] << "primitive array node: "
            << tIter->_Type << ":" << tIter->_Name);
      }
      // For struct pointer,
      // print type and name, increment level, and call this routine
      // recursively to print the contents of the struct.
      else
      {
//        DEBUG(sLogger,blanks[aLevel]);
        DEBUG(sLogger,blanks[aLevel] << "struct array node: "
            << tIter->_Type << ":" << tIter->_Name);
        Structure *tStruct = aStructBuilder->getStructure(tIter->_Type);
        if (tStruct != NULL)
        {
          std::string tMatch = buildMatchForStructArrayField(*tIter,aLevel);
          FieldItemData tData(FieldItemData::eStructArrayPtr,
              tIter->_Name,tIter->_Type,tMatch);
          FieldItem *dataItem = new FieldItem(tData,aParentItem);
          aParentItem->appendChild(dataItem);

          buildTree(dataItem,tStruct,aStructBuilder,++aLevel);
          --aLevel;
        }
      }
    }
    // For struct field,
    // print type and name, then call this routine recursively to print the
    // contents of the struct.
    else if (!aStructBuilder->isPrimitive(tIter->_Type))
    {
//      DEBUG(sLogger,blanks[aLevel]);
      DEBUG(sLogger,blanks[aLevel] << "struct node: "
          << tIter->_Type << ":" << tIter->_Name);
      Structure *tStruct = aStructBuilder->getStructure(tIter->_Type);
      if (tStruct != NULL)
      {
        std::string tMatch = buildMatchForStructField(*tIter,aLevel);
        FieldItemData tData(FieldItemData::eStruct,
            tIter->_Name,tIter->_Type,tMatch);
        FieldItem *dataItem = new FieldItem(tData,aParentItem);
        aParentItem->appendChild(dataItem);

        buildTree(dataItem,tStruct,aStructBuilder,++aLevel);
        --aLevel;
      }
      else
      {
//        DEBUG(sLogger,blanks[aLevel]);
        ERROR(sLogger,blanks[aLevel] << "Can't find struct " << tIter->_Name);
      }
    }
    // For primitive type field,
    // print field type and name at current indent level.
    else
    {
//      DEBUG(sLogger,blanks[aLevel]);
      DEBUG(sLogger,blanks[aLevel] << "primitive node: "
          << tIter->_Type << ":" << tIter->_Name);
      std::string tMatch = buildMatchForPrimitiveField(*tIter,aLevel);
      FieldItemData tData(FieldItemData::ePrimitive,
          tIter->_Name,tIter->_Type,tMatch);
      FieldItem *dataItem = new FieldItem(tData,aParentItem);

      aParentItem->appendChild(dataItem);
    }
  }
#if 0
  for( it = tStructure->_Fields.begin(); it != tStructure->_Fields.end(); it++)
  {
    std::string tName = it->_Name;
    std::string tType = it->_Type;
    std::string tFieldString = aPrefix + "." + tName;
    std::string tFieldDotString = getDotString(tType,tFieldString);
    if( tFieldDotString.length() )
    {
      tReturn << tFieldDotString;
    }
    else
    {
      tReturn << tFieldString << endl;
    }
  }
#endif
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool DataStructModel::processLinesIn(std::vector<std::string> &aLinesIn,
                                     std::vector<std::string> &aLinesOut)
{
  vector<std::string>::iterator aLineIter = aLinesIn.begin();
  bool tSuccess = _TopNodeItem->processLines(aLinesIn,aLineIter,aLinesOut);
  if (tSuccess)
  {
    DEBUG(sLogger,"successfully processed " << _TopNodeItem->getData().getName());
  }
  else
  {
    ERROR(sLogger,"Processing: " << _TopNodeItem->getData().getName());
  }
  return tSuccess;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void DataStructModel::printInLines()
{
  _TopNodeItem->printInLines();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string DataStructModel::getDotString(StructorBuilder *aStructBuilder,
    std::string aName,std::string aPrefix)
{
  stringstream tReturn;

  Structure *tStructure = aStructBuilder->getStructure(aName);

  if (tStructure != NULL)
  {
    vector<Field>::iterator it;
    for( it = tStructure->_Fields.begin(); it != tStructure->_Fields.end(); it++)
    {
      std::string tName = it->_Name;
      std::string tType = it->_Type;
      std::string tFieldString = aPrefix + "." + tName;
      std::string tFieldDotString = getDotString(aStructBuilder,tType,tFieldString);
      if( tFieldDotString.length() )
      {
        tReturn << tFieldDotString;
      }
      else
      {
        tReturn << tFieldString << endl;
      }
    }
  }

  return tReturn.str();
}

//-------------------------------------------------------------------------------
// Not sure this will be used or what it was originally for.
//-------------------------------------------------------------------------------
std::string DataStructModel::getMatchString(FieldItem *aFieldItem)
{
  std::string tString;

  tString += aFieldItem->getData().getType() + ","
           + aFieldItem->getData().getMatch() + "\n";

  for (int i = 0; i < aFieldItem->childCount(); i++)
  {
    tString += getMatchString(aFieldItem->child(i));
  }

  return tString;
}

//-------------------------------------------------------------------------------
// Gets the match string for the simple reader.
//-------------------------------------------------------------------------------
std::string DataStructModel::getMatchString()
{
  std::string tMatchString(".*");

  std::vector<FieldItem *> tCheckedFieldItems = getCheckedFields();
  int tNumCheckFields = tCheckedFieldItems.size();
  if (tNumCheckFields > 0)
  {
    tMatchString.clear();
    bool tFirstMatchField = true;
    for (int tIdx = 0; tIdx < tNumCheckFields; tIdx++)
    {
      std::string tFieldMatch = tCheckedFieldItems[tIdx]->getData().getMatch();
      if (tFieldMatch.length() == 0)
      {
        continue;
      }

      if (tFirstMatchField == true)
      {
        tFirstMatchField = false;
      }
      else
      {
        tMatchString += "|";
      }
      tMatchString += tFieldMatch;
    }
  }
  DEBUG(sLogger,"Match string: " << tMatchString);
  return tMatchString;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
std::vector<FieldItem *> DataStructModel::getCheckedFields()
{
  std::vector<FieldItem *> tFieldItems;
  addCheckedFields(_TopNodeItem,tFieldItems);
  return tFieldItems;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void DataStructModel::addCheckedFields(
    FieldItem *aNode,std::vector<FieldItem *> &aFieldItems)
{
  if (aNode->getData().getCheckState() == Qt::Checked)
  {
    aFieldItems.push_back(aNode);
  }
  for (int tIdx = 0; tIdx < aNode->childCount(); tIdx++)
  {
    addCheckedFields(aNode->child(tIdx),aFieldItems);
  }
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QVariant DataStructModel::data(const QModelIndex &index,int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  FieldItem *item = static_cast<FieldItem*>(index.internalPointer());

  switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
      return item->data(index.column());
      break;
    case Qt::CheckStateRole:
      if (index.column() == eColFieldName)
      {
        return static_cast<int>(item->getData().getCheckState());
      }
      break;
    case Qt::FontRole:
      if( item->getData().getNodeType() == FieldItemData::eStructArrayPtr
       || item->getData().getNodeType() == FieldItemData::ePrimitiveArrayPtr)
      {
        return kArrayFont;
      }
      else if (
          item->parentItem()->getData().getNodeType()
                                    == FieldItemData::eStructArrayPtr
       || item->parentItem()->getData().getNodeType()
                                    == FieldItemData::ePrimitiveArrayPtr)
      {
        return kArrayFont;
      }
      break;
  }

  return QVariant();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool DataStructModel::setData(
    const QModelIndex &index,const QVariant &value,int role)
{
  int aRow = index.row();
  int aCol = index.column();

  if( role == Qt::CheckStateRole && aCol == eColFieldName)
  {
    FieldItem *item = static_cast<FieldItem*>(index.internalPointer());

    Qt::CheckState tNewState = Qt::PartiallyChecked;

    if (value == Qt::Checked)
    {
      tNewState = Qt::Checked;
    }
    else if (value == Qt::Unchecked)
    {
      tNewState = Qt::Unchecked;
    }

    item->setCheckState(tNewState);
    emit dataChanged(index,index);

    if (item->childCount() > 0)
    {
      setChildrenCheckStates(index,tNewState);
    }

    updateParentCheckState(index,tNewState);
  }
  else if (role == Qt::EditRole)
  {
    FieldItem *item = static_cast<FieldItem*>(index.internalPointer());
    if (aCol ==  eColMatchRegex)
    {
      item->setFieldMatch(value);
      emit dataChanged(index,index);
    }
    else if (aCol == eColPostfix)
    {
      item->setFieldPostfix(value);
      emit dataChanged(index,index);
    }
  }
  return true;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Qt::ItemFlags DataStructModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return 0;
  }

  Qt::ItemFlags tFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if (index.column() == eColFieldName)
  {
    tFlags |= Qt::ItemIsUserCheckable;
  }

  if (index.column() == eColMatchRegex || index.column() == eColPostfix)
  {
    tFlags |= Qt::ItemIsEditable;
  }

  return tFlags;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QVariant DataStructModel::headerData(int section,Qt::Orientation orientation,
      int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    if (section == eColFieldName)
    {
      return QVariant("Field Name");
    }
    else if (section == eColFieldType)
    {
      return QVariant("Field Type");
    }
    else if (section == eColMatchRegex)
    {
      return QVariant("Match Regex");
    }
    else if (section == eColPostfix)
    {
      return QVariant("Postfix");
    }
  }

  return QVariant();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QModelIndex DataStructModel::index(int row,int col,
      const QModelIndex &parent) const
{
  if (!hasIndex(row,col,parent))
  {
    return QModelIndex();
  }

  FieldItem *parentItem;

  if (!parent.isValid())
  {
    parentItem = _RootItem;
  }
  else
  {
    parentItem = static_cast<FieldItem*>(parent.internalPointer());
  }

  FieldItem *childItem = parentItem->child(row);
  if (childItem)
  {
    return createIndex(row,col,childItem);
  }
  else
  {
    return QModelIndex();
  }
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QModelIndex DataStructModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return QModelIndex();
  }

  FieldItem *childItem = static_cast<FieldItem *>(index.internalPointer());
  FieldItem *parentItem = childItem->parentItem();

  if (parentItem == _RootItem)
  {
    return QModelIndex();
  }

  return createIndex(parentItem->row(),0,parentItem);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int DataStructModel::rowCount(const QModelIndex &parent) const
{
  FieldItem *parentItem;
  if (parent.column() > 0) // what's purpose of this test?
  {
    return 0;
  }

  if (!parent.isValid())
  {
    parentItem = _RootItem;
  }
  else
  {
    parentItem = static_cast<FieldItem*>(parent.internalPointer());
  }

  return parentItem->childCount();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int DataStructModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return static_cast<FieldItem*>(parent.internalPointer())->columnCount();
  }
  else
  {
    return _RootItem->columnCount();
  }
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void DataStructModel::setChildrenCheckStates(
    const QModelIndex &aParentIndex,Qt::CheckState aNewState)
{
  FieldItem *aParentItem =
      static_cast<FieldItem*>(aParentIndex.internalPointer());

  int tChildCount = aParentItem->childCount();
  if (tChildCount == 0)
  {
    return;
  }

  for (int i = 0; i < tChildCount; i++)
  {
    FieldItem *aChild = aParentItem->child(i);
    aChild->setCheckState(aNewState);
    QModelIndex aChildIndex = index(i,0,aParentIndex);
    setChildrenCheckStates(aChildIndex,aNewState);
  }

  QModelIndex tFirstIndex = index(0,0,aParentIndex);
  QModelIndex tLastIndex = index(tChildCount-1,0,aParentIndex);
  emit dataChanged(tFirstIndex,tLastIndex);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void DataStructModel::updateParentCheckState(
    const QModelIndex &aChildIndex,Qt::CheckState aNewState)
{
  QModelIndex aParentIndex = parent(aChildIndex);

  if (!aParentIndex.isValid())
  {
    return;
  }

  FieldItem *tParentItem =
      static_cast<FieldItem*>(aParentIndex.internalPointer());

  bool tAllChildrenMatch = true;
  int tChildCount = tParentItem->childCount();
  for (int i = 0; i < tChildCount; i++)
  {
    FieldItem *tChild = tParentItem->child(i);
    if (tChild->getData().getCheckState() != aNewState)
    {
      tAllChildrenMatch = false;
      break;
    }
  }

  bool tChangedParent = false;
  if (tAllChildrenMatch)
  {
    if (tParentItem->getData().getCheckState() != aNewState)
    {
      tParentItem->setCheckState(aNewState);
      emit dataChanged(aParentIndex,aParentIndex);
      tChangedParent = true;
    }
  }
  else
  {
    DEBUG(sLogger,"updateParentCheckState(): all children DO NOT match");
    if (tParentItem->getData().getCheckState() != Qt::PartiallyChecked)
    {
      tParentItem->setCheckState(Qt::PartiallyChecked);
      emit dataChanged(aParentIndex,aParentIndex);
      tChangedParent = true;
    }
  }

  // If we've changed the check state of the parent, then recursively call
  // this function on the parent of the parent.
  if (tChangedParent)
  {
    updateParentCheckState(aParentIndex,aNewState);
  }
}
