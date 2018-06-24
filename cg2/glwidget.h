#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    double x_ellipse;
    double y_ellipse;
    double angle;
    double d_angle;
    double x_rot;
    double y_rot;
    double x_scale;
    double y_scale;
    double k_x;
    double k_y;
    int key;

private:
    QTimer timer;
};

#endif // GLWIDGET_H
