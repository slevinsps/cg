#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <vector>
#include "math.h"
#include <QMessageBox>
#include <algorithm>
#define SCREEN_H 600
#define SCREEN_W 600

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, SCREEN_W, SCREEN_H);
    ui->graphicsView->setScene(scene);
}


MainWindow::~MainWindow()
{
    delete ui;
}

int sign(double k)
{
    if (k < 0)
        return -1;
    if (k > 0)
        return 1;
    if (k == 0)
        return 0;
}


double func0(double x, double z)
{
    return cos(x)*cos(z);
}

double func1(double x, double z)
{
    return x*z / (sqrt(x*x + z*z));
}
double func2(double x, double z)
{
    return cos(x+z);
}
double func3(double x, double z)
{
    return cos(x) + cos(z);
}


void check_vis(double x, double y, std::vector<double> up_hor, std::vector<double> down_hor, int& flag)
{

    int x_int = (int)round(x);
    if (x_int >= down_hor.size() || x_int < 0)
    {
        //qDebug() << "error 1 in check_vis";
        return;
    }
    if (y < up_hor[x_int] && y > down_hor[x_int])
        flag = 0;
    else if (y >= up_hor[x_int])
        flag = 1;
    else if (y <= down_hor[x_int])
        flag = -1;
    //qDebug() << "y " << y << " up_hor[x_int] " << up_hor[x_int] << " down_hor[x_int] " << down_hor[x_int] << " flag " << flag;
}

double my_max(double a1, double a2)
{
    if (a1 >= a2)
        return a1;
    else
        return a2;
}

double my_min(double a1, double a2)
{
    if (a1 <= a2)
        return a1;
    else
        return a2;
}

// рабочая версия
void MainWindow::horizont(double x1, double y1, double x2, double y2, std::vector<double>& up_hor, std::vector<double>& down_hor, int delta)
{
    QPen pen(Qt::black);
    int x1_int = (int)round(x1);
    int x2_int = (int)round(x2);

    double dx = fabs(x2 - x1);
    double dy = fabs(y2 - y1);

    if (dx == 0 && dy == 0)
    {
        if (y1 >= up_hor[x1_int])
        {
            up_hor[x1_int] = y1;
            scene->addLine(x1, y1, x1, y1, pen);
        }

        if (y1 <= down_hor[x1_int])
        {
            down_hor[x1_int] = y1;
            scene->addLine(x1, y1, x1, y1, pen);
        }
        return;
    }

    double l;
    if (dx > dy)
        l = dx;
    else
        l = dy;
    double sx = (x2 - x1)/l;
    double sy = (y2 - y1)/l;
    double x = x1;
    double y = y1;
    //qDebug() << l;
    for (int i = 1; i <= l+delta; i++)
    {
         int x_int = (int)round(x);
         int y_int = (int)round(y);
         if (y >= up_hor[x_int])
         {
             up_hor[x_int] = y;
             scene->addLine(x_int, y_int, x_int, y_int, pen);
         }
         if (y <= down_hor[x_int])
         {
             down_hor[x_int] = y;
             scene->addLine(x_int, y_int, x_int, y_int, pen);
         }
         x += sx;
         y += sy;
    }


    /*double k = (y2 - y1)/(x2 - x1);

    double yi = y1;
    double xi = x1;
    for (int x_func = x1_int; x_func <= x2_int; x_func++)
    {
        if (yi >= up_hor[x_func])
        {
            up_hor[x_func] = yi;
            scene->addLine(x_func, yi, x_func, yi, pen);
        }
        if (yi <= down_hor[x_func])
        {
            down_hor[x_func] = yi;
            scene->addLine(x_func, yi, x_func, yi, pen);
        }
        yi += k;
    }*/
    //qDebug() << "c_sign " <<c_sign;
    //if (fabs(yi - k - hor[(int)roundf(xi) - 1]) <= fabs(yi - hor[(int)roundf(xi)]))
    //{
    //    yi = yi - k;
    //    xi = xi - 1;
    //}
}


