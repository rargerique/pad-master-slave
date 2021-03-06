#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys\timeb.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/* Structure to control the image attributes */
struct PGMImage{
	char *type[2];
	int numRows;
	int numColumns;
	int maxValue;
	int** image;
};

struct Params{
	int beginRow;
	int endRow;
	int beginColumn;
	int endColumn;
	struct PGMImage *image;
	FILE *input;
};

/* Updates the pixel values of a given portion of the image */
void *updateValues(void *parameters){
	struct Params *params = parameters;
	int currentValue, numRows, numColumns;
	
	/* Reads and updates in the struct image the values in every pixel */
	for(numRows=params->beginRow; numRows<params->endRow; numRows++){
		for(numColumns=params->beginColumn; numColumns<params->endColumn; numColumns++){
			fscanf(params->input, "%d", &currentValue);
			/* Making sure we do not try to set a pixel with more than 255 */
			if(currentValue <= 200){
				params->image->image[numRows][numColumns] = currentValue + 55;
			} else{
				params->image->image[numRows][numColumns] = 255;
			}
		}
	}
}

/* Read the image */
void readImage(char *fileName[], struct PGMImage *image, int nThreads){
	/* Variables to assist with the image reading */
	FILE *input;
	int i;
	
	/* Variables to assist with the threads control */
	int threadCounter, rowsByThread, columnsByThread;
	pthread_t threadId[nThreads];
	struct Params params[nThreads];
	
	/* Variables to control the time measuring */
	struct timeb start, end;
	int finalTime;
	
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
	
	/* Dinamically alloc size for the image matrix */
	image->image = malloc(image->numRows * sizeof(int*));
	for(i=0; i<image->numRows; i++){
		image->image[i] = malloc(image->numColumns * sizeof(int));
	}
	
	/* Reads each of the pixels and adds up 55 pixels to each */
	printf("\nStart reading the pixels of the image"); 
	
	/* Calculate the number of rows/columns each thread will handle */
	rowsByThread = image->numRows/nThreads;
	columnsByThread = image->numColumns/nThreads;
	
	/* Start a counter */
	ftime(&start);
	
	/* If the number of threads is 1 we execute the reading in the main thread, otherwise we create children */
	if(nThreads>1){
		for(threadCounter=0; threadCounter<nThreads; threadCounter++){
			/* Set the parameters to be passed to the method that set the Image struct */
			params[threadCounter].beginColumn = threadCounter*columnsByThread;
			params[threadCounter].beginRow = threadCounter*rowsByThread;
			params[threadCounter].endColumn = ((threadCounter+1)*columnsByThread)-1;
			params[threadCounter].endRow = ((threadCounter+1)*rowsByThread)-1;
			params[threadCounter].image = image;
			params[threadCounter].input = input;
			
			pthread_create(&threadId[threadCounter], NULL, updateValues, &params[threadCounter]);	
		}
		
		for(threadCounter=0; threadCounter<nThreads; threadCounter++){
			pthread_join(threadId[threadCounter], NULL);
		}
	} else{
		/* Set the parameters to be passed to the method that set the Image struct */
		params[0].beginColumn = 0;
		params[0].beginRow = 0;
		params[0].endColumn = image->numColumns;
		params[0].endRow = image->numRows;
		params[0].image = image;
		params[0].input = input;
		
		updateValues(&params[0]);
	}
	
	printf("\nFinish reading the pixels of the image");
	
	/* End the timer and get result */
	ftime(&end);
	finalTime = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
	printf("\nTime to read file and record on image struct with %d thread(s): %d milliseconds", nThreads, finalTime);
	
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
	int numThreads = 1;
	
	/* Calls method to read PGM image */
	readImage(argv[1], &image, numThreads);
	
	/* Calls method to write the new PGM image*/
	saveImage(argv[2], &image);

	return 0;
}
