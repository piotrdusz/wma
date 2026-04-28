# Widzenie maszynowe - Politechnika Warszawska - Wydział Mechatroniki
Szablony programów do widzenia maszynowego. Każdy szablon, będący jednocześnie instrukcją wykonania zadania znajduje się na osobnym branchu. W katalogu src/ znajduje się kod źródłowy, w data/ należy umieścić dane do naszego zadania, a w build/ będzie budowany program.

## 0. Instalacja OpenCV
Instalacja OpenCV na Windowsie bywa kłopotliwa, z tego względu sugeruję przejście na jakąś dystrybucję Linuksa, np. Ubuntu.

Instalacja Ubuntu: [Install Ubuntu Desktop](https://documentation.ubuntu.com/desktop/en/latest/tutorial/install-ubuntu-desktop/). Można je zainstalować jako jedyny system lub obok istniejącego.

Alternatywnie, jeśli ktoś nie ma ochoty na instalowanie osobnego systemu to polecam zapoznać się z [WSLem](https://learn.microsoft.com/en-us/windows/wsl/) (ang. *Windows Subsystem for Linux*). Jest on znacznie wygodniejszy niż tradycyjne maszyny wirtualne, oraz nie tak kłopotliwy jak dual-boot.

Oczywiście zainstalowanie OpenCV na Windowsie też jest możliwe, ale w razie trudności raczej nie będę umiał pomóc.

Żeby zainstalować OpenCV należy wpisać w terminalu polecenie:
```
sudo apt install build-essential cmake libopencv-dev
```

Instaluje to stabilną (nie najnowszą, ale w zupełności wystarczającą do tych zajęć) wersję biblioteki. Jeśli jednak ktoś chce zainstalować najnowszą wersję to należy postępować zgodnie z [instrukcją](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html). Sugeruję zbudować biblioteką z dodatkowymi modułami, czyli ramkę **Build with opencv_contrib**.

Żeby sprawdzić, czy instalacja przebiegła pomyślnie trzeba sklonować to repozytorium, otworzyć terminal (WSLa) i przejść do katalogu go zawierającego, a następnie:

```
cd build # Przejść do katalogu, gdzie program będzie budowany
cmake .. # Generuje pliki konfiguracyjne
make # Buduje program
./main # Uruchamia program
```

Jeśli wszystko poszło zgodnie z planem powinien wyświetlić się komunikat z wersją OpenCV. Można oczywiście do pisania i budowania programu użyć ulubionego edytora/środowiska.

## 1. i 2. Podstawowe operacje na obrazie.
Celem ćwiczenia jest nauka podstawowych funkcji z OpenCV, takich jak:
* wczytywanie obrazu/filmu,
* wyświetlanie obrazu,
* operacje bezkontekstowe,
* operacje morfologiczne,
* filtrowanie,
* operacje geometryczne.

## 3. Wykrywanie znaków nakazu.
Celem ćwiczenia jest napisanie programu, który będzie wykrywał znaki nakazu, zaznaczał je na obrazie oraz wykonywał prostą klasyfikację.

## 4. Skaner dokumentów.
Celem ćwiczenia jest napisanie programu, który będzie wykrywał gdzie znajduje się dokument, "prostował" go, a następnie czytał tekst przy pomocy OCRa.

## 5. TODO
Jeszcze się zastanawiam.

## 6. Śledzenie obiektu.
Celem ćwiczenia jest napisanie programu, służącego do śledzenia ruchomego obiektu oraz wyznaczenie jego trajektorii ruchu.

## 7. Sieci neuronowe - wersja A
Nauczenie prostego klasyfikatora przy pomocy konwolucyjnej sieci neuronowej.

## 7. Sieci neuronowe - wersja B
Zadanie mające na celu lepsze zrozumienie pomiędzy konwolucyjnymi sieciami neuronowymi, a klasycznymi metodami przetwarzania obrazów.