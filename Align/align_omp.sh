#!/bin/bash
inicio=1
fin=8
salto=1

for i in $(seq $inicio $salto $fin); do
   ./align 100 $((10**$i)) 4
done
