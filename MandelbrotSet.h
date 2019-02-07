#pragma once

#include <opencv2/opencv.hpp>
#include <complex>



class MandelbrotSet {

public:

    MandelbrotSet(const std::complex<float>& topLeftPoint,
                  const std::complex<float>& bottomRightPoint,
                  size_t maxIteration);

    virtual ~MandelbrotSet() = default;

    void draw(cv::Mat& image, bool isParallel = true);

private:

    void drawSequential(cv::Mat& image);

    void drawParallel(cv::Mat& image);

    uchar getColor(const std::complex<float>& z0);

    size_t getIteration(const std::complex<float>& z0);

    std::complex<float> mandelbrotFunction(const std::complex<float>& z,
                                           const std::complex<float>& c);

    std::complex<float> imageToComplexPlane(const cv::Point2i& pixel,
                                            const cv::Point2f& scale);

    cv::Point2f getScale(const cv::Size2i& imageSize);

    cv::Size2f getComplexPlaneSize();

    static float abs2(const std::complex<float>& z);


private:
    std::complex<float> _topLeftPoint;
    std::complex<float> _bottomRightPoint;
    size_t _maxIteration;
};
