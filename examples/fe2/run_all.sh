#!/bin/bash
COUNTER=0
INLOOPCOUNTER=0
for L in {0..999}; do
  ../../fe2-opt -i gamma_multiapp_square.i n=8 n_micro=8 sample=$L 2> err_output/$L.txt > command_output/$L.txt &
  (( COUNTER ++ ))
  if [ $COUNTER = 16 ]; then
    wait
    COUNTER=0
    (( INLOOPCOUNTER ++ ))
    echo $INLOOPCOUNTER
  fi
done