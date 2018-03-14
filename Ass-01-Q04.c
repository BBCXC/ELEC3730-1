//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

#include "Ass-01.h"

int string_parser(char *inp, char **array_of_words_p[])
{

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

	for (int i = 0; inp[i] != '\0'; i++){
		if ((i > 0 && inp[i] == ' ' && inp[i-1] != ' ') || (inp[i+1] == '\0' && inp[i] != ' ')){	// same word check as before
			if (inp[i+1] == '\0'){	//if this is a word and it is the last word
				letter_count++;		// off by one error says i need to increment the letter count...
			}
			(*array_of_words_p)[word_count] = (char*)malloc(sizeof(char)*(letter_count+1));		//allocate memory of lettercount size at this words point in the array
//			printf("word %d length: %d\n", word_count, letter_count);
			letter_count = 0;		//reset lettercount for the next word
			word_count++;
		}
		if (i > 0 && inp[i] == ' ' && inp[i-1] == ' '){		// if this is a space and the previous character was a space
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

	for (int i = 0; inp[i] != '\0'; i++){
		if ((i > 0 && inp[i] == ' ' && inp[i-1] != ' ') || (inp[i+1] == '\0' && inp[i] != ' ')){	// same word check again
			if (inp[i+1] == '\0'){	//if this is a word and it is the last word
				(*array_of_words_p)[word_count][letter_count] = inp[i];
				letter_count++;
			}
			(*array_of_words_p)[word_count][letter_count] = '\0';
//			printf("word created: %s\n", (*array_of_words_p)[word_count]);
			word_count++;
			letter_count = 0;
		}
		else if (inp[i] == ' '){		// if this is a space and the previous character was a space
			continue;										// dont increment the letter count just continue reading
		}
		else{
//			printf("word: %d letter: %d = %c\n", word_count, letter_count, inp[i]);
			(*array_of_words_p)[word_count][letter_count] = inp[i];
			letter_count++;
		}
	}
	return word_count;
}
