#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "math.h"


#include "QStandardItemModel"
#include "QStandardItem"
#include <QMessageBox>


#include <stdio.h>    // Заголовочный файл для стандартной библиотеки ввода/вывода
#include <gl\gl.h>    // Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>   // Заголовочный файл для библиотеки GLu32
#include <gl\glaux.h> // Заголовочный файл для библиотеки GLaux
#include <math.h>     // Заголовочный файл для математической библиотеки ( НОВОЕ )
#include <stdarg.h>   // Заголовочный файл для функций для работы с переменным

typedef struct point_s
{

    double x;
    double y;
}point;


typedef struct triangle_s
{
    point p_a;
    point p_b;
    point p_c;
}triangle;



typedef struct coord_v_s
{
    point p_1;
    point p_2;
}coord_v;

typedef struct coord_m_s
{
    point p_1;
    point p_2;
}coord_m;

typedef struct max_min_win_s
{
    point p_x;
    point p_y;
}max_min_win;

std::vector<point> points;

float max_cos = -1;
triangle max_cos_triangle;
coord_m max_cos_med;
coord_v max_cos_vis;
max_min_win drow_size;
double koef;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ui->tableWidget->resizeRowsToContents();
    // ui->tableWidget->resizeColumnsToContents();


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //this->setFixedSize(607,343);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void max_min_size(triangle max_cos_triangle,coord_v vis,max_min_win *size)
{
    double x[4];
    double y[4];
    double a;
    x[0] = max_cos_triangle.p_a.x;
    x[1] = max_cos_triangle.p_b.x;
    x[2] = max_cos_triangle.p_c.x;
    x[3] = vis.p_2.x;

    y[0] = max_cos_triangle.p_a.y;
    y[1] = max_cos_triangle.p_b.y;
    y[2] = max_cos_triangle.p_c.y;
    y[3] = vis.p_2.y;
    for (int i = 0; i < 4; i++)
    {
        for (int j = i; j < 4; j++)
        {
            if (x[i] > x[j])
            {
                a = x[i];
                x[i] = x[j];
                x[j] = a;
            }
            if (y[i] > y[j])
            {
                a = y[i];
                y[i] = y[j];
                y[j] = a;
            }
        }
    }
    size->p_x.x = x[0];
    size->p_x.y = x[3];
    size->p_y.x = y[0];
    size->p_y.y = y[3];

}


void MainScene::initializeGL()
{
     glClearColor(1.0, 1.0, 1.0, 1.0);
}

void MainScene::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    koef = fabs(drow_size.p_x.y) > fabs(drow_size.p_y.y) ? fabs(drow_size.p_x.y)/5 : fabs(drow_size.p_y.y)/5;

    double k_min = fabs(drow_size.p_x.x) > fabs(drow_size.p_y.x) ? fabs(drow_size.p_x.x)/5 : fabs(drow_size.p_y.x)/5;

    koef = koef > k_min ? koef : k_min;

    printf("!!! %f %f %f\n", fabs(drow_size.p_x.y)/5, fabs(drow_size.p_y.y), koef);

    double l_border = drow_size.p_x.x < drow_size.p_y.x ? drow_size.p_x.x : drow_size.p_y.x;
    double r_border = drow_size.p_x.y > drow_size.p_y.y ? drow_size.p_x.y : drow_size.p_y.y;

    //glOrtho(drow_size.p_x.x-koef, drow_size.p_x.y+koef, drow_size.p_y.x-koef, drow_size.p_y.y+koef, -1.0, 1.0);
    glOrtho(l_border-koef, r_border+koef, l_border-koef, r_border+koef, -1.0, 1.0);

    // glOrtho(drow_size.p_x.x-koef, r_border+koef, l_border-koef, r_border+koef, -1.0, 1.0);

    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}


void MainScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //glColor3f(0.25, 0.87, 0.81);
    QFont font;
    font.setPointSize(14);

//    glBegin(GL_LINES);
//    glColor3d(0,0,0);
//    glVertex2d(drow_size.p_x.x - koef/2, drow_size.p_y.x - koef/2);
//    glVertex2d(drow_size.p_x.y + koef/2, drow_size.p_y.x - koef/2);

