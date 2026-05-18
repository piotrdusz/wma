#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>
#include <vector>
#include <array>
#include <opencv2/text/ocr.hpp>

#include "polar_line.h"

using namespace std;
using namespace cv;
namespace fs = filesystem;

void showScaled(const string &winname, const Mat &inputImage)
{
    if (inputImage.empty()) {
        return;
    }

    int screenWidth = 1920;
    int screenHeight = 1080;

    const double maxWidth = screenWidth * 0.9;
    const double maxHeight = screenHeight * 0.9;
    const double scaleX = maxWidth / inputImage.cols;
    const double scaleY = maxHeight / inputImage.rows;
    const double scale = min(1.0, min(scaleX, scaleY));

    Mat preview;
    resize(inputImage, preview, Size(), scale, scale, INTER_AREA);
    imshow(winname, preview);
    waitKey();
}

Point2f intersection(PolarLine firstLine, PolarLine secondLine)
{
    double a = cos(firstLine.theta);
    double b = sin(firstLine.theta);
    double c = firstLine.rho;

    double d = cos(secondLine.theta);
    double e = sin(secondLine.theta);
    double f = secondLine.rho;

    // TODO: Wyznacz punkt przeciecia prostych na podstawie ukladu rownan:
    //       a * x + b * y = c
    //       d * x + e * y = f
    //       Mozna uzyc wzorow Cramera.
    double x = 0;
    double y = 0;
    return Point2f(x, y);
}

array<Point2f, 4> detectDocumentLines(const Mat& inputImage)
{
    Mat linePreview = inputImage.clone();

    Mat edges;
    // TODO: Przygotuj obraz krawedzi do wykrywania linii.
    //       - Przekonwertuj obraz wejsciowy do skali szarosci (cvtColor()).
    //       - Wykryj krawedzie, np. algorytmem Canny'ego (Canny()).
    //       - W razie potrzeby wyswietl wynik posredni.

    vector<Vec4i> lineSegments;
    // TODO: Wykryj odcinki prostych na obrazie krawedzi.
    //       - Uzyj probabilistycznej transformaty Hougha (HoughLinesP()).
    //       - Dobierz parametry tak, aby pozostawic glownie krawedzie dokumentu.
    //       - Zapisz wynik do wektora 'lineSegments'.

    // Podzielenie wykrytych odcinkow na linie poziome i pionowe.
    auto [horizontalLines, verticalLines] = collectDocumentLines(lineSegments);

    // Wybranie skrajnych linii jako gornej, prawej, dolnej i lewej krawedzi dokumentu.
    PolarLine topLine = horizontalLines.front();
    PolarLine rightLine = verticalLines.back();
    PolarLine bottomLine = horizontalLines.back();
    PolarLine leftLine = verticalLines.front();

    // Narysowanie wybranych krawedzi na kopii obrazu w celu wizualizacji wyniku detekcji.
    topLine.draw(linePreview, Scalar(255, 0, 0));
    rightLine.draw(linePreview, Scalar(255, 0, 255));
    bottomLine.draw(linePreview, Scalar(0, 0, 255));
    leftLine.draw(linePreview, Scalar(255, 255, 0));


    array<Point2f, 4> detectedCorners;
    // TODO: Uzupelnij wyznaczanie pozostalych przeciec po uzupełnieniu funkcji intersection().
    //       Kolejnosc punktow w tablicy powinna byc nastepujaca: top-left, top-right, bottom-right, bottom-left.
    detectedCorners[0] = intersection(leftLine, topLine);
    // detectedCorners[1] = 
    // detectedCorners[2] = 
    // detectedCorners[3] = 
    return detectedCorners;
}

