#ifndef CONVERTIMAGE_HPP
#define CONVERTIMAGE_HPP

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <QObject>

inline bool archIsLittleEndian()
{
    const int x = 1;
    return (*reinterpret_cast<const unsigned char *>(&x));
}

inline unsigned char float2uchar_bounded(const float &in)
{
    return (in < 0.0f) ? 0 : ((in > 255.0f) ? 255 : static_cast<unsigned char>(in));
}

class ConvertImage
{
public:
    ConvertImage();
    static cv::Mat qimageToMatRef(QImage &img, int format)
    {
        return cv::Mat(img.height(),
                       img.width(),
                       format,
                       img.bits(),
                       static_cast<size_t>(img.bytesPerLine()));
    }
    static cv::Mat qimageToMat(QImage img, int format)
    {
        return cv::Mat(img.height(),
                       img.width(),
                       format,
                       img.bits(),
                       static_cast<size_t>(img.bytesPerLine()));
    }
    static QImage matToQimageRef(cv::Mat &mat, QImage::Format format)
    {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
    }
    static QImage matToQimage(cv::Mat mat, QImage::Format format)
    {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
    }
};

#endif // CONVERTIMAGE_HPP
