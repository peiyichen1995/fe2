#!/bin/bash
COUNTER=0
INLOOPCOUNTER=0
for L in {0..499}; do
  echo "====================================="
  echo $L
  mpiexec -n 70 ../../fe2-opt -i tension.i sample=$L
done