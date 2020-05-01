#include "preprocessimage.h"

PreprocessImage::PreprocessImage() {}

cv::UMat PreprocessImage::deskewRotate(cv::UMat image)
{
    double ratio = image.rows / 500.0;
    cv::UMat orig = image.clone();

    cv::UMat gray, edged, warped;
    preProcess(image, edged);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> approx;
    cv::findContours(edged, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    approx.resize(contours.size());
    size_t i, j;
    for (i = 0; i < contours.size(); i++) {
        double peri = cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx[i], 0.02 * peri, true);
    }
    sort(approx.begin(), approx.end(), &compareContourAreas);

    for (i = 0; i < approx.size(); i++) {
        cv::drawContours(image, approx, static_cast<int>(i), cv::Scalar(255, 50, 0), 2);

        if (approx[i].size() == 4) {
            break;
        }
    }

    if (i < approx.size()) {
        cv::drawContours(image, approx, static_cast<int>(i), cv::Scalar(0, 255, 180), 2);

        for (j = 0; j < approx[i].size(); j++) {
            approx[i][j] *= ratio;
        }

        fourPointTransform(orig, warped, approx[i]);

        cv::imshow("contour", image);
        cvtColor(warped, warped, cv::COLOR_BGRA2GRAY, 1);
        adaptiveThreshold(warped, warped, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 15);
        GaussianBlur(warped, warped, cv::Size(3, 3), 0);
        cvtColor(warped, warped, cv::COLOR_GRAY2BGRA);
    }
    if (warped.rows > image.rows / 5)
        return warped;
    else
        return orig;
}

cv::Mat PreprocessImage::deskewRotate(cv::Mat image)
{
    cv::Mat orig = image.clone();

    cv::Mat gray, edged, warped;
    preProcess(image, edged);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> approx;
    cv::findContours(edged, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    approx.resize(contours.size());
    size_t i;
    for (i = 0; i < contours.size(); i++) {
        double peri = cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx[i], 0.02 * peri, true);
    }
    sort(approx.begin(), approx.end(), &compareContourAreas);

    for (i = 0; i < approx.size(); i++) {
        cv::drawContours(image, approx, static_cast<int>(i), cv::Scalar(255, 50, 0), 2);

        if (approx[i].size() == 4) {
            break;
        }
    }

    if (i < approx.size()) {
        cv::drawContours(image, approx, static_cast<int>(i), cv::Scalar(0, 0, 255), 4);

        if (poi.size() == 4)
            approx[i] = poi;

        fourPointTransform(orig, warped, approx[i]);

        cvtColor(warped, warped, cv::COLOR_RGB2BGRA);
    }

    return warped;
}

void PreprocessImage::applyThreshold(cv::Mat &image,
                                     int threshValue,
                                     bool isAdaptive,
                                     uint8_t blurValue)
{
    cvtColor(image, image, cv::COLOR_BGRA2GRAY, 1);
    if (isAdaptive)
        adaptiveThreshold(image, image, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 15);
    else
        cv::threshold(image, image, threshValue, 255, cv::THRESH_OTSU);
    GaussianBlur(image, image, cv::Size(blurValue, blurValue), 0);
    cvtColor(image, image, cv::COLOR_GRAY2BGRA);
}

bool PreprocessImage::compareContourAreas(std::vector<cv::Point> contour1,
                                          std::vector<cv::Point> contour2)
{
    double i = fabs(contourArea(cv::Mat(contour1)));
    double j = fabs(contourArea(cv::Mat(contour2)));
    return (i > j);
}

bool PreprocessImage::compareXCords(cv::Point p1, cv::Point p2)
{
    return (p1.x < p2.x);
}

bool PreprocessImage::compareYCords(cv::Point p1, cv::Point p2)
{
    return (p1.y < p2.y);
}

bool PreprocessImage::compareDistance(std::pair<cv::Point, cv::Point> p1,
                                      std::pair<cv::Point, cv::Point> p2)
{
    return (cv::norm(p1.first - p1.second) < cv::norm(p2.first - p2.second));
}

