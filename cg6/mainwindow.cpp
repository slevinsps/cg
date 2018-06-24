#include "mainwindow.h"
#include <vector>
#include <QGridLayout>
#include <QGraphicsItem>
#include <qDebug>
#include <QMessageBox>

#include <QColorDialog>
#include <QColor>
#include <QShortcut>
QColor fone_color(Qt::white);
QColor fill_color(Qt::black);
QColor border_color(Qt::black);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     mgw = new MyQGraphicsView();
     mgw->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
     mgw->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
     ui->gridLayout->addWidget( mgw);
     QString qss = QString("background-color: %1").arg(border_color.name());
     ui->pushButton_color_line->setStyleSheet(qss);
     ui->pushButton_color_fone->setStyleSheet(qss);
     connect( mgw, SIGNAL( send_point( QPointF) ), this, SLOT( get_point( QPointF ) ) );
     connect(this , SIGNAL( short_fig_sig() ), mgw, SLOT( short_fig() ) );
     connect(this , SIGNAL( fill_fig_sig() ), mgw, SLOT( fill_fig() ) );
     connect(this , SIGNAL( fill_zfig_sig() ), mgw, SLOT( fill_zfig() ) );
     connect(this , SIGNAL( clear_fig_sig() ), mgw, SLOT( clear_fig() ) );
     connect(this , SIGNAL( add_point_button_sig(int, int) ), mgw, SLOT( add_point_button(int, int) ) );
     connect(this , SIGNAL( add_circle_sig(int,int,int)), mgw, SLOT( add_circle(int, int, int) ) );
     connect(ui->radioButton , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox_zatr_flag() ) );
     connect(ui->radioButton_2 , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox_circle() ) );
     connect(ui->radioButton_3 , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox_line() ) );

}


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

point_str zatr_pixel{0,0};
point_str center_circle{0,0};
std::vector<edges> my_edges;
std::vector<QPointF> my_points;
edges new_edge{-1,-1,-1,-1};
int nach_index = 0;
QImage image(600,600,QImage::Format_RGB32);
int kol_vo_points = 0;
bool check_short = false;
bool check_fill = false;

MyQGraphicsView::MyQGraphicsView(QWidget *parent) :
        QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setSceneRect(0, 0, 600, 600);
    this->setScene(scene);
    image.fill(Qt::white);
}


void correct_coord(std::vector<QPointF> my_points, QPointF& point)
{
    for (int i = 0; i < my_points.size(); i++)
    {
        if ((int)my_points[i].x() == (int)point.x() && (int)my_points[i].y() == (int)point.y())
        {
            //qDebug() << "hey";
             point.setX((int)point.x()+1);
        }
    }
}
void correct_edges(std::vector<edges> my_edges, edges& ed)
{
    for (int i = 0; i < my_edges.size(); i++)
    {
        if (ed.y1 == ed.y2)
        {
            if (my_edges[i].x1 == ed.x1 && my_edges[i].y1 == ed.y1 ||  my_edges[i].x2 == ed.x1 && my_edges[i].y2 == ed.y1)
            {
                //qDebug() << "hey12";
                ed.y1++;
            }
            if (my_edges[i].x1 == ed.x2 && my_edges[i].y1 == ed.y2 ||  my_edges[i].x2 == ed.x2 && my_edges[i].y2 == ed.y2)
            {
                //qDebug() << "hey22";
                ed.y2++;
            }
        }
        else
        {
            if (my_edges[i].x1 == ed.x1 && my_edges[i].y1 == ed.y1 ||  my_edges[i].x2 == ed.x1 && my_edges[i].y2 == ed.y1)
             {
                //qDebug() << "hey12";
                ed.x1++;
             }
             if (my_edges[i].x1 == ed.x2 && my_edges[i].y1 == ed.y2 ||  my_edges[i].x2 == ed.x2 && my_edges[i].y2 == ed.y2)
            {
                //qDebug() << "hey22";
                ed.x2++;
            }
        }
    }
}

