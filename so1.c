#include <stdio.h>
#include <string.h>

#define archivo1	"/proc/cpuinfo"
#define dato1		"cpu MHz"
#define strsize		30
#define buffersize	4096

unsigned char buffer_archivo(char* archivo, char* buffer, int buffer_size);
unsigned char get_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado);


unsigned char buffer_archivo(char* archivo, char* buffer, int buffer_size){
	FILE* file;
	size_t bytes;

	file = fopen(archivo, "r");

	bytes = fread (buffer, 1, buffer_size, file);
	fclose (file);

	if (bytes == 0){
		printf ("Error buffer_archivo.\n");
		return 0;
	}
	else if (bytes == buffer_size){
		printf ("Error buffer_archivo.\n");
		return 0;
		} 

	buffer[bytes] = '\0';
	return 1;
}

unsigned char get_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado)
{
	char* match;

	/* Locate the line that starts with “cpu MHz”.  */
	match = strstr (buffer_archivo, dato_buscado);
	if (match == NULL)	return 0;

	/* Parse the line to extract the clock speed.  */
	int i=0;
	while ((match[i]) != '\n'){
		dato_encontrado[i] = match[i];
		i++;
	}

	return 1;
}

int main(){

	char buffer [buffersize] = "";

	if(!buffer_archivo(archivo1,buffer,sizeof(buffer))) printf("Error leyendo archivo.\n");

	char dato_buscado [strsize] = "";

	if (get_data(buffer,dato1,dato_buscado)) printf("%s\n",dato_buscado);
	else {printf ("Fallo en get_data\n");}	
}