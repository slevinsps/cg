#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "math.h"
#include "glwidget.h"
#include "time.h"

#include "QStandardItemModel"
#include "QStandardItem"
#include <QMessageBox>
#include <QColorDialog>

#include <stdio.h>    // Заголовочный файл для стандартной библиотеки ввода/вывода
#include <iostream>
#include <gl\gl.h>    // Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>   // Заголовочный файл для библиотеки GLu32
//#include <gl\glaux.h> // Заголовочный файл для библиотеки GLaux
#include <math.h>     // Заголовочный файл для математической библиотеки ( НОВОЕ )
#include <stdarg.h>   // Заголовочный файл для функций для работы с переменным
#include "structs.h"
#include "draw_control.h"


options_s  options;
QColor color_fone;
QColor color_line;
QColor color_line2;
std::vector<options_s> opt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->pushButton_color_line->setStyleSheet( " background-color: black;" );
    ui->pushButton_color_fone->setStyleSheet( " background-color: white;" );
    color_fone = QColor("white");
    color_line = QColor("black");
    color_line2 = QColor("black");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_color_line_clicked()
{
    QColor color = QColorDialog::getColor(color_line, this, "Choose color" );
    if(color.isValid())
    {
        color_line = color.name();
        QString qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_color_line->setStyleSheet(qss);
    }

}

void MainWindow::on_pushButton_color_fone_clicked()
{
    QColor color = QColorDialog::getColor(color_fone, this, "Choose color" );
    if(color.isValid())
    {
        color_fone = color.name();
        QString qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_color_fone->setStyleSheet(qss);

    }

}

void MainWindow::on_pushButton_2_clicked()
{
    opt.clear();
    ui->widget->update();
}

#include "qcustomplot.h"

double time_cda;
double time_br_d;
double time_br_i;

/*
Widget::Widget(QWidget *parent) :
    QWidget(parent)
{

    resize(500,400);
    setWindowTitle(QString::fromUtf8("Сравнение быстродействия алгоритмов"));
    customPlot = new QCustomPlot(this);
    vbox = new QVBoxLayout(this);


    vbox->addWidget(customPlot);
    res = new QPushButton(QString::fromUtf8("Показать или обновить гистограмму"),this);
    vbox->addWidget(res);
    setLayout(vbox);

    // Сигналы
    connect(res,SIGNAL(clicked()),this,SLOT(rndres()));
    this->rndres();

}



qint64 max_zn(std::vector<qint64> array)
{
    qint64 max = array[0];
    for (int i = 1; i < array.size(); i++)
    {
        if (max < array[i])
            max = array[i];
    }
    return max;
}

void Widget::rndres()
{

    //srand(time(NULL));
    customPlot->removePlottable(fossil);
    QElapsedTimer timer;
    double angle = 30;
    QColor color = "black";
    //double tb = tick();
    std::vector<qint64> array(10);
    array[0] = 0;
    array[1] = 0;
    array[2] = 0;
    array[3] = 0;
    array[4] = 0;
    //array[5] = 0;
    int k = 100;
    for (int i = 0; i < k; i++)
    {
        timer.start();
        draw_lines(angle, 150,  1, color, 0);
        array[0] += timer.nsecsElapsed();
        //double te = tick();
        //time_cda = te - tb;

        //tb = tick();
        timer.start();
        draw_lines(angle, 150, 2, color, 0);
        array[1] += timer.nsecsElapsed();
        //te = tick();
        //time_br_d = te - tb;

        //tb = tick();

        timer.start();
        draw_lines(angle, 150, 3, color, 0);
        //te = tick();
        array[2] += timer.nsecsElapsed();

        timer.start();
        draw_lines(angle, 150, 4, color, 0);
        //te = tick();
        array[3] += timer.nsecsElapsed();

        timer.start();
        draw_lines(angle, 150, 5, color, 0);
        //te = tick();
        array[4] += timer.nsecsElapsed();

    }
    array[0] /= k;
    array[1] /= k;
    array[2] /= k;
    array[3] /= k;
    array[4] /= k;
    //array[5] /= k;

    qDebug()  << "cda = "
              <<  array[0]
              << " br_d = "
              <<  array[1]
              << " br_i = "
              <<  array[2]
              << " step = "
              <<  array[3]
              << " wu = "
              <<  array[4]
              //<< " st = "
              //<<  array[5]
              << endl;


    fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);

    customPlot->addPlottable(fossil);
    // Установки цвета:
    QPen pen;
    pen.setWidthF(1.5);//Толщина контура столбца
    fossil->setName(QString::fromUtf8("длина линий = 150")); // Легенда
    pen.setColor(QColor(50, 50, 100));// Цвет контура столбца
    fossil->setPen(pen);
    // Цвет самого столбца, четвертый параметр - прозрачность
    fossil->setBrush(QColor(50, 50, 250, 70));

    // Установки значений оси X:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5;
    labels << "CDA" << "BR_d" << "BR_i" << "BR_st" << "Wu";
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, 6);

    // Установки значений оси Y:
    customPlot->yAxis->setRange(0, max_zn(array) + 200);
    customPlot->yAxis->setPadding(5);
    customPlot->yAxis->setLabel(QString::fromUtf8("Время работы"));
    customPlot->yAxis->grid()->setSubGridVisible(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);


    // Данные:
    QVector<double> fossilData;
    qsrand (time(NULL));
    fossilData  << array[0]
                << array[1]
                << array[2]
                << array[3]
                << array[4];
    fossil->setData(ticks, fossilData);

    // Легенда:
    customPlot->legend->setVisible(true);
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);

    //customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->replot();

    // Сброс всех установок графика:


}



void MainWindow::on_pushButton_4_clicked()
{
    Widget *scene = new Widget;
    scene->resize(600, 600);

    //scene->rndres();
    scene->show();

}*/

