#pragma once

#include <opencv2/opencv.hpp>

#include <utility>
#include <vector>

// Reprezentacja prostej w postaci polarnej: kat normalnej i odleglosc od poczatku ukladu.
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

// Dzieli wykryte odcinki na linie poziome i pionowe oraz usuwa bardzo podobne duplikaty.
std::pair<std::vector<PolarLine>, std::vector<PolarLine>> collectDocumentLines(const std::vector<cv::Vec4i>& lineSegments);