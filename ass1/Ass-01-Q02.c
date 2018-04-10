//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 2

#include "Ass-01.h"

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename)
{
	FILE *file = fopen(filename, "rb"); //open the wave file as a binary file

	if (!file){			// if file not sucessfully opened
		printf("Error: File not found. File: %s\n", filename);
		return -1;
	}

	fseek(file, 0, SEEK_END);		//send iterator to the end of the file
	long int filesize = ftell(file);// get the position of the iterator ( == the size of the file)
	fseek(file, 0, SEEK_SET);		// send iterator back to the begining to start reading from the start

	int error = 0;		// allow for multiple errors to be reported each file

	if (fread(header_p, sizeof(pcm_wavefile_header_t), 1, file) != 1){	// read in the header file. if it was not succeddfully read
		printf("Error: File size is less than header size. File: %s\n", filename);	// print error that header file was not read
		error = 1;
	}

	if (filesize != header_p->ChunkSize +8){		// if filesize does not equal the header declared size (chunksize + 8)
//		printf("filesize: %ld expected %d", filesize, header_p->ChunkSize +8);
		printf("Error: File is of unexpected size. File: %s Expected: %d Actual: %ld\n", filename, (header_p->ChunkSize + 8), filesize);
		error = 1;
	}

	if (header_p->ChunkSize != header_p->Subchunk1Size + header_p->Subchunk2Size + 20){			// if the chunksize does not match the rest of the file
		printf("Error: ChunkSize does not match the actual size. File: %s ChunkSize: %d Actual: %d\n", filename, header_p->ChunkSize, header_p->Subchunk1Size + header_p->Subchunk2Size + 20);
		error = 1;
	}

	if (header_p->ChunkID[0] != 'R' || header_p->ChunkID[1] != 'I' || header_p->ChunkID[2] != 'F' || header_p->ChunkID[3] != 'F' ){ //if chunkID is not of RIFF
		printf("Error: ChunkID is of unexpected type. File: %s Expected: 'RIFF' Actual: '%c%c%c%c'\n", filename, header_p->ChunkID[0], header_p->ChunkID[1], header_p->ChunkID[2], header_p->ChunkID[3]);
		error = 1;
	}

	if (header_p->Subchunk1ID[0] != 'f' || header_p->Subchunk1ID[1] != 'm' || header_p->Subchunk1ID[2] != 't' || header_p->Subchunk1ID[3] != ' ' ){ //if SubchunSubchunk1ID is not of RIFF
		printf("Error: Subchunk1ID is of unexpected type. File: %s Expected: 'fmt ' Actual: '%c%c%c%c'\n", filename, header_p->Subchunk1ID[0], header_p->Subchunk1ID[1], header_p->Subchunk1ID[2], header_p->Subchunk1ID[3]);
		error = 1;
	}

	if (header_p->Subchunk1Size != 16){	// if the Subchunk1size spesified in the header is not 16 (standard for pcm)
		printf("Error: Subchunk1 is of unexpected size. File: %s Expected size: %d Size %d\n", filename, 16, header_p->Subchunk1Size);
		error = 1;
	}

	if (header_p->AudioFormat != 1){ // if the audio format is not 1 (probably not pcm encoded)
		printf("Error: AudioFormat is of unexpected type. File %s Expected value: 1 Value %d\n", filename, header_p->AudioFormat);
		error = 1;
	}

	if (header_p->Subchunk2ID[0] != 'd' || header_p->Subchunk2ID[1] != 'a' || header_p->Subchunk2ID[2] != 't' || header_p->Subchunk2ID[3] != 'a' ){ //if SubchunSubchunk2ID is not of data
		printf("Error: Subchunk2ID is of unexpected type. File: %s Expected: 'data' Actual: '%c%c%c%c'\n", filename, header_p->Subchunk2ID[0], header_p->Subchunk2ID[1], header_p->Subchunk2ID[2], header_p->Subchunk2ID[3]);
		error = 1;
	}

	if (header_p->Subchunk2Size != filesize-sizeof(pcm_wavefile_header_t)){ // if subchunk2 is an unreasonable value
		printf("Error: Subchunk2 is of unexpected size: File: %s Expected: %d Actual: %ld\n", filename, header_p->Subchunk2Size, filesize-sizeof(pcm_wavefile_header_t));
		fclose(file);
		return -1;		//this returns because the data cannot be read if this size is off
	}

	*data_p = (char*)malloc((size_t)(sizeof(char) * header_p->Subchunk2Size)); //allocate memory to read the data to of size subchunk2 spesified in header)

	if (fread(*data_p, sizeof(char), header_p->Subchunk2Size, file) != header_p->Subchunk2Size){	// read in the data portion. if the data read was not the size that was expected
		printf("Error: Data is of unexpected size. File: %s\n", filename);
		error = 1;
	}

	if (error == 1){
		fclose(file);
		return -1;
	}

	printf("Opening input file %s\n", filename);			// print all the values in the header
	printf("ChunkID = %c%c%c%c\n", header_p->ChunkID[0], header_p->ChunkID[1], header_p->ChunkID[2], header_p->ChunkID[3]);
	printf("ChunkSize = %d\n", header_p->ChunkSize);
	printf("ChunkFormat = %c%c%c%c\n", header_p->Format[0], header_p->Format[1], header_p->Format[2], header_p->Format[3]);
	printf("\n");
	printf("Subchunk1ID = %c%c%c%c\n", header_p->Subchunk1ID[0], header_p->Subchunk1ID[1], header_p->Subchunk1ID[2], header_p->Subchunk1ID[3]);
	printf("Subchunk1Size = %d\n", header_p->Subchunk1Size);
	printf("AudioFormat = %d\n", header_p->AudioFormat);
	printf("NumChannels = %d\n", header_p->NumChannels);
	printf("SampleRate = %d\n", header_p->SampleRate);
	printf("ByteRate = %d\n", header_p->ByteRate);
	printf("BlockAlign = %d\n", header_p->BlockAlign);
	printf("BitsPerSample = %d\n", header_p->BitsPerSample);
	printf("\n");
	printf("Subchunk2ID = %c%c%c%c\n", header_p->Subchunk2ID[0], header_p->Subchunk2ID[1], header_p->Subchunk2ID[2], header_p->Subchunk2ID[3]);
	printf("Subchunk2Size = %d\n", header_p->Subchunk2Size);

	fclose(file);
	return 0;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename)
{
	FILE *file = fopen(filename, "wb");		//open the file as write binary
	//no need for a check file because write will always return a valid pointer to a file
	fwrite(header_p, sizeof(pcm_wavefile_header_t), 1, file);	// write the header
	fwrite(data, sizeof(char), header_p->Subchunk2Size, file);	// write the filtered data

	fclose(file);
	return 0;
}
