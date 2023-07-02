#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>

std::mutex reversoComplementarioMutex;

// Función secuencial para calcular el reverso complementario de una cadena de ADN
std::string calcularReversoComplementarioSecuencial(const std::string& cadena)
{
    std::string reversoComplementario(cadena.rbegin(), cadena.rend());
    for (char& nucleotido : reversoComplementario) {
        switch (nucleotido) {
            case 'A': nucleotido = 'T'; break;
            case 'T': nucleotido = 'A'; break;
            case 'C': nucleotido = 'G'; break;
            case 'G': nucleotido = 'C'; break;
        }
    }
    return reversoComplementario;
}

// Función paralela para calcular el reverso complementario de una subcadena de ADN
void calcularReversoComplementarioParalelo(const std::string& subcadena, std::string& reversoComplementario)
{
    std::string subcadenaReversa(subcadena.rbegin(), subcadena.rend());
    for (char& nucleotido : subcadenaReversa) {
        switch (nucleotido) {
            case 'A': nucleotido = 'T'; break;
            case 'T': nucleotido = 'A'; break;
            case 'C': nucleotido = 'G'; break;
            case 'G': nucleotido = 'C'; break;
        }
    }

    std::lock_guard<std::mutex> lock(reversoComplementarioMutex);
    reversoComplementario += subcadenaReversa;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Uso: " << argv[0] << " <largo_cadena_ADN> <numero_hilos>" << std::endl;
        return 1;
    }

    int largoCadenaADN = std::stoi(argv[1]);
    int numeroHilos = std::stoi(argv[2]);

    // Generar cadena de ADN aleatoria
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);

    std::string cadenaADN;
    for (int i = 0; i < largoCadenaADN; ++i) {
        switch (dis(gen)) {
            case 0: cadenaADN += 'A'; break;
            case 1: cadenaADN += 'T'; break;
            case 2: cadenaADN += 'C'; break;
            case 3: cadenaADN += 'G'; break;
        }
    }

    // Calcular reverso complementario de manera secuencial
    auto startTime = std::chrono::high_resolution_clock::now();
    std::string reversoComplementarioSecuencial = calcularReversoComplementarioSecuencial(cadenaADN);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto secuencialDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    // Calcular reverso complementario de manera paralela
    startTime = std::chrono::high_resolution_clock::now();
    std::string reversoComplementarioParalelo;
    int subcadenaSize = cadenaADN.size() / numeroHilos;
    std::vector<std::thread> threads;

    for (int i = 0; i < numeroHilos; ++i) {
        int startIndex = i * subcadenaSize;
        int endIndex = (i == numeroHilos - 1) ? cadenaADN.size() : startIndex + subcadenaSize;
        std::string subcadena = cadenaADN.substr(startIndex, endIndex - startIndex);
        threads.emplace_back(calcularReversoComplementarioParalelo, std::cref(subcadena), std::ref(reversoComplementarioParalelo));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    endTime = std::chrono::high_resolution_clock::now();
    auto paraleloDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    // Imprimir resultados y tiempo transcurrido
    //std::cout << "Cadena de ADN: " << cadenaADN << std::endl;
    //std::cout << "Reverso complementario secuencial: " << reversoComplementarioSecuencial << std::endl;
    //std::cout << "Reverso complementario paralelo: " << reversoComplementarioParalelo << std::endl;
    std::cout << "Tiempo secuencial: " << secuencialDuration.count() << " microsegundos" << std::endl;
    std::cout << "Tiempo paralelo: " << paraleloDuration.count() << " microsegundos" << std::endl;

    return 0;
}
