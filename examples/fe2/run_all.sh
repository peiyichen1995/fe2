#!/bin/bash
COUNTER=0
INLOOPCOUNTER=0
for L in {0..1}; do
  ../../fe2-opt -i gamma_multiapp_square.i n=2 n_micro=2 sample=$L 2> err_output/$L.txt > command_output/$L.txt &
  (( COUNTER ++ ))
  if [ $COUNTER = 2 ]; then
    wait
    COUNTER=0
    (( INLOOPCOUNTER ++ ))
    echo $INLOOPCOUNTER
  fi
done