#include "draw_circle.h"
#include "draw_ellipse.h"

typedef struct steps_angle_s
{
    int angle;
    int steps;
}steps_angle;

QVector<double> array_canon(100);
QVector<double> array_param(100);
QVector<double> array_brezen(100);
QVector<double> array_middle(100);
QVector<double> array_x;

void fill_array_time_circle(void)
{
    QElapsedTimer timer;
    array_x.clear();


    options_s opt;
    int plus = 10;
    for (int i = 0; i <= 90; i+=plus)
    {
        array_canon[i/plus] = 0;
        array_x.push_back(i);
        opt.my_circle.r_c = i;
        opt.my_circle.x_n = 300;
        opt.my_circle.y_n = 300;
        int iter = 10;
        for (int j = 0; j <= iter; j++)
        {
            timer.start();
            draw_canonich_circle(opt, 0);
            array_canon[i/plus] += timer.nsecsElapsed();

            timer.start();
            draw_param_circle(opt, 0);
            array_param[i/plus] += timer.nsecsElapsed();

            timer.start();
            draw_brez_circle(opt, 0);
            array_brezen[i/plus] += timer.nsecsElapsed();

            timer.start();
            draw_midle_circle(opt, 0);
            array_middle[i/plus] += timer.nsecsElapsed();
        }
        array_canon[i/plus] /= iter;
        array_param[i/plus] /= iter;
        array_brezen[i/plus] /= iter;
        array_middle[i/plus] /= iter;
    }

}


void fill_array_time_ellipse(void)
{
    QElapsedTimer timer;
    array_x.clear();


    options_s opt;
    int plus = 10;
    for (int i = 0; i <= 90; i+=plus)
    {
        array_canon[i/plus] = 0;
        array_x.push_back(i);
        opt.my_ellipse.a = i;
        opt.my_ellipse.b = i;
        opt.my_ellipse.x_n = 300;
        opt.my_ellipse.y_n = 300;
        int iter = 10;
        for (int j = 0; j <= iter; j++)
        {
            timer.start();
            draw_canonich_ellipse(opt, 0);
            array_canon[i/plus] += timer.nsecsElapsed();

            timer.start();
            draw_param_ellipse(opt, 0);
            array_param[i/plus] += timer.nsecsElapsed();

            timer.start();
            draw_brez_ellipse(opt, 0);
            array_brezen[i/plus] += timer.nsecsElapsed();

            timer.start();
            draw_midle_ellipse(opt, 0);
            array_middle[i/plus] += timer.nsecsElapsed();
        }
        array_canon[i/plus] /= iter;
        array_param[i/plus] /= iter;
        array_brezen[i/plus] /= iter;
        array_middle[i/plus] /= iter;
    }

}

