#include <iostream>
#include "FieldItemData.hh"

//ccl::Logger FieldItemData::sLogger("FieldItemData");

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItemData::FieldItemData()
  : _NodeType(eNone),
    _CheckState(Qt::Unchecked),
    _Name(""),
    _Type(""),
    _Match(""),
    _Postfix("")
{
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItemData::FieldItemData(
      NodeType aNodeType,
      std::string aName,
      std::string aType,
      std::string aMatch,
      std::string aPostfix,
      Qt::CheckState aCheckState)
  : _NodeType(aNodeType),
    _Name(aName),
    _Type(aType),
    _Match(aMatch),
    _Postfix(aPostfix),
    _CheckState(aCheckState)
  {

  }

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItemData::~FieldItemData()
{
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

Qt::CheckState FieldItemData::getCheckState() const
{
  return _CheckState;
}

void FieldItemData::setCheckState(Qt::CheckState checkState)
{
  _CheckState = checkState;
}

const std::string& FieldItemData::getMatch() const
{
  return _Match;
}

void FieldItemData::setMatch(const std::string& match)
{
  _Match = match;
}

const std::string& FieldItemData::getName() const
{
  return _Name;
}

void FieldItemData::setName(const std::string& name)
{
  _Name = name;
}

FieldItemData::NodeType FieldItemData::getNodeType() const
{
  return _NodeType;
}

void FieldItemData::setNodeType(NodeType nodeType)
{
  _NodeType = nodeType;
}

const std::string& FieldItemData::getPostfix() const
{
  return _Postfix;
}

void FieldItemData::setPostfix(const std::string& postfix)
{
  _Postfix = postfix;
}

const std::string& FieldItemData::getType() const
{
  return _Type;
}

void FieldItemData::setType(const std::string& type)
{
  _Type = type;
}
