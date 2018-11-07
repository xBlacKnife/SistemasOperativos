#!/bin/bash



SRC="./src" 

TEMP="./temp" 

MOUNT="./mount-point" 


FILE1="myFS.h"

FILE2="fuseLib.c"

FILE3="MyFileSystem.c"


rmdir $TEMP 

mkdir $TEMP 


#Primera parte 


cp $SRC/$FILE1 $TEMP/$FILE1	

cp $SRC/$FILE1 $MOUNT/$FILE1


cp $SRC/$FILE2 $TEMP/$FILE2	

cp $SRC/$FILE2 $MOUNT/$FILE2


#Parte dos 


./my-fsck virtual-disk


DIFF_FILE1=$(diff $MOUNT/$FILE1 $TEMP/$FILE1)

DIFF_FILE2=$(diff $MOUNT/$FILE2 $TEMP/$FILE2)


if [ "$DIFF_FILE1" == "" ]; 

then echo "|Son Iguales| : myFS.h del MOUNT es igual que el de TEMP(V)"

else echo "|Son Diferentes| : myFS.h del MOUNT es diferente que el de TEMP"

fi


if [ "$DIFF_FILE2" == "" ]; 

then echo "|Son Iguales| : fuseLib.c del MOUNT es igual que el de TEMP(V)"

else echo "|Son Diferentes| : fuseLib.c del MOUNT es diferente que el de TEMP"

fi


truncate --size=-4096 $TEMP/$FILE1

truncate --size=-4096 $MOUNT/$FILE1


echo "Se ha |TRUNCADO| myFS.h en TEMP y MOUNT"


#Parte 3


./my-fsck virtual-disk


DIFF_FILE1_TRUNCATED=$(diff $SRC/$FILE1 $MOUNT/$FILE1)


if [ "$DIFF_FILE1_TRUNCATED" == "" ]; 

then echo "|Son Iguales| : myFS.h original y el truncado son el mismo"

else echo "|Son Distintos| : myFS.h original y el truncado son distintos(V)"

fi



#PArte 4


cp $SRC/$FILE3 $MOUNT/$FILE3

#Parte 5



./my-fsck virtual-disk



DIFF_FILE3_COPIED=$(diff $SRC/$FILE3 $MOUNT/$FILE3)


if [ "$DIFF_FILE3_COPIED" == "" ]; 

then echo "|Son Iguales| : MyFileSystem.c del MOUNT es igual al original(V)"

else echo "|Son Distintos| : MyFileSystem.c del MOUNT es distinto al original"

fi



#PArte 6



truncate --size=+4096 $TEMP/$FILE2

truncate --size=+4096 $MOUNT/$FILE2



#Parte 7



./my-fsck virtual-disk


DIFF_MOUNT_TRUNCATED_FILE2=$(diff $SRC/$FILE2 $MOUNT/$FILE2) 

DIFF_TEMP_TRUNCATED_FILE2=$(diff $SRC/$FILE2 $TEMP/$FILE2) 



if [ "$DIFF_MOUNT_TRUNCATED_FILE2" == "" ]; 

then echo "|Son Iguales| : fuseLib.c del MOUNT es igual al original"

else echo "|Son Distintos| : fuseLib.c del MOUNT es distinto al original(V)"

fi



if [ "$DIFF_TEMP_TRUNCATED_FILE2" == "" ]; 

then echo "|Son Iguales| : fuseLib.c del TEMP es igual al original"

else echo "|Son Distintos| : fuseLib.c del TEMP es distinto al original(V)"

fi