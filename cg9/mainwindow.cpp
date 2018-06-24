#include "mainwindow.h"
#include <vector>
#include <QGridLayout>
#include <QGraphicsItem>
#include <qDebug>
#include <QMessageBox>

#include <QColorDialog>
#include <QColor>
#include <QShortcut>
QColor otsek_part_color(Qt::red);
QColor otsek_color(Qt::black);
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
     QString qss2 = QString("background-color: %1").arg(otsek_part_color.name());
     ui->pushButton_color_fone_2->setStyleSheet(qss2);
     connect(this , SIGNAL( short_fig_sig() ), mgw, SLOT( short_fig() ) );
     connect(this , SIGNAL( short_fig_all_sig() ), mgw, SLOT( short_fig_all() ) );

     connect( mgw, SIGNAL( send_point( edges) ), this, SLOT( get_point( edges ) ) );
     connect(this , SIGNAL( ots_fig_sig() ), mgw, SLOT( ots_fig() ) );
     connect(this , SIGNAL( clear_fig_sig() ), mgw, SLOT( clear_fig() ) );
     connect(this , SIGNAL( add_point_button_sig(int, int, int, int) ), mgw, SLOT( add_point_button(int, int, int, int) ) );
     connect(this , SIGNAL( add_otsek_sig(int, int)), mgw, SLOT( add_otsek(int, int) ) );
     connect(ui->radioButton , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox_zatr_flag() ) );
     connect(ui->radioButton_3 , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox_line() ) );
     connect(ui->checkBox_45 , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox() ) );
}






point_str zatr_pixel{0,0};
point_str center_circle{0,0};
std::vector<edges> my_edges;
std::vector<edges> my_otsek;
std::vector<point_str> otsek_points;
std::vector<point_str> fig_points;
edges new_edge{-1,-1,-1,-1};
edges otsek{-1,-1,-1,-1};
int nach_index = 0;
int nach_index2 = 0;
int check_convex = 0;
QImage image(600,600,QImage::Format_RGB32);
QImage image2(600,600,QImage::Format_RGB32);
int kol_vo_points_otsek = 0;
int kol_vo_points_fig = 0;
bool check_short = false;
bool check_fill = false;
int Normal = 1;

MyQGraphicsView::MyQGraphicsView(QWidget *parent) :
        QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setSceneRect(0, 0, 600, 600);
    this->setScene(scene);
    image.fill(Qt::white);
    image2.fill(Qt::white);
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

bool check_checkBox_flag = false;
void MyQGraphicsView::check_checkBox()
{
    check_checkBox_flag = !check_checkBox_flag;
    //qDebug() << check_checkBox_flag << "\n";
}

void angle_45(int &x2, int& y2, int x1, int y1)
{
    int x_copy = x2;
    int y_copy = y2;
    x_copy -= x1;
    y_copy -= y1;
    if (x_copy == 0)
        return;
    double tg = fabs(y_copy/x_copy);
    if (tg > tan(22.5 * M_PI / 180) && tg < tan(67.5 * M_PI / 180))
    {
        //qDebug() << x_copy << " " << y_copy;
        if (x_copy >= 0 && y_copy >= 0 || x_copy <= 0 && y_copy <= 0)
        {
            y2 =  x_copy + y1;

        }
        else
        {
            y2 =  -x_copy + y1;
            //qDebug() << y2;
        }
    }
    else if (tg < tan(22.5 * M_PI / 180))
    {
        y2 = y1;
    }
    else
    {
        x2 = x1;
    }
}