//    glVertex2d(drow_size.p_x.x - koef/2, drow_size.p_y.x - koef/2);
//    glVertex2d(drow_size.p_x.x - koef/2, drow_size.p_y.y + koef/2);
//    glEnd();
    double l_border = drow_size.p_x.x < drow_size.p_y.x ? drow_size.p_x.x : drow_size.p_y.x;
    double r_border = drow_size.p_x.y > drow_size.p_y.y ? drow_size.p_x.y : drow_size.p_y.y;

    glBegin(GL_LINES);
    glColor3d(0,0,0);
    glVertex2d(l_border - koef/2, l_border - koef/2);
    glVertex2d(r_border + koef/2, l_border - koef/2);

    glVertex2d(l_border - koef/2, l_border - koef/2);
    glVertex2d(l_border - koef/2, r_border + koef/2);
    glEnd();


    renderText(r_border + koef/2, l_border - koef/2,0, "X",font);
    renderText(l_border - koef/2, r_border + koef/2,0, "Y",font);


    renderText(max_cos_triangle.p_a.x, max_cos_triangle.p_a.y,0, " A",font);
    renderText(max_cos_triangle.p_b.x, max_cos_triangle.p_b.y,0, " B",font);
    renderText(max_cos_triangle.p_c.x, max_cos_triangle.p_c.y,0, " C",font);
    renderText(max_cos_med.p_2.x, max_cos_med.p_2.y,0, " M",font);
    renderText(max_cos_vis.p_2.x, max_cos_vis.p_2.y,0, " H",font);

    glBegin(GL_LINES);
    glColor3d(0,0,0);
    glVertex2d(max_cos_triangle.p_a.x, max_cos_triangle.p_a.y);
    glVertex2d(max_cos_triangle.p_b.x, max_cos_triangle.p_b.y);

    glVertex2d(max_cos_triangle.p_b.x, max_cos_triangle.p_b.y);
    glVertex2d(max_cos_triangle.p_c.x, max_cos_triangle.p_c.y);

    glVertex2d(max_cos_triangle.p_c.x, max_cos_triangle.p_c.y);
    glVertex2d(max_cos_triangle.p_a.x, max_cos_triangle.p_a.y);
    glColor3d(1,0,0);
    glVertex2d(max_cos_vis.p_1.x, max_cos_vis.p_1.y);
    glVertex2d(max_cos_vis.p_2.x, max_cos_vis.p_2.y);
    glColor3d(0,1,0);
    glVertex2d(max_cos_med.p_1.x, max_cos_med.p_1.y);
    glVertex2d(max_cos_med.p_2.x, max_cos_med.p_2.y);

    glEnd();

    glPointSize(5.0); // размер точки
    glColor3d(0,0,0);
    glBegin(GL_POINTS);
    glVertex2f(max_cos_triangle.p_a.x, max_cos_triangle.p_a.y);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(max_cos_triangle.p_b.x, max_cos_triangle.p_b.y);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(max_cos_triangle.p_c.x, max_cos_triangle.p_c.y);
    glEnd();

    glColor3d(0,1,0);
    glBegin(GL_POINTS);
    glVertex2f(max_cos_med.p_2.x, max_cos_med.p_2.y);
    glEnd();
    glColor3d(1,0,0);
    glBegin(GL_POINTS);
    glVertex2f(max_cos_vis.p_2.x, max_cos_vis.p_2.y);
    glEnd();
}


void MainWindow::on_pushButton_clicked()
{
    //ui->tableWidget->insertRow(0);
    QString x_s = ui->lineEdit->text();
    QString y_s = ui->lineEdit_2->text();

    bool ok;
    float x = x_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x!");
        return;
    }
    float y = y_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y!");
        return;
    }

    QTableWidgetItem *item1, *item2;
    item1 = new QTableWidgetItem(ui->lineEdit->text());
    item2 = new QTableWidgetItem(ui->lineEdit_2->text());

    ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 0, item1);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 1, item2);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //ui->tableWidget->resizeRowsToContents();
    //ui->tableWidget->resizeColumnsToContents();


    point p_points;
    p_points.x = x;
    p_points.y = y;

    points.push_back(p_points);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit->setFocus();

    /*
  QStandardItemModel *model = new QStandardItemModel;
  QStandardItem *item;
  item = new QStandardItem(ui->lineEdit->text());
  model->setItem(0, 0, item);
  ui->tableView->setModel(model);
   */
}


