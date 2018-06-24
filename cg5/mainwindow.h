#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
namespace Ui {
class MainWindow;
//class MyQGraphicsView;
}

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QShortcut>

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyQGraphicsView(QWidget *parent = 0);
    QPointF my_point;
    void draw_borders(void);
    void draw_line(QPixmap pix);


signals:
    void send_point(QPointF m_point);


public slots:
    void short_fig();
    void fill_fig();
    void fill_zfig();
    void clear_fig();
    void add_point_button(int x, int y);
    void mousePressEvent(QMouseEvent * e);
    void check_checkBox();


private:

    QGraphicsScene * scene;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    MyQGraphicsView *mgw;
    ~MainWindow();

public slots:
    void get_point(QPointF m_point);

private slots:
    void on_pushButton_fill_clicked();

    void on_pushButton_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_fill_2_clicked();

    void on_pushButton_color_line_clicked();

    void on_pushButton_color_fone_clicked();

signals:
    void short_fig_sig();
    void fill_fig_sig();
    void fill_zfig_sig();
    void clear_fig_sig();
    void add_point_button_sig(int x, int y);

private:
    QShortcut *keyF11;
    Ui::MainWindow *ui;

};







#endif // MAINWINDOW_H
