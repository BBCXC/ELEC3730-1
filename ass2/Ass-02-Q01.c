#include "Q1.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

#define COMMAND_COUNT 4

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

uint8_t* string;

#ifndef MESTRUCT
#define MESTRUCT
typedef struct {
	uint8_t* name;
	uint8_t length;
	uint8_t (*Function_p)(uint8_t argc, uint8_t* argv);
	uint8_t* help;
} command;
#endif

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
	char* debugmsg = (char*)malloc(sizeof(char)*100);
	sprintf(debugmsg, "Entered args validator\n");
	dout(debugmsg);
	for (int i = 1; i < args_count; i++){
		int zerocount = 0;
		sprintf(debugmsg, "checking arg %d: %f\n", i, atof(args[i]));
		dout(debugmsg);
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
				sprintf(debugmsg, "wordlength: %d zeros: %d points: %d\n", wordlength, zerocount, pointcount);
				dout(debugmsg);
				printf("Invalid number: '%s' is non a number\n", args[i]);
				free(debugmsg);
				return 0;
			}
		}
	}
	free(debugmsg);
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

void debug_function(uint8_t argc, uint8_t** args){
	if (argc == 2){
		if (str_compare(3, "on", args[1]) == 0){
			debugtoggle = 1;
			printf("debug toggled on\n");
		}
		else if (str_compare(4, "off", args[1]) == 0){
			debugtoggle = 0;
			printf("debug toggled off\n");
		}
		else {
			printf("invalid debug command: %s\n", args[1]);
		}
	}
	else {
		printf("too many arguments for debug, expected 2, got: %d\n", argc);
	}
}

void CommandLineParserInit(void)
{
	// Print welcome message
	printf("\014");
	printf("ELEC3730 Assignment 2\n");
	printf("Command Line Parser Example\n");
	string = (uint8_t*)malloc(sizeof(uint8_t)*20);
}

const command commandlist[] = {
	{ "add", 4, &add_numbers, "add <num1> .. <numN> : Prints the sum of the numbers\n" },
	{ "sub", 4, &sub_numbers, "sub <num1> <num2> : Prints the result of num1-num2\n" },
	{ "mul", 4, &mul_numbers, "mul <num1> .. <numN> : Prints the product of the numbers\n" },
	{ "div", 4, &div_numbers, "div <num1> <num2> : Prints the result of num_1 / num_2\n" },
	{ NULL, NULL, NULL, NULL }
};

void help_function(uint8_t argc, uint8_t** args){
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
	char* debugmsg = (char*)malloc(sizeof(char)*100);
	uint8_t c;
	static uint8_t pos = 0;
	static uint8_t length = 20;
	// Check for input and echo back
#ifdef STM32F407xx
	if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK){
		sprintf(debugmsg, "SERIAL: Got '%c'\n", c);
		dout(debugmsg);
//		HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4
		string[pos] = c;
		pos++;
		printf("%c", c);
		if (pos == length){
			sprintf(debugmsg, "console length doubled");
			dout(debugmsg);
			length *= 2;
			string = (uint8_t*)realloc(string, sizeof(uint8_t)*length);
		}
		string[pos+1] = '\0';
		sprintf(debugmsg, "current string %s", string);
		dout(debugmsg);
	}
	if (c == '\n' || c == '\r'){	// if enter was pressed
		sprintf(debugmsg, "enter pressed");
		dout(debugmsg);
		c = '\0';			// prevents infinite loop
		string[pos-1] = c;
		uint8_t **args;
		int wordcount = string_parser(string, &args);		// parse string into arguements
		for (int i = 0; i < wordcount; i++){
			printf("word %d: %s.\n", i, args[i]);
		}

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
				debug_function(wordcount, args);
			}
			else{
				printf("Unknown command: %s\n", args[0]);
			}
		}
		length = 20;
		pos = 0;
		string = (uint8_t*)malloc(sizeof(uint8_t)*length);
		free(args);
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

	free(debugmsg);
}
