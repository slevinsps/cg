#include "QColor"
#include <vector>
#include <QGraphicsScene>

#include <QDebug>
#include "glwidget.h"
#include "structs.h"

void draw_canonich_ellipse(options_s opt, int paint)
{
    if (opt.my_ellipse.a <= 0 || opt.my_ellipse.b <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());

    int y = opt.my_ellipse.b;
    int x = 0;
    int a = opt.my_ellipse.a;
    int aa = a*a;
    int b = opt.my_ellipse.b;
    int bb = b*b;
    double border_x = aa/sqrt(aa + bb);
    while(x <= border_x)
    {
        if (paint)
        {
            glBegin( GL_POINTS );
                glVertex2i(x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
                glVertex2i(-x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
                glVertex2i(x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
                glVertex2i(-x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
            glEnd();
        }

        x++;
        y = round(sqrt((1 - double(x) * x / aa) * bb));

    }

    while(y >= 0)
    {
        if (paint)
        {
            glBegin( GL_POINTS );
                glVertex2i(x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
                glVertex2i(-x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
                glVertex2i(x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
                glVertex2i(-x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
            glEnd();
        }
        y--;
        x = round(sqrt((1 - double(y) * y / bb) * aa));
    }

}


void draw_param_ellipse(options_s opt, int paint)
{
    if (opt.my_ellipse.a <= 0 || opt.my_ellipse.b <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());

    int x;
    int y;
    double pi_2 = M_PI/2;
    double max;
    if (opt.my_ellipse.a >= opt.my_ellipse.b)
        max = opt.my_ellipse.a;
    else
        max = opt.my_ellipse.b;
    double di = asin(1.0/max);
    for (double i = 0; i < pi_2 + di/2; i+=di)
    {
        x = round(opt.my_ellipse.a * cos(i));
        y = round(opt.my_ellipse.b * sin(i));

        if (paint)
        {
            glBegin( GL_POINTS );
                glVertex2i(x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
                glVertex2i(-x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
                glVertex2i(x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
                glVertex2i(-x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
            glEnd();
        }
    }
}


void choose_pixel_ellipse(int &x, int &y, int aa, int bb, double &delta)
{
    double b;
    if (delta < 0)
    {
        b = 2 * (delta + aa*y) - aa;
        if (b <= 0)
        {
            x += 1;
            delta += 2 * bb * x + bb;
        }
        else
        {
            x += 1;
            y -= 1;
            delta += 2 * bb * x - 2 * aa * y + aa + bb;
        }

    }
    else if (delta > 0)
    {
        b = 2 * (delta - bb * x) - bb;;
        if (b <= 0)
        {
            x += 1;
            y -= 1;
            delta += 2 * x * bb - 2 * aa * y + aa + bb;
        }
        else
        {
            y -= 1;
            delta += aa - 2 * aa * y;
        }
    }
    else
    {
        x += 1;
        y -= 1;
        delta += 2 * x * bb - 2 * aa * y + aa + bb;
    }
}

void draw_brez_ellipse(options_s opt, int paint)
{
    if (opt.my_ellipse.a <= 0 || opt.my_ellipse.b <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());
    int a = opt.my_ellipse.a;
    int aa = a*a;
    int b = opt.my_ellipse.b;
    int bb = b*b;
    int x = 0;
    int y = b;

    double delta = aa + bb - 2 * aa * y;
    while (y >= 0)
    {
        if (paint)
        {
            glBegin( GL_POINTS );
              glVertex2i(x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
              glVertex2i(-x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
              glVertex2i(x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
              glVertex2i(-x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
            glEnd();
        }
        choose_pixel_ellipse(x, y, aa, bb, delta);

    }
}


void draw_midle_ellipse(options_s opt, int paint)
{
    if (opt.my_ellipse.a <= 0 || opt.my_ellipse.b <= 0)
        return;
    glColor3f(opt.color.redF(),opt.color.greenF(),opt.color.blueF());
    int a = opt.my_ellipse.a;
    int aa = a*a;
    int b = opt.my_ellipse.b;
    int bb = b*b;

    int x = 0;
    int y = b;
    double border_x = aa/sqrt(aa + bb);
    int df_pr;
    int delta_f_pr;
    double f = (bb - aa*y + aa/4);

    while(x <= border_x)
    {
        if (paint)
        {
            glBegin( GL_POINTS );
              glVertex2i(x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
              glVertex2i(-x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
              glVertex2i(x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
              glVertex2i(-x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
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

    f = ((double)bb * (x + 0.5) * (x + 0.5) + (double)aa * (y - 1.0) * (y - 1.0) - (double)aa * bb  );

    while (y >= 0)
    {
        if (paint)
        {
            glBegin( GL_POINTS );
              glVertex2i(x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
              glVertex2i(-x + opt.my_ellipse.x_n, y + opt.my_ellipse.y_n);
              glVertex2i(x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
              glVertex2i(-x + opt.my_ellipse.x_n, -y + opt.my_ellipse.y_n);
            glEnd();
        }
        y--;

        df_pr = -2 * aa * y + aa;

        if (f >= 0)
        {
            f += df_pr;
        }
        else
        {
            x++;
            delta_f_pr = 2 * bb * x;
            f += df_pr + delta_f_pr;
        }
    }
}
