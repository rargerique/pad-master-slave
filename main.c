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
	int image[512][512];
};

/* Function executed by the thread */
void *do_something (void *param){
	/* Implement what the thread will have to do */
	printf("\nHello World! %d", (int)param);
}

/* Updates the pixel values of a given portion of the image */
void updateValues(int beginRow, int endRow, int beginColumn, int endColumn, struct PGMImage *image, FILE *input){
	int currentValue, numRows, numColumns;
	
	/* Reads and updates in the struct image the values in every pixel */
	for(numRows=beginRow; numRows<endRow; numRows++){
		for(numColumns=beginColumn; numColumns<endColumn; numColumns++){
			fscanf(input, "%d", &currentValue);
			/* Making sure we do not try to set a pixel with more than 255 */
			if(currentValue <= 200){
				image->image[numRows][numColumns] = currentValue + 55;
			} else{
				image->image[numRows][numColumns] = 255;
			}
		}
	}
	printf("\nFinish reading the pixels of the image");
}

/* Read the image */
void readImage(char *fileName[], struct PGMImage *image, int nThreads){
	/* Variables to assist with the image reading */
	FILE *input;
	
	/* Variable to assist with the threads control */
	int threadCounter;
	pthread_t threadId[nThreads];
	
	/* Open the file for reading */
	input = fopen(fileName, "r");
	printf("\nOpening file: %s", fileName);
	
	/* Read the first line related to the image type */
	fscanf(input, "%s", &image->type);
	printf("\nImage file of type: %s", image->type);
	
	/* Skip line */
	while(getc(input) != '\n');
	
	/* Skip whole comment line */
	while(getc(input) == '#'){
		while(getc(input) != '\n');
	}
	printf("\nSkiped comment line");
	
	/* Backup three positions */
	/* I literally have no idea why we have to do this */
	fseek(input, -3, SEEK_CUR);
	
	/* Read number of rows, columns and max value */
	fscanf(input, "%d %d %d", &image->numRows, &image->numColumns, &image->maxValue);
	printf("\nNumber of rows: %d \nNumber of columns: %d \nMax brightness value: %d", image->numRows, image->numColumns, image->maxValue);
	
	/* Reads each of the pixels and adds up 55 pixels to each */
	printf("\nStart reading the pixels of the image");
	
	/* If the number of threads is 1 we execute the reading in the main thread, otherwise we create children */
	if(nThreads>1){
		for(threadCounter=0; threadCounter<nThreads; threadCounter++){
			//pthread_create(&threadId[threadCounter], NULL, function, arguments);	
		}
	} else{
		updateValues(0, image->numRows, 0, image->numColumns, image, input);
	}
	
	/* Close the file */
	fclose(input);
	printf("\nClosed the file");	
}

/* Save the image */
void saveImage(char *fileName[], struct PGMImage *image){
	/* Variables to assist with the aving of the image */
	FILE *output;
	int numRows, numColumns;
	
	/* Open the file for writing */
	output = fopen(fileName, "w");
	printf("\nOpening file for writing: %s", fileName);
	
	/* Write the first line related to the image type */
	fprintf(output, &image->type);
	fprintf(output, "\n");
	printf("\nWrote type of file", image->type);
	
	/* Write number of rows, columns and max value */
	fprintf(output, "%d %d\n%d\n", image->numRows, image->numColumns, image->maxValue);
	printf("\nWriting number of rows, columns and max value of brightness");
	
	/* Writes the file with the new pixels value */
	printf("\nStart writing the pixels to the image");
	for(numRows=0; numRows<image->numRows; numRows++){
		for(numColumns=0; numColumns<image->numColumns; numColumns++){
			fprintf(output, "%d ", image->image[numRows][numColumns]);
		}
		fprintf(output, "\n");
	}
	printf("\nFinish reading the pixels of the image");
	
	/* Close the file */
	fclose(output);
	printf("\nClosed the new file");	
}

int main(int argc, char *argv[]) {
	/* Struct to save the image values */
	struct PGMImage image;
	
	/* Variable for the threads creation */	
	pthread_t thread0;
	int parameter = 0;
	int numThreads = 1;
	
	/* Calls method to read PGM image */
	readImage(argv[1], &image, 1);
	
	/* Calls method to write the new PGM image*/
	saveImage(argv[2], &image);
	
	pthread_create(&thread0, NULL, do_something, parameter);
	pthread_join(thread0, NULL);
	return 0;
}
