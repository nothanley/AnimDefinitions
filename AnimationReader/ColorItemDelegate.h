#ifndef COLORITEMDELEGATE_H
#define COLORITEMDELEGATE_H
#include <QItemDelegate>
#include <QPainter>
#include <QDebug>
#pragma once

class ColorItemDelegate : public QItemDelegate
{
public:
    ColorItemDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
        {
            painter->save();
            painter->setPen(Qt::NoPen);
            painter->drawRect(option.rect);
            painter->setFont(QFont("Microsoft Tai Le",8));
            painter->setPen(QColor(52, 171, 241));

            switch (index.data(Qt::UserRole).toUInt())
            {
                case(0x0):
                    painter->setPen(QColor(52, 171, 241));
                    break;
                case(0x1):
                    painter->setPen(QColor(98, 230, 232));
                    break;
                case(0x2):
                    painter->setPen(QColor(242,138,2));
                    break;
                case(0x3):
                    break;
                default:
                    break;
            }

            QStyleOptionViewItem *newViewItem = new QStyleOptionViewItem();
            newViewItem->rect = option.rect;
            newViewItem->rect.setTop(newViewItem->rect.top()+7);

            QString filterString = index.data().toString().remove("\"");
            painter->drawText(newViewItem->rect, filterString);
            painter->restore();
            delete newViewItem;
        }
};

#endif // COLORITEMDELEGATE_H