void v_coord(triangle tr, coord_v *vis)
{
    double A1, A2, B1, B2, C1, C2;

    vis->p_1.x = tr.p_a.x;
    vis->p_1.y = tr.p_a.y;


    if (tr.p_c.x - tr.p_b.x == 0)
    {
        vis->p_2.y = tr.p_a.y;
        vis->p_2.x = tr.p_c.x;
        return;
    }

    float k = (tr.p_c.y - tr.p_b.y)/(tr.p_c.x - tr.p_b.x);
    if (k == 0)
    {

        vis->p_2.y = tr.p_c.y;
        vis->p_2.x = tr.p_a.x;
        return;
    }
    float k_v = -1 / k;


    A1 = k;
    B1 = -1;
    C1 = tr.p_b.y - k * tr.p_b.x;

    A2 = k_v;
    B2 = -1;
    C2 = tr.p_a.y - k_v * tr.p_a.x;


    vis->p_2.x = -((C1*B2-C2*B1)/(A1*B2-A2*B1));
    vis->p_2.y = -((A1*C2-A2*C1)/(A1*B2-A2*B1));
}


void m_coord(triangle tr, coord_m *mis)
{
    mis->p_1.x = tr.p_a.x;
    mis->p_1.y = tr.p_a.y;
    mis->p_2.x = (tr.p_b.x + tr.p_c.x)/2;
    mis->p_2.y = (tr.p_b.y + tr.p_c.y)/2;
}


float angle_cos(coord_v vis, coord_m mis)
{
    float median_x = mis.p_2.x - mis.p_1.x;
    float median_y = mis.p_2.y - mis.p_1.y;
    float vis_x = vis.p_2.x - mis.p_1.x;
    float vis_y = vis.p_2.y - mis.p_1.y;
    //printf("@@ %f %f; %f %f\n",median_x, median_y, vis_x, vis_y);
    float angle_cos = (median_x * vis_x + median_y * vis_y)/(sqrt(median_x*median_x+median_y*median_y)*sqrt(vis_x*vis_x+vis_y*vis_y));
    return angle_cos;
}

int check_on_triangle(triangle tr)
{
    double a = sqrt((tr.p_a.x-tr.p_b.x)*(tr.p_a.x-tr.p_b.x) + (tr.p_a.y-tr.p_b.y)*(tr.p_a.y-tr.p_b.y));
    double b = sqrt((tr.p_b.x-tr.p_c.x)*(tr.p_b.x-tr.p_c.x) + (tr.p_b.y-tr.p_c.y)*(tr.p_b.y-tr.p_c.y));
    double c = sqrt((tr.p_a.x-tr.p_c.x)*(tr.p_a.x-tr.p_c.x) + (tr.p_a.y-tr.p_c.y)*(tr.p_a.y-tr.p_c.y));
    //printf("@!@! %f %f %f\n", a,b,c);
    if (a + b > c && a + c > b && b + c > a)
        return 1;
    return 0;
}

void MainScene::drawTriangle()
{
    glBegin(GL_LINES);
    glColor3d(0,0,0);
    glVertex2d(5, 4);
    glVertex2d(145, 51);
    glEnd();
}

