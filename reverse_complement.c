#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// Función para obtener el complemento de un nucleótido
char complemento(char nucleotido) {
    if (nucleotido == 'A') return 'T';
    if (nucleotido == 'T') return 'A';
    if (nucleotido == 'C') return 'G';
    if (nucleotido == 'G') return 'C';
    return nucleotido;
}

// Función para calcular el reverso complementario de una secuencia de ADN
char* reversoComplementario(const char* secuencia) {
    int len = strlen(secuencia);
    char* resultado = (char*) malloc((len + 1) * sizeof(char));

    // Obtener el número de threads
    int num_threads = omp_get_max_threads();

    // Calcular el tamaño de bloque para cada thread
    int block_size = len / num_threads;

    #pragma omp parallel
    {
        // Obtener el ID del thread
        int thread_id = omp_get_thread_num();

        // Calcular los índices de inicio y fin para el bloque asignado al thread
        int start_index = thread_id * block_size;
        int end_index = (thread_id == num_threads - 1) ? len : (start_index + block_size);

        // Calcular el reverso complementario para el bloque asignado al thread
        for (int i = start_index; i < end_index; i++) {
            int j = len - i - 1;
            resultado[j] = complemento(secuencia[i]);
        }
    }

    resultado[len] = '\0';
    return resultado;
}

// Función para generar una secuencia aleatoria de ADN
char* generarSecuenciaADN(int longitud) {
    char* secuencia = (char*) malloc((longitud + 1) * sizeof(char));

    srand(time(NULL));  // Inicializar la semilla aleatoria con la hora actual

    for (int i = 0; i < longitud; i++) {
        int aleatorio = rand() % 4;  // Generar un número aleatorio entre 0 y 3

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

    secuencia[longitud] = '\0';
    return secuencia;
}

int main() {
    const char* secuencia = generarSecuenciaADN(40);

    // Medir el tiempo inicial
    double start_time = omp_get_wtime();

    char* reverso_comp = reversoComplementario(secuencia);

    // Medir el tiempo final
    double end_time = omp_get_wtime();

    printf("Secuencia original: %s\n", secuencia);
    printf("Reverso complementario: %s\n", reverso_comp);
    printf("Tiempo de ejecución: %.6f segundos\n", end_time - start_time);

    free(reverso_comp);

    return 0;
}
