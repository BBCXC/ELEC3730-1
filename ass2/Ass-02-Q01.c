//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $
#include "Q1.h"
#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

#define COMMAND_COUNT 4

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

int debug = 0;


int string_parser(uint8_t *inp, uint8_t **array_of_words_p[]){

	int string_length = 0;
	for (int i = 0; inp[i] != '\0'; i++){		// reading though getting the length of the input string
		string_length++;
	}

	uint8_t *pinp = (uint8_t*)malloc(sizeof(uint8_t) * string_length);
	int pinp_it = 0;
	for (int i = 0; inp[i] != '\0'; i++){
		if (inp[i] != 8){
			pinp[pinp_it] = inp[i];
			pinp_it++;
		}
		else if (pinp_it > 0){
			pinp_it--;
		}
//		printf("inp[i]: %c pinp[j]: %c\n", inp[i], pinp[pinp_it]);
	}
	pinp[pinp_it] = '\0';

//	printf("PINP = %s\n", pinp);

	int letter_count = 0;
	int word_count = 0;
	for (int i = 0; inp[i] != '\0'; i++){
		if ((i > 0 && inp[i] == ' ' && inp[i-1] != ' ') 	// if the character is a space and the previous (reason for i > 0) character wasnt a space
		      || (inp[i+1] == '\0' && inp[i] != ' ')){		// OR this is the last word and the character isnt a space i.e. it is a word
			word_count++;
		}
	}

//	printf("word count: %d\n", word_count);

	*array_of_words_p = (uint8_t**)malloc(sizeof(uint8_t*) * (word_count));
	word_count = 0;

	for (int i = 0; pinp[i] != '\0'; i++){
		if ((i > 0 && pinp[i] == ' ' && pinp[i-1] != ' ') || (pinp[i+1] == '\0' && pinp[i] != ' ')){	// same word check as before
			if (pinp[i+1] == '\0'){	//if this is a word and it is the last word
				letter_count++;		// off by one error says i need to increment the letter count...
			}
			(*array_of_words_p)[word_count] = (uint8_t*)malloc(sizeof(uint8_t)*(letter_count+1));		//allocate memory of lettercount size at this words point in the array
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

int validate_args(int args_count, uint8_t** args){
//	printf("Entered args validator\n");
	for (int i = 1; i < args_count; i++){
		int zerocount = 0;
//		printf("checking arg %d: %f\n", i, atof(args[i]));
		if (atof(args[i]) == 0){
			int wordlength = 0;
			int pointcount = 0;
			for (wordlength = 0; args[i][wordlength] != '\0'; wordlength++){
				if (args[i][wordlength] == '0'){
					zerocount++;
				}
				else if (args[i][wordlength] == '.' && pointcount == 0){
					pointcount++;
				}
			}
			if (wordlength != zerocount+pointcount){
//				printf("wordlength: %d zeros: %d points: %d\n", wordlength, zerocount, pointcount);
				printf("Invalid number: '%s' is non a number\n", args[i]);
				return 0;
			}
		}
	}
	return 1;
}

//returns the number of characters different in the two strings
int str_compare(uint8_t strLength, uint8_t* str1, uint8_t* str2){
	int count = 0;
	for (int i = 0; i < strLength; i++){
		if (str1[i] != str2[i]){
			count++;
		}
	}
	return count;
}

void add_numbers(int args_count, uint8_t** args){
	printf("Entering add_numbers\n");
	float result = 0;
	int error = 0;
	for (int i = 1; i < args_count; i++){
		result += atof(args[i]);
	}
	if (!error){
		printf("Result : %.2f", result);
	}
}

void sub_numbers(int args_count, uint8_t** args){
	printf("Entering sub_numbers\n");
	int error = 0;
	if (args_count != 3){
		printf("Incorrect number of arguments: Subtraction takes 2 arguments, found: %d\n", args_count-1);
		error = 1;
	}
	float result = atof(args[1])-atof(args[2]);
	if (!error){
		printf("Result : %.2f", result);
	}
}

void mul_numbers(int args_count, uint8_t** args){
	printf("Entering mul_numbers\n");
	float result = 1;
	int error = 0;
	for (int i = 1; i < args_count; i++){
		result *= atof(args[i]);
	}
	if (!error){
		printf("Result : %.5f", result);
	}
}

void div_numbers(int args_count, uint8_t** args){
	printf("Entering div_numbers\n");
	int error = 0;
	if (args_count != 3){
		printf("Incorrect number of arguments: Division takes 2 arguments, found: %d\n", args_count-1);
		error = 1;
	}
	float result = atof(args[1])/atof(args[2]);
	if (!error){
		printf("Result : %.2f", result);
	}
}

void debug_function(){
	debug = (debug+1)%2;
	if (debugtoggle == 0){
		debugtoggle = 1;
		printf("debug toggled off");
	}
	else {
		debugtoggle = 0;
		printf("debug toggled off");
	}
}

void CommandLineParserInit(void)
{
	// Print welcome message
	printf("\014");
	printf("ELEC3730 Assignment 2\n");
	printf("Command Line Parser Example\n");
}

#ifndef MECOMMANDLIST
#define MECOMMANDLIST
const command commandlist[] = {
		{ "add", 4, &add_numbers, "add <num1> .. <numN> : Prints the sum of the numbers\n" },
		{ "sub", 4, &sub_numbers, "sub <num1> <num2> : Prints the result of num1-num2\n" },
		{ "mul", 4, &mul_numbers, "mul <num1> .. <numN> : Prints the product of the numbers\n" },
		{ "div", 4, &div_numbers, "div <num1> <num2> : Prints the result of num_1 / num_2\n" },
		{ NULL, NULL, NULL, NULL }
};
#endif
const command commandlist[] = {
	{ "add", 4, &add_numbers, "add <num1> .. <numN> : Prints the sum of the numbers\n" },
	{ "sub", 4, &sub_numbers, "sub <num1> <num2> : Prints the result of num1-num2\n" },
	{ "mul", 4, &mul_numbers, "mul <num1> .. <numN> : Prints the product of the numbers\n" },
	{ "div", 4, &div_numbers, "div <num1> <num2> : Prints the result of num_1 / num_2\n" },
	{ NULL, NULL, NULL, NULL }
};

void help_function(uint8_t argc, uint8_t** args){
	printf("help required you are seeking help\n");
	if (argc == 1){
		for (int i = 0; i < COMMAND_COUNT; i++){
			printf("%s", commandlist[i].help);
		}
		printf("debug <on|off> : Turns debug messages on or off\n");
		printf("help [command] : Prints help information for command\n");
	}
	else{
		int argFound = 0;
		for (int i = 0; i < COMMAND_COUNT && argFound == 0; i++){
			if (str_compare(commandlist[i].length, commandlist[i].name, args[1]) == 0){
				argFound = 1;
				printf("%s", commandlist[i].help);
			}
		}
		if (argFound == 0){
			if (str_compare(5, "help", args[1]) == 0){
				printf("help [command] : Prints help information for command\n");
			}
			else if (str_compare(6, "debug", args[1]) == 0){
				printf("debug <on|off> : Turns debug messages on or off\n");
			}
			else{
				printf("Unknown command : %s\n", args[1]);
			}
		}
	}
}

void CommandLineParserProcess(void)
{
	uint8_t c;

	// Check for input and echo back
#ifdef STM32F407xx
	if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK){
		printf("SERIAL: Got '%c'\n", c);
		HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4

		// STEPIEN: The following is some test code that can be removed
		//
		{
			int c;
			char si[]="1234";
			int i=111;
			char sf[]="r5b6c7d8";
			float f=2.22;

			printf("TEST: Float printf() test: %f\n", 1.234);
			sscanf(si, "%d", &i);
			c=sscanf(sf, "%f", &f);
			printf("TEST: Input string : '%s'\n", si);
			printf("TEST: Input int    : %d\n", i);
			printf("TEST: Input string : '%s'\n", sf);
			printf("TEST: Input float  : %f\n", f);
			printf("TEST: c=%d\n",c);
		}
	}
#else
	int length = 20;
	uint8_t* string = (uint8_t*)malloc(sizeof(uint8_t)*length);	// creating an array of size length. will increase if necessary

	int pos = 0;		//iterator
	while (c != '\n'){
		c = getchar();
//		printf("SERIAL: Got '%c'\n", c);
		string[pos] = c;
		pos++;
		if (pos == length){
			length *= 2;
			string = (uint8_t*)realloc(string, sizeof(uint8_t)*length);
		}
	}
	c = '\0';			// prevents infinite loop
	string[pos-1] = c;
//	printf("string = %s\n", string);
	uint8_t **args;
	int wordcount = string_parser(string, &args);

	for (int i = 0; i < wordcount; i++){
		printf("word %d: %s.\n", i, args[i]);
	}

	printf("compare: %d\n", str_compare(4, commandlist[0].name, args[0]));

	int argFound = 0;
	for (int i = 0; i < COMMAND_COUNT && argFound == 0; i++){
		if (str_compare(commandlist[i].length, commandlist[i].name, args[0]) == 0){
			argFound = 1;
			if (validate_args(wordcount, args) == 1){
				commandlist[i].Function_p(wordcount, args);
			}
			else{
				printf("NAN");
			}
		}
	}
	if (argFound == 0){
		if (str_compare(5, "help", args[0]) == 0){
			help_function(wordcount, args);
		}
		else if (str_compare(6, "debug", args[0]) == 0){
			debug_function();
		}
		else{
			printf("Unknown command: %s\n", args[0]);
		}
	}

#endif
}