void MainWindow::on_pushButton_2_clicked()
{

    triangle tr;
    coord_v vis;
    coord_m mis;
    float cos_angle;
    max_cos = -1;

    if (points.size() < 3)
    {
        QMessageBox::information(this, "Ошибка", "Слишком мало точек!");
        return;
    }

    for (int i = 0; i < points.size(); i++)
    {
        for (int j = i + 1; j < points.size(); j++)
        {
            for (int k = j + 1; k < points.size(); k++)
            {

                tr.p_a.x = points[i].x;
                tr.p_a.y = points[i].y;
                tr.p_b.x = points[j].x;
                tr.p_b.y = points[j].y;
                tr.p_c.x = points[k].x;
                tr.p_c.y = points[k].y;
                if (!check_on_triangle(tr))
                    continue;
                v_coord(tr, &vis);
                m_coord(tr, &mis);
                cos_angle = angle_cos(vis, mis);
                if (max_cos < cos_angle)
                {
                    max_cos = cos_angle;
                    max_cos_triangle = tr;
                    max_cos_med = mis;
                    max_cos_vis = vis;
                }
                tr.p_a.x = points[j].x;
                tr.p_a.y = points[j].y;
                tr.p_b.x = points[i].x;
                tr.p_b.y = points[i].y;
                tr.p_c.x = points[k].x;
                tr.p_c.y = points[k].y;
                v_coord(tr, &vis);
                m_coord(tr, &mis);
                cos_angle = angle_cos(vis, mis);
                if (max_cos < cos_angle)
                {
                    max_cos = cos_angle;
                    max_cos_triangle = tr;
                    max_cos_med = mis;
                    max_cos_vis = vis;
                }
                tr.p_a.x = points[k].x;
                tr.p_a.y = points[k].y;
                tr.p_b.x = points[i].x;
                tr.p_b.y = points[i].y;
                tr.p_c.x = points[j].x;
                tr.p_c.y = points[j].y;
                v_coord(tr, &vis);
                m_coord(tr, &mis);
                cos_angle = angle_cos(vis, mis);
                if (max_cos < cos_angle)
                {
                    max_cos = cos_angle;
                    max_cos_triangle = tr;
                    max_cos_med = mis;
                    max_cos_vis = vis;
                }
            }
        }
    }
    max_min_size(max_cos_triangle,max_cos_vis, &drow_size);

    if (max_cos == -1)
        QMessageBox::information(this, "Ошибка", "Только вырожденные треугольники!");
    else
    {
        double pi = 3.1415926535;
        ui->label->setText("A, B, C - вершины треугольника\nAH - высота, AM - медиана\n");
        char  s[500];
        sprintf(s,"A = [%f; %f]\n",max_cos_triangle.p_a.x,  max_cos_triangle.p_a.y);
        ui->label->setText(ui->label->text() + s);
        sprintf(s,"B = [%f; %f]\n",max_cos_triangle.p_b.x,  max_cos_triangle.p_b.y);
        ui->label->setText(ui->label->text() + s);
        sprintf(s,"C = [%f; %f]\n",max_cos_triangle.p_c.x,  max_cos_triangle.p_c.y);
        ui->label->setText(ui->label->text() + s);
        sprintf(s,"M = [%f; %f]\n",max_cos_med.p_2.x,  max_cos_med.p_2.y);
        ui->label->setText(ui->label->text() + s);
        sprintf(s,"H = [%f; %f]\n",max_cos_vis.p_2.x,  max_cos_vis.p_2.y);
        ui->label->setText(ui->label->text() + s);
        sprintf(s,"Угол межуду AH и AM:\n%f градусов",acos(max_cos)*(180/pi));
        ui->label->setText(ui->label->text() + s);
        //
        //sprintf(s,"%f %f; %f %f %f %f\n%f;  ",max_cos_triangle.p_a.x, max_cos_triangle.p_a.y, max_cos_triangle.p_b.x, max_cos_triangle.p_b.y, max_cos_triangle.p_c.x, max_cos_triangle.p_c.y, max_cos);
        //QMessageBox::information(this, "Ошибка", s);
        MainScene *scene = new MainScene;
        scene->resize(600, 600);
        scene->show();
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    if (points.empty())
    {
        QMessageBox::information(this, "Информация", "Список точек пуст!");
    }
    else
    {
        int i = ui->tableWidget->currentRow();
        if (i < 0)
        {
            return;
        }
        ui->tableWidget->removeRow(i);
        points.erase(points.begin()+i);
//        char  s[500];
//        char  c[500];
//        for (int i = 0; i<points.size(); i++ )
//        {
//            sprintf(c,"%f %f;  ",points[i].x, points[i].y);
//            strcat(s,c);
//        }
//        QMessageBox::information(this, "Информация", s);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if (points.empty())
    {
        QMessageBox::information(this, "Информация", "Список точек пуст!");
    }
    else
    {
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        QStringList name_table;
        name_table << "X" << "Y";
        ui->tableWidget->setHorizontalHeaderLabels(name_table);
        points.clear();
        QMessageBox::information(this, "Информация", "Список точек очищен!");
    }
}
