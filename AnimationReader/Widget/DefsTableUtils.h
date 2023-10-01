#ifndef DEFSTABLEUTILS_H
#define DEFSTABLEUTILS_H

#include <QString>
#include <QTableWidget>
#include "AnimDefinitions/AnimDefHandler.h"
#pragma once

static QVariant GetItemVariant( StateNode::KeyValueProp* kvElement)
{
    QString tableString = "";
    int emptyType = -1;
    QVariant variant = emptyType;

    switch (ntohl(kvElement->streamKey)) {
        case (0x46C0C6BF):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0xA8FA930E):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0x4D245F2B):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0x6782648B):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0xDF4B0B8E):
            if (kvElement->val64 == 0x0){ return QVariant::fromValue(&kvElement->chars);}
            return QVariant::fromValue(&kvElement->val64);
            break;
        case (0x60C09DC4):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0xA6D0A4EB):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0x5D651EEF):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0x4425DADA):
            return QVariant::fromValue(&kvElement->val32);
            break;
        case (0x45991D01):
            return QVariant::fromValue(&kvElement->chars);
            break;
        case (0x48DE1308):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0x73EDDE33):
            return QVariant::fromValue(&kvElement->val64);
            break;
        case (0x0C3F823A):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0x654680ED):
            return QVariant::fromValue(&kvElement->val32);
            break;
        case (0xC248BCF4 ):
            return QVariant::fromValue(&kvElement->flag);
            break;
        case (0x8AE38AFE):
            return QVariant::fromValue(&kvElement->val32);
            break;
        case (0x8E0B4BDF):
            return QVariant::fromValue(&kvElement->val64);
            break;
        case (0xBE83F7FD):
            return QVariant::fromValue(&kvElement->chars);
            break;
        case (0x78D548C7):
            return QVariant::fromValue(&kvElement->val32);
            break;
        case (0xC8B22C04):
            return QVariant::fromValue(&kvElement->valFloat);
            break;
        case (0x19981A6F):
            return QVariant::fromValue(&kvElement->valFloat);
            break;
        case (0x9E3F108C):
            return QVariant::fromValue(&kvElement->chars);
            break;
        case (0x4C980DCF):
            return QVariant::fromValue(&kvElement->valFloat);
            break;
        case (0xE39D3DA5):
            return QVariant::fromValue(&kvElement->valFloat);
            break;
        case (0x0A2E3AF4):
            return QVariant::fromValue(&kvElement->flag);
            break;
        default:
            break;
    }

    return variant;
}


static QString FormatKeyValueString( const StateNode::KeyValueProp kvElement, bool isolateValue=false)
{
    QString tableString = "";
    QString keyString = QString::number(kvElement.streamKey,16).toUpper();
    QString propertyString;

    switch (ntohl(kvElement.streamKey)) {
        case (0x46C0C6BF):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0xA8FA930E):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0x4D245F2B):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0x6782648B):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0xDF4B0B8E):
            propertyString = "\"" + QString::number(kvElement.val64,16) + "\"";
            break;
        case (0x60C09DC4):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0xA6D0A4EB):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0x5D651EEF):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0x4425DADA):
            propertyString = QString::number(kvElement.val32);
            break;
        case (0x45991D01):
            propertyString = "\"" + QString(kvElement.chars.c_str()) + "\"";
            break;
        case (0x48DE1308):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0x73EDDE33):
            propertyString = "\"" + QString::number(kvElement.val64,16) + "\"";
            break;
        case (0x0C3F823A):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0x654680ED):
            propertyString = QString::number(kvElement.val32);
            break;
        case (0xC248BCF4 ):
            propertyString = kvElement.flag ? "true" : "false";
            break;
        case (0x8AE38AFE):
            propertyString = QString::number(kvElement.val32);
            break;
        case (0x8E0B4BDF):
            propertyString = "\"" + QString::number(kvElement.val64,16) + "\"";
            break;
        case (0xBE83F7FD):
            propertyString = "\"" + QString(kvElement.chars.c_str()) + "\"";
            break;
        case (0x78D548C7):
            propertyString = QString::number(kvElement.val32);
            break;
        case (0xC8B22C04):
            propertyString = QString::number( kvElement.valFloat );
            break;
        case (0x19981A6F):
            propertyString = QString::number( kvElement.valFloat );
            break;
        case (0x9E3F108C):
            propertyString = "\"" + QString(kvElement.chars.c_str()) + "\"";
            break;
        case (0x4C980DCF):
            propertyString = QString::number( kvElement.valFloat );
            break;
        case (0xE39D3DA5):
            propertyString = QString::number( kvElement.valFloat );
            break;
        case (0x0A2E3AF4):
            propertyString = kvElement.flag ? "true" : "false";
            break;

        default:
            break;
    }



    tableString = "\"" + keyString + "\" :";// : " + propertyString;

    if (isolateValue){ tableString = propertyString; }
    return tableString;
}

#endif // DEFSTABLEUTILS_H
