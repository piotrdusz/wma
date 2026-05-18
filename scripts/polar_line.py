import math
import cv2
import numpy as np


class PolarLine:
    def __init__(self, line=None):
        self.theta = 0
        self.rho = 0
        
        if line is not None:
            # Zamiana odcinka na parametry prostej w postaci polarnej.
            dx = line[2] - line[0]
            dy = line[3] - line[1]
            
            self.theta = math.atan2(dy, dx) + math.pi / 2
            self.rho = line[0] * math.cos(self.theta) + line[1] * math.sin(self.theta)
            
            # Ujednolicenie znaku rho, aby podobne linie mialy porownywalna reprezentacje.
            if self.rho < 0:
                self.rho = -self.rho
                self.theta += math.pi
            
            # Normalizacja kata do zakresu <0, 2pi).
            self.theta = self.theta % (2 * math.pi)
            while self.theta < 0:
                self.theta += 2 * math.pi

    def __eq__(self, other):
        if not isinstance(other, PolarLine):
            return False
        
        theta_error = abs(self.theta - other.theta)
        rho_error = abs(self.rho - other.rho)
        
        # Dwie linie traktujemy jako rowne, jesli maja zblizony kat i polozenie.
        if theta_error < 0.1 and rho_error < 200:
            return True
        else:
            return False

    def __lt__(self, other):
        return self.rho < other.rho

    def is_horizontal(self):
        # Kierunek normalnej pozwala rozroznic, czy linia jest blizej poziomej czy pionowej.
        return abs(math.sin(self.theta)) > abs(math.cos(self.theta))

    def draw(self, image, color):
        a = math.cos(self.theta)
        b = math.sin(self.theta)
        x0 = a * self.rho
        y0 = b * self.rho
        
        # Rysowanie bardzo dlugiego odcinka, aby pokazac cala prosta na obrazie.
        point1 = (int(x0 - 20000 * (-b)), int(y0 - 20000 * a))
        point2 = (int(x0 + 20000 * (-b)), int(y0 + 20000 * a))
        cv2.line(image, point1, point2, color, 10)


def collect_document_lines(line_segments):
    horizontal_lines = []
    vertical_lines = []
    
    for line_segment in line_segments:
        polar_line = PolarLine(line_segment)
        
        if polar_line.is_horizontal():
            # Dodaj tylko nowe reprezentanty podobnych linii poziomych.
            if polar_line not in horizontal_lines:
                horizontal_lines.append(polar_line)
        else:
            # Dodaj tylko nowe reprezentanty podobnych linii pionowych.
            if polar_line not in vertical_lines:
                vertical_lines.append(polar_line)
    
    # Sortowanie po rho ulatwia pozniejszy wybor skrajnych krawedzi dokumentu.
    horizontal_lines.sort()
    vertical_lines.sort()
    
    return horizontal_lines, vertical_lines
