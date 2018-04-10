//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

#define COLUMNS 3

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	FILE *file = fopen(filename, "rb");	// opening the coefficient file as a binary file
	if (!file){							// if the file was not successfully opened
		printf("Error: File not found. %s was not found\n", filename);
		fclose(file);	// probably not nessecary to close a null file
		return -1;
	}

	fseek(file, 0, SEEK_END);		//send iterator to the end of the file
	long int filesize = ftell(file);// get the position of the iterator ( == the size of the file)
	fseek(file, 0, SEEK_SET);		// send iterator back to the begining to start reading from the start

	fread(coeff_num_p, sizeof(byte), 4, file);	// read the first 4 bytes (int) which is the number of coefficients found in the file
//	printf("number of coefficients: %d\n", *coeff_num_p);

	if (filesize != *coeff_num_p * 8 + 4){		// if the file is not of the correct size based on the number of coefficients
		printf("Error: File is of incorrect size. Predicted: %d Actual: %ld\n", *coeff_num_p*8 + 4, filesize);
		fclose(file);
		return -1;
	}

	*coeff_values_p = (double*)malloc(sizeof(double) * *coeff_num_p);	// allocate memory to store all the coefficients
	fread(*coeff_values_p, sizeof(double), *coeff_num_p, file);			// read in the coefficients

	printf("File name:\t%s\n", filename);		// start printing the report
	printf("Array size:\t%d\n", *coeff_num_p);
	for (int i = 0; i < *coeff_num_p; i+=COLUMNS){
		for (int col = 0; col < COLUMNS; col++){// format the data based on the COLUMNS constant
			if (i+col < *coeff_num_p){
				printf("%.3lf ", (*coeff_values_p)[i+col]);
			}
		}
		printf("\n");
	}
	fclose(file);
	return 0;
}
