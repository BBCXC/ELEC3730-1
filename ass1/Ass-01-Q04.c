//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

#include "Ass-01.h"

int string_parser(char *inp, char **array_of_words_p[])
{

	int string_length = 0;
	for (int i = 0; inp[i] != '\0'; i++){		// reading though getting the length of the input string
		string_length++;
		printf("inp[i]: %d\n", inp[i]);
	}

	char *pinp = (char*)malloc(sizeof(char) * string_length);
	int pinp_it = 0;
	for (int i = 0; inp[i] != '\0'; i++){
		if (inp[i] != 8){
			pinp[pinp_it] = inp[i];
			pinp_it++;
		}
		else if (pinp_it > 0){
			pinp_it--;
		}
		printf("inp[i]: %c pinp[j]: %c\n", inp[i], pinp[pinp_it]);
	}
	pinp[pinp_it] = '\0';

	printf("PINP = %s\n", pinp);

	int letter_count = 0;
	int word_count = 0;
	for (int i = 0; inp[i] != '\0'; i++){
		if ((i > 0 && inp[i] == ' ' && inp[i-1] != ' ') 	// if the character is a space and the previous (reason for i > 0) character wasnt a space
		      || (inp[i+1] == '\0' && inp[i] != ' ')){		// OR this is the last word and the character isnt a space i.e. it is a word
			word_count++;
		}
	}

//	printf("word count: %d\n", word_count);

	*array_of_words_p = (char**)malloc(sizeof(char*) * word_count);
	word_count = 0;

	for (int i = 0; pinp[i] != '\0'; i++){
		if ((i > 0 && pinp[i] == ' ' && pinp[i-1] != ' ') || (pinp[i+1] == '\0' && pinp[i] != ' ')){	// same word check as before
			if (pinp[i+1] == '\0'){	//if this is a word and it is the last word
				letter_count++;		// off by one error says i need to increment the letter count...
			}
			(*array_of_words_p)[word_count] = (char*)malloc(sizeof(char)*(letter_count+1));		//allocate memory of lettercount size at this words point in the array
//			printf("word %d length: %d\n", word_count, letter_count);
			letter_count = 0;		//reset lettercount for the next word
			word_count++;
		}
		if (i > 0 && pinp[i] == ' ' && pinp[i-1] == ' '){		// if this is a space and the previous character was a space
			continue;										// dont increment the letter count just continue reading
		}
		else {
			letter_count++;
		}
	}

	word_count = 0;
	letter_count = 0;
//	(*array_of_words_p)[0][1] = 'a';
//	printf("letter[0] %c", (*array_of_words_p)[2][0]);

	for (int i = 0; pinp[i] != '\0'; i++){
		if ((i > 0 && pinp[i] == ' ' && pinp[i-1] != ' ') || (pinp[i+1] == '\0' && pinp[i] != ' ')){	// same word check again
			if (pinp[i+1] == '\0'){	//if this is a word and it is the last word
				(*array_of_words_p)[word_count][letter_count] = pinp[i];
				letter_count++;
			}
			(*array_of_words_p)[word_count][letter_count] = '\0';
//			printf("word created: %s\n", (*array_of_words_p)[word_count]);
			word_count++;
			letter_count = 0;
		}
		else if (pinp[i] == ' '){		// if this is a space and the previous character was a space
			continue;										// dont increment the letter count just continue reading
		}
		else{
//			printf("word: %d letter: %d = %c\n", word_count, letter_count, pinp[i]);
			(*array_of_words_p)[word_count][letter_count] = pinp[i];
			letter_count++;
		}
	}
	free(pinp);

	return word_count;

}