Widget_graf::Widget_graf(QWidget *parent) :
    QWidget(parent)
{
    resize(500,400);
    setWindowTitle(QString::fromUtf8("График времени построения окружностей"));
    customPlot = new QCustomPlot(this);
    vbox = new QVBoxLayout(this);


    vbox->addWidget(customPlot);
    //res = new QPushButton(QString::fromUtf8("Показать или обновить гистограмму"),this);
    //vbox->addWidget(res);
    setLayout(vbox);


    customPlot->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в widget
    //customPlot->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    customPlot->legend->setVisible(true);   //Включаем Легенду графика
        // Устанавливаем Легенду в левый верхний угол графика
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

        // Инициализируем график и привязываем его к Осям
    QCPGraph *graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic);  // Устанавливаем график на полотно
    graphic->setName("Каноническое уравнение");       // Устанавливаем
    graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
    graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic->setData(array_x, array_canon);

    QCPGraph *graphic2 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic2);  // Устанавливаем график на полотно
    graphic2->setName("Параметрическое уравнение");       // Устанавливаем
    graphic2->setPen(QPen(QColor(Qt::green))); // Устанавливаем цвет графика
    graphic2->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic2->setData(array_x, array_param);

    QCPGraph *graphic3 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic3);  // Устанавливаем график на полотно
    graphic3->setName("Алгоритм Брезенхема");       // Устанавливаем
    graphic3->setPen(QPen(QColor(Qt::blue))); // Устанавливаем цвет графика
    graphic3->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic3->setData(array_x, array_brezen);

    QCPGraph *graphic4 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic4);  // Устанавливаем график на полотно
    graphic4->setName("Алгоритм средней точки");       // Устанавливаем
    graphic4->setPen(QPen(QColor(Qt::black))); // Устанавливаем цвет графика
    graphic4->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic4->setData(array_x, array_middle);



    //Подписываем оси Ox и Oy
    customPlot->xAxis->setLabel("radius");
    customPlot->yAxis->setLabel("time");

    //Установим область, которая будет показываться на графике
    customPlot->xAxis->setRange(0, 90);//Для оси Ox

    //Для показа границ по оси Oy сложнее, так как надо по правильному
    //вычислить минимальное и максимальное значение в векторах

    customPlot->yAxis->setRange(0, 10000);//Для оси Oy
    customPlot->legend->setVisible(true);
    //И перерисуем график на нашем widget
    customPlot->replot();
}

Widget_graf_ellipse::Widget_graf_ellipse(QWidget *parent) :
    QWidget(parent)
{
    resize(500,400);
    setWindowTitle(QString::fromUtf8("График времени построения эллипсов"));
    customPlot = new QCustomPlot(this);
    vbox = new QVBoxLayout(this);


    vbox->addWidget(customPlot);
    //res = new QPushButton(QString::fromUtf8("Показать или обновить гистограмму"),this);
    //vbox->addWidget(res);
    setLayout(vbox);


    customPlot->clearGraphs();//Если нужно, но очищаем все графики
    //Добавляем один график в widget
    //customPlot->addGraph();
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    customPlot->legend->setVisible(true);   //Включаем Легенду графика
        // Устанавливаем Легенду в левый верхний угол графика
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

        // Инициализируем график и привязываем его к Осям
    QCPGraph *graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic);  // Устанавливаем график на полотно
    graphic->setName("Каноническое уравнение");       // Устанавливаем
    graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
    graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic->setData(array_x, array_canon);

    QCPGraph *graphic2 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic2);  // Устанавливаем график на полотно
    graphic2->setName("Параметрическое уравнение");       // Устанавливаем
    graphic2->setPen(QPen(QColor(Qt::green))); // Устанавливаем цвет графика
    graphic2->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic2->setData(array_x, array_param);

    QCPGraph *graphic3 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic3);  // Устанавливаем график на полотно
    graphic3->setName("Алгоритм Брезенхема");       // Устанавливаем
    graphic3->setPen(QPen(QColor(Qt::blue))); // Устанавливаем цвет графика
    graphic3->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic3->setData(array_x, array_brezen);

    QCPGraph *graphic4 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic4);  // Устанавливаем график на полотно
    graphic4->setName("Алгоритм средней точки");       // Устанавливаем
    graphic4->setPen(QPen(QColor(Qt::black))); // Устанавливаем цвет графика
    graphic4->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic4->setData(array_x, array_middle);



    //Подписываем оси Ox и Oy
    customPlot->xAxis->setLabel("radius");
    customPlot->yAxis->setLabel("time");

    //Установим область, которая будет показываться на графике
    customPlot->xAxis->setRange(0, 90);//Для оси Ox

    //Для показа границ по оси Oy сложнее, так как надо по правильному
    //вычислить минимальное и максимальное значение в векторах

    customPlot->yAxis->setRange(0, 10000);//Для оси Oy
    customPlot->legend->setVisible(true);
    //И перерисуем график на нашем widget
    customPlot->replot();
}


void MainWindow::on_pushButton_5_clicked()
{
    fill_array_time_circle();
    Widget_graf *scene = new Widget_graf;
    scene->resize(600, 600);

    scene->show();
}



void MainWindow::on_pushButton_6_clicked()
{
    color_line2 = color_line;
    color_line = color_fone;
    QString qss = QString("background-color: %1").arg(color_line.name());
    ui->pushButton_color_line->setStyleSheet(qss);

}
#include "draw_circle.h"