/*//  рабочая вещь
void MainWindow::horizont(double x1, double y1, double x2, double y2, std::vector<double>& up_hor, std::vector<double>& down_hor)
{
    QPen pen(Qt::black);
    double x = x1;
    double y = y1;
    double dx = x2 - x1;
    double dy = y2 - y1;
    int sx = sign(dx);
    int sy = sign(dy);
    dx = fabs(dx);
    dy = fabs(dy);



    if (dx == 0 && dy == 0)
    {
        if (y >= up_hor[(int)roundf(x)])
        {
            up_hor[(int)roundf(x)] = y;
            scene->addLine(x, y, x, y, pen);
        }

        if (y <= down_hor[(int)roundf(x)])
        {
            down_hor[(int)roundf(x)] = y;
            scene->addLine(x, y, x, y, pen);
        }
        return;
    }
    if (dx == 0)
    {

    }

    double change = 0;
    if (dy > dx)
    {
        double tmp = dy;
        dy = dx;
        dx = tmp;
        change = 1;
    }

    double y_max_curr = up_hor[x];
    double y_min_curr = down_hor[x];
    double e = 2 * dy - dx;

    int i = 1;
    while (i <= dx)
    {
        if (0 <= x < 600)
        {
            if (y >= up_hor[x])
            {
                if (y >= y_max_curr)
                    y_max_curr = y;
                scene->addLine(x, y, x, y, pen);
            }

            if (y <= down_hor[x])
            {
                if (y <= y_min_curr)
                    y_min_curr = y;
                scene->addLine(x, y, x, y, pen);
            }
        }

        if (e >= 0)
        {
            if (change)
            {
                up_hor[x] = y_max_curr;
                down_hor[x] = y_min_curr;
                x += sx;
                y_max_curr = up_hor[x];
                y_min_curr = down_hor[x];
            }
            else
                y += sy;
            e -= 2 * dx;
        }
        if (e < 0)
        {
            if (!change)
            {
                up_hor[x] = y_max_curr;
                down_hor[x] = y_min_curr;

                x += sx;

                y_max_curr = up_hor[x];
                y_min_curr = down_hor[x];
            }

            else
                y += sy;

            e += 2 * dy;
        }

        i += 1;
    }
}
*/

void MainWindow::treatment_rib(double x, double y, double& x_rib, double& y_rib, std::vector<double>& up_hor, std::vector<double>& down_hor)
{

    if (x_rib != -1)
        horizont(x_rib, y_rib, x, y, up_hor, down_hor, 1);
    x_rib = x;
    y_rib = y;

}

void find_intersection(double x1, double y1, double x2, double y2, std::vector<double>& hor, double& xi, double& yi)
{

    /*xi = x2;
    yi = y2;
    return;*/


    int x1_int = (int)round(x1);
    int x2_int = (int)round(x2);
    /*
    if (x2_int >= hor.size() || x2_int < 0)
    {
        return;
    }
    if (x1_int >= hor.size() || x1_int < 0)
    {
        return;
    }
    if (x2 - x1 == 0)
    {
        xi = x2;
        yi = hor[x2_int];
        return;
    }

    double k1 = (hor[x2_int] - hor[x1_int]) / (x2 - x1);
    //double b1 = (x2*hor[x1_int] - x1*hor[x2_int]) / (x2 - x1);
    double b1 = hor[x2_int] - k1 * x2;
    double k2 = (y2 - y1) / (x2 - x1);
    //double b2 = (x2 * y1 - x1 * y2) / (x2 - x1);
    double b2 = y2 - k2 * x2;
    if (k1 - k2 == 0)
    {
        return;
    }
    xi = (b2 - b1) / (k1 - k2);
    yi = k2*(xi-x1)+y1;
    */


    //qDebug() << "k1: " << k1 << " b1: " << b1 << " k2: " << k2 << " b2: " << b2;
    //qDebug() << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << "hor[x1_int] " << hor[x1_int] << "hor[x2_int] " << hor[x2_int];
    //qDebug() << xi << " " << yi;

    if (x2 - x1 == 0)
    {
        xi = x2;
        yi = hor[x2_int];
        return;
    }
    double mp =  (hor[x2_int] - hor[x1_int])/(x2 - x1);
    double mc =  (y2 - y1)/(x2 - x1);
    if (mp - mc ==  0)
    {
        return;
    }
    xi = (y1 - hor[x1_int])/(mp - mc) + x1;
    yi = mc*(xi-x1)+y1;


    //}

}

double rot_x(double& x, double& y, double& z, double teta)
{
    teta = teta * M_PI / 180;
    double buf = y;
    y = cos(teta) * y - sin(teta) * z;
    z = cos(teta) * z + sin(teta) * buf;
}

