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
      Qt::CheckState aCheckState,
      std::string aName,
      std::string aType,
      std::string aMatch,
      std::string aPostfix)
  : _NodeType(aNodeType),
    _CheckState(aCheckState),
    _Name(aName),
    _Type(aType),
    _Match(aMatch),
    _Postfix(aPostfix)
  {

  }

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
FieldItemData::~FieldItemData()
{
}