void MyQGraphicsView::mousePressEvent(QMouseEvent * e)
{
    QPen mypen(border_color);
    QBrush mybrush(Qt::black);
    QPainter paint;

    paint.begin(&image);
    paint.setPen(border_color);

    my_point = mapToScene(e->pos());

    point_str point{(int)my_point.x(), (int)my_point.y()};

    if (check_checkBox_zatr_flags)
    {

        otsek_points.push_back(point);
        kol_vo_points_otsek++;
        paint.setPen(otsek_color);
        QPen mypen_ots(otsek_color);
        if (otsek.x1 == -1)
        {
            otsek.x1 = (int)my_point.x();
            otsek.y1 = (int)my_point.y();

            scene->addEllipse(otsek.x1-2, otsek.y1-2, 4, 4, mypen_ots, mybrush);
        }
        else
        {

            otsek.x2 = (int)my_point.x();
            otsek.y2 = (int)my_point.y();
            if (check_checkBox_flag)
                angle_45(otsek.x2, otsek.y2, otsek.x1, otsek.y1);
            //correct_edges(my_edges, new_edge);

            scene->addEllipse(otsek.x2-2, otsek.y2-2, 4, 4, mypen_ots, mybrush);

            paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y2);
            scene->addLine(otsek.x1, otsek.y1, otsek.x2, otsek.y2,mypen_ots);

            my_otsek.push_back(otsek);
            emit send_point(otsek);
            otsek.x1 = otsek.x2;
            otsek.y1 = otsek.y2;
        }
        return;
    }
    fig_points.push_back(point);
    kol_vo_points_fig++;
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
        if (check_checkBox_flag)
            angle_45(new_edge.x2, new_edge.y2, new_edge.x1, new_edge.y1);
        //correct_edges(my_edges, new_edge);

        scene->addEllipse(new_edge.x2-2, new_edge.y2-2, 4, 4, mypen, mybrush);

        paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
        scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2,mypen);

        my_edges.push_back(new_edge);
        emit send_point(new_edge);
        new_edge.x1 = new_edge.x2;
        new_edge.y1 = new_edge.y2;
    }
    paint.end();


}

void MyQGraphicsView::add_otsek(int x1, int y1)
{
    kol_vo_points_otsek++;
    //qDebug() << otsek.x1 << " " << otsek.y1 << " " << otsek.x2 << " " << otsek.y2;
    QBrush mybrush(otsek_color);

    QPainter paint;
    paint.begin(&image);
    paint.setPen(otsek_color);

    QPen mypen_ots(otsek_color);
    if (otsek.x1 == -1)
    {
        otsek.x1 = x1;
        otsek.y1 = y1;
        scene->addEllipse(otsek.x1-2, otsek.y1-2, 4, 4, mypen_ots, mybrush);
    }
    else
    {

        otsek.x2 = x1;
        otsek.y2 = y1;
        if (check_checkBox_flag)
            angle_45(otsek.x2, otsek.y2, otsek.x1, otsek.y1);
        //correct_edges(my_edges, new_edge);

        scene->addEllipse(otsek.x2-2, otsek.y2-2, 4, 4, mypen_ots, mybrush);

        paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y2);
        scene->addLine(otsek.x1, otsek.y1, otsek.x2, otsek.y2,mypen_ots);

        my_otsek.push_back(otsek);
        otsek.x1 = otsek.x2;
        otsek.y1 = otsek.y2;
    }
    paint.end();
}

void MyQGraphicsView::add_point_button(int x1, int y1, int x2, int y2)
{
    QPen pen(border_color);
    QBrush brush(Qt::black);
    QPainter paint;

    paint.begin(&image);
    paint.setPen(border_color);

    new_edge.x1 = x1;
    new_edge.y1 = y1;
    scene->addEllipse(new_edge.x1-2, new_edge.y1-2, 4, 4, pen, brush);

    new_edge.x2 = x2;
    new_edge.y2 = y2;
    //correct_edges(my_edges, new_edge);
    scene->addEllipse(new_edge.x2-2, new_edge.y2-2, 4, 4, pen, brush);
    paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
    scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2,pen);

    my_edges.push_back(new_edge);
    new_edge.x1 = -1;
    new_edge.y1 = -1;

    paint.end();
}

void MainWindow::get_point(edges m_point)
{
    if (check_checkBox_zatr_flags)
    {
        //ui->lineEdit_x_za->setText(QString::number(otsek.x1));
        //ui->lineEdit_y_za->setText(QString::number(otsek.y1));
        return;
    }

    QString s1 = QString::number(m_point.x1);
    QString s2 = QString::number(m_point.y1);
    QString s3 = QString::number(m_point.x2);
    QString s4 = QString::number(m_point.y2);

    QTableWidgetItem *item1, *item2, *item3, *item4;
    item1 = new QTableWidgetItem(s1);
    item2 = new QTableWidgetItem(s2);
    item3 = new QTableWidgetItem(s3);
    item4 = new QTableWidgetItem(s4);

    ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 0, item1);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 1, item2);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 2, item3);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 3, item4);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
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


void get_t(std::vector<int> otsek_vect, int x, int y, std::vector<int>& T)
{
    T.clear();
    if (x < otsek_vect[0])
        T.push_back(1);
    else
        T.push_back(0);

    if (x > otsek_vect[1])
        T.push_back(1);
    else
        T.push_back(0);

    if (y < otsek_vect[2])
        T.push_back(1);
    else
        T.push_back(0);

    if (y > otsek_vect[3])
        T.push_back(1);
    else
        T.push_back(0);
}

