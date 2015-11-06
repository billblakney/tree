#ifndef FIELDITEMDATA_HH_
#define FIELDITEMDATA_HH_

#include <string>
#include <QCheckBox>
//#include <QCheckState>
#include "Logger.hh"

/**
 * FieldItemData represents a node in the data structure tree.
 */
class FieldItemData
{
public:

  enum NodeType {eNone, eRoot, ePrimitive, eStruct, ePrimitiveArrayPtr,
    eStructArrayPtr};

  FieldItemData();

  FieldItemData(
      NodeType aNodeType,
      Qt::CheckState aCheckState,
      std::string aName,
      std::string aType,
      std::string aMatch,
      std::string aPostfix);

  virtual ~FieldItemData();

protected:

//  static ccl::Logger sLogger;

  NodeType           _NodeType;
  Qt::CheckState     _CheckState;
  std::string        _Name;
  std::string        _Type;
  std::string        _Match;
  std::string        _Postfix;
};

#endif /* FIELDITEMDATA_HH_ */