double PreprocessImage::_distance(cv::Point p1, cv::Point p2)
{
    return sqrt(((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y)));
}

void PreprocessImage::resizeToHeight(cv::UMat src, cv::UMat &dst, int height)
{
    cv::Size2d s = cv::Size2d(src.cols * (height / double(src.rows)), height);
    cv::resize(src, dst, s, cv::INTER_AREA);
}

void PreprocessImage::resizeToHeight(cv::Mat src, cv::Mat &dst, int height)
{
    cv::Size2d s = cv::Size2d(src.cols * (height / double(src.rows)), height);
    cv::resize(src, dst, s, cv::INTER_AREA);
}

void PreprocessImage::orderPoints(std::vector<cv::Point> inpts, std::vector<cv::Point> &ordered)
{
    sort(inpts.begin(), inpts.end(), &compareXCords);
    std::vector<cv::Point> lm(inpts.begin(), inpts.begin() + 2);
    std::vector<cv::Point> rm(inpts.end() - 2, inpts.end());

    sort(lm.begin(), lm.end(), &compareYCords);
    cv::Point tl(lm[0]);
    cv::Point bl(lm[1]);
    std::vector<std::pair<cv::Point, cv::Point>> tmp;
    for (size_t i = 0; i < rm.size(); i++) {
        tmp.push_back(std::make_pair(tl, rm[i]));
    }

    sort(tmp.begin(), tmp.end(), &compareDistance);
    cv::Point tr(tmp[0].second);
    cv::Point br(tmp[1].second);

    ordered.push_back(tl);
    ordered.push_back(tr);
    ordered.push_back(br);
    ordered.push_back(bl);
}

std::vector<cv::Point2f> PreprocessImage::fourPointTransform(cv::UMat src,
                                                             cv::UMat &dst,
                                                             std::vector<cv::Point> pts)
{
    std::vector<cv::Point> ordered_pts;
    orderPoints(pts, ordered_pts);

    std::vector<cv::Point2f> src_;
    std::vector<cv::Point2f> dst_;

    double wa = _distance(ordered_pts[2], ordered_pts[3]);
    double wb = _distance(ordered_pts[1], ordered_pts[0]);
    double mw = fmax(wa, wb);

    double ha = _distance(ordered_pts[1], ordered_pts[2]);
    double hb = _distance(ordered_pts[0], ordered_pts[3]);
    double mh = fmax(ha, hb);

    src_ = {
        cv::Point(ordered_pts[0].x, ordered_pts[0].y),
        cv::Point(ordered_pts[1].x, ordered_pts[1].y),
        cv::Point(ordered_pts[2].x, ordered_pts[2].y),
        cv::Point(ordered_pts[3].x, ordered_pts[3].y),
    };
    dst_ = {cv::Point(0, 0),
            cv::Point(static_cast<int>(mw) - 1, 0),
            cv::Point(static_cast<int>(mw) - 1, static_cast<int>(mh) - 1),
            cv::Point(0, static_cast<int>(mh) - 1)};
    cv::UMat m = getPerspectiveTransform(src_, dst_).getUMat(cv::ACCESS_RW);
    cv::warpPerspective(src, dst, m, cv::Size(static_cast<int>(mw), static_cast<int>(mh)));

    return src_;
}

