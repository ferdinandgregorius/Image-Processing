#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openImageButton_clicked()
{
    // choose the image to open
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    // read the image
    image = imread(filename.toLocal8Bit().constData(), IMREAD_GRAYSCALE);

    // update the UI
    ui->transformButton->setEnabled(true);
    ui->filterButton->setEnabled(true);

    // we are using OpenCV windows, since QImage has some visualization problems with this type of grayscale images
    // create a named image window
    namedWindow("Original Image");
    // show the image
    imshow("Original Image", image);

}

void MainWindow::on_transformButton_clicked()
{
    Mat padded = optimizeImgDimension(image);

    // convert values in float and add another channel to complexImage
    // for storing the additional information given by the transformation
    planes[0] = Mat_<float> (padded);
    planes[1] = Mat::zeros(padded.size(), CV_32F);
    merge(planes, 2, complexImage);

    // DFT
    dft(complexImage, complexImage);

    // optimization
    Mat magnitude = createOptimizeMagnitudeSpectrum(complexImage, true);

    // show only the real part
    namedWindow("Magnitude");
    imshow("Magnitude", magnitude);

    // update the UI
    ui->antiTransformButton->setEnabled(true);
}

void MainWindow::on_antiTransformButton_clicked()
{
    // anti-transform
    idft(complexImage,complexImage);

    Mat final;
    split(complexImage, planes);
    normalize(planes[0],final,0,1, NORM_MINMAX);

    // show the image
    namedWindow("Anti-transform");
    imshow("Anti-transform", final);
}

void MainWindow::on_filterButton_clicked()
{
    // apply a median filter
    Mat filteredImage;
    int filterDim = QInputDialog::getInt(this, tr("Median Filter"), tr("Filter dimension"), 0, 3, 15, 2);
    medianBlur(image,filteredImage,filterDim);

    // show the result
    namedWindow("Filter");
    imshow("Filter", filteredImage);
}

// add some pixels to the image borders, in order to optimize its dimension for DFT
Mat MainWindow::optimizeImgDimension(Mat &image)
{
    Mat padded;
    int addPixelRows = getOptimalDFTSize(image.rows); // # pixels to horizontally add to the padded image
    int addPixelCols = getOptimalDFTSize(image.cols); // # pixels to vertically add to the padded image
    copyMakeBorder(image,
                   padded,
                   0,
                   addPixelRows - image.rows,
                   0,
                   addPixelCols - image.cols,
                   BORDER_CONSTANT,
                   Scalar::all(0)
                   );
    return padded;
}

