#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

/*	Se pide:
				Tipo y modelo de CPU.
				Versión de Kernel.
				Cantidad de tiempo transcurrido desde que se inició el sistema operativo, con el formato ddD hh:mm:ss
				Cantidad de sistemas de archivo soportados por el kernel
				*/



#define archivo1	"/proc/cpuinfo"
#define archivo_kernel_version	"/proc/sys/kernel/osrelease"
#define archivo3	"/proc/uptime"

#define dato0		"vendor_id"
#define dato1		"model name"
#define dato2		"cpu MHz"
#define strsize		100
#define buffersize	9999

/* Enum para los tipos de impresión: NORMAL, EXTENDIDO y POR INTERVALOS*/
enum tipo_impresion { SIMPLE, S, L };

/* Declaraciones de funciones*/
unsigned char buffer_archivo(char* archivo, char* buffer, int buffer_size);
unsigned char get_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado);
unsigned char print_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado);
void print_simple(char* buffer_archivo, char* dato_encontrado);
void print_s(char* buffer_archivo, char* dato_encontrado);
void print_l(char* buffer_archivo, char* dato_encontrado);
void print(enum tipo_impresion mi_tipo_impresion);


int main(int argc, char **argv){

	int c;

	enum tipo_impresion mi_tipo_impresion = SIMPLE;

	while (1){

		static struct option long_options[] = {
        	  {"s", no_argument,	0,	's'},
						{"l",	no_argument,	0,	'l'},
         	  {0, 0, 0, 0}
        	};

		int option_index = 0;

		c = getopt_long(argc,argv,"s,l",long_options, &option_index);

		if (c == -1)	break;

		switch (c) {
			case -1 :	printf("C = -1\n");mi_tipo_impresion = SIMPLE;
					break;
			case 's':	printf("Opcion S.\n");mi_tipo_impresion = S;
					break;
			case 'l':printf("Opcion L.\n");mi_tipo_impresion = L;
					break;
			default:	printf("Opcion -default- de switch.\n");
					abort();
		}

	}

	print(mi_tipo_impresion);
}


/* Print que se comporta de forma distinta según el tipo de impresión
	 que debe realizar: SIMPLE, S, L.
*/
void print(enum tipo_impresion mi_tipo_impresion){

	char buffer [buffersize] = "";
	char dato_encontrado [strsize] = "";

	if(!buffer_archivo(archivo1,buffer,sizeof(buffer))) printf("Error leyendo archivo(buffer_archivo).\n");

	switch (mi_tipo_impresion) {
		case SIMPLE	:	print_simple(buffer,dato_encontrado);break;
		case S 			:	print_simple(buffer,dato_encontrado);print_s(buffer,dato_encontrado);break;
		case L 			:	printf("%s\n", "Acá viene el LOOPED");
	}

}

/* Print usado sin opciones, modo SIMPLE */
void print_simple(char* buffer_archivo, char* dato_encontrado){
	print_data(buffer_archivo,dato0,dato_encontrado);
	print_data(buffer_archivo,dato1,dato_encontrado);
	print_data(buffer_archivo,dato2,dato_encontrado);
}

/* Print usado con la opción 's' */
void print_s(char* buffer_archivo, char* dato_encontrado){
	;
}

/* Print usado con la opción 'l' */
void print_l(char* buffer_archivo, char* dato_encontrado){
	;
}

/* Sección separada para evitar ensuciar el main.
	 Utiliza "get_data" de forma segura, y por eso
	 "get_data" no debe usarse directamente.
*/
unsigned char print_data (char* buffer_archivo, char* dato_buscado, char* dato_encontrado){
	memset(dato_encontrado,'\0',strsize);
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

/* Función que recibe un buffer de datos (string) y busca dentro del mismo la ocurrencia
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

/* Función al vicio por ahora.*/
long int tamanio_buffer(char* archivo){
	FILE* file;
	size_t bytes;

	file = fopen(archivo, "r");

	fseek(file, 0L, SEEK_END);

	unsigned int tamanio = ftell(file);

	fclose(file);

	return tamanio;

}
