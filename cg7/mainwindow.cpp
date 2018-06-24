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
     connect( mgw, SIGNAL( send_point( edges) ), this, SLOT( get_point( edges ) ) );
     connect(this , SIGNAL( ots_fig_sig() ), mgw, SLOT( ots_fig() ) );
     connect(this , SIGNAL( clear_fig_sig() ), mgw, SLOT( clear_fig() ) );
     connect(this , SIGNAL( add_point_button_sig(int, int, int, int) ), mgw, SLOT( add_point_button(int, int, int, int) ) );
     connect(this , SIGNAL( add_otsek_sig(int, int, int, int)), mgw, SLOT( add_otsek(int, int, int, int) ) );
     connect(ui->radioButton , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox_zatr_flag() ) );
     connect(ui->radioButton_3 , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox_line() ) );
     connect(ui->checkBox_45 , SIGNAL( clicked(bool)), mgw, SLOT( check_checkBox() ) );
}




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
edges otsek{-1,-1,-1,-1};
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
    if (check_checkBox_zatr_flags)
    {
        if (otsek.x1 != -1 && otsek.y1 != -1 && otsek.x2 != -1 && otsek.y2 != -1)
        {
            QPen mypen1(Qt::white);
            QBrush mybrush1(Qt::white);
            scene->addEllipse(otsek.x1-2, otsek.y1-2, 4, 4, mypen1, mybrush1);
            scene->addEllipse(otsek.x2-2, otsek.y2-2, 4, 4, mypen1, mybrush1);
            //paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1);
            scene->addLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1, mypen1);

            //paint.drawLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2);
            scene->addLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2, mypen1);

            //paint.drawLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2);
            scene->addLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2, mypen1);

            //paint.drawLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1);
            scene->addLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1, mypen1);
            otsek.x1 = -1; otsek.y1 = -1; otsek.x2 = -1; otsek.y2 = -1;
        }


        if (otsek.x1 == -1)
        {
            otsek.x1 = (int)my_point.x();
            otsek.y1 = (int)my_point.y();
            scene->addEllipse(otsek.x1-2, otsek.y1-2, 4, 4, mypen, mybrush);
        }
        else
        {
            QPen mypen1(otsek_color);
            otsek.x2 = (int)my_point.x();
            otsek.y2 = (int)my_point.y();
            //correct_edges(my_edges, new_edge);

            scene->addEllipse(otsek.x2-2, otsek.y2-2, 4, 4, mypen, mybrush);

            //paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1);
            scene->addLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1, mypen1);

            //paint.drawLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2);
            scene->addLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2, mypen1);

            //paint.drawLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2);
            scene->addLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2, mypen1);

            //paint.drawLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1);
            scene->addLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1, mypen1);

            emit send_point(otsek);
        }
        return;
    }
    my_points.push_back(my_point);

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
        if (check_checkBox_flag)
            angle_45(new_edge.x2, new_edge.y2, new_edge.x1, new_edge.y1);
        //correct_edges(my_edges, new_edge);

        scene->addEllipse(new_edge.x2-2, new_edge.y2-2, 4, 4, mypen, mybrush);

        paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
        scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2,mypen);

        my_edges.push_back(new_edge);
        emit send_point(new_edge);
        new_edge.x1 = -1;
        new_edge.y1 = -1;
    }
    paint.end();


}

