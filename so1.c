#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define archivo1	"/proc/cpuinfo"

#define dato0		"vendor_id"
#define dato1		"model name"
#define dato2		"cpu MHz"
#define strsize		30
#define buffersize	9999

unsigned char buffer_archivo(char* archivo, char* buffer, int buffer_size);
unsigned char get_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado);
unsigned char print_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado);

int main(int argc, char **argv){

	int c;

	while (1){

		static struct option long_options[] = {
        	  {"s", no_argument,	0,	's'},
         	  {0, 0, 0, 0}
        	};

		int option_index = 0;
	
		c = getopt_long(argc,argv,"s",long_options, &option_index);

		if (c == -1)	break;

		switch (c) {
			case -1 :	printf("C = -1\n");
					break;
			case 's':	printf("Opcion S.\n");
					break;
			default:	printf("Opcion -default- de switch.\n");
					abort();
		}

	}	

	char buffer [buffersize] = "";
	char dato_buscado [strsize] = "";

	if(!buffer_archivo(archivo1,buffer,sizeof(buffer))) printf("Error leyendo archivo(buffer_archivo).\n");

	print_data(buffer,dato2,dato_buscado);
}

/* Sección separada para evitar ensuciar el main.*/
unsigned char print_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado){
	if (get_data(buffer_archivo,dato_buscado,dato_encontrado)) printf("%s\n",dato_encontrado);
	else {printf ("Fallo en get_data\n");}
}
/* Función que recibe un archivo y guarda su contenido en un buffer */
unsigned char buffer_archivo(char* archivo, char* buffer, int buffer_size){
	FILE* file;
	size_t bytes;

	file = fopen(archivo, "r");

	bytes = fread (buffer, 1, buffer_size, file);
	fclose (file);

	if (bytes == 0){
		printf ("Error buffer_archivo, ningun byte leido.\n");
		return 0;
	}
	else if (bytes == buffer_size){
		printf ("Error buffer_archivo, buffer lleno.\n");
		return 0;
		} 

	buffer[bytes] = '\0';
	return 1;
}

/*
Función que recibe un buffer de datos (string) y busca dentro del mismo la ocurrencia
de otro string, luego devuelve la línea del string encontrado.
*/
unsigned char get_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado){
	char* match;

	/* Locate the line that starts with “cpu MHz”.  */
	match = strstr (buffer_archivo, dato_buscado);
	if (match == NULL)	return 0;

	/* Parse the line to extract the clock speed.  */
	int i;
	i=0;
	while ((match[i]) != '\n'){
		dato_encontrado[i] = match[i];
		i++;
	}

	return 1;
}

long int tamanio_buffer(char* archivo){
	FILE* file;
	size_t bytes;

	file = fopen(archivo, "r");

	fseek(file, 0L, SEEK_END);

	unsigned int tamanio = ftell(file);

	fclose(file);

	return tamanio;

}
