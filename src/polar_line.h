#pragma once

#include <opencv2/opencv.hpp>

class PolarLine
{
public:
    double theta;
    double rho;

    PolarLine() = default;
    explicit PolarLine(cv::Vec4i line);

    bool operator==(const PolarLine& other) const;
    bool operator<(const PolarLine& other) const;

    bool isHorizontal() const;
    void draw(cv::Mat image, cv::Scalar color) const;
};