void my_swap(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

bool check_shift = 0;

bool check_checkBox_zatr_flags = false;
bool check_checkBox_circles = false;
void MyQGraphicsView::check_checkBox_zatr_flag()
{
    check_checkBox_circles = false;
    check_checkBox_zatr_flags = true;
}



void MyQGraphicsView::check_checkBox_circle()
{
    check_checkBox_zatr_flags = false;
    check_checkBox_circles = true;
}


void MyQGraphicsView::check_checkBox_line()
{
    check_checkBox_zatr_flags = false;
    check_checkBox_circles = false;
}

void MyQGraphicsView::mousePressEvent(QMouseEvent * e)
{
    QPen mypen(border_color);
    QBrush mybrush(Qt::black);

    my_point = mapToScene(e->pos());
    if (check_checkBox_zatr_flags)
    {
        QPen mypen2(QColor(image.pixel(zatr_pixel.x, zatr_pixel.y-1.5)));
        QBrush mybrush2(QColor(image.pixel(zatr_pixel.x, zatr_pixel.y-1.5)));
        scene->addEllipse(zatr_pixel.x-1.5, zatr_pixel.y-1.5, 3, 3, mypen2, mybrush2);
        zatr_pixel.x = (int)my_point.x();
        zatr_pixel.y = (int)my_point.y();
        scene->addEllipse(zatr_pixel.x-1.5, zatr_pixel.y-1.5, 3, 3, mypen, mybrush);
        emit send_point(my_point);
        return;
    }
    if (check_checkBox_circles)
    {
        QPen mypen2(QColor(image.pixel(center_circle.x, center_circle.y-1.5)));
        QBrush mybrush2(QColor(image.pixel(center_circle.x, center_circle.y-1.5)));
        scene->addEllipse(center_circle.x-1.5, center_circle.y-1.5, 3, 3, mypen2, mybrush2);
        center_circle.x = (int)my_point.x();
        center_circle.y = (int)my_point.y();
        scene->addEllipse(center_circle.x-1.5, center_circle.y-1.5, 3, 3, mypen, mybrush);
        emit send_point(my_point);
        return;
    }
    //correct_coord(my_points, my_point);
    my_points.push_back(my_point);

    QPainter paint;

    paint.begin(&image);
    paint.setPen(border_color);

    kol_vo_points++;

    //paint.drawPoint((int)my_point.x(), (int)my_point.y());
    if (new_edge.x1 == -1)
    {
        new_edge.x1 = (int)my_point.x();
        new_edge.y1 = (int)my_point.y();
        scene->addEllipse(new_edge.x1-2, new_edge.y1-2, 4, 4, mypen, mybrush);
    }
    else
    {

        new_edge.x2 = (int)my_point.x();
        new_edge.y2 = (int)my_point.y();
        //correct_edges(my_edges, new_edge);
        scene->addEllipse(new_edge.x2-2, new_edge.y2-2, 4, 4, mypen, mybrush);

        paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
        scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2,mypen);

        my_edges.push_back(new_edge);
        new_edge.x1 = new_edge.x2;
        new_edge.y1 = new_edge.y2;
    }
    paint.end();

    emit send_point(my_point);
}

void MyQGraphicsView::add_circle(int x, int y, int r)
{
    QPen pen(border_color);
    QPainter paint;

    paint.begin(&image);
    paint.setPen(border_color);

    QPen mypen2(QColor(image.pixel(zatr_pixel.x, zatr_pixel.y-1.5)));
    QBrush mybrush2(QColor(image.pixel(zatr_pixel.x, zatr_pixel.y-1.5)));
    scene->addEllipse(zatr_pixel.x-1.5, zatr_pixel.y-1.5, 3, 3, mypen2, mybrush2);
    scene->addEllipse(x-r/2, y-r/2, r, r, pen);
    paint.drawEllipse(x-r/2, y-r/2, r, r);
    paint.end();
}

void MyQGraphicsView::add_point_button(int x, int y)
{
    QPen pen(border_color);
    QBrush brush(Qt::black);
    QPainter paint;

    paint.begin(&image);
    paint.setPen(border_color);

    paint.drawPoint(x, y);
    if (new_edge.x1 == -1)
    {
        new_edge.x1 = x;
        new_edge.y1 = y;
        scene->addEllipse(new_edge.x1-2, new_edge.y1-2, 4, 4, pen, brush);
    }
    else
    {
        new_edge.x2 = x;
        new_edge.y2 = y;
        //correct_edges(my_edges, new_edge);
        scene->addEllipse(new_edge.x2-2, new_edge.y2-2, 4, 4, pen, brush);
        paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
        scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2,pen);

        my_edges.push_back(new_edge);
        new_edge.x1 = new_edge.x2;
        new_edge.y1 = new_edge.y2;
    }
    paint.end();
}

