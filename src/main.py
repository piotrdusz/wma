import sys
from pathlib import Path
import cv2 as cv
import numpy as np


def testOneImage(imagePath: str):
    # TODO 1: Wczytaj obraz do zmiennej 'img' (imread()).
    #         Wyświetl go w oknie (imshow(), waitKey()).
    img = None

    # TODO 2: Utwórz obraz binarny 'bin' zawierający segmentację koloru.
    #         - Przekonwertuj obraz z przestrzeni BGR do HSV (cvtColor()).
    #         - Wybierz zakres koloru niebieskiego (inRange()).
    #         - Wyświetl wynik po progowaniu (imshow()).
    bin = None

    # TODO 3 (opcjonalnie): Zastosuj operacje morfologiczne, aby odszumić obraz binarny.
    #         - Utwórz element strukturalny (getStructuringElement()).
    #         - Wykonaj wybrane operacje morfologiczne.
    #         - Wyświetl wynik po morfologii.

    # TODO 4: Znajdź kontury na obrazie binarnym (findContours()).
    #         Zapisz je do wektora 'contours'.
    contours = []

    # TODO 5: Wybierz kontury o kształcie zbliżonym do koła i odpowiednio dużym polu.
    #         - Dla każdego konturu policz pole (contourArea()) i obwód (arcLength()).
    #         - Zdefiniuj miarę "okrągłości", np. współczynnik Malinowskiej
    #         - Jeśli współczynnik jest mały i pole jest wystarczająco duże, dodaj kontur do 'circles'.
    circles = []

    # Wizualizacja: narysuj wybrane kontury (przybliżone koła) na kopii obrazu.
    signs = img.copy()
    cv.drawContours(signs, circles, -1, (0, 255, 0), 3)
    cv.imshow("Road signs", signs)

    # Wczytywanie szablonów
    templateNames = ["c2.png", "c5.png", "c9.png", "c12.png"]
    templates = []
    for name in templateNames:
        templates.append(cv.imread("./templates/" + name, cv.IMREAD_GRAYSCALE))

    # Dla każdego znalezionego konturu wycinany jest pojedynczy znak oraz zostawiana tylko jego zawartość
    # Tak przygotowany obraz należy porównać z szablonami i znaleźć ten o najlepszym dopasowaniu
    if bin is not None:
        for circ in circles:
            # Wycięcie ROI pojedynczego znaku na podstawie konturu.
            x, y, w, h = cv.boundingRect(circ)
            tmp = bin.copy()
            cv.fillConvexPoly(tmp, circ, 255)
            out = cv.bitwise_xor(tmp, bin)
            oneSign = out[y:y + h, x:x + w]
            cv.imshow("One sign", oneSign)
            cv.waitKey(0)

            # TODO 6 Porównaj 'one_sign' z każdym templatem przy użyciu matchShapes().
            # Wypisz nazwę najlepszego dopasowania.
    cv.waitKey(0)


def testWholeDir(dirPath: str):
    # Przejdź rekurencyjnie po katalogu i przetwarzaj pliki graficzne.
    for entry in Path(dirPath).rglob("*"):
        extension = entry.suffix
        # Filtruj tylko pliki .jpg oraz .png
        if extension != ".jpg" and extension != ".png":
            continue
        testOneImage(str(entry))


if __name__ == "__main__":
    p = Path(sys.argv[1])

    # W zależności od typu ścieżki, przetwarzany jest pojedynczy plik albo cały katalog.
    if p.is_file():
        testOneImage(str(p))
    elif p.is_dir():
        testWholeDir(str(p))
    else:
        print("Podana ścieżka nie jest plikiem ani katalogiem.", file=sys.stderr)
        sys.exit(1)

    sys.exit(0)
