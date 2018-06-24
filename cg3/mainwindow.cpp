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


typedef struct options_ss
{
    double x_n;
    double y_n;
    double x_k;
    double y_k;
    int method;
    int lines;
    double angle;
    double rad;
    QColor color;
}options_s;


options_s  options;
QColor color_fone;
QColor color_line;
std::vector<options_s> opt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::red);
    ui->widget_color_line->setAutoFillBackground(true);
    ui->widget_color_line->setPalette(Pal);
    ui->widget_color_line->show();*/


    ui->pushButton_color_line->setStyleSheet( " background-color: black;" );
    ui->pushButton_color_fone->setStyleSheet( " background-color: white;" );
    color_fone = QColor("white");
    color_line = QColor("black");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString x_n_s = ui->lineEdit_1_x->text();
    QString y_n_s = ui->lineEdit_1_y->text();
    QString x_k_s = ui->lineEdit_2_x->text();
    QString y_k_s = ui->lineEdit_2_y->text();

    bool ok;
    float x_n = x_n_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_нач!");
        return;
    }
    float y_n = y_n_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y_нач!");
        return;
    }

    float x_k = x_k_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_кон!");
        return;
    }
    float y_k = y_k_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y_кон!");
        return;
    }

    options.x_n = x_n;
    options.y_n = y_n;
    options.x_k = x_k;
    options.y_k = y_k;
    options.lines = 0;

    if (ui->radioButton_cda->isChecked())
    {
       options.method = 1;
    }
    if (ui->radioButton_br_d->isChecked())
    {
       options.method = 2;
    }
    if (ui->radioButton_br_i->isChecked())
    {
       options.method = 3;
    }
    if (ui->radioButton_br_st->isChecked())
    {
       options.method = 4;
    }
    if (ui->radioButton_vu->isChecked())
    {
       options.method = 5;
    }
    if (ui->radioButton_standart->isChecked())
    {
       options.method = 6;
    }
    options.color = color_line;

    opt.push_back(options);

    ui->widget->update();
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

        ui->widget->update();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    opt.clear();
    ui->widget->update();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString angle_s = ui->lineEdit_angle->text();
    bool ok;
    float angle = angle_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен угол!");
        return;
    }


    QString rad_s = ui->lineEdit_rad->text();
    float rad = rad_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен радиус!");
        return;
    }

    options.lines = 1;
    if (ui->radioButton_cda->isChecked())
    {
       options.method = 1;
    }
    if (ui->radioButton_br_d->isChecked())
    {
       options.method = 2;
    }
    if (ui->radioButton_br_i->isChecked())
    {
       options.method = 3;
    }
    if (ui->radioButton_br_st->isChecked())
    {
       options.method = 4;
    }
    if (ui->radioButton_vu->isChecked())
    {
       options.method = 5;
    }
    if (ui->radioButton_standart->isChecked())
    {
       options.method = 6;
    }

    options.angle = angle;
    options.rad = rad;
    options.color = color_line;

    opt.push_back(options);

    ui->widget->update();
}


#include "qcustomplot.h"

double time_cda;
double time_br_d;
double time_br_i;


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

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d));
    return d;
}


extern void draw_lines(double angle,double rad, int i, QColor color, int paint);


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

        /*timer.start();
        draw_lines(angle, 150, 6, color, 0);
        //te = tick();
        array[5] += timer.nsecsElapsed();*/
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

}


extern void draw_brez_int(options_s  options, int paint, int &steps);
extern void rotate_func(double *x, double *y, double angle, double xc, double yc);

typedef struct steps_angle_s
{
    int angle;
    int steps;
}steps_angle;



QVector<double> x_arr, y_arr;

void points_for_graf(void)
{
    options_s opt;
    double xc = 300;
    double yc = 300;
    double xk = 450;
    double yk = 300;
    for(int i = 0; i <= 90; i+=1)
    {
        double angle_g = -i * 3.1415926/180;
        opt.x_n = xc;
        opt.y_n = yc;
        opt.x_k = xk;
        opt.y_k = yk;
        opt.color = "black";
        rotate_func(&opt.x_k, &opt.y_k, angle_g, xc, yc);

        int steps;
        draw_brez_int(opt, 0, steps);
        x_arr.push_back(i);
        y_arr.push_back(steps);
    }
}



Widget_graf::Widget_graf(QWidget *parent) :
    QWidget(parent)
{


    resize(500,400);
    setWindowTitle(QString::fromUtf8("График количества ступенек от угла наклона"));
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
        graphic->setName("Целочисленный алгоритм Брезенхема; длина линии = 150");       // Устанавливаем
        graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
        graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
        //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    graphic->setData(x_arr, y_arr);

    //Подписываем оси Ox и Oy
    customPlot->xAxis->setLabel("angle");
    customPlot->yAxis->setLabel("steps");

    //Установим область, которая будет показываться на графике
    customPlot->xAxis->setRange(0, 90);//Для оси Ox

    //Для показа границ по оси Oy сложнее, так как надо по правильному
    //вычислить минимальное и максимальное значение в векторах

    customPlot->yAxis->setRange(0, 130);//Для оси Oy
    customPlot->legend->setVisible(true);
    //И перерисуем график на нашем widget
    customPlot->replot();
}



void MainWindow::on_pushButton_5_clicked()
{
    points_for_graf();
    Widget_graf *scene = new Widget_graf;
    scene->resize(600, 600);

    //scene->rndres();
    scene->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    color_line = color_fone;
    QString qss = QString("background-color: %1").arg(color_line.name());
    ui->pushButton_color_line->setStyleSheet(qss);
}
