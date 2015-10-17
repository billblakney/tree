#include <sstream>
#include <boost/regex.hpp>
#include <QVariant>
#include "DataStructModel.hh"

QFont DataStructModel::kArrayFont;

DataStructModel::DataStructModel()
{
  QList<QVariant> rootData;
  QVariant tRootVariant("Root");
  rootData.append(tRootVariant);
  rootItem = new FieldItem(FieldItem::eRoot,rootData);

  QList<QVariant> data1;
  QVariant data1Variant("data1");
  data1.append(data1Variant);
  FieldItem *data1Item = new FieldItem(FieldItem::eStruct,data1,rootItem);
  rootItem->appendChild(data1Item);

  QList<QVariant> data2;
  QVariant data2Variant("data2");
  data2.append(data2Variant);
  FieldItem *data2Item = new FieldItem(FieldItem::ePrimitive,data2,rootItem);
  rootItem->appendChild(data2Item);

  QList<QVariant> data3;
  QVariant data3Variant("data1a");
  data3.append(data3Variant);
  FieldItem *data3Item = new FieldItem(FieldItem::ePrimitive,data3,data1Item);
  data1Item->appendChild(data3Item);

}

DataStructModel::DataStructModel(
    Structure *aStructure,StructorBuilder *aStructBuilder)
{
  kArrayFont.setItalic(true);

  QList<QVariant> rootData;
  QVariant tRootVariant(aStructure->_Name.c_str());
  rootData.append(tRootVariant);
  rootItem = new FieldItem(FieldItem::eRoot,rootData);

  buildTree(rootItem,aStructure,aStructBuilder);
}

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
        QList<QVariant> data;
        std::string tPointerItemString(tIter->_Name + "[]");
        QVariant dataVariant(tPointerItemString.c_str());
        data.append(dataVariant);
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
          QList<QVariant> data;
          std::string tPointerItemString(tIter->_Name + "[]");
          QVariant dataVariant(tPointerItemString.c_str());
          data.append(dataVariant);
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
        QList<QVariant> data;
        QVariant dataVariant(tIter->_Name.c_str());
        data.append(dataVariant);
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

      QList<QVariant> data;
      QVariant dataVariant(tIter->_Name.c_str());
      data.append(dataVariant);
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
Qt::ItemFlags DataStructModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return 0;
  }

  return QAbstractItemModel::flags(index);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
QVariant DataStructModel::headerData(int section,Qt::Orientation orientation,
      int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    return rootItem->data(section);
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
    parentItem = rootItem;
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

  if (parentItem == rootItem)
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
    parentItem = rootItem;
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
    return rootItem->columnCount();
  }
}

