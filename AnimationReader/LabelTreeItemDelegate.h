#ifndef LABELTREEITEMDELEGATE_H
#define LABELTREEITEMDELEGATE_H

#include <QTreeWidget>
#include <QPainter>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QTreeWidgetItem>
#include "DefsTreeWidgetItem.h"
#include <QApplication>
#pragma once

class LabelTreeItemDelegate : public QStyledItemDelegate
{
public:
    LabelTreeItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyleOptionViewItem opt = option;
        this->initStyleOption(&opt, index);

        const QWidget *widget = option.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        painter->setBrush(QColor(214, 46, 255,0));
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

        int squareSize = 38; // Adjust this size as needed
        int margin = 14;     // Adjust the margin as needed
        QRect itemRect = option.rect;
        QRect squareRect = itemRect.adjusted(margin, margin*1.25, -itemRect.width() + squareSize + margin, -margin*1.25);

        // Set the painter to draw the red square
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(214, 46, 255));

        // Access the custom attribute from the QTreeWidgetItem
        QString labelText = "DEFN";
        switch (index.data(Qt::UserRole+1).toUInt())
        {
            case(STAT):
                labelText = "STAT";
                painter->setBrush(QColor(86, 2, 173));
                break;
            case(NODE):
                labelText = "NODE";
                painter->setBrush(QColor(79, 87, 245));
                break;
            case(SYNC):
                labelText = "SYNC";
                painter->setBrush(QColor(71, 194, 192));
                break;
            case(DTT_):
                labelText = "DTT_";
                painter->setBrush(QColor(245, 135, 0));
                break;
            case(TOVR):
                labelText = "TOVR";
                painter->setBrush(QColor(60, 140, 60));
                break;
            case(DESC):
                labelText = "DESC";
                painter->setBrush(QColor(81, 104, 150));
                break;
            case(EVNT):
                labelText = "EVNT";
                painter->setBrush(QColor(120, 200, 150));
                break;
            case(GRP_):
                labelText = "GRP_";
                painter->setBrush(QColor(174, 76, 174));
                break;
            case(MEMB):
                labelText = "MEMB";
                painter->setBrush(QColor(160, 160, 160));
                break;
            case(SELS):
                labelText = "SELS";
                painter->setBrush(QColor(160, 160, 160));
                break;
            case(FRMS):
                labelText = "FRMS";
                painter->setBrush(QColor(160, 160, 160));
                break;
            case(CAND):
                labelText = "CAND";
                painter->setBrush(QColor(160, 160, 160));
                break;
            default:
                break;
        }

        int cornerRadius = 1; // Adjust the corner radius as needed
        painter->drawRoundedRect(squareRect, cornerRadius, cornerRadius);

        // Set the painter to draw the Item text
        painter->setFont(QFont("Microsoft YaHei UI",8,55));
        painter->setPen( QColor(170, 190, 225) ); // You can change the text color as needed
        QRect textRect = itemRect.adjusted(squareSize + 1.75 * margin, 0, 0, 0); // Adjust the padding as needed
        painter->drawText(textRect, Qt::AlignVCenter, index.data().toString() );

        // Set the painter to draw the "dummy" text
        painter->setPen(Qt::white); // You can change the text color as needed
        QRect labelRect = itemRect.adjusted( squareSize - 20, -2, 0, 0); // Adjust the padding as needed
        painter->setFont(QFont("Microsoft YaHei UI",7,61));
        painter->drawText(labelRect, Qt::AlignVCenter, labelText );

        // Draw the item's text after the square
        QRect textClipRect = option.rect.adjusted(squareSize + 2 * margin, 0, 0, 0); // Clip the text to avoid overlapping
        painter->setClipRect(textClipRect);
        painter->restore();
    }

    void setTreeWidget(QTreeWidget* treePtr){ this->m_TreeWidget = treePtr; }

private:
    QTreeWidget* m_TreeWidget = nullptr;
};

#endif // LABELTREEITEMDELEGATE_H
