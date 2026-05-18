import cv2
import os
import numpy as np
from pathlib import Path
from polar_line import PolarLine, collect_document_lines


def show_scaled(winname, input_image):
    if input_image is None or input_image.size == 0:
        return
    
    screen_width = 1920
    screen_height = 1080
    
    max_width = screen_width * 0.9
    max_height = screen_height * 0.9
    scale_x = max_width / input_image.shape[1]
    scale_y = max_height / input_image.shape[0]
    scale = min(1.0, min(scale_x, scale_y))
    
    preview = cv2.resize(input_image, None, fx=scale, fy=scale, interpolation=cv2.INTER_AREA)
    cv2.imshow(winname, preview)
    cv2.waitKey()


def intersection(first_line, second_line):
    a = np.cos(first_line.theta)
    b = np.sin(first_line.theta)
    c = first_line.rho
    
    d = np.cos(second_line.theta)
    e = np.sin(second_line.theta)
    f = second_line.rho
    
    # TODO: Wyznacz punkt przeciecia prostych na podstawie ukladu rownan:
    #       a * x + b * y = c
    #       d * x + e * y = f
    #       Mozna uzyc wzorow Cramera.
    x = 0
    y = 0
    return (x, y)


def detect_document_lines(input_image):
    line_preview = input_image.copy()
    
    edges = None
    # TODO: Przygotuj obraz krawedzi do wykrywania linii.
    #       - Przekonwertuj obraz wejsciowy do skali szarosci (cvtColor()).
    #       - Wykryj krawedzie, np. algorytmem Canny'ego (Canny()).
    #       - W razie potrzeby wyswietl wynik posredni.
    
    line_segments = []
    # TODO: Wykryj odcinki prostych na obrazie krawedzi.
    #       - Uzyj probabilistycznej transformaty Hougha (HoughLinesP()).
    #       - Dobierz parametry tak, aby pozostawic glownie krawedzie dokumentu.
    #       - Zapisz wynik do listy 'line_segments'.
    
    # Podzielenie wykrytych odcinkow na linie poziome i pionowe.
    horizontal_lines, vertical_lines = collect_document_lines(line_segments)
    
    if len(horizontal_lines) < 2 or len(vertical_lines) < 2:
        return np.array([], dtype=np.float32)
    
    # Wybranie skrajnych linii jako gornej, prawej, dolnej i lewej krawedzi dokumentu.
    top_line = horizontal_lines[0]
    right_line = vertical_lines[-1]
    bottom_line = horizontal_lines[-1]
    left_line = vertical_lines[0]
    
    # Narysowanie wybranych krawedzi na kopii obrazu w celu wizualizacji wyniku detekcji.
    top_line.draw(line_preview, (255, 0, 0))
    right_line.draw(line_preview, (255, 0, 255))
    bottom_line.draw(line_preview, (0, 0, 255))
    left_line.draw(line_preview, (255, 255, 0))
    
    detected_corners = np.array([
        intersection(left_line, top_line),
        np.array([0, 0], dtype=np.float32),
        np.array([0, 0], dtype=np.float32),
        np.array([0, 0], dtype=np.float32)
    ], dtype=np.float32)
    
    # TODO: Uzupelnij wyznaczanie pozostalych przeciec po uzupełnieniu funkcji intersection().
    #       Kolejnosc punktow w tablicy powinna byc nastepujaca: top-left, top-right, bottom-right, bottom-left.
    # detected_corners[1] = 
    # detected_corners[2] = 
    # detected_corners[3] = 
    return detected_corners