double rot_y(double& x, double& y, double& z, double teta)
{
    teta = teta * M_PI / 180;
    double buf = x;
    x = cos(teta) * x - sin(teta) * z;
    z = cos(teta) * z + sin(teta) * buf;
}

double rot_z(double& x, double& y, double& z, double teta)
{
    teta = teta * M_PI / 180;
    double buf = x;
    x = cos(teta) * x - sin(teta) * y;
    y = cos(teta) * y + sin(teta) * buf;
}

void scale(double& coord, double k, double m)
{
    coord = coord * k + m;
}

double K_SCALE = 30;
double M_SCALE = 300;

typedef struct rot_com_s
{
    int num_com;
    double degree;
}rot_com;

std::vector<rot_com> rot_arr;

void transf(double x, double y, double z, double& x_c, double& y_c, double& z_c, double tx, double ty, double tz)
{
    double m = M_SCALE;
    double k = K_SCALE;
    x_c = x;
    y_c = y;
    z_c = z;
    for (int i = 0; i < rot_arr.size(); i++)
    {
        if (rot_arr[i].num_com == 1)
        {
            rot_x(x_c, y_c, z_c, rot_arr[i].degree);
        }
        if (rot_arr[i].num_com == 2)
        {
            rot_y(x_c, y_c, z_c, rot_arr[i].degree);
        }
        if (rot_arr[i].num_com == 3)
        {
            rot_z(x_c, y_c, z_c, rot_arr[i].degree);
        }
    }

    scale(x_c, k, m);
    scale(y_c, k, m);
}

void transf(double& x_c, double& y_c, double z_c, double tx, double ty, double tz)
{
    double k = K_SCALE;
    double m = M_SCALE;
    for (int i = 0; i < rot_arr.size(); i++)
    {
        if (rot_arr[i].num_com == 1)
        {
            rot_x(x_c, y_c, z_c, rot_arr[i].degree);
        }
        if (rot_arr[i].num_com == 2)
        {
            rot_y(x_c, y_c, z_c, rot_arr[i].degree);
        }
        if (rot_arr[i].num_com == 3)
        {
            rot_z(x_c, y_c, z_c, rot_arr[i].degree);
        }
    }
    scale(x_c, k, m);
    scale(y_c, k, m);
}



void change_hor(double x1, double y1, double x2, double y2, std::vector<double>& up_hor, std::vector<double>& down_hor)
{
    int x1_int = (int)round(x1);
    int x2_int = (int)round(x2);


    if (x2_int >= up_hor.size() || x2_int >= down_hor.size() || x2_int < 0)
    {
        //qDebug() << "error 1 in change_hor";
        return;
    }
    if (x1_int >= up_hor.size() || x1_int >= down_hor.size()  || x1_int < 0)
    {
        //qDebug() << "error 2 in change_hor" << x1_int;
        return;
    }
    double dx = fabs(x2 - x1);
    double dy = fabs(y2 - y1);

    if (dx == 0 && dy == 0)
    {
        if (y1 >= up_hor[x1_int])
        {
            up_hor[x1_int] = y1;
        }

        if (y1 <= down_hor[x1_int])
        {
            down_hor[x1_int] = y1;
        }
        return;
    }

    double l;
    if (dx > dy)
        l = dx;
    else
        l = dy;
    double sx = (x2 - x1)/l;
    double sy = (y2 - y1)/l;
    double x = x1;
    double y = y1;
    for (int i = 1; i <= l; i++)
    {
         int x_int = (int)round(x);
         if (y > up_hor[x_int])
         {
             up_hor[x_int] = y;
         }
         if (y < down_hor[x_int])
         {
             down_hor[x_int] = y;
         }
         x += sx;
         y += sy;
    }

}

