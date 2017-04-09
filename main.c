#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/* Structure to control the image attributes */
struct PGMImage{
	char *type[2];
	int numRows;
	int numColumns;
	int maxValue;
	char image[512][512];
};

/* Function executed by the thread */
void *do_something (void *param){
	/* Implement what the thread will have to do */
	printf("\nHello World! %d", (int)param);
}

/* Read the image */
void readImage(char *fileName[], struct PGMImage image){
	/* Variables to assist with the image reading */
	FILE *input;
	int currentValue, numRows, numColumns;
	
	/* Open the file (filename received as parameter) for reading */
	input = fopen(fileName, "r");
	printf("\nOpening file: %s", fileName);
	
	/* Read the first line related to the image type */
	fscanf(input, "%s", &image.type);
	printf("\nImage file of type: %s", image.type);
	
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
	fscanf(input, "%d %d %d", &image.numRows, &image.numColumns, &image.maxValue);
	printf("\nNumber of rows: %d \nNumber of columns: %d \nMax brightness value: %d", image.numRows, image.numColumns, image.maxValue);
	
	/* Reads each of the pixels and adds up 55 pixels to each */
	printf("\nStart reading the pixels of the image");
	for(numRows=image.numRows-1; numRows>=0; numRows--){
		for(numColumns=image.numColumns-1; numColumns>=0; numColumns--){
			fscanf(input, "%d", &currentValue);
			/* Making sure we do not try to set a pixel with more than 255 */
			if(currentValue < 200){
				image.image[numRows][numColumns] = currentValue + 55;
			}
		}
	}
	printf("\nFinish reading the pixels of the image");
	
	/* Close the file */
	fclose(input);
	printf("\nClosed the file");	
}

int main(int argc, char *argv[]) {
	/* Struct to save the image values */
	struct PGMImage image;
	
	/* Variable for the threads creation */	
	pthread_t thread0;
	int parameter = 0;
	
	/* Calls method to read PGM image */
	readImage(argv[1], image);
	
	pthread_create(&thread0, NULL, do_something, parameter);
	pthread_join(thread0, NULL);
	return 0;
}
