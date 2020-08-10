/*****************************************************************************
 * mycroplayer.hpp
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