void MainWindow::draw_all(double x_minn, double x_maxx, double d_xx, double z_minn, double z_maxx, double d_zz, double tx, double ty, double tz, myfunc func)
{
    QPen pen(Qt::black);

    double screen_height = SCREEN_H;
    double screen_width = SCREEN_W;
    std::vector<double> up_hor;
    std::vector<double> down_hor;
    double x_min = x_minn, x_max = x_maxx, d_x = d_xx;
    double z_min = z_minn, z_max = z_maxx, d_z = d_zz;
    double x_left = -1, x_right = -1, y_left = -1, y_right = -1;

    for (int i = 0; i < screen_width; i++)
    {
        up_hor.push_back(0);
        down_hor.push_back(screen_height);
    }

    double z = z_max;
    double x, y, z_buf;
    double x_c, y_c, z_c;
    double x_pred, y_pred, xi = 0, yi = 0;
    int pred_flag = 0, cur_flag = 0;
    while (z >= z_min)
    {
        x_pred = x_min;
        y_pred = func(x_min, z);
        z_buf = z;
        transf(x_pred, y_pred, z_buf, tx, ty, tz);
        if (x_left == -1)
        {
            x_left = x_pred;
            y_left = y_pred;
        }
        else
        {
            if (x_pred >= 5 && x_pred <= screen_width-5)
            {
                horizont(x_left, y_left, x_pred, y_pred, up_hor, down_hor, 1);
                //find_intersection(x_pred, y_pred, x_c, y_c, up_hor, xi, yi);
                //scene->addLine(x_left, y_left, x_pred, y_pred, pen);
                //change_hor(x_left, y_left, x_pred, y_pred, up_hor, down_hor);
            }
            x_left = x_pred;
            y_left = y_pred;
        }
        //check_vis(x_pred, y_pred, up_hor, down_hor, pred_flag);
        //change_hor(x_pred, y_pred, up_hor, down_hor);
        up_hor[0] = std::max(up_hor[0], y_pred);
        down_hor[0] = std::min(down_hor[0], y_pred);
        x = x_min + d_x;
        while (x <= x_max)
        {
            y = func(x, z);
            transf(x, y, z, x_c, y_c, z_c, tx, ty, tz);
            if (x_c < 5 || x_c > screen_width-5 || x_pred < 5 || x_pred > screen_width-5)
            {
                x_pred = x_c;
                y_pred = y_c;
                x += d_x;
                continue;
            }
            check_vis(x_pred, y_pred, up_hor, down_hor, pred_flag);
            check_vis(x_c, y_c, up_hor, down_hor, cur_flag);

            //qDebug() << "x_c " << x_c << " " << " y_c " << y_c << " pred_flag " << pred_flag << " cur_flag " << cur_flag << " up_hor " << up_hor[(int)round(x_c)] << " down_hor " << down_hor[(int)round(x_c)];
            //qDebug() << "x_c " << x_c << " " << " y_c " << y_c << " x_pred " << x_pred << " y_pred " << y_pred;
            if (cur_flag == pred_flag)
            {
                if (cur_flag == 1 || cur_flag == -1)
                {
                    scene->addLine((int)round(x_pred), (int)round(y_pred), (int)round(x_c), (int)round(y_c), QPen(pen));
                    change_hor(x_pred, y_pred, x_c, y_c, up_hor, down_hor);
                }
            }
            else
            {
                if (cur_flag == 0)
                {
                    if (pred_flag == 1)
                    {
                        find_intersection(x_pred, y_pred, x_c, y_c, up_hor, xi, yi);

                    }
                    else if (pred_flag == -1)
                        find_intersection(x_pred, y_pred, x_c, y_c, down_hor, xi, yi);
                    scene->addLine((int)round(x_pred), (int)round(y_pred), (int)round(xi), (int)round(yi), pen);
                    change_hor(x_pred, y_pred, xi, yi, up_hor, down_hor);

                }
                else if (cur_flag == 1)
                {
                    if (pred_flag == 0)
                    {
                        find_intersection(x_pred, y_pred, x_c, y_c, up_hor, xi, yi);
                        //scene->addLine(xi, yi, x_c, y_c, pen);
                        scene->addLine((int)round(xi), (int)round(yi), (int)round(x_c), (int)round(y_c), pen);
                        change_hor(xi, yi, x_c, y_c, up_hor, down_hor);
                    }
                    else
                    {
                        find_intersection(x_pred, y_pred, x_c, y_c, down_hor, xi, yi);
                        //scene->addLine(x_pred, y_pred, xi, yi, pen);
                        scene->addLine((int)round(x_pred), (int)round(y_pred), (int)round(xi), (int)round(yi), pen);
                        change_hor(x_pred, y_pred, xi, yi, up_hor, down_hor);

                        find_intersection(x_pred, y_pred, x_c, y_c, up_hor, xi, yi);
                        //scene->addLine(xi, yi, x_c, y_c, pen);
                        scene->addLine((int)round(xi), (int)round(yi), (int)round(x_c), (int)round(y_c), pen);
                        change_hor(xi, yi, x_c, y_c, up_hor, down_hor);
                    }
                }
                else
                {
                    if (pred_flag == 0)
                    {
                        find_intersection(x_pred, y_pred, x_c, y_c, down_hor, xi, yi);
                        //scene->addLine(xi, yi, x_c, y_c, pen);
                        scene->addLine((int)round(xi), (int)round(yi), (int)round(x_c), (int)round(y_c), pen);
                        change_hor(xi, yi, x_c, y_c, up_hor, down_hor);
                    }
                    else
                    {
                        find_intersection(x_pred, y_pred, x_c, y_c, up_hor, xi, yi);
                        //scene->addLine(x_pred, y_pred, xi, yi, pen);
                        scene->addLine((int)round(x_pred), (int)round(y_pred), (int)round(xi), (int)round(yi), pen);
                        change_hor(x_pred, y_pred, xi, yi, up_hor, down_hor);

                        find_intersection(x_pred, y_pred, x_c, y_c, down_hor, xi, yi);
                        //scene->addLine(xi, yi, x_c, y_c, pen);
                        scene->addLine((int)round(xi), (int)round(yi), (int)round(x_c), (int)round(y_c), pen);
                        change_hor(xi, yi, x_c, y_c, up_hor, down_hor);
                    }
                }
            }
            //pred_flag = cur_flag;
            x_pred = x_c;
            y_pred = y_c;
            x += d_x;
        }
        if (x_right == -1)
        {
            x_right = x_pred;
            y_right = y_pred;
        }
        else
        {

            if (x_pred >= 5 && x_pred <= screen_width-5)
            {
                horizont(x_right, y_right, x_pred, y_pred, up_hor, down_hor, 1);
                //scene->addLine(x_right, y_right, x_pred, y_pred, pen);
                //change_hor(x_right, y_right, x_pred, y_pred, up_hor, down_hor);
            }
            x_right = x_pred;
            y_right = y_pred;
        }
        z -= d_z;
    }
}


