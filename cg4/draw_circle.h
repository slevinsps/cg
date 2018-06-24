#ifndef DRAW_CIRCLE_H
#define DRAW_CIRCLE_H
#include "structs.h"
#include "QColor"
#include <vector>
#include <QGraphicsScene>


typedef struct myPoint_s
{
    int x;
    int y;
}myPoint;

void draw_canonich_circle(options_s opt, int paint);
void draw_brez_circle(options_s opt, int paint);
void draw_midle_circle(options_s opt, int paint);
void draw_param_circle(options_s opt, int paint);

#endif // DRAW_CIRCLE_H
