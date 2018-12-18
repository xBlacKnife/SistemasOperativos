#!/bin/bash

RESULT_DIR="resultados"
SIM_DIR="./sim"
RESULT_FILE="result"

echo "Introduce el nombre del fichero a simular:"

read fileName

while [ ! -f "$EXAMPLE_DIR$fileName" ]

do

echo "El archivo no es correcto introduce uno valido:"
read fileName

done

if [ ! -d "$RESULT_DIR" ]; then
mkdir resultados
fi

i='0'
incremento=1

cd $RESULT_DIR

while [ -f $RESULT_FILE$i".log" ]

do 

i=$((i+1))

done

cd ..

$SIM_DIR $fileName $RESULT_DIR/$RESULT_FILE$i".log"

cd ../gantt

./generate_gantt_chart ../sim/resultados/$RESULT_FILE$i".log"








