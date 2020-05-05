#include "mycroplayer.hpp"

MyCropLayer::MyCropLayer(const cv::dnn::LayerParams &params)
    : cv::dnn::Layer(params)
{}

cv::Ptr<cv::dnn::Layer> MyCropLayer::create(cv::dnn::LayerParams &params)
{
    return cv::Ptr<Layer>(new MyCropLayer(params));
}

bool MyCropLayer::getMemoryShapes(const std::vector<std::vector<int>> &inputs,
                                  const int requiredOutputs,
                                  std::vector<std::vector<int>> &outputs,
                                  std::vector<std::vector<int>> &internals) const
{
    CV_UNUSED(requiredOutputs);
    CV_UNUSED(internals);
    std::vector<int> outShape(4);
    outShape[0] = inputs[0][0]; // batch size
    outShape[1] = inputs[0][1]; // number of channels
    outShape[2] = inputs[1][2];
    outShape[3] = inputs[1][3];
    outputs.assign(1, outShape);
    return false;
}

void MyCropLayer::forward(std::vector<cv::Mat *> &input,
                          std::vector<cv::Mat> &output,
                          std::vector<cv::Mat> &internals)
{
    cv::Mat *inp = input[0];
    cv::Mat out = output[0];
    int ystart = (inp->size[2] - out.size[2]) / 2;
    int xstart = (inp->size[3] - out.size[3]) / 2;
    int yend = ystart + out.size[2];
    int xend = xstart + out.size[3];

    const int batchSize = inp->size[0];
    const int numChannels = inp->size[1];
    const int height = out.size[2];
    const int width = out.size[3];

    int sz[] = {static_cast<int>(batchSize), numChannels, height, width};
    out.create(4, sz, CV_32F);
    for (int i = 0; i < batchSize; i++) {
        for (int j = 0; j < numChannels; j++) {
            cv::Mat plane(inp->size[2], inp->size[3], CV_32F, inp->ptr<float>(i, j));
            cv::Mat crop = plane(cv::Range(ystart, yend), cv::Range(xstart, xend));
            cv::Mat targ(height, width, CV_32F, out.ptr<float>(i, j));
            crop.copyTo(targ);
        }
    }
}

void MyCropLayer::forward(cv::InputArrayOfArrays inputs_arr,
                          cv::OutputArrayOfArrays outputs_arr,
                          cv::OutputArrayOfArrays internals_arr)
{
    std::cerr << "MyCropLayer:forward ENTERNING" << std::endl << std::flush;

    std::vector<cv::Mat> inputs, outputs;
    inputs_arr.getMatVector(inputs);
    outputs_arr.getMatVector(outputs);

    cv::Mat &inp = inputs[0];
    cv::Mat &out = outputs[0];

    int ystart = (inp.size[2] - out.size[2]) / 2;
    int xstart = (inp.size[3] - out.size[3]) / 2;
    int yend = ystart + out.size[2];
    int xend = xstart + out.size[3];

    const int batchSize = inp.size[0];
    const int numChannels = inp.size[1];
    const int height = out.size[2];
    const int width = out.size[3];

    int sz[] = {static_cast<int>(batchSize), numChannels, height, width};
    out.create(4, sz, CV_32F);
    for (int i = 0; i < batchSize; i++) {
        for (int j = 0; j < numChannels; j++) {
            cv::Mat plane(inp.size[2], inp.size[3], CV_32F, inp.ptr<float>(i, j));
            cv::Mat crop = plane(cv::Range(ystart, yend), cv::Range(xstart, xend));
            cv::Mat targ(height, width, CV_32F, out.ptr<float>(i, j));
            crop.copyTo(targ);
        }
    }
}
