#include <stdio.h>
#include <string.h>

#define archivo1	"/proc/cpuinfo"
#define dato1		"cpu MHz"
#define strsize		30

int buffer_archivo(char* archivo, char* buffer, int buffer_size){
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

int get_data (char* buffer, char* nombre_dato, char* data)
{
	//FILE* fp;
	//char buffer[4096];
	//size_t bytes_read;
	char* match;
	float clock_speed;
	//char data [20];

	/* Read the entire contents of /proc/cpuinfo into the buffer.  */
	//fp = fopen (nombre_archivo, "r");
	//bytes_read = fread (buffer, 1, sizeof (buffer), fp);
	//fclose (fp);
	/* Bail if read failed or if buffer isn’t big enough.  */
	//if (bytes_read == 0 || bytes_read == sizeof (buffer))
	//	return 0;
	/* NUL-terminate the text.  */
	//buffer[bytes_read] = '\0';
	/* Locate the line that starts with “cpu MHz”.  */
	match = strstr (buffer, nombre_dato);
	if (match == NULL)
		return 0;
	/* Parse the line to extract the clock speed.  */
	int i=0;
	while ((match[i]) != '\n'){
		data[i] = match[i];
		i++;
	}

	return i;
}


int main(){
	char buffer [4096] = "";
	buffer_archivo(archivo1,buffer,sizeof(buffer));
	char aux [strsize] = "";
	if (get_data(buffer,dato1,aux)) printf ("Exito\n");
	else {printf ("Fallo en get_data\n");}
	printf("%s\n",aux);
}