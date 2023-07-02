from Bio.Seq import reverse_complement
import time
import random

def generate_random_dna_sequence(length):
    bases = "ATGC"
    random_seq = ''.join(random.choice(bases) for _ in range(length))
    return random_seq

def measure_execution_time(sequence_length):
    seq = generate_random_dna_sequence(sequence_length)
    start_time = time.time()
    result = reverse_complement(seq)
    end_time = time.time()
    execution_time = end_time - start_time
    #print("Input sequence:", seq)
    #print("Reverse complement:", result)
    print("Execution time:", execution_time, "seconds")

# Ejemplo de uso: medir el tiempo de ejecución para una secuencia de longitud 100
measure_execution_time(1000)