void MainWindow::get_point(QPointF my_point)
{
    if (check_checkBox_zatr_flags)
    {
        ui->lineEdit_x_za->setText(QString::number(zatr_pixel.x));
        ui->lineEdit_y_za->setText(QString::number(zatr_pixel.y));
        return;
    }
    if (check_checkBox_circles)
    {
        ui->lineEdit_x_cir->setText(QString::number(center_circle.x));
        ui->lineEdit_y_cir->setText(QString::number(center_circle.y));
        return;
    }

    QString s1 = QString::number(my_point.x());
    QString s2 = QString::number(my_point.y());


    QTableWidgetItem *item1, *item2;
    item1 = new QTableWidgetItem(s1);
    item2 = new QTableWidgetItem(s2);

    ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 0, item1);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 1, item2);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);


}


MainWindow::~MainWindow()
{
    delete ui;
}

int check_end_begin(void)
{
    if (my_edges[nach_index].x1 == my_edges[my_edges.size()-1].x2 && my_edges[nach_index].y1 == my_edges[my_edges.size()-1].y2)
        return 1;
    else
        return 0;
}


void MyQGraphicsView::short_fig()
{
    QPen pen(border_color);
    QPainter paint;
    paint.begin(&image);
    paint.setPen(border_color);
    new_edge.x2 = my_edges[nach_index].x1;
    new_edge.y2 = my_edges[nach_index].y1;
    new_edge.x1 = my_edges[my_edges.size()-1].x2;
    new_edge.y1 = my_edges[my_edges.size()-1].y2;
    paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
    scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2, pen);
    paint.end();
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);

    nach_index = my_edges.size()+1;
    my_edges.push_back(new_edge);
    new_edge.x1 = -1; new_edge.y1 = -1; new_edge.x2 = -1; new_edge.y2 = -1;
}
typedef struct min_max_s
{
    int max_x;
    int min_x;
    int max_y;
    int min_y;
}Min_max;

Min_max my_min_max;
void min_max(std::vector<edges> my_edges)
{
    my_min_max.min_x = my_edges[0].x1;
    my_min_max.max_x = my_edges[0].x1;
    my_min_max.min_y = my_edges[0].y1;
    my_min_max.max_y = my_edges[0].y1;
    for (int i = 0; i < my_edges.size(); i++)
    {
        if (my_min_max.min_x > my_edges[i].x1)
            my_min_max.min_x = my_edges[i].x1;
        if (my_min_max.max_x < my_edges[i].x1)
            my_min_max.max_x = my_edges[i].x1;

        if (my_min_max.min_x > my_edges[i].x2)
            my_min_max.min_x = my_edges[i].x2;
        if (my_min_max.max_x < my_edges[i].x2)
            my_min_max.max_x = my_edges[i].x2;

        if (my_min_max.min_y > my_edges[i].y1)
            my_min_max.min_y = my_edges[i].y1;
        if (my_min_max.max_y < my_edges[i].y1)
            my_min_max.max_y = my_edges[i].y1;

        if (my_min_max.min_y > my_edges[i].y2)
            my_min_max.min_y = my_edges[i].y2;
        if (my_min_max.max_y < my_edges[i].y2)
            my_min_max.max_y = my_edges[i].y2;
    }
}


void MyQGraphicsView::draw_borders(void)
{
    QPainter paint;
    paint.begin(&image);
    paint.setPen(border_color);
    for (int i = 0; i < my_edges.size(); i++)
    {
        paint.drawLine(my_edges[i].x1, my_edges[i].y1, my_edges[i].x2, my_edges[i].y2);
    }

    paint.end();
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);
}

int check_horizontal(edges my_edge)
{
    if (my_edge.y1 == my_edge.y2)
        return 1;
    else
        return 0;
}