/*// рабочая версия
void MainWindow::draw_all(double x_minn, double x_maxx, double d_xx, double z_minn, double z_maxx, double d_zz, double tx, double ty, double tz, myfunc func)
{
    QPen pen(Qt::black);

    double screen_height = SCREEN_H;
    double screen_width = SCREEN_W;
    std::vector<double> up_hor;
    std::vector<double> down_hor;
    double x_min = x_minn, x_max = x_maxx, d_x = d_xx;
    double z_min = z_minn, z_max = z_maxx, d_z = d_zz;
    double x_left = -1, x_right = -1, y_left = -1, y_right = -1;

    for (int i = 0; i < screen_width; i++)
    {
        up_hor.push_back(0);
        down_hor.push_back(screen_height);
    }

    double z = z_max;
    double x, y, z_buf;
    double x_c, y_c, z_c;
    double x_pred, y_pred;
    while (z >= z_min)
    {
        x_pred = x_min;
        y_pred = func(x_min, z);
        z_buf = z;
        transf(x_pred, y_pred, z_buf, tx, ty, tz);

        if (x_left == -1)
        {
            x_left = x_pred;
            y_left = y_pred;
        }
        else
        {
            if (x_pred >= 5 && x_pred <= screen_width-5)
                horizont(x_left, y_left, x_pred, y_pred, up_hor, down_hor, 1);
            x_left = x_pred;
            y_left = y_pred;
        }

        x = x_min;
        while (x <= x_max)
        {
            y = func(x, z);
            transf(x, y, z, x_c, y_c, z_c, tx, ty, tz);
            if (x_c < 5 || x_c > screen_width-5)
            {
                x_pred = x_c;
                y_pred = y_c;
                x += d_x;
                continue;
            }
            if (x + d_x > x_max)
                horizont(x_pred, y_pred, x_c, y_c, up_hor, down_hor, 0);
            else
                horizont(x_pred, y_pred, x_c, y_c, up_hor, down_hor, 1);

            x_pred = x_c;
            y_pred = y_c;
            x += d_x;
        }
        //transf(x_pred, y_pred, z_buf, tx, ty, tz);

        if (x_right == -1)
        {
            x_right = x_pred;
            y_right = y_pred;
        }
        else
        {

            if (x_pred >= 5 && x_pred <= screen_width-5)
            {
               // qDebug() << y_pred << " " <<  up_hor[x_pred] << " " << down_hor[x_pred];
                //if (fabs(y_pred - up_hor[x_pred]) < 0.01 || y_pred == down_hor[x_pred])
                 //   scene->addLine(x_right, y_right, x_pred, y_pred, pen);
                //else
                //{
                    horizont(x_right, y_right, x_pred, y_pred, up_hor, down_hor, 1);
                //}
            }
            x_right = x_pred;
            y_right = y_pred;
        }


        //treatment_rib(x_pred, y_pred, x_right, y_right, up_hor, down_hor);
        z -= d_z;
    }
}*/