std::vector<cv::Point2f> PreprocessImage::fourPointTransform(cv::Mat &src,
                                                             cv::Mat &dst,
                                                             std::vector<cv::Point> &pts)
{
    std::vector<cv::Point> ordered_pts;
    orderPoints(pts, ordered_pts);

    std::vector<cv::Point2f> src_;
    std::vector<cv::Point2f> dst_;

    double wa = _distance(ordered_pts[2], ordered_pts[3]);
    double wb = _distance(ordered_pts[1], ordered_pts[0]);
    double mw = fmax(wa, wb);

    double ha = _distance(ordered_pts[1], ordered_pts[2]);
    double hb = _distance(ordered_pts[0], ordered_pts[3]);
    double mh = fmax(ha, hb);

    src_ = {
        cv::Point(ordered_pts[0].x, ordered_pts[0].y),
        cv::Point(ordered_pts[1].x, ordered_pts[1].y),
        cv::Point(ordered_pts[2].x, ordered_pts[2].y),
        cv::Point(ordered_pts[3].x, ordered_pts[3].y),
    };
    dst_ = {cv::Point(0, 0),
            cv::Point(static_cast<int>(mw) - 1, 0),
            cv::Point(static_cast<int>(mw) - 1, static_cast<int>(mh) - 1),
            cv::Point(0, static_cast<int>(mh) - 1)};
    cv::Mat m = getPerspectiveTransform(src_, dst_);
    cv::warpPerspective(src, dst, m, cv::Size(static_cast<int>(mw), static_cast<int>(mh)));

    return src_;
}

std::vector<cv::Point2f> PreprocessImage::getPoints(cv::Mat src)
{
    double ratio = src.rows / 500.0;
    cv::Mat gray, edged;
    preProcess(src, edged);

    std::vector<cv::Point2f> src_;
    std::vector<cv::Point2f> dst_;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> approx;
    cv::findContours(edged, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    approx.resize(contours.size());
    size_t i, j;
    for (i = 0; i < contours.size(); i++) {
        double peri = cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx[i], 0.02 * peri, true);
    }
    sort(approx.begin(), approx.end(), compareContourAreas);

    for (i = 0; i < approx.size(); i++) {

        if (approx[i].size() == 4) {
            break;
        }
    }

    if (i < approx.size()) {
        for (j = 0; j < approx[i].size(); j++) {
            approx[i][j] *= ratio;
        }

        std::vector<cv::Point> ordered_pts;
        orderPoints(approx[i], ordered_pts);

        double wa = _distance(ordered_pts[2], ordered_pts[3]);
        double wb = _distance(ordered_pts[1], ordered_pts[0]);
        double mw = fmax(wa, wb);

        double ha = _distance(ordered_pts[1], ordered_pts[2]);
        double hb = _distance(ordered_pts[0], ordered_pts[3]);
        double mh = fmax(ha, hb);

        src_ = {
            cv::Point(ordered_pts[0].x, ordered_pts[0].y),
            cv::Point(ordered_pts[1].x, ordered_pts[1].y),
            cv::Point(ordered_pts[2].x, ordered_pts[2].y),
            cv::Point(ordered_pts[3].x, ordered_pts[3].y),
        };
        dst_ = {cv::Point(0, 0),
                cv::Point(static_cast<int>(mw) - 1, 0),
                cv::Point(static_cast<int>(mw) - 1, static_cast<int>(mh) - 1),
                cv::Point(0, static_cast<int>(mh) - 1)};
    }

    return src_;
}

void PreprocessImage::setPoints(std::vector<cv::Point2f> pt)
{
    poi.clear();
    for (auto i : pt) {
        poi.push_back(i);
    }
}

void PreprocessImage::preProcess(cv::UMat src, cv::UMat &dst)
{
    cv::UMat imageGrayed;
    cv::UMat imageOpen, imageClosed, imageBlurred;

    cvtColor(src, imageGrayed, cv::COLOR_BGRA2GRAY);

    cv::UMat structuringElmt = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4))
                                   .getUMat(cv::ACCESS_RW);
    cv::morphologyEx(imageGrayed, imageOpen, cv::MORPH_OPEN, structuringElmt);
    cv::morphologyEx(imageOpen, imageClosed, cv::MORPH_CLOSE, structuringElmt);

    cv::GaussianBlur(imageClosed, imageBlurred, cv::Size(7, 7), 0);
    cv::Canny(imageBlurred, dst, 75, 100);
}

