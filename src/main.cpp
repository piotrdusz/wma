#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>
#include <vector>
#include <array>

using namespace std;
using namespace cv;
namespace fs = filesystem;

void testOneImage(const string &imagePath)
{
    // TODO 1: Wczytaj obraz do zmiennej 'img' (imread()).
    //         Wyświetl go w oknie (imshow(), waitKey()).
    Mat img;

    // TODO 2: Utwórz obraz binarny 'bin' zawierający segmentację koloru.
    //         - Przekonwertuj obraz z przestrzeni BGR do HSV (cvtColor()).
    //         - Wybierz zakres koloru niebieskiego (inRange()).
    //         - Wyświetl wynik po progowaniu (imshow()).
    Mat bin;

    // TODO 3 (opcjonalnie): Zastosuj operacje morfologiczne, aby odszumić obraz binarny.
    //         - Utwórz element strukturalny (getStructuringElement()).
    //         - Wykonaj wybrane operacje morfologiczne.
    //         - Wyświetl wynik po morfologii.

    // TODO 4: Znajdź kontury na obrazie binarnym (findContours()).
    //         Zapisz je do wektora 'contours'.
    vector<vector<Point>> contours;

    // TODO 5: Wybierz kontury o kształcie zbliżonym do koła i odpowiednio dużym polu.
    //         - Dla każdego konturu policz pole (contourArea()) i obwód (arcLength()).
    //         - Zdefiniuj miarę "okrągłości", np. współczynnik Malinowskiej
    //         - Jeśli współczynnik jest mały i pole jest wystarczająco duże, dodaj kontur do 'circles'.
    vector<vector<Point>> circles;

    // Wizualizacja: narysuj wybrane kontury (przybliżone koła) na kopii obrazu.
    Mat signs = img.clone();
    drawContours(signs, circles, -1, Scalar(0, 255, 0), 3);
    imshow("Road signs", signs);


    // Wczytywanie szablonów
    const fs::path template_dir = TEMPLATE_DIR;
    const array<string, 4> templateNames = {"c2.png", "c5.png", "c9.png", "c12.png"};
    Mat templates[4];
    for (int i = 0; i < 4; ++i) {
        templates[i] = imread((template_dir / templateNames[i]).string(), IMREAD_GRAYSCALE);
    }

    // Dla każdego znalezionego konturu wycinany jest pojedynczy znak oraz zostawiana tylko jego zawartość
    // Tak przygotowany obraz należy porównać z szablonami i znaleźć ten o najlepszym dopasowaniu
    if (!bin.empty()) {
        for (const auto &circ : circles) {
            // Wycięcie ROI pojedynczego znaku na podstawie konturu.
            Rect bbox = boundingRect(circ);
            Mat tmp = bin.clone();
            fillConvexPoly(tmp, circ, Scalar(255));
            Mat out;
            bitwise_xor(tmp, bin, out);
            Mat oneSign = out(bbox);
            imshow("One sign", oneSign);
            waitKey();

            // TODO 6 Porównaj 'one_sign' z każdym templatem przy użyciu matchShapes().
            // Wypisz nazwę najlepszego dopasowania.
        }
    }
    waitKey();
}

void testWholeDir(const string &dirPath)
{
    // Przejdź rekurencyjnie po katalogu i przetwarzaj pliki graficzne.
    for (const auto &entry : fs::recursive_directory_iterator(dirPath)) {
        const string extension = entry.path().extension().string();
        // Filtruj tylko pliki .jpg oraz .png
        if (extension != ".jpg" && extension != ".png") {
            continue;
        }
        testOneImage(entry.path().string());
    }
}

int main(int argc, char **argv)
{
    fs::path p(argv[1]);
    error_code ec;

    // W zależności od typu ścieżki, przetwarzany jest pojedynczy plik albo cały katalog.
    if (fs::is_regular_file(p, ec)) {
        testOneImage(p.string());
    } else if (fs::is_directory(p, ec)) {
        testWholeDir(p.string());
    } else {
        cerr << "Podana ścieżka nie jest plikiem ani katalogiem." << endl;
        return 1;
    }

    return 0;
}
