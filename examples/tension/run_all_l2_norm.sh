#!/bin/bash
COUNTER=0
INLOOPCOUNTER=0
for L in {0..499}; do
  echo "====================================="
  echo $L
  ../../fe2-opt -i compare_result.i sample=$L
done