void sequencing_y(edges& my_edge)
{
    if (my_edge.y1 > my_edge.y2)
    {
        my_swap(my_edge.y1, my_edge.y2);
        my_swap(my_edge.x1, my_edge.x2);
    }
}




point_str ret_point(int x, int y)
{
    point_str p;
    p.x = x;
    p.y = y;
    return p;
}

#include <stack>
void MyQGraphicsView::draw_line(QPixmap pix)
{
    pix.convertFromImage(image);
    scene->addPixmap(pix);
    scene->update();
    /*QTime end = QTime::currentTime().addMSecs(5);
    while(QTime::currentTime() < end) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }*/
    //Sleep(1000);
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

void find_zatr_pixel(std::stack<point_str>& st, int x, int y, int xr)
{
    int Fl;
    while (x <= xr)
    {
        Fl = 0;
        while (QColor(image.pixel(x, y)) != border_color &&  QColor(image.pixel(x, y)) != fill_color &&  x <= xr)
        {
            Fl = 1;
            x = x + 1;
        }

        if (Fl == 1)
        {
            if (x == xr && QColor(image.pixel(x, y)) != border_color &&  QColor(image.pixel(x, y)) != fill_color)
            {
                st.push(ret_point(x, y));
            }
            else
                st.push(ret_point(x - 1, y));
            Fl = 0;
        }

        int xt = x;
        while ((QColor(image.pixel(x, y)) == border_color ||  QColor(image.pixel(x, y)) == fill_color) && x < xr)
            x = x + 1;

        if (x == xt)
            x = x + 1;
    }
}

void MyQGraphicsView::fill_fig()
{
    QPainter paint;
    paint.begin(&image);
    paint.setPen(border_color);

    paint.drawRect(0,0,599,599);

    paint.setPen(fill_color);

    std::stack<point_str> st;
    point_str p;
    int x, xt, y, Fl, xl, xr;
    p.x = zatr_pixel.x;
    p.y = zatr_pixel.y;
    st.push(p);
    while(!st.empty())
    {
        p = st.top();
        st.pop();
        x = p.x;
        y = p.y;
        xt = p.x;
        paint.drawPoint(x,y);
        x = x - 1;
        while (QColor(image.pixel(x, y)) != border_color)
        {
            paint.drawPoint(x,y);
            x = x - 1;


        }
        xl = x + 1;
        x = xt;
        x = x + 1;

        while (QColor(image.pixel(x, y)) != border_color)
        {
            paint.drawPoint(x,y);
            x = x + 1;
        }
        xr = x - 1;

        find_zatr_pixel(st, xl, y+1, xr);

        find_zatr_pixel(st, xl, y-1, xr);
    }
    paint.end();
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);
    //qDebug() << my_min_max.max_x << " " << my_min_max.min_x << " " << my_min_max.max_y << " " << my_min_max.min_y;
}

void MainWindow::on_pushButton_clicked()
{
    if (kol_vo_points < 3)
    {
         QMessageBox::information(this, "Информация", "Введите хотя бы 3 точки!");
    }
    else
    {
        kol_vo_points = 0;
        check_short = true;
        emit short_fig_sig();
    }
}

#include <QTime>
#include <chrono>
#include <thread>
#include <windows.h>


void MyQGraphicsView::fill_zfig()
{
    QPainter paint;
    QPixmap pix;
    paint.begin(&image);

    paint.setPen(border_color);

    paint.drawRect(0,0,599,599);

    paint.setPen(fill_color);
    std::stack<point_str> st;
    point_str p;
    int x, xt, y, Fl, xl, xr;
    p.x = zatr_pixel.x;
    p.y = zatr_pixel.y;
    st.push(p);
    while(!st.empty())
    {
        p = st.top();
        st.pop();
        x = p.x;
        y = p.y;
        xt = p.x;

        paint.drawPoint(x,y);
        x = x - 1;
        while (QColor(image.pixel(x, y)) != border_color)
        {
            paint.drawPoint(x,y);
            x = x - 1;
        }
        xl = x + 1;
        x = xt;
        x = x + 1;

        while (QColor(image.pixel(x, y)) != border_color)
        {
            paint.drawPoint(x,y);
            x = x + 1;
        }
        xr = x - 1;

        find_zatr_pixel(st, xl, y+1, xr);

        find_zatr_pixel(st, xl, y-1, xr);
        pix.convertFromImage(image);
        scene->addPixmap(pix);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    paint.end();

    pix.convertFromImage(image);
    scene->addPixmap(pix);
    check_fill = true;
    //qDebug() << my_min_max.max_x << " " << my_min_max.min_x << " " << my_min_max.max_y << " " << my_min_max.min_y;
}

void MyQGraphicsView::clear_fig()
{
    image.fill(Qt::white);
    my_edges.clear();
    my_points.clear();
    new_edge.x1 = -1; new_edge.y1 = -1; new_edge.x2 = -1; new_edge.y2 = -1;
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);
    nach_index = 0;
}

