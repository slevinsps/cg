#include "glwidget.h"
#include <gl\gl.h>    // Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>
#include <iostream>
#include "mainwindow.h"
#include "structs.h"
#include "draw_circle.h"
#include "draw_ellipse.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    //connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer.start(16);
    //this->angle = 0;
}

extern QColor color_fone;
extern std::vector<options_s> opt;

void draw_circle(options_s opt)
{
    draw_canonich_circle(opt,1);
}

void draw_ellipse(options_s opt)
{
    draw_canonich_ellipse(opt,1);
}

void GLWidget::initializeGL()
{

    /*glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);*/

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel( GL_FLAT );
}



void draw_konc_circle(options_s opt)
{
    int paint = 1;
    opt.my_circle.x_n = 300;
    opt.my_circle.y_n = 300;
    for (int i = 0; i < opt.my_circle.n; i++)
    {

        if (opt.method == 1)
        {
            draw_canonich_circle(opt, paint);
        }
        if (opt.method == 2)
        {
            draw_param_circle(opt, paint);
        }
        if (opt.method == 3)
        {
            draw_brez_circle(opt, paint);
        }
        if (opt.method == 4)
        {
            draw_midle_circle(opt, paint);
        }

        opt.my_circle.r_c += opt.my_circle.delta;
    }
}


void draw_konc_ellipse(options_s opt)
{
    int paint = 1;
    for (int i = 0; i < opt.my_ellipse.n; i++)
    {

        if (opt.method == 1)
        {
            draw_canonich_ellipse(opt, paint);
        }
        if (opt.method == 2)
        {
            draw_param_ellipse(opt, paint);
        }
        if (opt.method == 3)
        {
            draw_brez_ellipse(opt, paint);
        }
        if (opt.method == 4)
        {
            draw_midle_ellipse(opt, paint);
        }

        opt.my_ellipse.a += opt.my_ellipse.delta;
        opt.my_ellipse.b += opt.my_ellipse.delta;
    }
}

void GLWidget::paintGL()
{
    glClearColor(color_fone.redF(), color_fone.greenF(), color_fone.blueF(),1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int paint = 1;
    qDebug() << "Helldfdfdfdfo";
    for (int i = 0; i < opt.size(); i++)
    {
        if (opt[i].choose == 1)
        {

            if (opt[i].method == 1)
            {
                draw_canonich_circle(opt[i], paint);
            }
            if (opt[i].method == 2)
            {
                draw_param_circle(opt[i], paint);
            }
            if (opt[i].method == 3)
            {
                draw_brez_circle(opt[i], paint);
            }
            if (opt[i].method == 4)
            {
                draw_midle_circle(opt[i], paint);
            }
            if (opt[i].method == 5)
            {
                draw_circle(opt[i]);
            }
        }
        if (opt[i].choose == 2)
        {

            if (opt[i].method == 1)
            {
                draw_canonich_ellipse(opt[i], paint);
            }
            if (opt[i].method == 2)
            {
                draw_param_ellipse(opt[i], paint);
            }
            if (opt[i].method == 3)
            {
                draw_brez_ellipse(opt[i], paint);
            }
            if (opt[i].method == 4)
            {
                draw_midle_ellipse(opt[i], paint);
            }
            if (opt[i].method == 5)
            {
                draw_ellipse(opt[i]);
            }
        }
        if (opt[i].choose == 3)
        {
            draw_konc_circle(opt[i]);
        }
        if (opt[i].choose == 4)
        {
            draw_konc_ellipse(opt[i]);
        }
    }
}

void GLWidget::resizeGL(int w, int h)
{

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0, 600, 0, 600, -1.0, 1.0);

    glViewport(0, 0, (GLint)w, (GLint)h);
}
