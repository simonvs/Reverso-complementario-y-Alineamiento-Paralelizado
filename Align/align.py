import random
import time
from Bio import pairwise2

def generar_secuencia_adn(longitud):
    bases = ['A', 'T', 'C', 'G']
    secuencia = ''.join(random.choice(bases) for _ in range(longitud))
    return secuencia

def alinear_secuencias(secuencia1, secuencia2):
    alignments = pairwise2.align.globalxx(secuencia1, secuencia2)
    best_alignment = alignments[0]
    alineamiento1, alineamiento2, score, start, end = best_alignment
    print("Secuencia 1:", secuencia1)
    print("Secuencia 2:", secuencia2)
    print("Alineamiento 1:", alineamiento1)
    print("Alineamiento 2:", alineamiento2)

def main():
    longitud1 = int(input("Ingrese la longitud de la secuencia 1: "))
    longitud2 = int(input("Ingrese la longitud de la secuencia 2: "))

    secuencia1 = generar_secuencia_adn(longitud1)
    secuencia2 = generar_secuencia_adn(longitud2)

    start_time = time.time()
    alinear_secuencias(secuencia1, secuencia2)
    end_time = time.time()

    elapsed_time = end_time - start_time
    print("Tiempo de ejecución:", elapsed_time, "segundos")

if __name__ == "__main__":
    main()
