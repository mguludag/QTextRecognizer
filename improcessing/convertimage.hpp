/*****************************************************************************
 * convertimage.hpp
 *
 * Created: 5/28/2020 2020 by mguludag
 *
 * Copyright 2020 mguludag. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
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
