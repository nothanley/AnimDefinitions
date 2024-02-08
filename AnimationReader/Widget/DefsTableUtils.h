#include "DefsReader.h"
#pragma once

class QVariant;
class QString;

class DefsTableUtils{

public:

    static QVariant GetItemVariant( StateNode::KeyValueProp* kvElement);
    static QString FormatKeyValueString( const StateNode::KeyValueProp kvElement, bool isolateValue=false);

};