void find_koef_ots(std::vector<int> otsek_vect, edges my_edge, std::vector<int>& T1, std::vector<int>& T2, int &S1, int &S2, int &P, int &wid)
{
    get_t(otsek_vect, my_edge.x1, my_edge.y1, T1);
    get_t(otsek_vect, my_edge.x2, my_edge.y2, T2);

    //qDebug() << T1[0] << " " << T1[1] << " " << T1[2] << " " << T1[3];
    //qDebug() << T2[0] << " " << T2[1] << " " << T2[2] << " " << T2[3];

    S1 = 0; S2 = 0; P = 0; wid = 0;
    for (int i = 0; i < 4; i++)
    {
       S1 += T1[i];
       S2 += T2[i];
       P += T1[i] * T2[i];
    }
    /*qDebug() << "S1 " << S1;
    qDebug() << "S2 " << S2;
    qDebug() << "P " << P;*/
    if (S1 == 0 && S2 == 0)
        wid = 1;
    else if (P != 0)
        wid = -1;
    //qDebug() << "wid " << wid;
}

double my_scalar(point_str k1, point_str k2)
{
    return k1.x * k2.x + k1.y * k2.y;
}

bool MyQGraphicsView::check_visiable_point(point_str p0, point_str p1, point_str p2)
{
    double x2 = p0.x - p1.x;
    double y2 = p0.y - p1.y;
    double x1 = p2.x - p1.x;
    double y1 = p2.y - p1.y;

    double vect_pr = x1 * y2 - x2 * y1;
    if (Normal * vect_pr >= 0)
        return true;
    else
        return false;
}

