#ifndef QPAINTWIDGET_H
#define QPAINTWIDGET_H
#include "structs.h"
#include <QWidget>
class QPaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPaintWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // QPAINTWIDGET_H
