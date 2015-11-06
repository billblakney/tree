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
      std::string aName,
      std::string aType,
      std::string aMatch = "",
      std::string aPostfix = "newline (\"\\n\")",
      Qt::CheckState aCheckState = Qt::Unchecked);

  virtual ~FieldItemData();
  Qt::CheckState getCheckState() const;
  void SetCheckState(Qt::CheckState checkState);
  const std::string& getMatch() const;
  void SetMatch(const std::string& match);
  const std::string& getName() const;
  void SetName(const std::string& name);
  FieldItemData::NodeType getNodeType() const;
  void SetNodeType(NodeType nodeType);
  const std::string& getPostfix() const;
  void SetPostfix(const std::string& postfix);
  const std::string& getType() const;
  void SetType(const std::string& type);

//protected: TODO

//  static ccl::Logger sLogger;

  NodeType           _NodeType;
  Qt::CheckState     _CheckState;
  std::string        _Name;
  std::string        _Type;
  std::string        _Match;
  std::string        _Postfix;
};

#endif /* FIELDITEMDATA_HH_ */
