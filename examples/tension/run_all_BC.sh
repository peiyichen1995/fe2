#!/bin/bash
COUNTER=0
INLOOPCOUNTER=0
for L in {0..19999}; do
  echo "====================================="
  echo $L
  ../../fe2-opt -i tension.i sample=$L 2> err_output/$L.txt > command_output/$L.txt &
  (( COUNTER ++ ))
  if [ $COUNTER = 125 ]; then
    wait
    COUNTER=0
    (( INLOOPCOUNTER ++ ))
    echo $INLOOPCOUNTER
  fi
done