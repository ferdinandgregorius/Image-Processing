#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include <QInputDialog>

using namespace cv;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    cv::Mat auto_filter(cv::Mat magnitude_image);
    cv::Mat create_magnitude_image(cv::Mat *src);
    void swap_quadrants(cv::Mat *src);
    void swap_mat(cv::Mat *a, cv::Mat *b);
    cv::Mat draw_canny_contours(cv::Mat magnitude_image);
    cv::Mat apply_magnitude(cv::Mat *src, cv::Mat magnitude);
    cv::Mat extract_real_image(cv::Mat src);
    cv::Mat manual_filter(cv::Mat magnitude_image);
    static void mouse_callback_draw_zeros(int event, int x, int y, int d, void *userdata);
    void init_callback(std::string window_name, cv::Mat *frequency_mask);
    void fftshift(const Mat &input_img, Mat &output_img);
    void filtering(Mat &scr, Mat &dst, Mat &H);
    Mat construct_H(Mat &scr, String type, float D0);
private slots:
    void on_openImageButton_clicked();

    void on_transformButton_clicked();

    void on_antiTransformButton_clicked();

    void on_filterButton_clicked();

    //custom methods
    Mat optimizeImgDimension(Mat &image);
    void shiftDFT(Mat &image);
    Mat createOptimizeMagnitudeSpectrum(Mat &complexImage, bool flipQuadrants);


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Mat image; //the original image
    Mat complexImage; //complex image
    Mat planes[2]; //planes for real and not-real parts
};

#endif // MAINWINDOW_H
