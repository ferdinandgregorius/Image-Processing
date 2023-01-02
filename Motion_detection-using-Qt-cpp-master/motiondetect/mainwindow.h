#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();
    void processVideo();
    std::string intToString(int number);
    void searchForMovement(cv::Mat thresholdImage, cv::Mat &cameraFeed);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