bool postr = 0;
double ugol_x = 0;
double ugol_y = 0;
double ugol_z = 0;

double mx_min = 0, mx_max = 0, md_x = 0, mz_min = 0, mz_max = 0, md_z = 0;
void MainWindow::on_draw_clicked()
{
    rot_arr.clear();
    ugol_x = 0;
    ugol_y = 0;
    ugol_z = 0;
    QString x_min_s = ui->lineEdit_x->text();
    QString x_max_s = ui->lineEdit_y->text();
    QString d_x_s = ui->lineEdit_z->text();

    QString z_min_s = ui->lineEdit_x_2->text();
    QString z_max_s = ui->lineEdit_y_2->text();
    QString d_z_s = ui->lineEdit_z_2->text();

    bool ok;
    mx_min = x_min_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_min!");
        return;
    }
    mx_max = x_max_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x_max!");
        return;
    }
    md_x = d_x_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен d_x!");
        return;
    }
    mz_min = z_min_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен z_min!");
        return;
    }
    mz_max = z_max_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен z_max!");
        return;
    }
    md_z = d_z_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен d_z!");
        return;
    }

    scene->clear();

    if (ui->funcs->currentIndex() == 0)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, 0, 0, 0, func0);
    if (ui->funcs->currentIndex() == 1)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, 0, 0, 0, func1);
    if (ui->funcs->currentIndex() == 2)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, 0, 0, 0, func2);
    if (ui->funcs->currentIndex() == 3)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, 0, 0, 0, func3);
    postr = 1;
}





void MainWindow::on_pushButton_rot_clicked()
{
    if (!postr)
        return;
    QString x_n_s = ui->lineEdit_x_r->text();
    bool ok;
    float x_n = x_n_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x!");
        return;
    }
    ugol_x += x_n;
    rot_com newcom{1,x_n};
    rot_arr.push_back(newcom);
    scene->clear();
    if (ui->funcs->currentIndex() == 0)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func0);
    if (ui->funcs->currentIndex() == 1)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func1);
    if (ui->funcs->currentIndex() == 2)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func2);
    if (ui->funcs->currentIndex() == 3)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func3);

}


void MainWindow::on_pushButton_rot_3_clicked()
{
    if (!postr)
        return;
    QString y_n_s = ui->lineEdit_y_r->text();
    bool ok;
    float y_n = y_n_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x!");
        return;
    }
    ugol_y += y_n;
    rot_com newcom{2,y_n};
    rot_arr.push_back(newcom);
    scene->clear();
    if (ui->funcs->currentIndex() == 0)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func0);
    if (ui->funcs->currentIndex() == 1)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func1);
    if (ui->funcs->currentIndex() == 2)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func2);
    if (ui->funcs->currentIndex() == 3)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func3);
}

void MainWindow::on_pushButton_rot_2_clicked()
{
    if (!postr)
        return;
    QString z_n_s = ui->lineEdit_z_r->text();
    bool ok;
    float z_n = z_n_s.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::information(this, "Ошибка", "Неверно введен x!");
        return;
    }
    ugol_z+=z_n;
    rot_com newcom{3,z_n};
    rot_arr.push_back(newcom);
    scene->clear();
    if (ui->funcs->currentIndex() == 0)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func0);
    if (ui->funcs->currentIndex() == 1)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func1);
    if (ui->funcs->currentIndex() == 2)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func2);
    if (ui->funcs->currentIndex() == 3)
        draw_all(mx_min, mx_max, md_x, mz_min, mz_max, md_z, ugol_x, ugol_y, ugol_z, func3);
}
