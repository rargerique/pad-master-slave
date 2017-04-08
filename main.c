#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/* Function executed by the thread */
void *do_something (void *param){
	/* Implement what the thread will have to do */
	printf("\nHello World! %d", (int)param);
}

int main(int argc, char *argv[]) {
	/* Variables to read the PGM image */
	FILE *input;
	int numRows, numColumns, maxValue, currentValue, updatedValue;
	char *type[2];
	
	/* Variable for the threads creation */	
	pthread_t thread0;
	int parameter = 0;
	
	/* Open the file (filename received as parameter) for reading */
	input = fopen(argv[1], "r+");
	printf("\nOpening file: %s", argv[1]);
	
	/* Read the first line related to the image type */
	fscanf(input, "%s", &type);
	printf("\nImage file of type: %s", type);
	
	/* Skip line */
	while(getc(input) != '\n');
	
	/* Skip whole comment line */
	while(getc(input) == '#'){
		while(getc(input) != '\n');
	}
	printf("\nSkiped comment line");
	
	/* Backup one position */
	fseek(input, -1, SEEK_CUR);
	
	/* Read number of rows, columns and max value */
	fscanf(input, "%d %d %d", &numRows, &numColumns, &maxValue);
	printf("\nNumber of rows: %d \nNumber of columns: %d \nMax brightness value: %d", numRows, numColumns, maxValue);
	
	/* Reads each of the pixels and adds up 55 pixels to each */
	printf("\nStart reading the pixels of the image");
	for(numRows--; numRows>=0; numRows--){
		for(numColumns--; numColumns>=0; numColumns--){
			fscanf(input, "%d", &currentValue);
			/* Making sure we do not try to set a pixel with more than 255 */
			if(currentValue < 200){
				updatedValue = currentValue + 55;
			}
		}
	}
	printf("\nFinish reading the pixels of the image");
	
	/* Close the file */
	fclose(input);
	printf("\nClosed the file");
	
	pthread_create(&thread0, NULL, do_something, parameter);
	pthread_join(thread0, NULL);
	return 0;
}
