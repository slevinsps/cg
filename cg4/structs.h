#ifndef STRUCTS_H
#define STRUCTS_H
#include <QColor>

typedef struct Circle_s
{
    int x_n;
    int y_n;
    int r_c;
    int delta;
    int n;
}Circle_d;

typedef struct Ellipse_s
{
    int x_n;
    int y_n;
    int a;
    int b;
    int delta;
    int n;
}Ellipse_d;

typedef struct options_ss
{
    Circle_d my_circle;
    Ellipse_d my_ellipse;
    int method;
    int choose;
    QColor color;
}options_s;

#endif // STRUCTS_H
