#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QObject>

class ConvertImage
{
public:
    ConvertImage();
    static cv::Mat qimageToMatRef(QImage &img, int format);
    static cv::Mat qimageToMat(QImage img, int format);
    static QImage matToQimageRef(cv::Mat &mat, QImage::Format format);
    static QImage matToQimage(cv::Mat mat, QImage::Format format);
};

#endif // CONVERTIMAGE_H
