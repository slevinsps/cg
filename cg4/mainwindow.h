#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_pushButton_clicked();

    void on_pushButton_color_line_clicked();

    void on_pushButton_color_fone_clicked();

    void on_pushButton_2_clicked();

    //void on_pushButton_3_clicked();

    //void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_one_circle_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_k_c_clicked();

    void on_pushButton_k_e_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
};

#include <QVBoxLayout>
#include <QPushButton>
#include "qcustomplot.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

private:
    QCustomPlot *customPlot;
    QVBoxLayout *vbox;
    QPushButton *res;
    QCPBars *fossil;


private slots:
    //void rndres();
};



class Widget_graf : public QWidget
{
    Q_OBJECT

public:
    explicit Widget_graf(QWidget *parent = 0);

private:
    QCustomPlot *customPlot;
    QVBoxLayout *vbox;
    QPushButton *res;
    QCPBars *fossil;

};


class Widget_graf_ellipse : public QWidget
{
    Q_OBJECT

public:
    explicit Widget_graf_ellipse(QWidget *parent = 0);

private:
    QCustomPlot *customPlot;
    QVBoxLayout *vbox;
    QPushButton *res;
    QCPBars *fossil;

};
#endif // MAINWINDOW_H
