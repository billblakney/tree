#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <QVariant>
#include "DataStructModel.hh"

QFont DataStructModel::kArrayFont;

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
DataStructModel::DataStructModel(
    Structure *aStructure,StructorBuilder *aStructBuilder)
{
  kArrayFont.setItalic(true);

  // root item
  QList<QVariant> aRootData =
      buildDataList(aStructure->_Name,aStructure->_Name);
  _RootItem = new FieldItem(FieldItem::eRoot,aRootData);

  // top node item
  QList<QVariant> aTopNodeData =
      buildDataList("struct",aStructure->_Name,"struct");
  FieldItem *topNodeItem =
      new FieldItem(FieldItem::eRoot,aTopNodeData,_RootItem);

  _RootItem->appendChild(topNodeItem);

  buildTree(topNodeItem,aStructure,aStructBuilder);

cout << getDotString(aStructBuilder,aStructure->_Name,"aStruct") << endl;
cout << getMatchString(topNodeItem) << endl;
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
    std::cout << "root node: " << aStructure->_Name << std::endl;
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
      std::string tName = what[1];
      std::cout << blanks[aLevel];
      std::cout << "array size entry: "
          << tIter->_Type << ":" << tIter->_Name << std::endl;
    }
    else if (tIter->_IsPointer)
    {
      // For primitive pointer,
      // print type and name.
      if (aStructBuilder->isPrimitive(tIter->_Type))
      {
        QList<QVariant> data =
            buildDataList(tIter->_Name + "[]",tIter->_Type);

        FieldItem *dataItem = new FieldItem(
            FieldItem::ePrimitiveArrayPtr,data,aParentItem);

        aParentItem->appendChild(dataItem);

        std::cout << blanks[aLevel];
        std::cout << "primitive array node: "
            << tIter->_Type << ":" << tIter->_Name << std::endl;
      }
      // For struct pointer,
      // print type and name, increment level, and call this routine
      // recursively to print the contents of the struct.
      else
      {
        std::cout << blanks[aLevel];
        std::cout << "struct array node: "
            << tIter->_Type << ":" << tIter->_Name << std::endl;
        Structure *tStruct = aStructBuilder->getStructure(tIter->_Type);
        if (tStruct != NULL)
        {
          QList<QVariant> data =
              buildDataList(tIter->_Name + "[]",tIter->_Type);

          FieldItem *dataItem = new FieldItem(
              FieldItem::eStructArrayPtr,data,aParentItem);

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
      std::cout << blanks[aLevel];
      std::cout << "struct node: "
          << tIter->_Type << ":" << tIter->_Name << std::endl;
      Structure *tStruct = aStructBuilder->getStructure(tIter->_Type);
      if (tStruct != NULL)
      {
        QList<QVariant> data =
            buildDataList(tIter->_Name,tIter->_Type);

        FieldItem *dataItem = new FieldItem(
            FieldItem::eStruct,data,aParentItem);

        aParentItem->appendChild(dataItem);

        buildTree(dataItem,tStruct,aStructBuilder,++aLevel);
        --aLevel;
      }
      else
      {
        std::cout << blanks[aLevel];
        std::cout << "ERROR: can't find struct " << tIter->_Name << std::endl;
      }
    }
    // For primitive type field,
    // print field type and name at current indent level.
    else
    {
      std::cout << blanks[aLevel];
      std::cout << "primitive node: "
          << tIter->_Type << ":" << tIter->_Name << std::endl;

      QList<QVariant> data =
          buildDataList(tIter->_Name,tIter->_Type,tIter->_Name + ":");

      FieldItem *dataItem = new FieldItem(
          FieldItem::ePrimitive,data,aParentItem);

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
QList<QVariant> DataStructModel::buildDataList(
    const std::string aName,const std::string aType,const std::string aMatch)
{
  QList<QVariant> tList;

  tList.append(QVariant(aName.c_str()));
  tList.append(QVariant(aType.c_str()));
  tList.append(QVariant(aMatch.c_str()));

  return tList;
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
//-------------------------------------------------------------------------------
std::string DataStructModel::getMatchString(FieldItem *aFieldItem)
{
  std::string tString;

  tString += aFieldItem->getFieldType() + ","
           + aFieldItem->getFieldMatch() + "\n";

  for (int i = 0; i < aFieldItem->childCount(); i++)
  {
    tString += getMatchString(aFieldItem->child(i));
  }

  return tString;
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
    case Qt::DisplayRole:
      return item->data(index.column());
      break;
    case Qt::CheckStateRole:
      if (index.column() == 0)
      {
        return static_cast<int>(item->getCheckState());
      }
      break;
    case Qt::FontRole:
      if( item->getType() == FieldItem::eStructArrayPtr
       || item->getType() == FieldItem::ePrimitiveArrayPtr)
      {
        return kArrayFont;
      }
      else if (
          item->parentItem()->getType() == FieldItem::eStructArrayPtr
       || item->parentItem()->getType() == FieldItem::ePrimitiveArrayPtr)
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
  if( role == Qt::CheckStateRole && index.column() == 0)
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

  if (index.column() == 0)
  {
    tFlags |= Qt::ItemIsUserCheckable;
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
    if (section == 0)
    {
      return QVariant("Field Name");
    }
    else if (section == 1)
    {
      return QVariant("Field Type");
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
  if (parent.column() > 0)
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
    if (tChild->getCheckState() != aNewState)
    {
      tAllChildrenMatch = false;
      break;
    }
  }

  bool tChangedParent = false;
  if (tAllChildrenMatch)
  {
    if (tParentItem->getCheckState() != aNewState)
    {
      tParentItem->setCheckState(aNewState);
      emit dataChanged(aParentIndex,aParentIndex);
      tChangedParent = true;
    }
  }
  else
  {
    std::cout << "updateParentCheckState(): all children DO NOT match" << std::endl;
    if (tParentItem->getCheckState() != Qt::PartiallyChecked)
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
