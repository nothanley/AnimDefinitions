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

            if(index.data().toString().contains("true") || index.data().toString().contains("false"))
            {
                painter->setPen(QColor(242,138,2));
            }
            else if(!index.data().toString().contains("\"")){
                painter->setPen(QColor(52, 171, 241));
            }
            else{
                painter->setPen(QColor(98, 230, 232));
            }

            QStyleOptionViewItem *newViewItem = new QStyleOptionViewItem();
            newViewItem->rect = option.rect;
            newViewItem->rect.setTop(newViewItem->rect.top()+7);

            QString filterString = index.data().toString().remove("\"");
            painter->drawText(newViewItem->rect, filterString);
            painter->restore();
        }
};

#endif // COLORITEMDELEGATE_H
