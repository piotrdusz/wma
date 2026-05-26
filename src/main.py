import cv2
import sys

def diff_motion_detection(video_path):
    capture = cv2.VideoCapture(video_path)
    ret, prev_frame = capture.read()
    
    while capture.read()[0]:
        ret, frame = capture.read()
        preview = frame.copy()

        # TODO: Oblicz różnicę bezwzględną między bieżącą a poprzednią klatką (absdiff()).
        diff = None

        # TODO: Przygotuj maskę binarną na podstawie różnicy klatek.
        #       - Przekonwertuj obraz różnicy do skali szarości (cvtColor()).
        #       - Wykonaj progowanie (threshold()).
        binary_mask = None

        # TODO: Zastosuj operacje morfologiczne na masce binarnej, aby poprawić efekt progowania
        #       - Utwórz element strukturalny (getStructuringElement()).
        #       - Użyj morfologii (morphologyEx()).
        morph_mask = None

        # TODO: Znajdź kontury na masce morfologicznej (findContours()).
        contours = None

        # TODO: Przejdź pętlą po wykrytych konturach i wyznacz prostokąty otaczające (boundingRect()).
        #       Na podstawie własnego kryterium wybierz "najlepszy" prostokąt spośród wszystkich
        #       i narysuj go na obrazie podglądu (rectangle()).

        cv2.imshow("Preview", preview)
        cv2.waitKey(10)
        prev_frame = frame.copy()


def back_sub(video_path):
    capture = cv2.VideoCapture(video_path)

    # TODO: Utwórz obiekt subtraktora tła. Do wyboru:
    #       - createBackgroundSubtractorMOG2() - mieszanina gaussianów;
    #       - createBackgroundSubtractorKNN()  - metoda k najbliższych sąsiadów;
    #       Zwróć szczególną uwagę na dobór parametrów wpływających na czułość i stabilność maski.
    back_subtractor = None

    while capture.read()[0]:
        ret, frame = capture.read()
        preview = frame.copy()

        # TODO: Zastosuj subtractor tła na bieżącej klatce, aby uzyskać maskę pierwszego planu (apply()).
        fg_mask = None

        # TODO: Zastosuj operacje morfologiczne na masce, aby usunąć szum.
        #       - Utwórz element strukturalny (getStructuringElement()).
        #       - Użyj morfologii (morphologyEx()).
        morph_mask = None

        # TODO: Znajdź kontury na masce morfologicznej (findContours()).
        contours = None

        # TODO: Przejdź pętlą po wykrytych konturach i wyznacz prostokąty otaczające (boundingRect()).
        #       Na podstawie własnego kryterium wybierz "najlepszy" prostokąt spośród wszystkich
        #       i narysuj go na obrazie podglądu (rectangle()).

        cv2.imshow("Preview", preview)
        cv2.waitKey(10)


def optical_flow(video_path):
    capture = cv2.VideoCapture(video_path)
    ret, frame = capture.read()

    # Opcjonalnie: zmniejsz klatkę przed pętlą (pyrDown()), aby przyspieszyć obliczenia —
    # przepływ optyczny jest kosztowny obliczeniowo, im mniejszy obraz, tym szybciej.

    # TODO: Przekonwertuj pierwszą klatkę do skali szarości (cvtColor()),
    #       aby uzyskać punkt odniesienia dla pierwszej iteracji pętli.
    prev_gray = None

    while capture.read()[0]:
        ret, frame = capture.read()

        # Opcjonalnie: zmniejsz klatkę (pyrDown()) — analogicznie jak przed pętlą.

        # TODO: Przekonwertuj bieżącą klatkę do skali szarości (cvtColor()).
        gray_frame = None

        # TODO: Oblicz gęsty przepływ optyczny metodą Farnebäcka między poprzednią
        #       a bieżącą klatką (calcOpticalFlowFarneback()).
        #       Jako flagę wpisz 0
        flow = None

        # Rozdzielenie macierzy przepływu na składowe x i y.
        flow_channels = cv2.split(flow)

        # Obliczenie magnitudy i kąta z wektorów przepływu.
        magnitude, angle = cv2.cartToPolar(flow_channels[0], flow_channels[1], angleInDegrees=True)

        # Normalizacja magnitudy do zakresu [0, 255] na potrzeby wizualizacji.
        cv2.normalize(magnitude, magnitude, 0.0, 255.0, cv2.NORM_MINMAX)

        # Skalowanie kąta do zakresu [0, 180] — wartość kanału H w przestrzeni HSV.
        angle /= 2.0

        # Kodowanie przepływu jako obraz HSV: kąt -> barwa (kierunek), magnituda -> nasycenie i jasność.
        hsv_channels = [angle, magnitude, magnitude]
        hsv_image = cv2.merge(hsv_channels)
        hsv_image = hsv_image.astype('uint8')

        # Konwersja z HSV do BGR w celu wyświetlenia.
        bgr_image = cv2.cvtColor(hsv_image, cv2.COLOR_HSV2BGR)

        cv2.imshow("Flow", bgr_image)
        cv2.waitKey(10)
        prev_gray = gray_frame.copy()


if __name__ == "__main__":
    # Odkomentuj testowaną funkcję
    # diff_motion_detection(sys.argv[1])
    # back_sub(sys.argv[1])
    # optical_flow(sys.argv[1])
    pass
