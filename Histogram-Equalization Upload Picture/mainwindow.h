#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "histogram.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include "MatToQImage.h"
#include <QMouseEvent>
#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <QFileDialog>

#define scale  10
#define QLHeight scale*30
#define QLWidth  scale*40

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     QTimer *timer;

private:
    Ui::MainWindow *ui;
    VideoCapture cap;
    Mat capture, capture2;

private slots:
    void update();
    void on_btn_image_clicked();
};
#endif // MAINWINDOW_H
