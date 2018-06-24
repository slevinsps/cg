#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL/QGLWidget>


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
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
};



class MainScene : public QGLWidget
{
   protected:
      void initializeGL();
      void resizeGL(int w, int h);
      void paintGL();
      void drawTriangle();

   public:
      Scene3D(QWidget* parent = 0);
};

#endif // MAINWINDOW_H
