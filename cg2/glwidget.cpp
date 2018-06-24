#include "glwidget.h"
#include <gl\gl.h>    // Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>
#include "mainwindow.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    this->x_ellipse = 100;
    this->y_ellipse = 100;
    this->x_rot = this->x_ellipse;
    this->y_rot = this->x_ellipse;
    this->angle = 0;
    this->d_angle = 0;
    this->x_scale = this->x_ellipse;
    this->y_scale = this->y_ellipse;
    this->k_x = 1;
    this->k_y = 1;
    this->key = 1;

    //connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
   // timer.start(16);
    //this->angle = 0;
}


void GLWidget::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void rotate_func(double *x, double *y, double angle, double xc, double yc)
{

    double x_rot = xc + (*x - xc)*cos(angle) + (*y - yc)*sin(angle);
    double y_rot = yc - (*x - xc)*sin(angle) + (*y - yc)*cos(angle);
    *x = x_rot;
    *y = y_rot;
}

void scale_func(double *x, double *y, double k_x, double k_y, double x_scale, double y_scale)
{
    k_x *= 1;
    k_y *= 1;

    double x_sc = k_x * (*x) + (1 - k_x) * x_scale;
    double y_sc = k_y * (*y) + (1 - k_y) * y_scale;
    *x = x_sc;
    *y = y_sc;
}

void GLWidget::paintGL()
{

    //this->angle += 1;
    double angle =  -this->angle  * 3.14/180;
    double d_angle = -this->d_angle  * 3.14/180;
    if (this->key)
    {
        scale_func(&this->x_ellipse, &this->y_ellipse, this->k_x, this->k_y, this->x_scale, this->y_scale);
        rotate_func(&this->x_ellipse, &this->y_ellipse, d_angle, this->x_rot, this->y_rot);
        //printf("\nhey\n");
    }

    double pi = 3.1415926;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);

    glBegin( GL_POINTS ); // тело

        double i_2 = 0;
        while (i_2 <= 360)
        {
                double a = ((double)i_2 / 180) * pi;
                double x = cos( a ) * 8.0f + this->x_ellipse;
                double y = sin( a ) * 15.0f + this->y_ellipse;
                scale_func(&x, &y, k_x, k_y, this->x_ellipse, this->y_ellipse);

               // rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
                rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);


                glVertex2f( x, y);
                i_2 += 0.1;
        }
    glEnd();

    glBegin( GL_POINTS ); //голова
        double i_1 = 0;
        while (i_1 <= 360)
        {
            double a = ((double)i_1 / 180) * pi;
            double x = cos( a ) * 6.0f + this->x_ellipse;
            double y = sin( a ) * 6.0f + this->y_ellipse + 21;
            scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

            //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
            rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);
            glVertex2f( x, y);
            i_1 += 0.1;
        }
    glEnd();

    glBegin( GL_POINTS ); //глаз 1
        for(int i = 0; i <= 360; i++ )
        {
            double a = ((double)i / 180) * pi;
            double x = cos( a ) * 1.0f + this->x_ellipse - 2;
            double y = sin( a ) * 1.0f + this->y_ellipse + 22;
            scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

            //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
            rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

            glVertex2f( x, y);
    }
    glEnd();
    glBegin( GL_POINTS ); //глаз 2
        for(int i = 0; i <= 360; i++ ) {
            double a = ((double)i / 180) * pi;
            double x = cos( a ) * 1.0f + this->x_ellipse + 2;
            double y = sin( a ) * 1.0f + this->y_ellipse + 22;
            scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

            //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
            rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

            glVertex2f( x, y);
    }
    glEnd();

    glBegin( GL_LINES ); //усы
    {
        double x, y;
        x = x_ellipse - 8;
        y = y_ellipse + 21;
        scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

        glVertex2d(x, y);
        x = x_ellipse + 8;
        y = y_ellipse +16;
        scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

        glVertex2d(x, y);

        x = x_ellipse + 8;
        y = y_ellipse + 21;
       scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

        glVertex2d(x, y);
        x = x_ellipse - 8;
        y = y_ellipse + 16;
       scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

        glVertex2d(x, y);

        x = x_ellipse - 8;
        y = y_ellipse + 18.5;
        scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
         rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

        glVertex2d(x, y);
        x = x_ellipse + 8;
        y = y_ellipse + 18.5;
        scale_func(&x, &y, this->k_x, this->k_y, this->x_ellipse, this->y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, this->x_ellipse, this->y_ellipse);

        glVertex2d(x, y);
    }

    glEnd();

    glBegin( GL_LINES ); //уши
        double x, y;
        x = x_ellipse - 5;
        y = y_ellipse + 24.5;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);
        x = x_ellipse - 3;
        y = y_ellipse + 31;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);

        x = x_ellipse - 3;
        y = y_ellipse + 31;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
         rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);
        x = x_ellipse - 1;
        y = y_ellipse + 27;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);

        /*******/

        x = x_ellipse + 5;
        y = y_ellipse + 24.5;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);
        x = x_ellipse + 3;
        y = y_ellipse + 31;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);

        x = x_ellipse + 3;
        y = y_ellipse + 31;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

       // rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);
        x = x_ellipse + 1;
        y = y_ellipse + 27;
        scale_func(&x, &y, k_x, k_y, x_ellipse, y_ellipse);

        //rotate_func(&x, &y, d_angle, this->x_rot, this->y_rot);
        rotate_func(&x, &y, angle, x_ellipse, y_ellipse);

        glVertex2d(x, y);

    glEnd();

    glBegin( GL_POINTS );
        //rotate_func(&x_ellipse, &y_ellipse, angle, this->x_rot, this->y_rot);
    glEnd();




}

void GLWidget::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0, 200, 0, 200, -1.0, 1.0);

    glViewport(0, 0, (GLint)w, (GLint)h);
}