array<Point2f, 4> detectDocumentPoly(const Mat& inputImage)
{
    Mat binaryImage;
    // TODO: Przygotuj obraz binarny dokumentu.
    //       - Przekonwertuj obraz wejsciowy do skali szarosci (cvtColor()).
    //       - Wykonaj progowanie, np. metoda Otsu (threshold()).

    vector<vector<Point>> contours;
    // TODO: Znajdz kontury na obrazie binarnym i zapisz je do wektora 'contours' (findContours()).

    // Wybranie konturu o najwiekszym polu jako obrysu dokumentu.
    vector<Point> documentContour = *max_element(contours.begin(), contours.end(),
        [](const vector<Point> &firstContour, const vector<Point> &secondContour)
        {
            return contourArea(firstContour) < contourArea(secondContour);
        });

    // Narysowanie znalezionego konturu na kopii obrazu, aby pokazac wynik segmentacji.
    Mat contourPreview = inputImage.clone();
    polylines(contourPreview, documentContour, true, Scalar(0, 255, 0), 20);

    vector<Point> approximatedContour;
    // TODO: Przybliz kontur dokumentu wielokatem o mniejszej liczbie wierzcholkow.
    //       - Uzyj funkcji approxPolyDP().
    //       - Dobierz parametr aproksymacji na podstawie dlugosci konturu.
    // TODO: Narysuj przyblizony kontur na obrazie podgladowym i wyswietl wynik.

    // TODO: Wyznacz i przypisz cztery wierzcholki dokumentu na podstawie punktow z 'approximatedContour'.
    //       - Wierzcholek top-left ma najmniejsza sume wspolrzednych x + y.
    //       - Wierzcholek bottom-right ma najwieksza sume wspolrzednych x + y.
    //       - Wierzcholek top-right ma najwieksza roznice wspolrzednych x - y.
    //       - Wierzcholek bottom-left ma najmniejsza roznice wspolrzednych x - y.
    Point topLeft = Point(0, 0);
    Point bottomRight = Point(0, 0);
    Point topRight = Point(0, 0);
    Point bottomLeft = Point(0, 0);

    circle(contourPreview, topLeft, 20, Scalar(255, 0, 0), -1);
    circle(contourPreview, topRight, 20, Scalar(0, 255, 0), -1);
    circle(contourPreview, bottomRight, 20, Scalar(0, 0, 255), -1);
    circle(contourPreview, bottomLeft, 20, Scalar(255, 255, 0), -1);

    return {
        topLeft,
        topRight,
        bottomRight,
        bottomLeft
    };
}

Mat alignDocument(const Mat& inputImage, array<Point2f, 4> detectedPoints)
{
    float shortSide = 2480;
    float longSide = 3508;
    // TODO: Uzupelnij punkty docelowe dla wyprostowanego dokumentu.
    //       Kolejnosc punktow w tablicy powinna byc nastepujaca: top-left, top-right, bottom-right, bottom-left.
    //       Skorzystaj z longSide i shortSide
    array<Point2f, 4> targetPoints = {
        Point2f(0, 0),
        Point2f(0, 0),
        Point2f(0, 0),
        Point2f(0, 0)
    };

    // TODO: Wyznacz macierz transformacji perspektywicznej na podstawie punktow wejsciowych i docelowych.
    //       Uzyj funkcji getPerspectiveTransform().
    Mat transform;
    Mat output;
    // TODO: Przeksztalc obraz wejsciowy do widoku dokumentu z gory i zapisz wynik do zmiennej 'output'.
    //       Uzyj funkcji warpPerspective().
    return output;
}

void readText(const Mat& inputImage)
{
    Mat binaryImage;
    cvtColor(inputImage, binaryImage, COLOR_BGR2GRAY);
    threshold(binaryImage, binaryImage, 0, 255, THRESH_OTSU);

    // TODO: Uzupełnić funkcję create. W szczególności uwzględnić, że mają być rozpoznawane polskie litery
    // Więcej informacji na https://docs.opencv.org/4.x/d7/ddc/classcv_1_1text_1_1OCRTesseract.html
    auto ocr = text::OCRTesseract::create();

    string detectedText;
    // TODO: Do wykrycia tekstu użyć metody ocr->run() na podstawie dokumentacji  run() [4/4]:
    // https://docs.opencv.org/4.x/d7/ddc/classcv_1_1text_1_1OCRTesseract.html#af2bb9e7710a5bf616367d8898334001c
    cout << detectedText << endl;
}


void testOneImage(const string& imagePath)
{
    Mat image = imread(imagePath);

    // TODO: Uzupełnij funkcję do wykrywania wierzchołków dokumentu i użyj, żeby uzyskać wierzchołki
    // auto vertices = detectDocumentPoly(image);
    // auto vertices = detectDocumentLines(image);

    // TODO: Uzupełnij funkcję wyrównującą dokument na podstawie wcześniej znalezionych punktów
    // Mat alignedDocument = alignDocument(image, vertices);

    // TODO: Uzupełnij funkcję do czytania tekstu
    // readText(alignedDocument);
}


void testWholeDir(const string& dirPath)
{
    // Przejdź rekurencyjnie po katalogu i przetwarzaj pliki graficzne.
    for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
        const string extension = entry.path().extension().string();
        // Filtruj tylko pliki .jpg oraz .png
        if (extension != ".jpg" && extension != ".png") {
            continue;
        }
        testOneImage(entry.path().string());
    }
}

int main(int argc, char** argv)
{
    fs::path path(argv[1]);
    error_code error;

    // W zależności od typu ścieżki, przetwarzany jest pojedynczy plik albo cały katalog.
    if (fs::is_regular_file(path, error)) {
        testOneImage(path.string());
    } else if (fs::is_directory(path, error)) {
        testWholeDir(path.string());
    } else {
        cerr << "Podana ścieżka nie jest plikiem ani katalogiem." << endl;
        return 1;
    }

    return 0;
}
