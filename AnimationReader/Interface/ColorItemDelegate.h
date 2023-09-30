#ifndef COLORITEMDELEGATE_H
#define COLORITEMDELEGATE_H
#include <QItemDelegate>
#include <QPainter>
#include <QDebug>
#include "Interface/TypeHandler.h"
#include <cmath>
#pragma once

class ColorItemDelegate : public QItemDelegate
{
public:
    ColorItemDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    void DisplayUInt64(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(98, 230, 232));
         if (!type.isValid() || type.isNull()){ return;}
         uint64_t* value = qvariant_cast<uint64_t *>(type);
         *tableDisplayString = "\""+QString::number(*value,16)+"\"";
    }
    void DisplayBool(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(242,138,2));
         if (!type.isValid() || type.isNull()){ return;}
         bool* value = qvariant_cast<bool *>(type);
         *tableDisplayString = bool(*value) ? "true" : "false";
    }
    void DisplayFloat(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(219, 106, 134));
         if (!type.isValid() || type.isNull()){ return;}
         double value = static_cast<double> ((float)*qvariant_cast<float *>(type));
         *tableDisplayString = (std::floor(value) == value) ?
                     QString::number(value,'f',2) : QString::number(value,'f');
    }
    void DisplayUInt32(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(52, 171, 241));
         if (!type.isValid() || type.isNull()){ return;}
         uint32_t* value = qvariant_cast<uint32_t *>(type);
         *tableDisplayString = QString::number(*value);
    }
    void DisplayUInt16(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(52, 171, 241));
         if (!type.isValid() || type.isNull()){ return;}
         uint16_t* value = qvariant_cast<uint16_t *>(type);
         *tableDisplayString = QString::number(*value);
    }
    void DisplayInt32(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(52, 171, 241));
         if (!type.isValid() || type.isNull()){ return;}
         int32_t* value = qvariant_cast<int32_t *>(type);
         *tableDisplayString = QString::number(*value);
    }
    void DisplayQString(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(105, 221, 166));
         if (!type.isValid() || type.isNull()){ return;}
         QString* value = qvariant_cast<QString*>(type);
         *tableDisplayString = QString(*value);
    }
    void DisplayStdStringPtr(QString* tableDisplayString, QVariant type, QPainter *painter) const{
         painter->setPen(QColor(105, 221, 166));
         if (!type.isValid() || type.isNull()){ return;}
         std::string* value = qvariant_cast<std::string *>(type);
         *tableDisplayString = QString::fromStdString(*value);
    }
    void DisplayStdString(QString* tableDisplayString, QVariant type, QPainter *painter) const{
        painter->setPen(QColor(105, 221, 166));
        if (!type.isValid() || type.isNull()){ return;}
        std::string value = qvariant_cast<std::string>(type);
        *tableDisplayString = QString::fromStdString(value);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
        {
            painter->save();
            painter->setPen(Qt::NoPen);
            painter->drawRect(option.rect);
            painter->setFont(QFont("Microsoft Tai Le",8));
            painter->setPen(QColor(52, 171, 241));
            QString itemString;

            // Color Type
            switch (index.data(Qt::UserRole).toUInt())
            {
                case(0x1):
                    DisplayUInt64(&itemString,index.data(Qt::UserRole+21),painter);
                    break;
                case(0x2):
                    DisplayBool(&itemString,index.data(Qt::UserRole+22),painter);
                    break;
                case(0x3):
                    DisplayFloat(&itemString,index.data(Qt::UserRole+23),painter);
                    break;
                case(0x4):
                    DisplayUInt32(&itemString,index.data(Qt::UserRole+24),painter);
                    break;
                case(0x5):
                    DisplayUInt16(&itemString,index.data(Qt::UserRole+25),painter);
                    break;
                case(0x6):
                    DisplayInt32(&itemString,index.data(Qt::UserRole+26),painter);
                    break;
                case(0x7):
                    DisplayUInt32(&itemString,index.data(Qt::UserRole+27),painter);
                    break;
                case(0x8):
                    DisplayStdStringPtr(&itemString,index.data(Qt::UserRole+28),painter);
                    break;
                case(0x9):
                    DisplayStdString(&itemString,index.data(Qt::UserRole+29),painter);
                    break;
                default:
                    break;
            }


            QStyleOptionViewItem *newViewItem = new QStyleOptionViewItem();
            newViewItem->rect = option.rect;
            newViewItem->rect.setTop(newViewItem->rect.top()+7);
            painter->drawText(newViewItem->rect, itemString);
            painter->restore();
            delete newViewItem;
        }
};

#endif // COLORITEMDELEGATE_H
