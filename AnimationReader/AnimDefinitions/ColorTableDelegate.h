#ifndef COLORTABLEDELEGATE_H
#define COLORTABLEDELEGATE_H
#include <QItemDelegate>

class ColorTableDelegate : public QItemDelegate
{
public:
    ColorTableDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        // Copy the original style option and set the text color to purple
        QStyleOptionViewItem modifiedOption = option;
        modifiedOption.palette.setColor(QPalette::Text, QColorConstants::Svg::purple);

        // Call the base class paint method with the modified option
        QItemDelegate::paint(painter, modifiedOption, index);
    }
};

#endif // COLORTABLEDELEGATE_H
