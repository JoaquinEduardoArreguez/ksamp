#include <stdio.h>
#include <string.h>

#define archivo1	"/proc/cpuinfo"
#define dato1		"cpu MHz"

int get_cpu_clock_speed (char* nombre_archivo, char* nombre_dato, char* data)
{
	FILE* fp;
	char buffer[99999];
	size_t bytes_read;
	char* match;
	float clock_speed;
	//char data [20];

	/* Read the entire contents of /proc/cpuinfo into the buffer.  */
	fp = fopen (nombre_archivo, "r");
	bytes_read = fread (buffer, 1, sizeof (buffer), fp);
	fclose (fp);
	/* Bail if read failed or if buffer isn’t big enough.  */
	if (bytes_read == 0 || bytes_read == sizeof (buffer))
		return 0;
	/* NUL-terminate the text.  */
	buffer[bytes_read] = '\0';
	/* Locate the line that starts with “cpu MHz”.  */
	match = strstr (buffer, dato1);
	if (match == NULL)
		return 0;
	/* Parse the line to extract the clock speed.  */
	int i;
	while ((match[i]) != '\n'){
		data[i] = match[i];
		//printf("%c\n",match[i]);
		i++;
	}

	//sscanf (match, "%s %s %s %s %s", data);
	return 1;
}

int main(){
	char aux [20];
	get_cpu_clock_speed(archivo1,"b",aux);
	printf("%s\n",aux);
	//printf("%f\n",aux);
}