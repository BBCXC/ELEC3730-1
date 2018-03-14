//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file){
		printf("Error: File not found. %s was not found\n", filename);
		fclose(file);
		return -1;
	}

	fseek(file, 0, SEEK_END);
	long int filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	fread(coeff_num_p, sizeof(int), 1, file);
//	printf("number of coefficients: %d\n", *coeff_num_p);

	if (filesize != *coeff_num_p * 8 + 4){
		printf("Error: File is of incorrect size. Predicted: %d Actual: %ld\n", *coeff_num_p*8 + 4, filesize);
		fclose(file);
		return -1;
	}

	*coeff_values_p = (double*)malloc(sizeof(double) * *coeff_num_p);
	fread(*coeff_values_p, sizeof(double), *coeff_num_p, file);
	printf("File name:\t%s\n", filename);
	printf("Array size:\t%d\n", *coeff_num_p);
	for (int i = 0; i < *coeff_num_p; i+=3){
		for (int col = 0; col < 3; col++){
			if (i+col < *coeff_num_p){
				printf("%.3lf ", (*coeff_values_p)[i+col]);
			}
		}
		printf("\n");
	}
	fclose(file);
	return 0;
}
