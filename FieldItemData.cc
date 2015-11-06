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
