#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define buffer_archivo_size 9999
#define REQINFO 1
#define NREQINFO 0

/*  Estructura que define una información requerida y una función especial de
 * impresión si fuera necesaria. */
struct info_req {
  char *info;
  void (*format)(char *string);
};

/*  Struct donde se guardan y manejan los datos referentes a cada archivo
 * accesado en "/proc/"*/
struct data_getter {
  char *path_de_archivo;
  char requiere_info;
  int cantidad_de_info_requerida;
  struct info_req *info_requerida;
  char archivo_en_buffer[buffer_archivo_size];
  void (*buffer_archivo)(struct data_getter *self);
  void (*print_info_requerida)(struct data_getter *self);
  void (*get_info_from_buffer)(struct data_getter *self, int index_info);
  void (*formatted_print)(char *string);
  void (*constructor)(struct data_getter *self, char *path, char req_info,
                      int cant_info, struct info_req *info_req,
                      void (*formatted_print)(char *string));
};

/*  Declaración de funciones  */
void constructor(struct data_getter *self, char *path, char req_info,
                 int cant_info, struct info_req *info_req,
                 void (*formatted_print)(char *string));
void buffer_archivo(struct data_getter *self);
void print_info_requerida(struct data_getter *self);
void get_info_from_buffer(struct data_getter *self, int index_info);
void print_formatted_uptime(char *string);
void print_formatted_n_filesystems(char *string);
void print_formatted_CPU_time(char *string);
void print_formatted_context_change(char *string);
void print_formatted_fecha_booteo(char *string);
void print_formatted_n_procesos(char *string);
void print_formatted_kern_vers(char *string);

/*  Funciones Principales para los tipos de impresion */
void impresion_simple();
void impresion_s();
void impresion_l();

/*    IMPLEMENTACION DE FUNCIONES   */

/*  Constructor que inicializa las estructuras "data_getter"*/
void constructor(struct data_getter *self, char *path, char req_info,
                 int cant_info, struct info_req *info_req,
                 void (*formatted_print)(char *string)) {
  self->path_de_archivo = path;
  self->requiere_info = req_info;
  self->cantidad_de_info_requerida = cant_info;
  self->info_requerida = info_req;

  /*  Funciones hardcodeadas, se pueden agregar punteros a función entre los
   * parámetros, y asignarlos acá.  */
  self->buffer_archivo = buffer_archivo;
  self->print_info_requerida = print_info_requerida;
  self->get_info_from_buffer = get_info_from_buffer;
  self->formatted_print = formatted_print;
}

/*  Función que copia el contenido del archivo "data_getter.path_de_archivo" a
 * "data_getter.archivo_en_buffer"*/
void buffer_archivo(struct data_getter *self) {
  FILE *file;
  size_t bytes;

  file = fopen(self->path_de_archivo, "r");

  bytes = fread(self->archivo_en_buffer, 1, buffer_archivo_size, file);
  fclose(file);

  if (bytes == 0) {
    printf("Error buffer_archivo, ningun byte leido.\n");
  } else if (bytes == buffer_archivo_size) {
    printf("Error buffer_archivo, buffer lleno.\n");
  }

  self->archivo_en_buffer[bytes] = '\0';
}

/*  Función que imprime por pantalla la info de "data_getter.archivo_en_buffer"
 * requerida según "data_getter.info_requerida"*/
void print_info_requerida(struct data_getter *self) {

  /*  Pongo en "data_getter->archivo_en_buffer" los datos de
   * "data_getter->path_de_archivo".  */
  (self->buffer_archivo)(self);

  if (self->requiere_info == 0) {
    if (self->formatted_print == NULL)
      printf("%s", self->archivo_en_buffer);
    else {
      (*(self->formatted_print))(self->archivo_en_buffer);
    }
  }

  else {
    for (int i = 0; i < self->cantidad_de_info_requerida; i++) {
      self->get_info_from_buffer(self, i);
    }
  }
}

/*  Función auxiliar para el uso de "print_info_requerida.  "*/
void get_info_from_buffer(struct data_getter *self, int i) {

  char matchFlag = 0;

  char *match;
  char linea[100];

  /* Encontrar una ocurrencia de "data_getter->info_requerida[i]" en
   * "data_getter->archivo_en_buffer".  */
  match = strstr(self->archivo_en_buffer, (self->info_requerida[i]).info);

  if (match == NULL)
    printf("No existe la información \"%s\" en el archivo \"%s\"\n",
           (self->info_requerida[i]).info, self->path_de_archivo);

  /*  Si hay coincidencia, guardo la línea completa en "linea[100]" y seteo
   * "matchFlag"*/
  if (match != NULL) {
    matchFlag = 1;
    int i = 0;
    while (match[i] != '\n') {
      linea[i] = match[i];
      i++;
    }
    linea[i] = '\0';
  }

  if (matchFlag && (self->info_requerida[i].format == NULL)) {
    printf("%s\n", linea);
  } else if (matchFlag && (self->info_requerida[i].format != NULL)) {
    (*(self->info_requerida[i].format))(linea);
  }
}

