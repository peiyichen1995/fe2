# #!/bin/bash
# COUNTER=0
# INLOOPCOUNTER=0
# for L in {0..19999}; do
#   echo "====================================="
#   echo $L
#   ../../fe2-opt -i save_bc.i sample=$L n=5
# done

#!/bin/bash
COUNTER=0
INLOOPCOUNTER=0
for L in {0..19999}; do
  echo "====================================="
  echo $L
  ../../fe2-opt -i save_bc.i sample=$L n=5 &
  (( COUNTER ++ ))
  if [ $COUNTER = 120 ]; then
    wait
    COUNTER=0
    (( INLOOPCOUNTER ++ ))
    echo $INLOOPCOUNTER
  fi
done