bool MyQGraphicsView::check_intersection(point_str s1, point_str s2, point_str p1, point_str p2)
{
    bool v1 = check_visiable_point(s1, p1, p2);
    bool v2 = check_visiable_point(s2, p1, p2);

    if ((v1 && !v2) || (!v1 && v2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

point_str MyQGraphicsView::find_intersection(point_str p1, point_str p2, point_str q1, point_str q2)
{
     double chisl = (q1.x - p1.x) * (q1.y - q2.y) - (q1.x - q2.x) * (q1.y - p1.y);
     double znam = (p2.x - p1.x) * (q1.y - q2.y) - (q1.x - q2.x) * (p2.y - p1.y);
     if (fabs(znam) < 0.0001)
         return p2;
     double t = chisl/znam;
     point_str res;
     res.x = p1.x + t * (p2.x - p1.x);
     res.y = p1.y + t * (p2.y - p1.y);
     return res;
}

void change_array(std::vector<point_str>& m1, std::vector<point_str>& m2)
{
    m2.clear();
    for (int i = 0; i < m1.size(); i++)
    {
        m2.push_back(m1[i]);
    }
}
void MyQGraphicsView::ots_fig()
{
    QPainter paint;
    paint.begin(&image);
    paint.setPen(Qt::white);
    QPen pen(otsek_part_color);
    //qDebug() << Normal;
    //qDebug() << otsek_points.size();
    //qDebug() << fig_points.size();
    //qDebug() << "check_visiable_point " << check_visiable_point(fig_points[0], otsek_points[0], otsek_points[1]);
    //qDebug() << "check_intersection " << check_intersection(fig_points[0],fig_points[1], otsek_points[0], otsek_points[1]);

    otsek_points.push_back(otsek_points[0]);
    std::vector<point_str> N;
    point_str F, S, I;
    for (int i = 0; i < otsek_points.size()-1; i++)
    {
        N.clear();
        for (int j = 0; j < fig_points.size(); j++)
        {
            if (j == 0)
                F = fig_points[j];
            else
            {
                if (check_intersection(S, fig_points[j], otsek_points[i], otsek_points[i+1]))
                {
                    I = find_intersection(S, fig_points[j], otsek_points[i], otsek_points[i+1]);
                    N.push_back(I);
                }
            }
            S.x = fig_points[j].x;
            S.y = fig_points[j].y;
            if (check_visiable_point(S, otsek_points[i], otsek_points[i+1]))
            {
                N.push_back(S);
            }
        }
        if (!N.empty())
        {
            if (check_intersection(S, F, otsek_points[i], otsek_points[i+1]))
            {
                I = find_intersection(S, F, otsek_points[i], otsek_points[i+1]);
                N.push_back(I);
            }
        }
        change_array(N, fig_points);
        //break;
    }

    QPainter paint2;
    paint2.begin(&image);
    if (!fig_points.empty())
    {



        pen.setWidth(2);
        //paint2.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        paint.setPen(otsek_part_color);
        for (int j = 0; j < fig_points.size()-1; j++)
        {
            scene->addLine(fig_points[j].x,fig_points[j].y,fig_points[j+1].x,fig_points[j+1].y,pen);
            //paint.drawLine(fig_points[j].x,fig_points[j].y,fig_points[j+1].x,fig_points[j+1].y);
            paint.drawLine(fig_points[j].x,fig_points[j].y,fig_points[j+1].x,fig_points[j+1].y);
        }
        //paint.drawLine(fig_points[fig_points.size()-1].x,fig_points[fig_points.size()-1].y,fig_points[0].x,fig_points[0].y);
        scene->addLine(fig_points[fig_points.size()-1].x,fig_points[fig_points.size()-1].y,fig_points[0].x,fig_points[0].y,pen);
        paint.drawLine(fig_points[fig_points.size()-1].x,fig_points[fig_points.size()-1].y,fig_points[0].x,fig_points[0].y);
        paint2.end();
    }
    paint.end();

    QPixmap pix;
    pix.convertFromImage(image);
    //scene->addPixmap(pix);
}

#include <QTime>
#include <chrono>
#include <thread>
#include <windows.h>

void MyQGraphicsView::clear_fig()
{
    image.fill(Qt::white);
    my_edges.clear();
    otsek_points.clear();
    fig_points.clear();
    my_otsek.clear();
    new_edge.x1 = -1; new_edge.y1 = -1; new_edge.x2 = -1; new_edge.y2 = -1;
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);
    nach_index = 0;
    nach_index2 = 0;
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
        name_table << "X1" << "Y2" << "X2" << "Y2";
        ui->tableWidget->setHorizontalHeaderLabels(name_table);
        kol_vo_points_otsek = 0;
        kol_vo_points_fig = 0;
        check_fill = false;
        otsek.x1 = -1; otsek.y1 = -1; otsek.x2 = -1; otsek.y2 = -1;
        new_edge.x1 = -1; new_edge.y1 = -1; new_edge.x2 = -1; new_edge.y2 = -1;
        emit clear_fig_sig();
    //}
}


void MainWindow::on_pushButton_add_clicked()
{
    QString x1_s = ui->lineEdit_x->text();
    QString y1_s = ui->lineEdit_y->text();
    QString x2_s = ui->lineEdit_x_2->text();
    QString y2_s = ui->lineEdit_y_2->text();

    bool ok;
    int x1 = x1_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x1!");
        return;
    }
    int y1 = y1_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y1!");
        return;
    }

    int x2 = x2_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x2!");
        return;
    }
    int y2 = y2_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y2!");
        return;
    }

    QTableWidgetItem *item1, *item2, *item3, *item4;
    item1 = new QTableWidgetItem(x1_s);
    item2 = new QTableWidgetItem(y1_s);
    item3 = new QTableWidgetItem(x2_s);
    item4 = new QTableWidgetItem(y2_s);

    ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 0, item1);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 1, item2);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 2, item3);
    ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1, 3, item4);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    kol_vo_points_otsek++;
    emit add_point_button_sig(x1, y1, x2, y2);
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
    QColor color = QColorDialog::getColor(otsek_color, this, "Choose color" );
    if(color.isValid())
    {
        otsek_color = color.name();
        QString qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_color_fone->setStyleSheet(qss);

    }
}


double sign(double k)
{
    if (k <= 0)
        return -1;
    else
        return 1;
}