/*  Prints especialmente formateados  */

void print_formatted_n_filesystems(char *string) {
  int cont = 0;
  int i = 0;
  while (string[i] != '\0') {
    if (string[i] == '\n') {
      cont++;
    }
    i++;
  }
  printf("Filesystems Supp\t: %d\n", cont);
}

void print_formatted_uptime(char *string) {
  int uptime;
  sscanf(string, "%d", &uptime);
  time_t segundos = uptime;
  struct tm *tiempo = gmtime(&segundos);
  printf("Tiempo Uptime\t\t: %dD %d:%d:%d\n", tiempo->tm_yday, tiempo->tm_hour,
         tiempo->tm_min, tiempo->tm_sec);
}

void print_formatted_CPU_time(char *string) {
  int user, niced, sys, idle;
  int clockspeed = sysconf(_SC_CLK_TCK);
  sscanf(string, "cpu %d %d %d %d", &user, &niced, &sys, &idle);
  printf("Tiempo de CPU:\n\tUsuario\t%d\n\tSistema\t%d\n\tIdle\t%d\n",
         (user + niced) / clockspeed, sys / clockspeed, idle / clockspeed);
}

void print_formatted_context_change(char *string) {
  int n_cambios_contexto;
  sscanf(string, "ctxt  %d", &n_cambios_contexto);
  printf("Cambios de contexto\t: %d\n", n_cambios_contexto);
}

void print_formatted_fecha_booteo(char *string) {
  int fecha_boot;
  sscanf(string, "btime %d", &fecha_boot);

  time_t segundos = fecha_boot;
  struct tm *tiempo = localtime(&segundos);

  char buf[80];

  strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", tiempo);
  printf("Encendido desde\t\t%s\n", buf);
}

void print_formatted_n_procesos(char *string) {
  int n_proc;
  sscanf(string, "processes %d", &n_proc);
  printf("Procesos creados\t: %d\n", n_proc);
}

void print_formatted_kern_vers(char *string) {
  printf("Kernel Version\t\t: %s", string);
}

void print_formatted_peticiones_disco(char *string) {
  int peticiones;
  sscanf(string, "sda %d", &peticiones);
  printf("Peticiones a disco\t: %d\n", peticiones);
}

void print_formatted_mem_total(char *string) {
  int memtotal;
  sscanf(string, "MemTotal: %d", &memtotal);
  printf("Memoria Total\t\t: %d\tMb\n", memtotal / 1000);
}

void print_formatted_mem_disp(char *string) {
  int memav;
  sscanf(string, "MemAvailable: %d", &memav);
  printf("Memoria Disp\t\t: %d\tMb\n", memav / 1000);
}

void print_formatted_loadavg(char *string) {
  float load_min;
  sscanf(string, "%f", &load_min);
  printf("Load Average (1 min)\t: %f\n", load_min);
}

/*  Declaración de Estructuras  */

struct data_getter archivo_cpuinfo;
struct info_req inforeq_archivo_cpuinfo[2] = {{"vendor_id", NULL},
                                              {"model name", NULL}};

struct data_getter archivo_kernel_version;

struct data_getter archivo_uptime;

struct data_getter archivo_filesystems;

struct data_getter archivo_stat;
struct info_req inforeq_archivo_stat[4] = {
    {"cpu", print_formatted_CPU_time},
    {"ctxt", print_formatted_context_change},
    {"btime", print_formatted_fecha_booteo},
    {"processes", print_formatted_n_procesos}};

struct data_getter archivo_diskstats;

struct data_getter archivo_meminfo;
struct info_req inforeq_archivo_meminfo[2] = {
    {"MemTotal:", print_formatted_mem_total},
    {"MemAvailable:", print_formatted_mem_disp}};

struct data_getter archivo_loadavg;

// enum tipo_impresion {SIMPLE,S,L};
// enum tipo_impresion mi_tipo_impresion [3]= {SIMPLE,SIMPLE,SIMPLE};
char S_flag = 0;
char L_flag = 0;
int L_interval = 0;
int L_tiempo = 0;
char *L_flag_args = "";

