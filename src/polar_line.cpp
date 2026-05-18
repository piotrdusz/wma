#include "polar_line.h"

#include <algorithm>
#include <cmath>

using namespace cv;

PolarLine::PolarLine(Vec4i line)
{
    double dx = line[2] - line[0];
    double dy = line[3] - line[1];

    // Zamiana odcinka na parametry prostej w postaci polarnej.
    theta = atan2(dy, dx) + M_PI_2;
    rho = line[0] * cos(theta) + line[1] * sin(theta);

    // Ujednolicenie znaku rho, aby podobne linie mialy porownywalna reprezentacje.
    if (rho < 0)
    {
        rho = -rho;
        theta += CV_PI;
    }

    // Normalizacja kata do zakresu <0, 2pi).
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

    // Dwie linie traktujemy jako rowne, jesli maja zblizony kat i polozenie.
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
    // Kierunek normalnej pozwala rozroznic, czy linia jest blizej poziomej czy pionowej.
    return abs(sin(theta)) > abs(cos(theta));
}

void PolarLine::draw(Mat image, Scalar color) const
{
    double a = cos(theta);
    double b = sin(theta);
    double x0 = a * rho;
    double y0 = b * rho;

    // Rysowanie bardzo dlugiego odcinka, aby pokazac cala prosta na obrazie.
    Point point1(x0 - 20000 * (-b), y0 - 20000 * a);
    Point point2(x0 + 20000 * (-b), y0 + 20000 * a);
    line(image, point1, point2, color, 10);
}

std::pair<std::vector<PolarLine>, std::vector<PolarLine>> collectDocumentLines(const std::vector<Vec4i>& lineSegments)
{
    std::vector<PolarLine> horizontalLines;
    std::vector<PolarLine> verticalLines;
    for (const auto& lineSegment : lineSegments)
    {
        PolarLine polarLine(lineSegment);

        if (polarLine.isHorizontal())
        {
            // Dodaj tylko nowe reprezentanty podobnych linii poziomych.
            auto existingLine = std::find(horizontalLines.begin(), horizontalLines.end(), polarLine);
            if (existingLine == horizontalLines.end())
            {
                horizontalLines.push_back(polarLine);
            }
        }
        else
        {
            // Dodaj tylko nowe reprezentanty podobnych linii pionowych.
            auto existingLine = std::find(verticalLines.begin(), verticalLines.end(), polarLine);
            if (existingLine == verticalLines.end())
            {
                verticalLines.push_back(polarLine);
            }
        }
    }

    // Sortowanie po rho ulatwia pozniejszy wybor skrajnych krawedzi dokumentu.
    std::sort(horizontalLines.begin(), horizontalLines.end());
    std::sort(verticalLines.begin(), verticalLines.end());

    return {horizontalLines, verticalLines};
}