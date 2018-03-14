//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file){
		printf("\nFile not found");
		fclose(file);
		return -1;
	}

	fseek(file, 0, SEEK_END);
	long int file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	fread(coeff_num_p, sizeof(byte), 4, file);
	printf("\nnumber of coefficients: %d", *coeff_num_p);
	if (file_size != *coeff_num_p * 8 + 4){
		printf("\nFile is of incorrect size. Predicted: %d Actual: %ld", *coeff_num_p*8 + 4, file_size);
		fclose(file);
		return -1;
	}
	*coeff_values_p = (double*)malloc(sizeof(double) * *coeff_num_p);
	fread(*coeff_values_p, sizeof(double), *coeff_num_p, file);
	printf("\nFile name:\t%s", filename);
	printf("\nArray size:\t%d", *coeff_num_p);
	for (int i = 0; i < *coeff_num_p; i++){
		printf("\n%lf", (*coeff_values_p)[i]);
	}
	fclose(file);
	return 1;
}
