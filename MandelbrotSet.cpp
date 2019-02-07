#include "MandelbrotSet.h"



MandelbrotSet::MandelbrotSet(const std::complex<float>& topLeftPoint, 
                             const std::complex<float>& bottomRightPoint, 
                             size_t maxIteration) 
    : _topLeftPoint(topLeftPoint)
    , _bottomRightPoint(bottomRightPoint)
    , _maxIteration(maxIteration) {
}

void MandelbrotSet::draw(cv::Mat& image, bool isParallel) {

    if (isParallel) {
        drawParallel(image);
    } else {
        drawSequential(image);
    }
}

void MandelbrotSet::drawSequential(cv::Mat& image) {

    auto scale = getScale({image.cols, image.rows});

    for (int i = 0; i < image.rows; ++i) {
        auto rowPtr = image.ptr<uchar>(i);
        for (int j = 0; j < image.cols; ++j) {

            auto z0 = imageToComplexPlane({j, i}, scale);

            rowPtr[j] = getColor(z0);
        }
    }
}

void MandelbrotSet::drawParallel(cv::Mat& image) {

    auto scale = getScale({image.cols, image.rows});

    auto f = [&](const cv::Range& range) {

        for (int r = range.start; r < range.end; ++r) {

            int i = r / image.cols;
            int j = r % image.cols;

            auto z0 = imageToComplexPlane({j, i}, scale);

            image.ptr<uchar>(i)[j] = getColor(z0);
        }
    };

    cv::parallel_for_(cv::Range(0, image.rows * image.cols), f);
}

uchar MandelbrotSet::getColor(const std::complex<float>& z0) {

    size_t iteration = getIteration(z0);

    return static_cast<uchar>(
        (_maxIteration != iteration)
        ? cvRound(std::sqrt(iteration / static_cast<float>(_maxIteration)) * 255)
        : 0
    );
}

size_t MandelbrotSet::getIteration(const std::complex<float>& z0) {

    std::complex<float> z = z0;

    for (size_t iteration = 0; iteration < _maxIteration; ++iteration) {
        if (abs2(z) > 4.0f) {
            return iteration;
        }
        z = mandelbrotFunction(z, z0);
    }

    return _maxIteration;
}

std::complex<float> MandelbrotSet::mandelbrotFunction(const std::complex<float>& z, 
                                                      const std::complex<float>& c) {
    return z * z + c;
}

std::complex<float> MandelbrotSet::imageToComplexPlane(const cv::Point2i& pixel,
                                                       const cv::Point2f& scale) {
    return {
        pixel.x / scale.x + _topLeftPoint.real(),
        pixel.y / scale.y + _topLeftPoint.imag()
    };
}

cv::Point2f MandelbrotSet::getScale(const cv::Size2i& imageSize) {

    auto complexPlaneSize = getComplexPlaneSize();

    return cv::Point2f(
        imageSize.width / complexPlaneSize.width,
        imageSize.height / complexPlaneSize.height
    );
}

cv::Size2f MandelbrotSet::getComplexPlaneSize() {
    return cv::Size2f(
        _bottomRightPoint.real() - _topLeftPoint.real(),
        _bottomRightPoint.imag() - _topLeftPoint.imag()
    );
}

float MandelbrotSet::abs2(const std::complex<float>& z) {
    return z.real() * z.real() + z.imag() * z.imag();
}
