#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "math.h"
#include "glwidget.h"

#include "QStandardItemModel"
#include "QStandardItem"
#include <QMessageBox>


#include <stdio.h>    // Заголовочный файл для стандартной библиотеки ввода/вывода
#include <gl\gl.h>    // Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>   // Заголовочный файл для библиотеки GLu32
#include <gl\glaux.h> // Заголовочный файл для библиотеки GLaux
#include <math.h>     // Заголовочный файл для математической библиотеки ( НОВОЕ )
#include <stdarg.h>   // Заголовочный файл для функций для работы с переменным


typedef struct options_s
{
    double x_ellipse_g;
    double y_ellipse_g;
    double angle_g;
    double d_angle_g;
    double x_rot_g;
    double y_rot_g;
    double x_scale_g;
    double y_scale_g;
    double k_x_g;
    double k_y_g;
}options_s;


options_s  options;
std::vector<options_s> opt;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    options.x_ellipse_g = 100;
    options.y_ellipse_g = 100;
    options.angle_g = 0;
    options.d_angle_g = 0;
    options.x_rot_g = options.x_ellipse_g;
    options.y_rot_g = options.y_ellipse_g;
    options.x_scale_g = options.x_ellipse_g;
    options.y_scale_g = options.y_ellipse_g;
    options.k_x_g = 1;
    options.k_y_g = 1;
    opt.push_back(options);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_b_rotate_clicked()
{
    this->ui->widget->key = 1;
    QString x_rot_s = ui->x_rot->text();
    QString y_rot_s = ui->y_rot->text();
    QString angle_s = ui->angle->text();

    bool ok;
    double x_rot = x_rot_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x центра поворота!", "Ok");
        return;
    }
    double y_rot = y_rot_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y центра поворота!", "Ok");
        return;
    }
    double angle = angle_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен угол!", "Ok");
        return;
    }

    options.x_ellipse_g = this->ui->widget->x_ellipse;
    options.y_ellipse_g = this->ui->widget->y_ellipse;
    options.angle_g = this->ui->widget->angle;
    options.d_angle_g = this->ui->widget->d_angle;
    options.x_rot_g = this->ui->widget->x_rot;
    options.y_rot_g = this->ui->widget->y_rot;
    options.x_scale_g = this->ui->widget->x_scale;
    options.y_scale_g = this->ui->widget->y_scale;
    options.k_x_g = this->ui->widget->k_x;
    options.k_y_g = this->ui->widget->k_y;
    opt.push_back(options);

    this->ui->widget->angle += angle;
    this->ui->widget->d_angle = angle;

    this->ui->widget->x_rot = x_rot;
    this->ui->widget->y_rot = y_rot;

    this->ui->widget->x_scale = this->ui->widget->x_ellipse;
    this->ui->widget->y_scale = this->ui->widget->y_ellipse;

    this->ui->widget->update();
}

void MainWindow::on_b_scale_clicked()
{
    this->ui->widget->key = 1;
    QString k_x_s = ui->k_x->text();
    QString k_y_s = ui->k_y->text();
    QString x_scale_s = ui->x_scale->text();
    QString y_scale_s = ui->y_scale->text();

    bool ok;
    double k_x = k_x_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен коэфициент масштаба по x!", "Ok");
        return;
    }
    double k_y = k_y_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен коэфициент масштаба по y!", "Ok");
        return;
    }
    double x_scale = x_scale_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно x центра масштабирования!", "Ok");
        return;
    }
    double y_scale = y_scale_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно y центра масштабирования!", "Ok");
        return;
    }

    options.x_ellipse_g = this->ui->widget->x_ellipse;
    options.y_ellipse_g = this->ui->widget->y_ellipse;
    options.angle_g = this->ui->widget->angle;
    options.d_angle_g = this->ui->widget->d_angle;
    options.x_rot_g = this->ui->widget->x_rot;
    options.y_rot_g = this->ui->widget->y_rot;
    options.x_scale_g = this->ui->widget->x_scale;
    options.y_scale_g = this->ui->widget->y_scale;
    options.k_x_g = this->ui->widget->k_x;
    options.k_y_g = this->ui->widget->k_y;
    opt.push_back(options);

    if (this->ui->widget->k_x == 0)
        this->ui->widget->k_x = k_x;
    else
        this->ui->widget->k_x *= k_x;
    if (this->ui->widget->k_y == 0)
        this->ui->widget->k_y = k_y;
    else
        this->ui->widget->k_y *= k_y;
    this->ui->widget->x_scale = x_scale;
    this->ui->widget->y_scale = y_scale;

    this->ui->widget->x_rot = this->ui->widget->x_ellipse;
    this->ui->widget->y_rot = this->ui->widget->y_ellipse;

    this->ui->widget->update();
}