def detect_document_poly(input_image):
    binary_image = None
    # TODO: Przygotuj obraz binarny dokumentu.
    #       - Przekonwertuj obraz wejsciowy do skali szarosci (cvtColor()).
    #       - Wykonaj progowanie, np. metoda Otsu (threshold()).
    
    contours = None
    # TODO: Znajdz kontury na obrazie binarnym i zapisz je do listy 'contours' (findContours()).
    
    if not contours:
        return np.array([], dtype=np.float32)
    
    # Wybranie konturu o najwiekszym polu jako obrysu dokumentu.
    document_contour = max(contours, key=cv2.contourArea)
    
    # Narysowanie znalezionego konturu na kopii obrazu, aby pokazac wynik segmentacji.
    contour_preview = input_image.copy()
    cv2.polylines(contour_preview, [document_contour], True, (0, 255, 0), 20)
    
    approximated_contour = None
    # TODO: Przybliz kontur dokumentu wielokatem o mniejszej liczbie wierzcholkow.
    #       - Uzyj funkcji approxPolyDP().
    #       - Dobierz parametr aproksymacji na podstawie dlugosci konturu.
    # TODO: Narysuj przyblizony kontur na obrazie podgladowym i wyswietl wynik.
    
    # TODO: Wyznacz i przypisz cztery wierzcholki dokumentu na podstawie punktow z 'approximated_contour'.
    #       - Wierzcholek top-left ma najmniejsza sume wspolrzednych x + y.
    #       - Wierzcholek bottom-right ma najwieksza sume wspolrzednych x + y.
    #       - Wierzcholek top-right ma najwieksza roznice wspolrzednych x - y.
    #       - Wierzcholek bottom-left ma najmniejsza roznice wspolrzednych x - y.
    top_left = (0, 0)
    bottom_right = (0, 0)
    top_right = (0, 0)
    bottom_left = (0, 0)
    
    cv2.circle(contour_preview, top_left, 20, (255, 0, 0), -1)
    cv2.circle(contour_preview, top_right, 20, (0, 255, 0), -1)
    cv2.circle(contour_preview, bottom_right, 20, (0, 0, 255), -1)
    cv2.circle(contour_preview, bottom_left, 20, (255, 255, 0), -1)
    
    return np.array([top_left, top_right, bottom_right, bottom_left], dtype=np.float32)


def align_document(input_image, detected_points):
    short_side = 2480
    long_side = 3508
    
    # TODO: Uzupelnij punkty docelowe dla wyprostowanego dokumentu.
    #       Kolejnosc punktow w tablicy powinna byc nastepujaca: top-left, top-right, bottom-right, bottom-left.
    #       Skorzystaj z long_side i short_side
    target_points = np.array([
        [0, 0],
        [0, 0],
        [0, 0],
        [0, 0]
    ], dtype=np.float32)
    
    # TODO: Wyznacz macierz transformacji perspektywicznej na podstawie punktow wejsciowych i docelowych.
    #       Uzyj funkcji getPerspectiveTransform().
    transform = None
    output = None
    # TODO: Przeksztalc obraz wejsciowy do widoku dokumentu z gory i zapisz wynik do zmiennej 'output'.
    #       Uzyj funkcji warpPerspective().
    
    return output


def read_text(input_image):
    binary_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2GRAY)
    _, binary_image = cv2.threshold(binary_image, 0, 255, cv2.THRESH_OTSU)
    
    # TODO: Uzupełnić funkcję create. W szczególności uwzględnić, że mają być rozpoznawane polskie litery
    # Więcej informacji na https://docs.opencv.org/4.x/d7/ddc/classcv_1_1text_1_1OCRTesseract.html
    ocr = cv2.text.OCRTesseract_create()
    
    detected_text = ""
    # TODO: Do wykrycia tekstu użyć metody ocr.run() na podstawie dokumentacji run() [4/4]:
    # https://docs.opencv.org/4.x/d7/ddc/classcv_1_1text_1_1OCRTesseract.html#af2bb9e7710a5bf616367d8898334001c
    
    print(detected_text)


def test_one_image(image_path):
    image = cv2.imread(image_path)
    
    # TODO: Uzupełnij funkcję do wykrywania wierzchołków dokumentu i użyj, żeby uzyskać wierzchołki
    # vertices = detect_document_poly(image)
    # vertices = detect_document_lines(image)
    
    # TODO: Uzupełnij funkcję wyrównującą dokument na podstawie wcześniej znalezionych punktów
    # aligned_document = align_document(image, vertices)
    
    # TODO: Uzupełnij funkcję do czytania tekstu
    # read_text(aligned_document)


def test_whole_dir(dir_path):
    # Przejdź rekurencyjnie po katalogu i przetwarzaj pliki graficzne.
    for root, dirs, files in os.walk(dir_path):
        for file in files:
            extension = os.path.splitext(file)[1].lower()
            # Filtruj tylko pliki .jpg oraz .png
            if extension not in [".jpg", ".png"]:
                continue
            file_path = os.path.join(root, file)
            test_one_image(file_path)


def main(argv):
    if len(argv) < 2:
        print("Podana ścieżka nie jest plikiem ani katalogiem.")
        return 1
    
    path = argv[1]
    
    # W zależności od typu ścieżki, przetwarzany jest pojedynczy plik albo cały katalog.
    if os.path.isfile(path):
        test_one_image(path)
    elif os.path.isdir(path):
        test_whole_dir(path)
    else:
        print("Podana ścieżka nie jest plikiem ani katalogiem.")
        return 1
    
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv))
