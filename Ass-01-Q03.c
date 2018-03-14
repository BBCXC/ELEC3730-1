//     $Date: 2017-03-13 08:12:38 +1100 (Mon, 13 Mar 2017) $
// $Revision: 821 $
//   $Author: Peter $

#include "Ass-01.h"

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename)
{
	int coeff_num;					// the number of coefficients foudn in the file
	double *coeff_values;			// the values of those coefficients
	pcm_wavefile_header_t header;	// the header of the file
	char *data;						// the data contained in the file (after the header)

	if (read_coefficients(&coeff_num, &coeff_values, filter_filename) != 0){	// if reading the coefficients failed
		printf("Error: Failed to read coefficients. File %s\n", filter_filename);	// report the file that failed
		return -1;
	}

	if (read_pcm_wavefile(&header, &data, input_wavefilename) != 0){	// if reading the wave file failed
		printf("Error: Failed to read wavefile. File %s\n", input_wavefilename);	//report the file that failed
		return -1;
	}

	printf("Filter the input...\n");
	char *new_data = (char*)malloc((size_t)(sizeof(char) * header.Subchunk2Size));	// allocating memory for the new data
	for (int k = 0; k < header.Subchunk2Size; k++){		// for every byte in the data file
		new_data[k] = 0;	// clearing junk out of memory just in case
		for (int n = 0; n < coeff_num; n++){			// for each coefficient
			if (k-n < 0){		// if byte is out of bounds
				new_data[k] += 0;
			}
			else{				// else apply filter
				new_data[k] += data[k-n] * coeff_values[n];
			}
		}
	}
	printf("Done.\n");

	printf("Write output wavefile %s...\n", output_wavefilename);
	if (write_pcm_wavefile(&header, new_data, output_wavefilename) != 0){ //if there as an error writing to file (even though thats impossible)
		printf("Error: Could not write to file.\n");
		return -1;
	}
	printf("Done.\n");

//	free(coeff_values);
//	free(data);
	return 0;
}
