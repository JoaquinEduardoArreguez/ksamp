#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define buffer_archivo_size 9999
#define REQINFO   1
#define NREQINFO  0

/*  Estructura que define una información requerida y una función especial de impresión si fuera necesaria. */
struct info_req{
  char*   info;
  void (*format)  (char* string);
};

/*  Struct donde se guardan y manejan los datos referentes a cada archivo accesado en "/proc/"*/
struct data_getter{
  char*   path_de_archivo;
  char    requiere_info;
  int     cantidad_de_info_requerida;
  char**  info_requerida;
  char    archivo_en_buffer[buffer_archivo_size];
  void   (*buffer_archivo)        (struct data_getter* self);
  void   (*print_info_requerida)  (struct data_getter* self);
  void   (*get_info_from_buffer)  (struct data_getter* self, char* info_requerida);
  void   (*formatted_print)       (char* string);
  void   (*constructor)
  (struct data_getter* self,char* path, char req_info, int cant_info,char** info_req,void (*formatted_print)(char* string));
};

/*  Declaración de funciones de "data_getter" */
void constructor(struct data_getter* self,char* path, char req_info, int cant_info,char** info_req,void (*formatted_print)(char* string));
void buffer_archivo (struct data_getter* self);
void print_info_requerida(struct data_getter* self);
void get_info_from_buffer (struct data_getter* self, char* info_requerida);
void print_formatted_uptime(char* string);

/*  Constructor que inicializa las estructuras "data_getter"*/
void constructor(struct data_getter* self,char* path, char req_info, int cant_info,char** info_req,void (*formatted_print)(char* string)){
  self->path_de_archivo=path;
  self->requiere_info=req_info;
  self->cantidad_de_info_requerida= cant_info;
  self->info_requerida=info_req;

  /*  Funciones hardcodeadas, se pueden agregar punteros a función entre los parámetros, y asignarlos acá.  */
  self->buffer_archivo=buffer_archivo;
  self->print_info_requerida  = print_info_requerida;
  self->get_info_from_buffer  = get_info_from_buffer;
  self->formatted_print       = formatted_print;

  /*  Pongo en "data_getter->archivo_en_buffer" los datos de "data_getter->path_de_archivo".  */
  (self->buffer_archivo)(self);

}

/*  Función que copia el contenido del archivo "data_getter.path_de_archivo" a "data_getter.archivo_en_buffer"*/
void buffer_archivo (struct data_getter* self){
  FILE* file;
	size_t bytes;

	file = fopen(self->path_de_archivo, "r");

	bytes = fread (self->archivo_en_buffer, 1, buffer_archivo_size, file);
	fclose (file);

	if (bytes == 0){printf ("Error buffer_archivo, ningun byte leido.\n");}
	else if (bytes == buffer_archivo_size){printf ("Error buffer_archivo, buffer lleno.\n");}

	self->archivo_en_buffer[bytes] = '\0';
}

/*  Función que imprime por pantalla la info de "data_getter.archivo_en_buffer" requerida según "data_getter.info_requerida"*/
void print_info_requerida(struct data_getter* self){

  if (self->requiere_info == 0) printf("%s", self->archivo_en_buffer);

  else {
    for (int i=0;i<self->cantidad_de_info_requerida;i++){
      self->get_info_from_buffer(self,self->info_requerida[i]);
    }
  }
}

void get_info_from_buffer (struct data_getter* self, char* info_requerida){

	char* match;

	/* Encontrar una ocurrencia de "data_getter->info_requerida[i]" en "data_getter->archivo_en_buffer".  */
	match = strstr (self->archivo_en_buffer, info_requerida);
	if (match == NULL)	printf("No existe la información \"%s\" en el archivo \"%s\"\n", info_requerida, self->path_de_archivo );

  else {
    int i=0;
    while (match[i] !=  '\n'){
      printf("%c", match[i] );
      i++;
    }
    printf("\n");
  }
}

int main(int argc, char const *argv[]) {

  struct data_getter archivo_cpuinfo;
  archivo_cpuinfo.constructor = constructor;
  char* inforeq_archivo_cpuinfo [2] = {"vendor_id","model name"};
  (*archivo_cpuinfo.constructor)(&archivo_cpuinfo,"/proc/cpuinfo",REQINFO,sizeof(inforeq_archivo_cpuinfo)/sizeof(char*),inforeq_archivo_cpuinfo,NULL);

  //(*archivo_cpuinfo.print_info_requerida)(&archivo_cpuinfo);

  struct  data_getter archivo_kernel_version;
  archivo_kernel_version.constructor  = constructor;
  (*archivo_kernel_version.constructor)(&archivo_kernel_version,"/proc/sys/kernel/osrelease",NREQINFO,0,NULL,NULL);

  //(*archivo_kernel_version.print_info_requerida)(&archivo_kernel_version);

  struct  data_getter archivo_uptime;
  archivo_uptime.constructor  = constructor;
  (*archivo_uptime.constructor)(&archivo_uptime,"/proc/uptime",NREQINFO,0,NULL,NULL);
  (*archivo_uptime.print_info_requerida)(&archivo_uptime);

  print_formatted_uptime("36018.27");

  return 0;
}

void print_formatted_uptime(char* string){
  int uptime;
  sscanf(string,"%d",&uptime);
  time_t  segundos = uptime;
  struct tm*  tiempo = gmtime (&segundos);

  printf("%dD %d:%d:%d\n", tiempo->tm_yday, tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec );
}
