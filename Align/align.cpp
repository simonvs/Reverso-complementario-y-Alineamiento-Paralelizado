#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <omp.h>

using namespace std;

// Función para obtener el máximo de tres números
int max(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

// Función para generar una secuencia aleatoria de ADN de una longitud dada
string generarSecuenciaADN(int longitud) {
    string secuencia;
    secuencia.reserve(longitud);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 3);

    #pragma omp parallel for
    for (int i = 0; i < longitud; i++) {
        int aleatorio = dis(gen);

        switch (aleatorio) {
            case 0:
                secuencia[i] = 'A';
                break;
            case 1:
                secuencia[i] = 'T';
                break;
            case 2:
                secuencia[i] = 'C';
                break;
            case 3:
                secuencia[i] = 'G';
                break;
        }
    }

    return secuencia;
}

// Función para realizar el alineamiento de dos secuencias de ADN
void alinearSecuencias(const string& secuencia1, const string& secuencia2, int gap_penalty, int match_score, int mismatch_penalty) {
    int len1 = secuencia1.length();
    int len2 = secuencia2.length();

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));  // Matriz para almacenar los puntajes de alineamiento
    vector<vector<char>> path(len1 + 1, vector<char>(len2 + 1));  // Matriz para almacenar las direcciones de los movimientos

    // Inicializar la primera fila y la primera columna de la matriz de puntajes
    #pragma omp parallel for
    for (int i = 0; i <= len1; i++) {
        dp[i][0] = i * gap_penalty;
    }

    #pragma omp parallel for
    for (int j = 0; j <= len2; j++) {
        dp[0][j] = j * gap_penalty;
    }

    // Calcular los puntajes de alineamiento y las direcciones de los movimientos
    #pragma omp parallel for collapse(2)
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int match = (secuencia1[i - 1] == secuencia2[j - 1]) ? match_score : -mismatch_penalty;

            int diagonal = dp[i - 1][j - 1] + match;
            int arriba = dp[i - 1][j] + gap_penalty;
            int izquierda = dp[i][j - 1] + gap_penalty;

            dp[i][j] = max(diagonal, arriba, izquierda);

            if (dp[i][j] == diagonal) {
                path[i][j] = 'D';  // Diagonal
            } else if (dp[i][j] == arriba) {
                path[i][j] = 'U';  // Arriba
            } else {
                path[i][j] = 'L';  // Izquierda
            }
        }
    }

    // Realizar el traceback para obtener los alineamientos
    string alineamiento1, alineamiento2;
    int i = len1;
    int j = len2;

    while (i > 0 && j > 0) {
        switch (path[i][j]) {
            case 'D':
                alineamiento1 = secuencia1[i - 1] + alineamiento1;
                alineamiento2 = secuencia2[j - 1] + alineamiento2;
                i--;
                j--;
                break;
            case 'U':
                alineamiento1 = secuencia1[i - 1] + alineamiento1;
                alineamiento2 = '-' + alineamiento2;
                i--;
                break;
            case 'L':
                alineamiento1 = '-' + alineamiento1;
                alineamiento2 = secuencia2[j - 1] + alineamiento2;
                j--;
                break;
        }
    }

    while (i > 0) {
        alineamiento1 = secuencia1[i - 1] + alineamiento1;
        alineamiento2 = '-' + alineamiento2;
        i--;
    }

    while (j > 0) {
        alineamiento1 = '-' + alineamiento1;
        alineamiento2 = secuencia2[j - 1] + alineamiento2;
        j--;
    }

    cout << "Secuencia 1: " << secuencia1 << endl;
    cout << "Secuencia 2: " << secuencia2 << endl;
    cout << "Alineamiento 1: " << alineamiento1 << endl;
    cout << "Alineamiento 2: " << alineamiento2 << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <longitud_secuencia1> <longitud_secuencia2>" << endl;
        return 1;
    }

    int longitud1 = stoi(argv[1]);
    int longitud2 = stoi(argv[2]);

    int gap_penalty = -1;
    int match_score = 1;
    int mismatch_penalty = 2;

    int num_threads = 4;

    omp_set_num_threads(num_threads);

    // Medir el tiempo inicial
    double start_time = omp_get_wtime();

    string secuencia1 = generarSecuenciaADN(longitud1);
    string secuencia2 = generarSecuenciaADN(longitud2);

    alinearSecuencias(secuencia1, secuencia2, gap_penalty, match_score, mismatch_penalty);

    // Medir el tiempo final
    double end_time = omp_get_wtime();

    cout << "Tiempo de ejecución: " << end_time - start_time << " segundos" << endl;

    return 0;
}
