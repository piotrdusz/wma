#include "polar_line.h"

#include <cmath>

using namespace cv;

PolarLine::PolarLine(Vec4i line)
{
    double dx = line[2] - line[0];
    double dy = line[3] - line[1];
    theta = atan2(dy, dx) + M_PI_2;
    rho = line[0] * cos(theta) + line[1] * sin(theta);
    if (rho < 0)
    {
        rho = -rho;
        theta += CV_PI;
    }

    theta = fmod(theta, 2 * CV_PI);
    while (theta < 0)
    {
        theta += 2 * CV_PI;
    }
}

bool PolarLine::operator==(const PolarLine& other) const
{
    double thetaError = abs(theta - other.theta);
    double rhoError = abs(rho - other.rho);
    if (thetaError < 0.1 && rhoError < 200)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PolarLine::operator<(const PolarLine& other) const
{
    return rho < other.rho;
}

bool PolarLine::isHorizontal() const
{
    return abs(sin(theta)) > abs(cos(theta));
}

void PolarLine::draw(Mat image, Scalar color) const
{
    double a = cos(theta);
    double b = sin(theta);
    double x0 = a * rho;
    double y0 = b * rho;
    Point point1(x0 - 20000 * (-b), y0 - 20000 * a);
    Point point2(x0 + 20000 * (-b), y0 + 20000 * a);
    line(image, point1, point2, color, 10);
}