/*
 * Q1.h
 *
 *  Created on: 3May,2018
 *      Author: Kyle
 */

#ifndef Q1_H_
#define Q1_H_
#include "Ass-02.h"
//uint8_t checkForNumericArgument(uint8_t processingIntegerFlag);
//uint8_t checkArgumentLength(uint8_t flag, uint8_t expectedWordCount);

int debugtoggle;

#ifndef MESTRUCT
#define MESTRUCT
typedef struct{
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
} Rectangle;

typedef struct{
	uint8_t* symbol;
	uint8_t command;
	Rectangle border;
	Rectangle text;
} Button;

typedef struct{
	double symnum;
	int is_symbol;
} Symnum;

typedef struct {
	uint8_t* name;
	uint8_t length;
	uint8_t (*Function_p)(uint8_t argc, uint8_t* argv);
	uint8_t* help;
} command;
#endif

void dout(const char* str);
int string_parser(uint8_t *inp, uint8_t **array_of_words_p[]);
int validate_args(int args_count, uint8_t** args);
int str_compare(uint8_t strLength, uint8_t* str1, uint8_t* str2);
void add_numbers(int args_count, uint8_t** args);
void sub_numbers(int args_count, uint8_t** args);
void mul_numbers(int args_count, uint8_t** args);
void div_numbers(int args_count, uint8_t** args);
void debug_function(uint8_t argc, uint8_t** args);
void help_function(uint8_t argc, uint8_t** args);

uint8_t rect_collision(Rectangle r, uint16_t x, uint16_t y);
void draw_button(Button b);
Symnum* parse_expression(uint8_t exp_length, uint8_t* exp, int* argcout);
Symnum eval_expression(uint8_t argc, Symnum* args);

const command commandList[5]; //todo alter later?

#endif /* Q1_H_ */
