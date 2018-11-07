

MYTAR_FILE=filetar.mtar
TMP_DIRECTORY=tmp
OUT_DIRECTORY=out
FILENAME1=file1.txt
FILENAME2=file2.txt
FILENAME3=file3.dat

if [ ! -e ./mytar ]; then #Comprueba que el archivo existe en ese directorio
    echo "No se ha encontrado el archivo mytar"
    exit 1 

elif [ ! -x ./mytar ]; then #Si no es ejecutable
  echo "El archivo mytar no es un archivo ejecutable"
    exit 1
fi

echo "El archivo mytar existe y es ejecutable" # Si ha llegado aqui no hay ningun problema

if [ -d "$TMP_DIRECTORY" ]; then
  rm -rf -- $TMP_DIRECTORY 
  echo "El directorio TMP_DIRECTORY se ha borrado correctamente"
fi

mkdir $TMP_DIRECTORY 
cd $TMP_DIRECTORY   

# Se crean los 3 ficheros (file1,file2,file3)

if [ ! -e $FILENAME1 ]; then
  touch $FILENAME1 
  echo "Hello World!" > $FILENAME1
    echo "FILENAME1 creado correctamente"
fi
if [ ! -e $FILENAME2 ]; then
  touch $FILENAME2 
  head -10 /etc/passwd > $FILENAME2 
    echo "FILENAME2 creado correctamente"
fi
if [ ! -e $FILENAME3 ]; then
  touch $FILENAME3 
  head -c 1024 /dev/urandom > $FILENAME3
    echo "FILENAME3 creado correctamente"
fi

./../mytar -c -f $filetar.mtar $FILENAME1 $FILENAME2 $FILENAME3

if [ ! -d $OUT_DIRECTORY ]; then
  mkdir $OUT_DIRECTORY 
  echo "Se ha creado el directorio OUT_DIRECTORY"
fi

cp ./$filetar.mtar ./$OUT_DIRECTORY/$filetar.mtar #copia filetar.mtar en el dir OUT_DIRECTORY

cd $OUT_DIRECTORY
./../../mytar -x -f $filetar.mtar 

if diff ../$FILENAME1 $FILENAME1 >/dev/null ; then
    echo "FILENAME1 son iguales "
else
    echo "FILENAME1 no son iguales!"
    exit 1
fi

if diff ../$FILENAME2 $FILENAME2 >/dev/null ; then
    echo "FILENAME2 son iguales "
else
    echo "FILENAME2 no son iguales!"
    exit 1
fi

if diff ../$FILENAME3 $FILENAME3 >/dev/null ; then
    echo "FILENAME3 son iguales "
else
    echo "FILENAME3 no son iguales!"
    exit 1
fi

# si ha llegado aqui es porque no ha habido fallo

cd ../..
echo "Correct!!!"
exit 0 