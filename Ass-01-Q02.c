//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 2

#include "Ass-01.h"

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename)
{
	FILE *file = fopen(filename, "rb");
//	printf("wavefilesize%d\n", sizeof(pcm_wavefile_header_t));
	if (!file){
		printf("Error: File not found. %s was not found\n", filename);
		return -1;
	}

//	fseek(file, 0, SEEK_END);
//	int filesize = ftell(file);
//	fseek(file, 0, SEEK_SET);
//	printf("%d", fread(header_p, sizeof(pcm_wavefile_header_t), 1, file));
	if (fread(header_p, sizeof(pcm_wavefile_header_t), 1, file) != 1){
		printf("Error: Header is of unexpected size. File: %s.\n", filename);
		fclose(file);
		return -1;
	}

	if (header_p->ChunkID[0] != 'R' || header_p->ChunkID[1] != 'I' || header_p->ChunkID[2] != 'F' || header_p->ChunkID[3] != 'F' ){
		printf("Error: ChunkID is of unexpected type. File: %s Expected: RIFF Actual: %c%c%c%c\n", filename, header_p->ChunkID[0], header_p->ChunkID[1], header_p->ChunkID[2], header_p->ChunkID[3]);
			fclose(file);
			return -1;
	}

	if (header_p->Subchunk1Size != 16){
		printf("Error: Subchunk1 is of unexpected size. File: %s Expected size: %d, Size %d\n", filename, 16, header_p->Subchunk1Size);
		fclose(file);
		return -1;
	}

	if (header_p->AudioFormat != 1){
		printf("Error: AudioFormat is of unexpected type. File %s Expected value: 1 Value %d", filename, header_p->AudioFormat);
		fclose(file);
		return -1;
	}

	*data_p = (char*)malloc((size_t)(sizeof(char) * header_p->Subchunk2Size));

	if (fread(*data_p, sizeof(char), header_p->Subchunk2Size, file) != header_p->Subchunk2Size){
		printf("Error: Data is of unexpected size. File: %s\n", filename);
		fclose(file);
		return -1;
	}
	printf("Opening input file %s\n", filename);
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
	FILE *file = fopen(filename, "wb");
	fwrite(header_p, sizeof(pcm_wavefile_header_t), 1, file);
	fwrite(data, sizeof(char), header_p->Subchunk2Size, file);
	fclose(file);
	return 0;
}
