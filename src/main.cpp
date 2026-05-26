#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void diffMotionDetection(const string& videoPath)
{
    VideoCapture capture(videoPath);
    Mat frame, prevFrame;
    capture.read(prevFrame);
    while (capture.read(frame))
    {
        Mat preview = frame.clone();

        Mat diff;
        // TODO: Oblicz różnicę bezwzględną między bieżącą a poprzednią klatką (absdiff()).

        Mat binaryMask;
        // TODO: Przygotuj maskę binarną na podstawie różnicy klatek.
        //       - Przekonwertuj obraz różnicy do skali szarości (cvtColor()).
        //       - Wykonaj progowanie (threshold()).

        Mat morphMask;
        // TODO: Zastosuj operacje morfologiczne na masce binarnej, aby poprawić efekt progowania
        //       - Utwórz element strukturalny (getStructuringElement()).
        //       - Użyj morfologii (morphologyEx()).

        vector<vector<Point>> contours;
        // TODO: Znajdź kontury na masce morfologicznej (findContours()).

        // TODO: Przejdź pętlą po wykrytych konturach i wyznacz prostokąty otaczające (boundingRect()).
        //       Na podstawie własnego kryterium wybierz "najlepszy" prostokąt spośród wszystkich
        //       i narysuj go na obrazie podglądu (rectangle()).

        imshow("Preview", preview);
        waitKey(10);
        prevFrame = frame.clone();
    }
}

void backSub(const string& videoPath)
{
    VideoCapture capture(videoPath);
    Mat frame;

    // TODO: Utwórz obiekt subtraktora tła. Do wyboru:
    //       - createBackgroundSubtractorMOG2() - mieszanina gaussianów;
    //       - createBackgroundSubtractorKNN()  - metoda k najbliższych sąsiadów;
    //       Zwróć szczególną uwagę na dobór parametrów wpływających na czułość i stabilność maski.

    while (capture.read(frame))
    {
        Mat preview = frame.clone();

        Mat fgMask;
        // TODO: Zastosuj subtractor tła na bieżącej klatce, aby uzyskać maskę pierwszego planu (apply()).

        Mat morphMask;
        // TODO: Zastosuj operacje morfologiczne na masce, aby usunąć szum.
        //       - Utwórz element strukturalny (getStructuringElement()).
        //       - Użyj morfologii (morphologyEx()).

        vector<vector<Point>> contours;
        // TODO: Znajdź kontury na masce morfologicznej (findContours()).

        // TODO: Przejdź pętlą po wykrytych konturach i wyznacz prostokąty otaczające (boundingRect()).
        //       Na podstawie własnego kryterium wybierz "najlepszy" prostokąt spośród wszystkich
        //       i narysuj go na obrazie podglądu (rectangle()).

        imshow("Preview", preview);
        waitKey(10);
    }
}

void opticalFlow(const string& videoPath)
{
    VideoCapture capture(videoPath);
    Mat frame, prevGray;
    capture.read(frame);

    // Opcjonalnie: zmniejsz klatkę przed pętlą (pyrDown()), aby przyspieszyć obliczenia —
    // przepływ optyczny jest kosztowny obliczeniowo, im mniejszy obraz, tym szybciej.

    // TODO: Przekonwertuj pierwszą klatkę do skali szarości (cvtColor()),
    //       aby uzyskać punkt odniesienia dla pierwszej iteracji pętli.

    while (capture.read(frame))
    {
        // Opcjonalnie: zmniejsz klatkę (pyrDown()) — analogicznie jak przed pętlą.

        Mat grayFrame;

        // TODO: Przekonwertuj bieżącą klatkę do skali szarości (cvtColor()).

        Mat flow(grayFrame.size(), CV_32FC2);
        // TODO: Oblicz gęsty przepływ optyczny metodą Farnebäcka między poprzednią
        //       a bieżącą klatką (calcOpticalFlowFarneback()).
        //       Jako flagę wpisz 0

        // Rozdzielenie macierzy przepływu na składowe x i y.
        Mat flowChannels[2];
        split(flow, flowChannels);

        // Obliczenie magnitudy i kąta z wektorów przepływu.
        Mat magnitude, angle;
        cartToPolar(flowChannels[0], flowChannels[1], magnitude, angle, true);

        // Normalizacja magnitudy do zakresu [0, 255] na potrzeby wizualizacji.
        normalize(magnitude, magnitude, 0.0, 255.0, NORM_MINMAX);

        // Skalowanie kąta do zakresu [0, 180] — wartość kanału H w przestrzeni HSV.
        angle /= 2.0;

        // Kodowanie przepływu jako obraz HSV: kąt -> barwa (kierunek), magnituda -> nasycenie i jasność.
        Mat hsvChannels[3] = {angle, magnitude, magnitude};
        Mat hsvImage, bgrImage;
        merge(hsvChannels, 3, hsvImage);
        hsvImage.convertTo(hsvImage, CV_8U);

        // Konwersja z HSV do BGR w celu wyświetlenia.
        cvtColor(hsvImage, bgrImage, COLOR_HSV2BGR);

        imshow("Flow", bgrImage);
        waitKey(10);
        prevGray = grayFrame.clone();
    }
}

int main(int argc, char** argv)
{
    // Odkomentuj testowaną funkcję
    // diffMotionDetection(argv[1]);
    // backSub(argv[1]);
    // opticalFlow(argv[1]);
}
