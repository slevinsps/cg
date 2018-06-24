#include "structs.h"
#include "QColor"
#include <vector>
#include <QGraphicsScene>
#include "structs.h"
#include "draw_circle.h"
#include <QDebug>
#include "glwidget.h"

void draw_canonich_circle(options_s opt, int paint)
{
    if (opt.my_circle.r_c <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());
    int y;
    int range = (int)round(opt.my_circle.r_c * sqrt(2)/2); // так как если рассматривать [0;R] будут наложения
    double r2 = opt.my_circle.r_c * opt.my_circle.r_c;
    for (int i = 0; i <= range; i++)
    {

        if (paint)
        {
            glBegin( GL_POINTS );
                glVertex2i(i + opt.my_circle.x_n, y + opt.my_circle.y_n);
                glVertex2i(-i + opt.my_circle.x_n, y + opt.my_circle.y_n);
                glVertex2i(i + opt.my_circle.x_n, -y + opt.my_circle.y_n);
                glVertex2i(-i + opt.my_circle.x_n, -y + opt.my_circle.y_n);
                glVertex2i(y + opt.my_circle.x_n, i + opt.my_circle.y_n);
                glVertex2i(y + opt.my_circle.x_n, -i + opt.my_circle.y_n);
                glVertex2i(-y + opt.my_circle.x_n, i + opt.my_circle.y_n);
                glVertex2i(-y + opt.my_circle.x_n, -i + opt.my_circle.y_n);
            glEnd();
        }
        y = (int)round(sqrt(r2 - i*i));
    }
}


void draw_param_circle(options_s opt, int paint)
{
    if (opt.my_circle.r_c <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());
    int x;
    int y;
    double pi_4 = M_PI/4;
    double di = asin(1.0/opt.my_circle.r_c);
    for (double i = 0; i <= pi_4; i+=di)
    {
        x = (int)round(opt.my_circle.r_c * cos(i));
        y = (int)round(opt.my_circle.r_c * sin(i));
        if (paint)
        {
            glBegin( GL_POINTS );
                glVertex2i(x + opt.my_circle.x_n, y + opt.my_circle.y_n);
                glVertex2i(-x + opt.my_circle.x_n, y + opt.my_circle.y_n);
                glVertex2i(x + opt.my_circle.x_n, -y + opt.my_circle.y_n);
                glVertex2i(-x + opt.my_circle.x_n, -y + opt.my_circle.y_n);
                glVertex2i(y + opt.my_circle.x_n, x + opt.my_circle.y_n);
                glVertex2i(y + opt.my_circle.x_n, -x + opt.my_circle.y_n);
                glVertex2i(-y + opt.my_circle.x_n, x + opt.my_circle.y_n);
                glVertex2i(-y + opt.my_circle.x_n, -x + opt.my_circle.y_n);
            glEnd();
        }
    }
}

void choose_pixel(int &x, int &y, int r, double &delta)
{
    double b;
    if (delta < 0)
    {
        b = 2 * (delta + y) - 1;
        if (b <= 0)
        {
            x += 1;
            delta += 2 * x + 1;
        }
        else
        {
            x += 1;
            y -= 1;
            delta += 2 * x - 2 * y + 2;
        }
    }
    else if (delta > 0)
    {
        b = 2 * (delta - x) + 1;;
        if (b <= 0)
        {
            x += 1;
            y -= 1;
            delta += 2 * x - 2 * y + 2;
        }
        else
        {
            y -= 1;
            delta += 1 - 2 * y;
        }
    }
    else
    {
        x += 1;
        y -= 1;
        delta += 2 * x - 2 * y + 2;
    }
}

void draw_brez_circle(options_s opt, int paint)
{
    if (opt.my_circle.r_c <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());
    int r = opt.my_circle.r_c;
    int x = 0;
    int y = r;

    double delta = 2*(1 - r);
    while (y >= 0)
    {
        if (paint)
        {
            glBegin( GL_POINTS );
              glVertex2i(x + opt.my_circle.x_n, y + opt.my_circle.y_n);
              glVertex2i(-x + opt.my_circle.x_n, y + opt.my_circle.y_n);
              glVertex2i(x + opt.my_circle.x_n, -y + opt.my_circle.y_n);
              glVertex2i(-x + opt.my_circle.x_n, -y + opt.my_circle.y_n);
            glEnd();
        }
        choose_pixel(x, y, r, delta);


    }
}

void draw_midle_circle(options_s opt, int paint)
{
    if (opt.my_circle.r_c <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());
    int a = opt.my_circle.r_c;
    int aa = a*a;
    int b = opt.my_circle.r_c;
    int bb = b*b;
    int x = 0;
    int y = b;
    int border_x = aa/sqrt(aa + bb);
    int df_pr;
    int delta_f_pr;
    double f = (bb - aa*y + aa/4);

    while(x <= border_x + 1)
    {
        if (paint)
        {
            glBegin( GL_POINTS );
                glVertex2i(x + opt.my_circle.x_n, y + opt.my_circle.y_n);
                glVertex2i(-x + opt.my_circle.x_n, y + opt.my_circle.y_n);
                glVertex2i(x + opt.my_circle.x_n, -y + opt.my_circle.y_n);
                glVertex2i(-x + opt.my_circle.x_n, -y + opt.my_circle.y_n);
                glVertex2i(y + opt.my_circle.x_n, x + opt.my_circle.y_n);
                glVertex2i(y + opt.my_circle.x_n, -x + opt.my_circle.y_n);
                glVertex2i(-y + opt.my_circle.x_n, x + opt.my_circle.y_n);
                glVertex2i(-y + opt.my_circle.x_n, -x + opt.my_circle.y_n);
            glEnd();
        }
        x++;
        df_pr = bb * (2 * x + 1);

        if (f <= 0)
        {
            f += df_pr;
        }
        else
        {
            y--;
            delta_f_pr = - 2 * aa * y;
            f += df_pr + delta_f_pr;
        }

    }
}

