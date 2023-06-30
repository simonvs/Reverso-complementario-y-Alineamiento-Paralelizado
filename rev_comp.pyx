import numpy as np
import random
from cython.parallel import prange, parallel
from cython import boundscheck, wraparound
cimport cython
cimport openmp
cimport numpy as cnp

def generate_random_dna_sequence(n):
    bases = ['A', 'T', 'C', 'G']
    sequence = ''.join(random.choices(bases, k=n))
    return sequence

@boundscheck(False)
@wraparound(False)
def parallel_reverse_complement(cnp.ndarray sequence):
    cdef int i, n = len(sequence)
    cdef cnp.ndarray result = cnp.empty(n, dtype='S1')
    
    with nogil, parallel():
        for i in prange(n, schedule='static'):
            if sequence[i] == b'A':
                result[n-i-1] = b'T'
            elif sequence[i] == b'T':
                result[n-i-1] = b'A'
            elif sequence[i] == b'C':
                result[n-i-1] = b'G'
            elif sequence[i] == b'G':
                result[n-i-1] = b'C'
    
    return result.tostring().decode('utf-8')

sequence = generate_random_dna_sequence(10)
reverse_complement = parallel_reverse_complement(sequence.encode('utf-8'))

# Imprimir el reverso complementario
print("Secuencia original:", sequence)
print("Reverso complementario:", reverse_complement)