// image crop (to return to original dimension) and quadrants swap to properly show the magnitude
void MainWindow::shiftDFT(Mat &image)
{
    //crop
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));


    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = image.cols/2;
    int cy = image.rows/2;

    Mat q0(image, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(image, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(image, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(image, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

}

void MainWindow::fftshift(const Mat &input_img, Mat &output_img)
{
    output_img = input_img.clone();
    int cx = output_img.cols / 2;
    int cy = output_img.rows / 2;
    Mat q1(output_img, Rect(0, 0, cx, cy));
    Mat q2(output_img, Rect(cx, 0, cx, cy));
    Mat q3(output_img, Rect(0, cy, cx, cy));
    Mat q4(output_img, Rect(cx, cy, cx, cy));

    Mat temp;
    q1.copyTo(temp);
    q4.copyTo(q1);
    temp.copyTo(q4);
    q2.copyTo(temp);
    q3.copyTo(q2);
    temp.copyTo(q3);
}

void MainWindow::filtering(Mat &scr, Mat &dst, Mat &H)
{
    fftshift(H, H);
    Mat planesH[] = { Mat_<float>(H.clone()), Mat_<float>(H.clone()) };

    Mat planes_dft[] = { scr, Mat::zeros(scr.size(), CV_32F) };
    split(scr, planes_dft);

    Mat planes_out[] = { Mat::zeros(scr.size(), CV_32F), Mat::zeros(scr.size(), CV_32F) };
    qDebug()<<planesH[0].rows<<planes_dft[0].rows;
    planes_out[0] = planesH[0].mul(planes_dft[0]);
    planes_out[1] = planesH[1].mul(planes_dft[1]);

    merge(planes_out, 2, dst);

}

Mat MainWindow::construct_H(Mat &scr, String type, float D0)
{
    Mat H(scr.size(), CV_32F, Scalar(1));
    float D = 0;
    if (type == "Ideal")
    {
        for (int u = 0; u < H.rows; u++)
        {
            for (int  v = 0; v < H.cols; v++)
            {
                D = sqrt((u - scr.rows / 2)*(u - scr.rows / 2) + (v - scr.cols / 2)*(v - scr.cols / 2));
                if (D > D0)
                {
                    H.at<float>(u, v) = 0;
                }
            }
        }
        return H;
    }
    else if (type == "Gaussian")
    {
        for (int  u = 0; u < H.rows; u++)
        {
            for (int v = 0; v < H.cols; v++)
            {
                D = sqrt((u - scr.rows / 2)*(u - scr.rows / 2) + (v - scr.cols / 2)*(v - scr.cols / 2));
                H.at<float>(u, v) = exp(-D*D / (2 * D0*D0));
            }
        }
        return H;
    }
}

// split the complex image in the real and imaginary part
// return the magnitude and normalize its value according to the log scale
Mat MainWindow::createOptimizeMagnitudeSpectrum(Mat &complexImage, bool flipQuadrants)
{
    // split the transformed image in two planes, for real and imaginary part
    split(complexImage, planes);
    // magnitude: M = sqrt(planes[0]^2 + planes[1]^2);
    magnitude(planes[0], planes[1], planes[0]);
    // log scale: log(1 + M)
    Mat mag = planes[0].clone();
    mag += Scalar::all(1);
    log(mag, mag);

    if (flipQuadrants)
        shiftDFT(mag);
    // transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    normalize(mag, mag, 0, 1, NORM_MINMAX);

    return mag;
}

// apply thresholded magnitude image to complex image
cv::Mat
MainWindow::apply_magnitude(cv::Mat* src, cv::Mat magnitude)
{
    // threshold the magnitude image so all non-zero pixels are 1
    cv::Mat thresholded;
    cv::threshold( magnitude, thresholded, 0, 1, cv::THRESH_BINARY );

    // multiply planes of complex images by threshold by applying as mask
    cv::Mat planes[2];
    cv::split( *src, planes );

    cv::Mat real;
    planes[0].copyTo( real , thresholded );
    cv::Mat img;
    planes[1].copyTo( img, thresholded );
    // write_img_to_file_as_text<float>( cv::Mat_<float>( thresholded ) , "./out", "threhs.tif");

    // and merge them into a new complex image
    cv::Mat planes2[] = { real, img };
    cv::merge( planes2, 2, *src );
    cv::imshow("planes2: ", real);

    // swap quandrants
    swap_quadrants( src );

    thresholded.release();
    return *src;
}

// create a normalized real plane image from a thresholded complex image
cv::Mat
MainWindow::extract_real_image(cv::Mat src)
{
    // split the complex image into planes
    cv::Mat planes[2];
    cv::split( src, planes );

    cv::Mat normal_real_plane;
    cv::normalize( planes[0], normal_real_plane, 0, 1, cv::NORM_MINMAX );
    normal_real_plane.convertTo( normal_real_plane, CV_8U, 255 );

    return normal_real_plane;
}

void MainWindow::on_pushButton_clicked()
{
    cv::Mat magnitude_image = create_magnitude_image(&complexImage);
    Mat imgIn;
    complexImage.convertTo(imgIn, CV_32F);

       // filter the periodic noise
//       cv::Mat freq_filter_mask = auto_filter( magnitude_image );
//       magnitude_image.release();

//       cv::imshow(" Frequency Mask", freq_filter_mask );
       // write_img_to_file( freq_filter_image, "./out", "freq_mask_" + output_image_filename);

       // apply magnitude to new complex image
       //complex_image = apply_magnitude( &complex_image, freq_filter_mask );
       //freq_filter_mask.release();

       //complexImage = apply_magnitude( &complexImage, freq_filter_mask );
        Mat H;
        H = construct_H(imgIn, "Ideal", 85);

       filtering(complexImage, complexImage, H);
//       cv::imshow(" complex_image", complexImage );
//           freq_filter_mask.release();

           // apply inverse fourier transform
//           cv::idft( complexImage, complexImage );

           // extract real plane and crop to size of original
           cv::Mat normal_real_plane = extract_real_image( complexImage );
           cv::imshow(" Filter_Image", normal_real_plane );
}

// draw contours of canny edge detection
cv::Mat
MainWindow::draw_canny_contours(cv::Mat magnitude_image)
{
    cv::Mat canny_output;
    magnitude_image.copyTo( canny_output );
    cv::Canny( canny_output, canny_output, 10, 15 );

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );

    cv::Mat canvas = cv::Mat::zeros( canny_output.size(), CV_8U );
    for (size_t i = 0; i < contours.size(); i++) {
        cv::drawContours( canvas, contours, i, cv::Scalar(255), cv::FILLED, cv::LINE_8, hierarchy, 0 );
    }

    // cv::imshow(" Contours Image", canvas );

    canny_output.release();
    return canvas;
}

// remove periodic noise from a frequency magnitude image
cv::Mat
MainWindow::auto_filter(cv::Mat magnitude_image)
{
    // draw canny contours

    //cv::Mat canny_output = draw_canny_contours( magnitude_image );
    cv::Mat canny_output ;
    magnitude_image.copyTo(canny_output);
    // create contour mask (which contours to get rid of)
    cv::Mat mask = cv::Mat( canny_output.size(), CV_8U );
    mask = cv::Scalar::all(0);
    // exclude the middle of the mask
    cv::circle( mask, cv::Point( mask.cols/2, mask.rows/2 ), 30, cv::Scalar(255), cv::FILLED );
    imshow("mask: ", mask);
    // apply the mask to magnitude image
    cv::bitwise_and( mask, mask, canny_output );
    imshow("canny_output: ", canny_output);
    mask.release();
    // return the new magnitude image, outer frequencies filtered
    //cv::bitwise_not( canny_output, canny_output );
    //cv::bitwise_not( mask, mask );


    return canny_output;
}

// remove periodic noise manually
cv::Mat
MainWindow::manual_filter(cv::Mat magnitude_image)
{
    // display normalized magnitude image
    // cv::threshold( magnitude_image, magnitude_image, 1, 255, cv::THRESH_BINARY );
    cv::imshow( "Magnitude Image", magnitude_image );
    // initialize mask
    cv::Mat mask = cv::Mat( magnitude_image.size(), CV_8U );
    mask = cv::Scalar::all(255);
    // initialize mouse callback
    init_callback( "Magnitude Image", &mask );
    return mask;
}


// draw zeros on image
void
MainWindow::mouse_callback_draw_zeros(int event, int x, int y, int d, void* userdata)
{
    cv::Mat* frequency_mask = (cv::Mat*) userdata;

    switch (event) {
        case cv::EVENT_LBUTTONUP:
            // push the new point
            // draw a circle mask at chosen points
            cv::circle( *frequency_mask, cv::Point2f( x, y ), 4, cv::Scalar(0), cv::FILLED );
            // show the copy of the image
            cv::imshow( "tmp:", *frequency_mask );
            break;
    }
}

// assign mouse callbacks
void
MainWindow::init_callback(std::string window_name, cv::Mat* frequency_mask)
{
    cv::setMouseCallback( window_name, mouse_callback_draw_zeros, frequency_mask );
}

// create a normalized magnitude image from complex image
cv::Mat
MainWindow::create_magnitude_image(cv::Mat* src)
{
    // swap quadrants of complex image
    swap_quadrants( src );

    // split swapped complex image into real and imaginary
    cv::Mat planes[2];
    cv::split( *src, planes );

    // compute magnitude
    cv::Mat magnitude_image;
    cv::magnitude( planes[0], planes[1], magnitude_image );

    // normalize magnitude
    cv::normalize( magnitude_image, magnitude_image, 0, 1, cv::NORM_MINMAX);
    magnitude_image.convertTo( magnitude_image, CV_8U, 255 );

    return magnitude_image;
}

// swap images
void
MainWindow::swap_mat(cv::Mat* a, cv::Mat* b)
{
    cv::Mat tmp;
    a->copyTo(tmp);
    b->copyTo(*a);
    tmp.copyTo(*b);
    tmp.release();
}

// swap quandrants of an image
void
MainWindow::swap_quadrants(cv::Mat* src)
{
    // assert cols and rows are even
    assert( !( src->cols & 1 || src->rows & 1) );
    int c_x = src->cols / 2;
    int c_y = src->rows / 2;

    cv::Mat q0( *src, cv::Rect( 0, 0, c_x, c_y )); // top_left
    cv::Mat q1( *src, cv::Rect( c_x, 0, c_x, c_y )); // top_right
    cv::Mat q2( *src, cv::Rect( 0, c_y, c_x, c_y )); // bottom_left
    cv::Mat q3( *src, cv::Rect( c_x, c_y, c_x, c_y )); // bottom_right

    // swap quandrants
    swap_mat( &q0, &q3 );
    swap_mat( &q1, &q2 );

    q0.release();
    q1.release();
    q2.release();
    q3.release();
}