void PreprocessImage::preProcess(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat imageGrayed;
    cv::Mat imageOpen, imageClosed, imageBlurred;

    cvtColor(src, imageGrayed, cv::COLOR_BGRA2GRAY);

    cv::Mat structuringElmt = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));
    cv::morphologyEx(imageGrayed, imageOpen, cv::MORPH_OPEN, structuringElmt);
    cv::morphologyEx(imageOpen, imageClosed, cv::MORPH_CLOSE, structuringElmt);

    cv::GaussianBlur(imageClosed, imageBlurred, cv::Size(7, 7), 0);
    cv::Canny(imageBlurred, dst, 75, 100);
}

std::string PreprocessImage::getOutputFileName(std::string path, std::string name)
{
    std::string fname, ext;

    size_t sep = path.find_last_of("\\/");
    if (sep != std::string::npos) {
        path = path.substr(sep + 1, path.size() - sep - 1);

        size_t dot = path.find_last_of(".");
        if (dot != std::string::npos) {
            fname = path.substr(0, dot);
            ext = path.substr(dot, path.size() - dot);
        } else {
            fname = path;
            ext = "";
        }
    }

    return fname + "_" + name + ext;
}

double PreprocessImage::computeSkew(cv::UMat src)
{
    cv::cvtColor(src, src, cv::COLOR_BGRA2GRAY);
    cv::Size size = src.size();
    cv::threshold(src, src, 180, 255, cv::THRESH_OTSU);
    cv::bitwise_not(src, src);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(src, lines, 1, CV_PI / 180, 100, size.width / 2., 10);
    cv::UMat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
    double angle = 0.;
    unsigned nb_lines = static_cast<unsigned>(lines.size());
    for (unsigned i = 0; i < nb_lines; ++i) {
        cv::line(disp_lines,
                 cv::Point(lines[i][0], lines[i][1]),
                 cv::Point(lines[i][2], lines[i][3]),
                 cv::Scalar(255, 0, 0));
        angle += atan2(static_cast<double>(lines[i][3]) - lines[i][1],
                       static_cast<double>(lines[i][2]) - lines[i][0]);
    }
    angle /= nb_lines;
    return angle * 180 / CV_PI;
}

double PreprocessImage::computeSkew(cv::Mat src)
{
    cv::cvtColor(src, src, cv::COLOR_BGRA2GRAY);
    cv::Size size = src.size();
    cv::threshold(src, src, 180, 255, cv::THRESH_OTSU);
    cv::bitwise_not(src, src);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(src, lines, 1, CV_PI / 180, 100, size.width / 2., 10);
    cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
    double angle = 0.;
    unsigned nb_lines = static_cast<unsigned>(lines.size());
    for (unsigned i = 0; i < nb_lines; ++i) {
        cv::line(disp_lines,
                 cv::Point(lines[i][0], lines[i][1]),
                 cv::Point(lines[i][2], lines[i][3]),
                 cv::Scalar(255, 0, 0));
        angle += atan2(static_cast<double>(lines[i][3]) - lines[i][1],
                       static_cast<double>(lines[i][2]) - lines[i][0]);
    }
    angle /= nb_lines;
    return angle * 180 / CV_PI;
}

cv::Mat PreprocessImage::adjustBrightness(cv::Mat &in, int value)
{
    cv::Mat out;
    in.convertTo(out, -1, 1, value);
    return out;
}

cv::Mat PreprocessImage::adjustContrast(cv::Mat &in, double value)
{
    cv::Mat out;
    in.convertTo(out, -1, value, 0);
    return out;
}

cv::Mat PreprocessImage::manualThreshold(cv::Mat &image,
                                         int threshValue,
                                         bool isAdaptive,
                                         uint8_t blurValue)
{
    cv::Mat img = image;
    cvtColor(img, img, cv::COLOR_BGRA2GRAY, 1);
    cv::threshold(img, img, threshValue, 255, cv::THRESH_BINARY);
    GaussianBlur(img, img, cv::Size(blurValue, blurValue), 0);
    cvtColor(img, img, cv::COLOR_GRAY2BGRA);
    return img;
}
