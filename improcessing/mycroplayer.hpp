#ifndef MYCROPLAYER_HPP
#define MYCROPLAYER_HPP

#include <opencv2/dnn.hpp>
#include <opencv2/dnn/layer.details.hpp>
#include <opencv2/dnn/shape_utils.hpp>

namespace cv {
namespace dnn {
class MyCropLayer;
}
} // namespace cv

class MyCropLayer : public cv::dnn::Layer
{
public:
    explicit MyCropLayer(const cv::dnn::LayerParams &params);
    static cv::Ptr<Layer> create(cv::dnn::LayerParams &params);
    virtual bool getMemoryShapes(const std::vector<std::vector<int>> &inputs,
                                 const int requiredOutputs,
                                 std::vector<std::vector<int>> &outputs,
                                 std::vector<std::vector<int>> &internals) const CV_OVERRIDE;
    virtual void forward(std::vector<cv::Mat *> &input,
                         std::vector<cv::Mat> &output,
                         std::vector<cv::Mat> &internals) CV_OVERRIDE;
    virtual void forward(cv::InputArrayOfArrays inputs_arr,
                         cv::OutputArrayOfArrays outputs_arr,
                         cv::OutputArrayOfArrays internals_arr) CV_OVERRIDE;
};

#endif // MYCROPLAYER_HPP