void MyQGraphicsView::add_otsek(int x1, int y1, int x2, int y2)
{
    //qDebug() << otsek.x1 << " " << otsek.y1 << " " << otsek.x2 << " " << otsek.y2;
    QPen mypen(otsek_color);
    QBrush mybrush(otsek_color);

    QPainter paint;
    paint.begin(&image);
    paint.setPen(otsek_color);

    if (otsek.x1 != -1 && otsek.y1 != -1 && otsek.x2 != -1 && otsek.y2 != -1)
    {
        QPen mypen1(Qt::white);
        QBrush mybrush1(Qt::white);
        scene->addEllipse(otsek.x1-2, otsek.y1-2, 4, 4, mypen1, mybrush1);
        scene->addEllipse(otsek.x2-2, otsek.y2-2, 4, 4, mypen1, mybrush1);
        //paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1);
        scene->addLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1, mypen1);

        //paint.drawLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2);
        scene->addLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2, mypen1);

        //paint.drawLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2);
        scene->addLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2, mypen1);

        //paint.drawLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1);
        scene->addLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1, mypen1);
        otsek.x1 = -1; otsek.y1 = -1; otsek.x2 = -1; otsek.y2 = -1;
    }

    otsek.x1 = x1;
    otsek.y1 = y1;
    otsek.x2 = x2;
    otsek.y2 = y2;

    scene->addEllipse(otsek.x1-2, otsek.y1-2, 4, 4, mypen, mybrush);
    scene->addEllipse(otsek.x2-2, otsek.y2-2, 4, 4, mypen, mybrush);

    //paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1);
    scene->addLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1, mypen);

    //paint.drawLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2);
    scene->addLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2, mypen);

    //paint.drawLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2);
    scene->addLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2, mypen);

    //paint.drawLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1);
    scene->addLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1, mypen);

    //paint.drawEllipse(x-r/2, y-r/2, r, r);
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
        ui->lineEdit_x_za->setText(QString::number(otsek.x1));
        ui->lineEdit_y_za->setText(QString::number(otsek.y1));
        ui->lineEdit_x_za_2->setText(QString::number(otsek.x2));
        ui->lineEdit_y_za_2->setText(QString::number(otsek.y2));
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
void MyQGraphicsView::ots_fig()
{
    QPainter paint;
    paint.begin(&image);
    paint.setPen(otsek_part_color);
    int flag;
    double m = 1;


    if (otsek.x1 > otsek.x2)
        my_swap(otsek.x1, otsek.x2);
    if (otsek.y1 > otsek.y2)
        my_swap(otsek.y1, otsek.y2);

    std::vector<int> otsek_vect;
    otsek_vect.push_back(otsek.x1);
    otsek_vect.push_back(otsek.x2);
    otsek_vect.push_back(otsek.y1);
    otsek_vect.push_back(otsek.y2);

    QPen this_pts(otsek_color);
    QBrush this_bts(Qt::white);
    scene->addRect(otsek_vect[0],otsek_vect[2],otsek_vect[1] - otsek_vect[0], otsek_vect[3] - otsek_vect[2], this_pts, this_bts);

    std::vector<int> T1; std::vector<int> T2; int S1; int S2; int P; int wid;
    qDebug() << otsek_vect[0] << " " << otsek_vect[1] << " " << otsek_vect[2] << " " << otsek_vect[3];

    paint.drawLine(otsek.x1, otsek.y1, otsek.x2, otsek.y1);
    paint.drawLine(otsek.x2, otsek.y1, otsek.x2, otsek.y2);
    paint.drawLine(otsek.x2, otsek.y2, otsek.x1, otsek.y2);
    paint.drawLine(otsek.x1, otsek.y2, otsek.x1, otsek.y1);


    QPen this_pen(otsek_part_color);

    for (int i = 0; i < my_edges.size(); i++)
    {
        flag = 0;
        m = 1;
        if (my_edges[i].x2 - my_edges[i].x1 == 0)
            flag = -1;
        else
        {
            m = ((double)my_edges[i].y2 - my_edges[i].y1) / ((double)my_edges[i].x2 - my_edges[i].x1);

            if (m == 0)
                flag = 1;
        }
        for (int j = 0; j < 4; j++)
        {

            find_koef_ots(otsek_vect, my_edges[i], T1, T2, S1, S2, P, wid);
            if (wid == -1)
            {
                break;
            }
            if (wid == 1)
            {
                break;
            }
            if (T1[j] == T2[j])
                continue;
            if (T1[j] == 0)
            {
                my_swap(my_edges[i].x1, my_edges[i].x2);
                my_swap(my_edges[i].y1, my_edges[i].y2);
            }
            if (flag != -1)
            {
                if (j < 2)
                {

                    my_edges[i].y1 = (int)round(m*(otsek_vect[j] - my_edges[i].x1) + my_edges[i].y1);
                    my_edges[i].x1 = otsek_vect[j];
                    continue;
                }
                else
                {
                    my_edges[i].x1 = (int)round((1/m)*(otsek_vect[j] - my_edges[i].y1) + my_edges[i].x1);
                }
            }
            my_edges[i].y1 = otsek_vect[j];
        }
        if (wid == -1)
            continue;
        scene->addLine(my_edges[i].x1, my_edges[i].y1, my_edges[i].x2, my_edges[i].y2, this_pen);
        //paint.drawLine(my_edges[i].x1, my_edges[i].y1, my_edges[i].x2, my_edges[i].y2);
    }
    paint.end();
    //QPixmap pix;
    //pix.convertFromImage(image);
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
        name_table << "X1" << "Y2" << "X2" << "Y2";
        ui->tableWidget->setHorizontalHeaderLabels(name_table);
        kol_vo_points = 0;
        check_fill = false;
        otsek.x1 = -1; otsek.y1 = -1; otsek.x2 = -1; otsek.y2 = -1;
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
    kol_vo_points++;
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



void MainWindow::on_pushButton_add_2_clicked()
{
    /*if (my_edges.empty())
    {
        QMessageBox::information(this, "Ошибка", "Введите хотя бы 3 точки!");
        return;
    }*/

    //if (check_end_begin())
    //{
        //check_short = false;
        emit ots_fig_sig();
    //}
    //else
    //    QMessageBox::information(this, "Ошибка", "Сначала замкните фигуру!");

}


void MainWindow::on_pushButton_add_4_clicked()
{
    QString x1_s = ui->lineEdit_x_za->text();
    QString y1_s = ui->lineEdit_y_za->text();
    QString x2_s = ui->lineEdit_x_za_2->text();
    QString y2_s = ui->lineEdit_y_za_2->text();

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


    emit add_otsek_sig(x1, y1, x2, y2);
}


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
