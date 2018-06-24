#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>

typedef double (*myfunc)(double, double);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void draw_all(double x_minn, double x_maxx, double d_xx, double z_minn, double z_maxx, double d_zz, double tx, double ty, double tz, myfunc func);
    void on_draw_clicked();
    void horizont(double x1, double y1, double x2, double y2, std::vector<double>& up_hor, std::vector<double>& down_hor, int delta);
    void treatment_rib(double x, double y, double& x_rib, double& y_rib, std::vector<double>& up_hor, std::vector<double>& down_hor);

    void on_pushButton_rot_clicked();

    void on_pushButton_rot_3_clicked();

    void on_pushButton_rot_2_clicked();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
