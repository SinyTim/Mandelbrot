#include <complex>
#include <opencv2/opencv.hpp>
#include "MandelbrotSet.h"
#include "TimeUtilities.h"



void testTime(const std::complex<float>& topLeftPoint, 
              const std::complex<float>& bottomRightPoint, 
              const size_t maxIter, 
              cv::Mat& mandelbrotImage) {

    TimeUtilities::measureTime([&]() {
        MandelbrotSet(topLeftPoint, bottomRightPoint, maxIter).draw(mandelbrotImage);
    });

    cv::imwrite("images/MandelbrotSetParallel.png", mandelbrotImage);  // 406s - 6.8min

    TimeUtilities::measureTime([&]() {
        MandelbrotSet(topLeftPoint, bottomRightPoint, maxIter).draw(mandelbrotImage, false);
    });

    cv::imwrite("images/MandelbrotSetSequential.png", mandelbrotImage);  // 865s - 14.4min
}

int main() {

    const std::complex<float> topLeftPoint(-0.72f, -0.227f);
    const std::complex<float> bottomRightPoint(-0.715f, -0.223f);
    const size_t maxIter = 500;

    const int k = 10000;
    const int height = k * (bottomRightPoint.imag() - topLeftPoint.imag());
    const int width = k * (bottomRightPoint.real() - topLeftPoint.real());

    cv::Mat mandelbrotImage(height, width, CV_8U);

    TimeUtilities::printCurrentTime();

    TimeUtilities::measureTime([&]() {
        MandelbrotSet(topLeftPoint, bottomRightPoint, maxIter).draw(mandelbrotImage);
    });

    cv::applyColorMap(mandelbrotImage, mandelbrotImage, cv::COLORMAP_HOT);

    cv::imwrite("images/MandelbrotSet0.png", mandelbrotImage);
    
    system("pause");
    return 0;
}
