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

typedef struct edges_s
{
    int x1;
    int y1;
    int x2;
    int y2;
}edges;

typedef struct point_str_s
{
    int x;
    int y;
}point_str;

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyQGraphicsView(QWidget *parent = 0);
    QPointF my_point;
    void draw_borders(void);
    void draw_line(QPixmap pix);
    void ots_rib(int i);


signals:
    void send_point(edges m_point);


public slots:
    void short_fig();
    void ots_fig();
    void clear_fig();
    void add_point_button(int x1, int y1, int x2, int y2);
    void mousePressEvent(QMouseEvent * e);
    void check_checkBox_zatr_flag();
    void check_checkBox_circle();
    void check_checkBox_line();
    void add_otsek(int, int);
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
    void get_point(edges m_point);

private slots:

    void on_pushButton_clear_clicked();

    void on_pushButton_add_clicked();

    //void on_pushButton_fill_2_clicked();

    void on_pushButton_color_line_clicked();

    void on_pushButton_color_fone_clicked();

    void on_pushButton_add_2_clicked();


    //void on_pushButton_add_4_clicked();

    void on_pushButton_color_fone_2_clicked();

    void on_pushButton_clicked();

signals:
    void short_fig_sig();
    void ots_fig_sig();
    void clear_fig_sig();
    void add_point_button_sig(int x1, int y1, int x2, int y2);
    void add_otsek_sig(int, int);
    void check_checkBox_zatr_flag_sig();
    void check_checkBox_circles_sig();
    void check_checkBox_line_sig();


private:
    QShortcut *keyF11;
    Ui::MainWindow *ui;

};







#endif // MAINWINDOW_H