void MainWindow::on_one_circle_clicked()
{
    QString x_n_s = ui->lineEdit_x_c->text();
    QString y_n_s = ui->lineEdit_y_c->text();
    QString r_c_s = ui->lineEdit_r_c->text();

    bool ok;
    float x_n = x_n_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_нач!");
        return;
    }
    float y_n = y_n_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y_нач!");
        return;
    }

    float r_c = r_c_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен радиус!");
        return;
    }

    options.my_circle.x_n = x_n;
    options.my_circle.y_n = y_n;
    options.my_circle.r_c = r_c;
    options.choose = 1;

    if (ui->radioButton_canonich->isChecked())
    {
       options.method = 1;
    }
    if (ui->radioButton_param->isChecked())
    {
       options.method = 2;
    }
    if (ui->radioButton_br->isChecked())
    {
       options.method = 3;
    }
    if (ui->radioButton_midle->isChecked())
    {
       options.method = 4;
    }
    if (ui->radioButton_standart->isChecked())
    {
       options.method = 5;
    }
    options.color = color_line;

    opt.push_back(options);

    ui->widget->update();

}

void MainWindow::on_pushButton_9_clicked()
{
    QString x_n_s = ui->lineEdit_x_e->text();
    QString y_n_s = ui->lineEdit_y_e->text();
    QString a_s = ui->lineEdit_a_e->text();
    QString b_s = ui->lineEdit_b_e->text();

    bool ok;
    float x_n = x_n_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_нач!");
        return;
    }
    float y_n = y_n_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y_нач!");
        return;
    }

    float a = a_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен радиус!");
        return;
    }
    float b = b_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен радиус!");
        return;
    }

    options.my_ellipse.x_n = x_n;
    options.my_ellipse.y_n = y_n;
    options.my_ellipse.a = a;
    options.my_ellipse.b = b;
    options.choose = 2;

    if (ui->radioButton_canonich->isChecked())
    {
       options.method = 1;
    }
    if (ui->radioButton_param->isChecked())
    {
       options.method = 2;
    }
    if (ui->radioButton_br->isChecked())
    {
       options.method = 3;
    }
    if (ui->radioButton_midle->isChecked())
    {
       options.method = 4;
    }
    if (ui->radioButton_standart->isChecked())
    {
       options.method = 5;
    }
    options.color = color_line;

    opt.push_back(options);

    ui->widget->update();
}

void MainWindow::on_pushButton_k_c_clicked()
{
    QString r_s = ui->lineEdit_r_k_c->text();
    QString dr_s = ui->lineEdit_dr_k_c->text();
    QString n_s = ui->lineEdit_n_k_c->text();

    bool ok;
    float r = r_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_нач!");
        return;
    }
    float dr = dr_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y_нач!");
        return;
    }

    float n = n_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен радиус!");
        return;
    }

    options.my_circle.r_c = r;
    options.my_circle.delta = dr;
    options.my_circle.n = n;
    options.choose = 3;

    if (ui->radioButton_canonich->isChecked())
    {
       options.method = 1;
    }
    if (ui->radioButton_param->isChecked())
    {
       options.method = 2;
    }
    if (ui->radioButton_br->isChecked())
    {
       options.method = 3;
    }
    if (ui->radioButton_midle->isChecked())
    {
       options.method = 4;
    }
    if (ui->radioButton_standart->isChecked())
    {
       options.method = 5;
    }
    options.color = color_line;

    opt.push_back(options);

    ui->widget->update();
}

void MainWindow::on_pushButton_k_e_clicked()
{
    QString a_s = ui->lineEdit_a_k_e->text();
    QString b_s = ui->lineEdit_b_k_e->text();
    QString del_s = ui->lineEdit_del_k_e->text();
    QString n_s = ui->lineEdit_n_k_e->text();

    bool ok;
    float a = a_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен радиус!");
        return;
    }
    float b = b_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен радиус!");
        return;
    }
    float del = del_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_нач!");
        return;
    }
    float n = n_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y_нач!");
        return;
    }

    options.my_ellipse.x_n = 300;
    options.my_ellipse.y_n = 300;
    options.my_ellipse.a = a;
    options.my_ellipse.b = b;
    options.my_ellipse.delta = del;
    options.my_ellipse.n = n;
    options.choose = 4;

    if (ui->radioButton_canonich->isChecked())
    {
       options.method = 1;
    }
    if (ui->radioButton_param->isChecked())
    {
       options.method = 2;
    }
    if (ui->radioButton_br->isChecked())
    {
       options.method = 3;
    }
    if (ui->radioButton_midle->isChecked())
    {
       options.method = 4;
    }
    if (ui->radioButton_standart->isChecked())
    {
       options.method = 5;
    }
    options.color = color_line;

    opt.push_back(options);

    ui->widget->update();
}

void MainWindow::on_pushButton_7_clicked()
{
    color_line = color_line2;
    QString qss = QString("background-color: %1").arg(color_line.name());
    ui->pushButton_color_line->setStyleSheet(qss);
}

void MainWindow::on_pushButton_8_clicked()
{
    fill_array_time_ellipse();
    Widget_graf_ellipse *scene = new Widget_graf_ellipse;
    scene->resize(600, 600);

    scene->show();
}