/*  Función Main. */
int main(int argc, char **argv) {

  archivo_cpuinfo.constructor = constructor;
  (*archivo_cpuinfo.constructor)(&archivo_cpuinfo, "/proc/cpuinfo", REQINFO,
                                 sizeof(inforeq_archivo_cpuinfo) /
                                     sizeof(struct info_req),
                                 inforeq_archivo_cpuinfo, NULL);

  archivo_kernel_version.constructor = constructor;
  (*archivo_kernel_version.constructor)(&archivo_kernel_version,
                                        "/proc/sys/kernel/osrelease", NREQINFO,
                                        0, NULL, print_formatted_kern_vers);

  archivo_uptime.constructor = constructor;
  (*archivo_uptime.constructor)(&archivo_uptime, "/proc/uptime", NREQINFO, 0,
                                NULL, print_formatted_uptime);

  archivo_filesystems.constructor = constructor;
  (*archivo_filesystems.constructor)(&archivo_filesystems, "/proc/filesystems",
                                     NREQINFO, 0, NULL,
                                     print_formatted_n_filesystems);

  archivo_stat.constructor = constructor;
  (*archivo_stat.constructor)(&archivo_stat, "/proc/stat", REQINFO,
                              sizeof(inforeq_archivo_stat) /
                                  sizeof(struct info_req),
                              inforeq_archivo_stat, NULL);

  archivo_diskstats.constructor = constructor;
  (*archivo_diskstats.constructor)(&archivo_diskstats, "/proc/diskstats",
                                   NREQINFO, 0, NULL,
                                   print_formatted_peticiones_disco);

  archivo_meminfo.constructor = constructor;
  (*archivo_meminfo.constructor)(&archivo_meminfo, "/proc/meminfo", REQINFO,
                                 sizeof(inforeq_archivo_meminfo) /
                                     sizeof(struct info_req),
                                 inforeq_archivo_meminfo, NULL);

  archivo_loadavg.constructor = constructor;
  (*archivo_loadavg.constructor)(&archivo_loadavg, "/proc/loadavg", NREQINFO, 0,
                                 NULL, print_formatted_loadavg);

  int c;

  while (1) {

    static struct option long_options[] = {{"s", no_argument, 0, 's'},
                                           {"l", required_argument, 0, 'l'},
                                           {0, 0, 0, 0}};

    int option_index = 0;

    c = getopt_long(argc, argv, "l:s", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case -1:
      printf("C = -1\n"); // mi_tipo_impresion = SIMPLE;
      break;

    case 's':
      // printf("Opcion S.\n");
      S_flag = 1;
      break;

    case 'l':

      if (optarg[0] == '-' ||
          sscanf(optarg, "%d %d", &L_interval, &L_tiempo) != 2) {
        printf("-l requiere 2 argumentos -> \"10 10\"\n");
        return 0;
      }

      printf("Opcion L. con valor intervalo: %d\ttiempo: %d\n", L_interval,
             L_tiempo);
      L_flag = 1;
      break;

    default:
      exit(0);
    }
  }

  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    putchar('\n');
  }

  if (S_flag) {
    if (L_flag) {
      for (int i = 0; i < L_tiempo / L_interval; i++) {
        system("clear");
        impresion_l();
        sleep(L_interval);
      }
    } else if (!L_flag) {
      impresion_s();
    }
  } else if (!S_flag && L_flag) {
    for (int i = 0; i < L_tiempo / L_interval; i++) {
      system("clear");
      impresion_l();
      sleep(L_interval);
    }
  }

  else {
    impresion_simple();
  }

  /*
    if (S_flag && !L_flag) {
      impresion_s();
    }

    else if (!S_flag && L_flag) {
      impresion_l();
    }

    else if ((S_flag && L_flag)) {
      for (int i = 0; i < L_tiempo / L_interval; i++) {
        system("clear");
        impresion_l();
        sleep(L_interval);
      }
    } else if (L_flag) {
      for (int i = 0; i < L_tiempo / L_interval; i++) {
        system("clear");
        impresion_l();
        sleep(L_interval);
      }

    }

    else {
      impresion_simple();
    }

    */

  exit(0);
}

void impresion_simple() {
  (*archivo_cpuinfo.print_info_requerida)(&archivo_cpuinfo);
  (*archivo_kernel_version.print_info_requerida)(&archivo_kernel_version);
  (*archivo_uptime.print_info_requerida)(&archivo_uptime);
  (*archivo_filesystems.print_info_requerida)(&archivo_filesystems);
}

void impresion_s() {
  impresion_simple();
  (*archivo_stat.print_info_requerida)(&archivo_stat);
}

void impresion_l() {
  impresion_s();
  (*archivo_diskstats.print_info_requerida)(&archivo_diskstats);
  (*archivo_meminfo.print_info_requerida)(&archivo_meminfo);
  (*archivo_loadavg.print_info_requerida)(&archivo_loadavg);
}
