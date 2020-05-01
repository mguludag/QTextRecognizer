#ifndef PREPROCESSIMAGE_H
#define PREPROCESSIMAGE_H

#include "convertimage.h"
#include <leptonica/allheaders.h>
#include <opencv2/core/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/text/ocr.hpp>
#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>
#include <vector>

class PreprocessImage
{
public:
    PreprocessImage();

    cv::UMat deskewRotate(cv::UMat image);
    cv::Mat deskewRotate(cv::Mat image);

    static void applyThreshold(cv::Mat &image,
                               int threshValue = 128,
                               bool isAdaptive = true,
                               uint8_t blurValue = 3);

    static bool compareContourAreas(std::vector<cv::Point> contour1,
                                    std::vector<cv::Point> contour2);

    static bool compareXCords(cv::Point p1, cv::Point p2);

    static bool compareYCords(cv::Point p1, cv::Point p2);

    static bool compareDistance(std::pair<cv::Point, cv::Point> p1,
                                std::pair<cv::Point, cv::Point> p2);

    static double _distance(cv::Point p1, cv::Point p2);

    static void resizeToHeight(cv::UMat src, cv::UMat &dst, int height);
    static void resizeToHeight(cv::Mat src, cv::Mat &dst, int height);

    static void orderPoints(std::vector<cv::Point> inpts, std::vector<cv::Point> &ordered);

    static std::vector<cv::Point2f> fourPointTransform(cv::UMat src,
                                                       cv::UMat &dst,
                                                       std::vector<cv::Point> pts);
    static std::vector<cv::Point2f> fourPointTransform(cv::Mat &src,
                                                       cv::Mat &dst,
                                                       std::vector<cv::Point> &pts);

    static std::vector<cv::Point2f> getPoints(cv::Mat src);
    void setPoints(std::vector<cv::Point2f> pt);

    static void preProcess(cv::UMat src, cv::UMat &dst);
    static void preProcess(cv::Mat &src, cv::Mat &dst);

    static std::string getOutputFileName(std::string path, std::string name);

    static double computeSkew(cv::UMat src);
    static double computeSkew(cv::Mat src);
    static cv::Mat adjustBrightness(cv::Mat &in, int value);
    static cv::Mat adjustContrast(cv::Mat &in, double value);
    static cv::Mat manualThreshold(cv::Mat &image,
                                   int threshValue = 128,
                                   bool isAdaptive = true,
                                   uint8_t blurValue = 3);

    std::vector<cv::Point> poi;

private:
};

#endif // PREPROCESSIMAGE_H
