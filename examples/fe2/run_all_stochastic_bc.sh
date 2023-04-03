#!/bin/bash
COUNTER=0
INLOOPCOUNTER=0
SAMPLE=500
input="random_F_100_scene_1.txt"
while IFS= read -r line
do
  echo "====================================="
  echo $SAMPLE
  ../../fe2-opt -i square_macro_stochastic_bc.i n=8 n_micro=8 sample=$SAMPLE $line 2> err_output/$SAMPLE.txt > command_output/$SAMPLE.txt &
  (( COUNTER ++ ))
  (( SAMPLE ++ ))
  if [ $COUNTER = 16 ]; then
    wait
    COUNTER=0
    (( INLOOPCOUNTER ++ ))
    echo $INLOOPCOUNTER
  fi
done < "$input"

# COUNTER=0
# INLOOPCOUNTER=0
# for SAMPLE in {352..499}; do
#   echo "====================================="
#   echo $SAMPLE
#   # ../../fe2-opt -i gamma_multiapp_square.i n=8 n_micro=8 sample=$L 2> err_output/$L.txt > command_output/$L.txt &
#   ../../fe2-opt -i square_macro_stochastic_bc.i n=8 n_micro=8 sample=$SAMPLE 2> err_output_scene_1/$SAMPLE.txt > command_output_scene_1/$SAMPLE.txt &
#   (( COUNTER ++ ))
#   if [ $COUNTER = 16 ]; then
#     wait
#     COUNTER=0
#     (( INLOOPCOUNTER ++ ))
#     echo $INLOOPCOUNTER
#   fi
# done