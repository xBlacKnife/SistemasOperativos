#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int
copynFile(FILE * origin, FILE * destination, int nBytes)
{
	if (origin == NULL || destination == NULL) return -1;

	char* buffer = malloc(sizeof(char)*nBytes);
	if(buffer == NULL) return -1;

	int readBytes = fread(buffer,1,nBytes,origin);
	int writeBytes = fwrite(buffer,1,readBytes,destination);

	if(readBytes == writeBytes) return writeBytes;

	
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)
{
	int mem = 0;
	while(getc(file) != '\0'){
		mem++;
	}
	char* buff = malloc(sizeof(char) * (mem + 1));

	if(buff == NULL) return NULL;

	fseek(file,-(mem +1),SEEK_CUR);
	fread(buff,1,mem+1,file);

	return buff;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles)
{
    stHeaderEntry* header=NULL; 

    int n;
    fread(&n,sizeof(int),1,tarFile);
    *nFiles = n;

    header= (stHeaderEntry*) malloc(sizeof(stHeaderEntry) * (*nFiles)); 
   
    for (int i  = 0; i < n; i++) 
    {
        header[i].name = loadstr(tarFile);
        fread(&header[i].size, sizeof(unsigned int), 1, tarFile);    
    }
	
	return header;

}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{
	FILE* auxFile = NULL;
	FILE* tarFileOut = NULL;

	tarFileOut = fopen(tarName,"w");
	printf("%s\n","tar abierto" );
	stHeaderEntry* header = NULL;

	header = (stHeaderEntry*) malloc(sizeof(stHeaderEntry) * nFiles);
	printf("%s\n","memoria reservada" );
	int headerBytes = sizeof(int) + sizeof(unsigned int) * nFiles;

	for (int i = 0; i < nFiles; ++i)
	{
		headerBytes+= strlen(fileNames[i]) + 1;
	}

	fseek(tarFileOut,headerBytes,SEEK_SET); //Se coloca en data
	printf("%s\n","punt posicionado" ); //AQUI

	for (int i = 0; i < nFiles; ++i)
	{
		auxFile = fopen(fileNames[i],"r");
		header[i].size = copynFile(auxFile,tarFileOut,INT_MAX);
		header[i].name = malloc(sizeof(fileNames[i]+1));
		strcpy(header[i].name,fileNames[i]);
		fclose(auxFile);
	}
	printf("%s\n","creados headers" );

	fseek(tarFileOut,0,SEEK_SET);
	fwrite(&nFiles,1,sizeof(int),tarFileOut);
	printf("%s\n","se escribe el n de archivos" );

	for (int i = 0; i < nFiles; ++i)
	{
		fwrite(header[i].name,1,(strlen(fileNames[i]) + 1),tarFileOut);
		fwrite(&header[i].size,1,sizeof(int),tarFileOut);
	}
	fclose(tarFileOut);
	printf("%s\n","se escriben los headers" );

	for (int i = 0; i < nFiles; ++i)
	{
		free(header[i].name);
	}
	free(header);

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{
	FILE *tarFile = NULL; 
    FILE *destination = NULL;

    tarFile = fopen(tarName,"r");

    int nFiles = 0;
    stHeaderEntry* headers = readHeader(tarFile,&nFiles);

    for (int i = 0; i < nFiles; i++)
	{
		destination = fopen(headers[i].name, "w");
		copynFile(tarFile, destination, headers[i].size);
		fclose(destination);
	} 
    fclose(tarFile);

return EXIT_SUCCESS;
    
}
