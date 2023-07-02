#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_LENGTH 100

// Función para obtener el máximo de tres números
int max(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

// Función para realizar el alineamiento de dos secuencias de ADN
void alinearSecuencias(const char* secuencia1, const char* secuencia2, int gap_penalty, int match_score, int mismatch_penalty) {
    int len1 = strlen(secuencia1);
    int len2 = strlen(secuencia2);

    int dp[MAX_LENGTH][MAX_LENGTH];  // Matriz para almacenar los puntajes de alineamiento
    char path[MAX_LENGTH][MAX_LENGTH];  // Matriz para almacenar las direcciones de los movimientos

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
    #pragma omp parallel for
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
    char alineamiento1[MAX_LENGTH * 2], alineamiento2[MAX_LENGTH * 2];
    int alineamiento_len = 0;
    int i = len1;
    int j = len2;

    while (i > 0 && j > 0) {
        switch (path[i][j]) {
            case 'D':
                alineamiento1[alineamiento_len] = secuencia1[i - 1];
                alineamiento2[alineamiento_len] = secuencia2[j - 1];
                i--;
                j--;
                break;
            case 'U':
                alineamiento1[alineamiento_len] = secuencia1[i - 1];
                alineamiento2[alineamiento_len] = '-';
                i--;
                break;
            case 'L':
                alineamiento1[alineamiento_len] = '-';
                alineamiento2[alineamiento_len] = secuencia2[j - 1];
                j--;
                break;
        }
        alineamiento_len++;
    }

    while (i > 0) {
        alineamiento1[alineamiento_len] = secuencia1[i - 1];
        alineamiento2[alineamiento_len] = '-';
        i--;
        alineamiento_len++;
    }

    while (j > 0) {
        alineamiento1[alineamiento_len] = '-';
        alineamiento2[alineamiento_len] = secuencia2[j - 1];
        j--;
        alineamiento_len++;
    }

    // Invertir los alineamientos
    char* alineamiento1_reverso = (char*) malloc((alineamiento_len + 1) * sizeof(char));
    char* alineamiento2_reverso = (char*) malloc((alineamiento_len + 1) * sizeof(char));

    #pragma omp parallel for
    for (int k = 0; k < alineamiento_len; k++) {
        alineamiento1_reverso[k] = alineamiento1[alineamiento_len - k - 1];
        alineamiento2_reverso[k] = alineamiento2[alineamiento_len - k - 1];
    }

    alineamiento1_reverso[alineamiento_len] = '\0';
    alineamiento2_reverso[alineamiento_len] = '\0';

    printf("Secuencia 1: %s\n", secuencia1);
    printf("Secuencia 2: %s\n", secuencia2);
    printf("Alineamiento 1: %s\n", alineamiento1_reverso);
    printf("Alineamiento 2: %s\n", alineamiento2_reverso);

    free(alineamiento1_reverso);
    free(alineamiento2_reverso);
}

int main() {
    const char* secuencia1 = "ATCGGCTA";
    const char* secuencia2 = "ATAGCT";

    int gap_penalty = -1;
    int match_score = 1;
    int mismatch_penalty = 2;

    int num_threads = 4;

    omp_set_num_threads(num_threads);

    alinearSecuencias(secuencia1, secuencia2, gap_penalty, match_score, mismatch_penalty);

    return 0;
}