void MainWindow::on_pushButton_clicked()
{
    ui->x_scale->setText(QString::number(this->ui->widget->x_ellipse));
    ui->y_scale->setText(QString::number(this->ui->widget->y_ellipse));
    ui->x_rot->setText(QString::number(this->ui->widget->x_ellipse));
    ui->y_rot->setText(QString::number(this->ui->widget->y_ellipse));
}

void MainWindow::on_perem_clicked()
{
    this->ui->widget->key = 1;
    QString d_x_s = ui->d_x->text();
    QString d_y_s = ui->d_y->text();

    bool ok;
    double d_x = d_x_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен d(x)!", "Ok");
        return;
    }
    double d_y = d_y_s.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен d(x)!", "Ok");
        return;
    }

    options.x_ellipse_g = this->ui->widget->x_ellipse;
    options.y_ellipse_g = this->ui->widget->y_ellipse;
    options.angle_g = this->ui->widget->angle;
    options.d_angle_g = this->ui->widget->d_angle;
    options.x_rot_g = this->ui->widget->x_rot;
    options.y_rot_g = this->ui->widget->y_rot;
    options.x_scale_g = this->ui->widget->x_scale;
    options.y_scale_g = this->ui->widget->y_scale;
    options.k_x_g = this->ui->widget->k_x;
    options.k_y_g = this->ui->widget->k_y;
    opt.push_back(options);
    this->ui->widget->x_ellipse += d_x;
    this->ui->widget->y_ellipse += d_y;
    this->ui->widget->x_scale = this->ui->widget->x_ellipse;
    this->ui->widget->y_scale = this->ui->widget->y_ellipse;
    this->ui->widget->x_rot = this->ui->widget->x_ellipse;
    this->ui->widget->y_rot = this->ui->widget->y_ellipse;

    this->ui->widget->update();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->ui->widget->key = 1;
    this->ui->widget->x_ellipse = 100;
    this->ui->widget->y_ellipse = 100;
    this->ui->widget->angle = 0;
    this->ui->widget->k_x = 1;
    this->ui->widget->k_y = 1;
    this->ui->widget->x_scale = this->ui->widget->x_ellipse;
    this->ui->widget->y_scale = this->ui->widget->y_ellipse;
    this->ui->widget->x_rot = this->ui->widget->x_ellipse;
    this->ui->widget->y_rot = this->ui->widget->y_ellipse;
    ui->d_x->clear();
    ui->d_y->clear();
    ui->angle->clear();
    ui->k_x->clear();
    ui->k_y->clear();
    ui->x_scale->setText(QString::number(this->ui->widget->x_ellipse));
    ui->y_scale->setText(QString::number(this->ui->widget->y_ellipse));
    ui->x_rot->setText(QString::number(this->ui->widget->x_ellipse));
    ui->y_rot->setText(QString::number(this->ui->widget->y_ellipse));
    options.x_ellipse_g = 100;
    options.y_ellipse_g = 100;
    options.angle_g = 0;
    options.d_angle_g = 0;
    options.x_rot_g = options.x_ellipse_g;
    options.y_rot_g = options.x_ellipse_g;
    options.x_scale_g = options.x_ellipse_g;
    options.y_scale_g = options.x_ellipse_g;
    options.k_x_g = 1;
    options.k_y_g = 1;

    this->ui->widget->update();
}



void MainWindow::on_pushButton_3_clicked()
{
    if (!opt.empty())
    {
        this->ui->widget->key = 0;
        this->ui->widget->x_ellipse = opt.back().x_ellipse_g;
        this->ui->widget->y_ellipse = opt.back().y_ellipse_g;
        this->ui->widget->angle = opt.back().angle_g;
        this->ui->widget->d_angle = opt.back().d_angle_g;
        this->ui->widget->k_x = opt.back().k_x_g;
        this->ui->widget->k_y = opt.back().k_y_g;
        this->ui->widget->x_scale = opt.back().x_scale_g;
        this->ui->widget->y_scale = opt.back().y_scale_g;
        this->ui->widget->x_rot = opt.back().x_rot_g;
        this->ui->widget->y_rot = opt.back(). y_rot_g;
        opt.pop_back();
        this->ui->widget->update();

        //char str[500];
        //sprintf(str,"%f %f\n%f %f\n%f %f", this->ui->widget->x_ellipse, this->ui->widget->y_ellipse, this->ui->widget->angle, this->ui->widget->d_angle, this->ui->widget->x_rot, this->ui->widget->y_rot);
        //printf("!! %s\n", str);
        //QMessageBox::information(this, "Ошибка",  str, "Ok");


    }
}
