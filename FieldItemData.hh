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
  const std::string& getMatch() const;
  const std::string& getName() const;
  FieldItemData::NodeType getNodeType() const;
  const std::string& getPostfix() const;
  const std::string& getType() const;

  void setCheckState(Qt::CheckState checkState);
  void setMatch(const std::string& match);
  void setName(const std::string& name);
  void setNodeType(NodeType nodeType);
  void setPostfix(const std::string& postfix);
  void setType(const std::string& type);

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
