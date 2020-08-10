/*****************************************************************************
 * preprocessimage.hpp
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
#ifndef PREPROCESSIMAGE_HPP
#define PREPROCESSIMAGE_HPP

#include "convertimage.hpp"
#include "mycroplayer.hpp"
#include <leptonica/allheaders.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>
#include <vector>

class PreprocessImage
{
public:
    PreprocessImage();
    static cv::Mat deskewRotate(cv::Mat &image);
    static void setPoints(std::vector<cv::Point2f> pt);
    static std::vector<cv::Point2f> getPoints(cv::Mat &src);
    static void preProcess(cv::Mat &src, cv::Mat &dst);
    static void adaptThreshold(cv::Mat &image,
                               bool isMorphOp = false,
                               uint8_t morphKernel = 3,
                               uint8_t blurValue = 3);
    static cv::Mat adjustBrightness(cv::Mat &in, int value);
    static cv::Mat adjustContrast(cv::Mat &in, double value);
    static cv::Mat manualThreshold(cv::Mat &image, int threshValue = 128, uint8_t blurValue = 3);
    static void hedEdgeDetectDNN(cv::Mat &image,
                                 std::string &prototxt,
                                 std::string &caffemodel,
                                 int size);
    static bool compareContourAreas(std::vector<cv::Point> &contour1,
                                    std::vector<cv::Point> &contour2);
    static bool compareXCords(cv::Point &p1, cv::Point &p2);
    static bool compareYCords(cv::Point &p1, cv::Point &p2);
    static bool compareDistance(std::pair<cv::Point, cv::Point> &p1,
                                std::pair<cv::Point, cv::Point> &p2);
    static double _distance(cv::Point &p1, cv::Point &p2);
    static void resizeToHeight(cv::Mat &src, cv::Mat &dst, int height);
    static void orderPoints(std::vector<cv::Point> &inpts, std::vector<cv::Point> &ordered);
    static void fourPointTransform(cv::Mat &src, cv::Mat &dst, std::vector<cv::Point> &pts);
    static double computeSkew(cv::Mat src);

private:
    static std::vector<cv::Point> poi;
};

#endif // PREPROCESSIMAGE_HPP
