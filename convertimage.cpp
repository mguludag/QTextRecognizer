#include "convertimage.h"

ConvertImage::ConvertImage()
{
    
}

inline bool archIsLittleEndian()
{
    const int x = 1;
    return (*reinterpret_cast<const unsigned char *>(&x));
}

inline unsigned char float2uchar_bounded(const float &in)
{
    return (in < 0.0f) ? 0 : ((in > 255.0f) ? 255 : static_cast<unsigned char>(in));
}

cv::Mat ConvertImage::qimageToMatRef(QImage &img, int format)
{
    return cv::Mat(img.height(),
                   img.width(),
                   format,
                   img.bits(),
                   static_cast<size_t>(img.bytesPerLine()));
}

cv::Mat ConvertImage::qimageToMat(QImage img, int format)
{
    return cv::Mat(img.height(),
                   img.width(),
                   format,
                   img.bits(),
                   static_cast<size_t>(img.bytesPerLine()));
}

QImage ConvertImage::matToQimageRef(cv::Mat &mat, QImage::Format format)
{
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
}

QImage ConvertImage::matToQimage(cv::Mat mat, QImage::Format format)
{
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
}