int is_convex(void)
{
    double dx2, dy2, dx1, dy1, res;
    dx1 = my_otsek[0].x2 - my_otsek[0].x1;
    dy1 = my_otsek[0].y2 - my_otsek[0].y1;
    dx2 = my_otsek[1].x2 - my_otsek[1].x1;
    dy2 = my_otsek[1].y2 - my_otsek[1].y1;
    res = dx1*dy2 - dx2*dy1;
    double n_sign = sign(res);
    for (int i = 1; i < my_otsek.size()-1; i++)
    {
        dx1 = my_otsek[i].x2 - my_otsek[i].x1;
        dy1 = my_otsek[i].y2 - my_otsek[i].y1;
        dx2 = my_otsek[i+1].x2 - my_otsek[i+1].x1;
        dy2 = my_otsek[i+1].y2 - my_otsek[i+1].y1;
        res = dx1*dy2 - dx2*dy1;
        if (n_sign != sign(res))
        {
            qDebug() << "middle";
            return 0;
        }
    }
    int last = my_otsek.size()-1;
    qDebug() << last;
    qDebug() << my_otsek[last].x1 << " " << my_otsek[last].y1 << " " << my_otsek[last].x2 << " " << my_otsek[last].y2 << " ";
    qDebug() << my_otsek[0].x1 << " " << my_otsek[0].y1 << " " << my_otsek[0].x2 << " " << my_otsek[0].y2 << " ";
    dx1 = my_otsek[last].x2 - my_otsek[last].x1;
    dy1 = my_otsek[last].y2 - my_otsek[last].y1;
    dx2 = my_otsek[0].x2 - my_otsek[0].x1;
    dy2 = my_otsek[0].y2 - my_otsek[0].y1;
    res = dx1*dy2 - dx2*dy1;
    if (n_sign != sign(res))
    {
        qDebug() << "last";
        return 0;
    }
    return n_sign;

}



void MainWindow::on_pushButton_add_2_clicked()
{
    Normal = is_convex();
    //qDebug() << "check_convex " << check_convex;
    if (Normal != 0)
        emit ots_fig_sig();
    else
        QMessageBox::information(this, "Ошибка", "Отсекатель не выпуклый!");

}

/*
void MainWindow::on_pushButton_add_4_clicked()
{
    QString x1_s = ui->lineEdit_x_za->text();
    QString y1_s = ui->lineEdit_y_za->text();

    bool ok;
    int x1 = x1_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x1!");
        return;
    }
    int y1 = y1_s.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y1!");
        return;
    }

    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен y2!");
        return;
    }
    emit add_otsek_sig(x1, y1);
    //ui->lineEdit_x_za->setText(x2_s);
    //ui->lineEdit_y_za->setText(y2_s);


}
*/

void MainWindow::on_pushButton_color_fone_2_clicked()
{
    QColor color = QColorDialog::getColor(otsek_part_color, this, "Choose color" );
    if(color.isValid())
    {
        otsek_part_color = color.name();
        QString qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_color_fone_2->setStyleSheet(qss);

    }
}



void MyQGraphicsView::short_fig()
{
    QPen pen(otsek_color);
    QPainter paint;
    paint.begin(&image);
    paint.setPen(otsek_color);
    otsek.x2 = my_otsek[nach_index].x1;
    otsek.y2 = my_otsek[nach_index].y1;
    otsek.x1 = my_otsek[my_otsek.size()-1].x2;
    otsek.y1 = my_otsek[my_otsek.size()-1].y2;
    paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y2);
    scene->addLine(otsek.x1, otsek.y1, otsek.x2, otsek.y2, pen);
    paint.end();
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);

    nach_index = my_otsek.size()+1;
    my_otsek.push_back(otsek);
    otsek.x1 = -1; otsek.y1 = -1; otsek.x2 = -1; otsek.y2 = -1;
}

void MyQGraphicsView::short_fig_all()
{
    QPen pen(border_color);
    QPainter paint;
    paint.begin(&image);
    paint.setPen(border_color);
    new_edge.x2 = my_edges[nach_index2].x1;
    new_edge.y2 = my_edges[nach_index2].y1;
    new_edge.x1 = my_edges[my_edges.size()-1].x2;
    new_edge.y1 = my_edges[my_edges.size()-1].y2;
    paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
    scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2, pen);
    paint.end();
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);

    nach_index2 = my_edges.size()+1;
    my_edges.push_back(new_edge);
    new_edge.x1 = -1; new_edge.y1 = -1; new_edge.x2 = -1; new_edge.y2 = -1;
}

void MainWindow::on_pushButton_clicked()
{
    //замкнуть
    if (check_checkBox_zatr_flags)
    {
        if (kol_vo_points_otsek < 3)
        {
             QMessageBox::information(this, "Информация", "Введите хотя бы 3 точки!");
        }
        else
        {
            kol_vo_points_otsek = 0;
            check_short = true;
            emit short_fig_sig();
        }
    }
    else
    {
        if (kol_vo_points_fig < 3)
        {
             QMessageBox::information(this, "Информация", "Введите хотя бы 3 точки!");
        }
        else
        {
            kol_vo_points_fig = 0;
            check_short = true;
            emit short_fig_all_sig();
        }
    }
}