void MainWindow::on_pushButton_clear_clicked()
{
    /*if (my_edges.empty())
    {
        QMessageBox::information(this, "Информация", "Список точек пуст!");
    }
    else
    {*/
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        QStringList name_table;
        name_table << "X" << "Y";
        ui->tableWidget->setHorizontalHeaderLabels(name_table);
        kol_vo_points = 0;
        check_fill = false;
        emit clear_fig_sig();
    //}
}


void MainWindow::on_pushButton_add_clicked()
{
    QString x_s = ui->lineEdit_x->text();
    QString y_s = ui->lineEdit_y->text();

    bool ok;
    int x = x_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x!");
        return;
    }
    int y = y_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y!");
        return;
    }

    QTableWidgetItem *item1, *item2;
    item1 = new QTableWidgetItem(x_s);
    item2 = new QTableWidgetItem(y_s);

    ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 0, item1);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 1, item2);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    kol_vo_points++;
    emit add_point_button_sig(x, y);
}

void MainWindow::on_pushButton_fill_2_clicked()
{
   /* if (my_edges.empty())
    {
        QMessageBox::information(this, "Ошибка", "Введите хотя бы 3 точки!");
        return;
    }*/

    //if (check_end_begin())
   // {
        //check_short = false;
        emit fill_zfig_sig();
   // }
    //else
     //   QMessageBox::information(this, "Ошибка", "Сначала замкните фигуру!");
}

void MainWindow::on_pushButton_color_line_clicked()
{
    /*if (check_fill)
    {
        QMessageBox::information(this, "Ошибка", "Сначала очистите область!");
        return;
    }*/


    QColor color = QColorDialog::getColor(border_color, this, "Choose color" );
    if(color.isValid())
    {
        border_color = color.name();
        QString qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_color_line->setStyleSheet(qss);
    }

}

void MainWindow::on_pushButton_color_fone_clicked()
{
    /*if (check_fill)
    {
        QMessageBox::information(this, "Ошибка", "Сначала очистите область!");
        return;
    }*/
    QColor color = QColorDialog::getColor(fill_color, this, "Choose color" );
    if(color.isValid())
    {
        fill_color = color.name();
        QString qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_color_fone->setStyleSheet(qss);

    }
}



void MainWindow::on_pushButton_add_2_clicked()
{
    /*if (my_edges.empty())
    {
        QMessageBox::information(this, "Ошибка", "Введите хотя бы 3 точки!");
        return;
    }*/

    QString x_s = ui->lineEdit_x_za->text();
    QString y_s = ui->lineEdit_y_za->text();

    bool ok;
    int x = x_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x!");
        return;
    }
    int y = y_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y!");
        return;
    }
    zatr_pixel.x = x;
    zatr_pixel.y = y;

    //if (check_end_begin())
    //{
        //check_short = false;
        emit fill_fig_sig();
    //}
    //else
    //    QMessageBox::information(this, "Ошибка", "Сначала замкните фигуру!");

}

void MainWindow::on_pushButton_add_3_clicked()
{
    QString x_s = ui->lineEdit_x_cir->text();
    QString y_s = ui->lineEdit_y_cir->text();
    QString r_s = ui->lineEdit_y_rad->text();
    bool ok;
    int x = x_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x!");
        return;
    }
    int y = y_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y!");
        return;
    }
    int r = r_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен r!");
        return;
    }
    emit add_circle_sig(x, y, r);
}
