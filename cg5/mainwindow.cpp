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
     //new QShortcut(QKeySequence(Qt::SHIFT), mgw, SLOT(angle()));
     /*QAction *foo = new QAction(this);
     foo->setShortcut(Qt::Key_Q | Qt::CTRL);

     connect(foo, SIGNAL(triggered()), this, SLOT(angle()));
     this->addAction(foo);*/
     //keyF11 = new QShortcut(this);   // Инициализируем объект
     //keyF11->setKey(QKeySequence("Shift"));    // Устанавливаем код клавиши
     // цепляем обработчик нажатия клавиши
     //connect(keyF11, SIGNAL(activated()), this, SLOT(angle()));
}


typedef struct edges_s
{
    int x1;
    int y1;
    int x2;
    int y2;
}edges;
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


bool check_checkBox_flag = false;
void MyQGraphicsView::check_checkBox()
{
    check_checkBox_flag = !check_checkBox_flag;
    //qDebug() << check_checkBox_flag << "\n";
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



void MyQGraphicsView::mousePressEvent(QMouseEvent * e)
{
    QPen mypen(border_color);
    QBrush mybrush(Qt::black);
    my_point = mapToScene(e->pos());
    //correct_coord(my_points, my_point);
    my_points.push_back(my_point);

    QPainter paint;
    paint.setPen(border_color);
    paint.begin(&image);

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

void MyQGraphicsView::add_point_button(int x, int y)
{
    QPen pen(border_color);
    QBrush brush(Qt::black);
    QPainter paint;
    paint.setPen(border_color);
    paint.begin(&image);


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
    if (my_edges[0].x1 == my_edges[my_edges.size()-1].x2 && my_edges[0].y1 == my_edges[my_edges.size()-1].y2)
        return 1;
    else
        return 0;
}
void MainWindow::on_pushButton_fill_clicked()
{
    if (my_edges.empty())
    {
        QMessageBox::information(this, "Ошибка", "Введите хотя бы 3 точки!");
        return;
    }

    if (check_short || check_end_begin())
    {
        check_short = false;
        emit fill_fig_sig();
    }
    else
        QMessageBox::information(this, "Ошибка", "Сначала замкните фигуру!");

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
    correct_edges(my_edges, new_edge);
    paint.drawLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2);
    scene->addLine(new_edge.x1, new_edge.y1, new_edge.x2, new_edge.y2, pen);
    paint.end();


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
/*
void MyQGraphicsView::fill_fig()
{
    QPainter paint;
    paint.begin(&image);
    paint.setPen(Qt::red);
    min_max(my_edges);
    int flag;
    int check_line;
    QColor my_color(Qt::black);
    for (int y = my_min_max.max_y; y > my_min_max.min_y; y--)
    {
        flag = 0;
        check_line = 1;
        for (int x = my_min_max.min_x; x < my_min_max.max_x; x++)
        {
            if (QColor(image.pixel(x, y)) == my_color)
            {
                if (!check_line)
                    continue;
                flag = !flag;
                check_line = 0;
                continue;
            }
            else
               check_line = 1;
            if (flag && check_line)
                paint.drawPoint(x,y);
        }
    }
    paint.end();
    QPixmap pix;
    pix.convertFromImage(image);
    scene->addPixmap(pix);
    qDebug() << my_min_max.max_x << " " << my_min_max.min_x << " " << my_min_max.max_y << " " << my_min_max.min_y;
}
*/

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

void MyQGraphicsView::fill_fig()
{
    QPainter paint;
    paint.begin(&image);
    paint.setPen(fill_color);
    min_max(my_edges);
    double x_start;
    double d_x;
    QColor cur_color;
    QPixmap pix;
    for (int i = 0; i < my_edges.size(); i++)
    {
        if (check_horizontal(my_edges[i]))
            continue;

        sequencing_y(my_edges[i]);

        x_start = my_edges[i].x1;
        d_x = ((double)my_edges[i].x2 - (double)my_edges[i].x1) / ((double)my_edges[i].y2 - (double)my_edges[i].y1);

        for (int y = my_edges[i].y1; y < my_edges[i].y2; y++)
        {
            for (int x = (int)round(x_start); x < my_min_max.max_x; x++)
            {
                cur_color =  QColor(image.pixel(x, y));
                if (cur_color == fone_color)
                {
                    paint.setPen(fill_color);
                }
                if (cur_color == fill_color)
                {
                    paint.setPen(fone_color);
                }
                paint.drawPoint(x,y);
            }
            x_start += d_x;
            //qDebug() << "dx x_start"<< d_x <<"  "<< x_start;
        }
    }
    paint.end();

    pix.convertFromImage(image);
    scene->addPixmap(pix);
    draw_borders();
    check_fill = true;
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
void MyQGraphicsView::fill_zfig()
{
    QPainter paint;
    paint.begin(&image);
    paint.setPen(fill_color);
    min_max(my_edges);
    double x_start;
    double d_x;
    QColor cur_color;
    QPixmap pix;
    for (int i = 0; i < my_edges.size(); i++)
    {
        if (my_edges[i].y1 == my_edges[i].y2)
        {
            continue;
        }

        if (my_edges[i].y1 > my_edges[i].y2)
        {
            my_swap(my_edges[i].y1, my_edges[i].y2);
            my_swap(my_edges[i].x1, my_edges[i].x2);
        }

        x_start = my_edges[i].x1;
        d_x = ((double)my_edges[i].x2 - (double)my_edges[i].x1) / ((double)my_edges[i].y2 - (double)my_edges[i].y1);

        for (int y = my_edges[i].y1; y < my_edges[i].y2; y++)
        {

            for (int x = (int)round(x_start); x < my_min_max.max_x; x++)
            {
                cur_color =  QColor(image.pixel(x, y));
                if (cur_color == fone_color)
                {
                    paint.setPen(fill_color);
                }
                if (cur_color == fill_color)
                {
                    paint.setPen(fone_color);
                }
                paint.drawPoint(x,y);
            }
            //QCoreApplication::processEvents(QEventLoop::AllEvents, 100000);

            //QCoreApplication::processEvents(QEventLoop::AllEvents, 100);


            draw_line(pix);
            //Sleep(1000);
            //
            x_start += d_x;
            //qDebug() << "dx x_start"<< d_x <<"  "<< x_start;
        }

    }
    paint.end();


    draw_borders();
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
    if (my_edges.empty())
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
        kol_vo_points = 0;
        check_fill = false;
        emit clear_fig_sig();
    }
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
    if (my_edges.empty())
    {
        QMessageBox::information(this, "Ошибка", "Введите хотя бы 3 точки!");
        return;
    }

    if (check_short || check_end_begin())
    {
        check_short = false;
        emit fill_zfig_sig();
    }
    else
        QMessageBox::information(this, "Ошибка", "Сначала замкните фигуру!");
}

void MainWindow::on_pushButton_color_line_clicked()
{
    if (check_fill)
    {
        QMessageBox::information(this, "Ошибка", "Сначала очистите область!");
        return;
    }


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
    if (check_fill)
    {
        QMessageBox::information(this, "Ошибка", "Сначала очистите область!");
        return;
    }
    QColor color = QColorDialog::getColor(fill_color, this, "Choose color" );
    if(color.isValid())
    {
        fill_color = color.name();
        QString qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_color_fone->setStyleSheet(qss);

    }
}


