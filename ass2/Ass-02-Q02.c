//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"

#define WIDTH 60
#define XBUTTONS 5
#define HEIGHT 40
#define YBUTTONS 4
#define XMARGIN (320-(XBUTTONS*WIDTH))/(XBUTTONS+1)
#define YMARGIN (240-(YBUTTONS*HEIGHT))/(YBUTTONS+1)
#define NUM_OF_BUTTONS (XBUTTONS*YBUTTONS)

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE 
//
uint8_t debugflag = 1;

typedef struct{
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;
} Rectangle;

typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Colour;

typedef struct{
	uint8_t* symbol;
	uint8_t command;
	Rectangle border;
	Rectangle text;
} Button;

void debug(const char* str){
	if (debugflag){
		printf("%s", str);
	}
}

uint8_t rect_collision(Rectangle r, uint16_t x, uint16_t y){
	if (x > r.x && x < (r.x+r.w) && y > r.y && y < (r.y+r.h)){
		return 1;
	}
	return 0;
}

void draw_button(Button b){
	// draw border
	BSP_LCD_DrawRect(b.border.x, b.border.y, b.border.w, b.border.h);
	// draw text
	BSP_LCD_DisplayStringAt(b.text.x, b.text.y , b.symbol, CENTER_MODE);
}

uint8_t* parse_expression(uint8_t exp_length, uint8_t* exp, int* argcout){
	uint8_t* exp_args_lengths = (uint8_t*)malloc(exp_length * 8);
	int arg_length = 0;
	int num_args = 0;
	for (int i = 0; i < exp_length; i++){
		if (	exp[i] != '+' &&
				exp[i] != '-' &&
				exp[i] != '*' &&
				exp[i] != '/' &&
				exp[i] != '(' &&
				exp[i] != ')' ){	//its a number or '.'
			arg_length++;
		}
		else{
			if (arg_length == 0){
				exp_args_lengths[i] = 1;
			}
			else{
				exp_args_lengths[i] = arg_length;
				arg_length = 0;
			}
			num_args++;
		}
	}
	//now have list of how long args are
	uint8_t** exp_args = (uint8_t**) malloc(num_args*sizeof(uint8_t));
	int pos = 0;
	for (int i = 0; i < num_args; i++){
		if (exp_args_lengths[i] == 1){
			exp_args[i] = (uint8_t*) malloc(sizeof(uint8_t));
			exp_args[i][0] = exp[pos];
			pos++;
		}
		else{
			exp_args[i] = (uint8_t*) malloc(sizeof(uint8_t) * exp_args_lengths[i]+1);
			for (int j = 0; i < exp_args_lengths[i]; i++){
				exp_args[i][j] = exp[pos];
				pos++;
			}
			exp_args[i][exp_args_lengths[i]] = '\0';
		}
	}
	double* exp_array = (double*)malloc(sizeof(double) * num_args);
	for (int i = 0; i < num_args; i++){
		if (	exp_args[i][0] != '+' &&
				exp_args[i][0] != '-' &&
				exp_args[i][0] != '*' &&
				exp_args[i][0] != '/' &&
				exp_args[i][0] != '(' &&
				exp_args[i][0] != ')' ){
			exp_array[i] = strtod(exp_args[i]);
		}
		else {
			exp_array[i] = (double) exp_args[i][0];
		}
	}
	*argcout = num_args;
	return exp_array;
}

double eval_expression(uint8_t argc, uint8_t* args){
	uint8_t wordcount = argc;
	uint8_t brackdone = 0;
	uint8_t muldivdone = 0;
	uint8_t addsubdone = 0;
	uint8_t* newargs;
	int j = 0;
	while (wordcount > 1){
		uint8_t brackflag = 0;    //flags are set to 1 by the end if their respective operation occured.
		uint8_t muldivflag = 0;
		uint8_t addsubflag = 0;
		for (int i = 0; i < wordcount; i++){    // looping through all arguments left to process
			j = 0;                      // second iterator to construct the next set of arguments
			newargs = (uint8_t*) malloc (8 * wordcount);    //allocating more than enough space for new arguements;
			if (!brackdone){                // handles brackets
				if (args[i] == '('){        // looking for open of bracketed section
					int sublen = 1;         //sub-expression length
					int depthcount = 0;    // incremented if another open bracket is found and decremented when closed (-1 because the first open bracket will make it 0)
					while (!(args[i+sublen] != ')' && depthcount != 0)){    // counting the length of the section in brackets
						if (args[i+sublen] == '('){
							depthcount++;
						}
						else if (args[i+sublen] == ')'){
							depthcount--;
						}
						sublen++;
					}
					uint8_t* subargs = (uint8_t*) malloc(8*sublen);	// allocating space for the contence inside the brackets
					for (int k = 0; k < sublen; k++){	       // copying contence in to the sub-expression arguments array
					    subargs[k] = args[i];
					    i++;
					}
					newargs[j] = eval_expression(sublen, subargs);    // recursive call to evaluate the sub-expression
					free(subargs);
					brackflag = 1;
					j++;
				}
			}
			else if (!muldivdone){
				if (args[i] == '*'){
					newargs[j] = newargs[j] * args[i+1];
					muldivflag = 1;
					i++;
					j++;
				}
				else if (args[i] == '/'){
					newargs[j] = newargs[j] / args[i+1];
					muldivflag = 1;
					i++;
					j++;
				}
				else{
					newargs[j] = args[i];
					j++;
				}
			}
			else{
				if (args[i] == '+'){
					newargs[j] = newargs[j] + args[i+1];
					addsubflag = 1;
					i++;
					j++;
				}
				else if (args[i] == '-'){
					newargs[j] = newargs[j] - args[i+1];
					addsubflag = 1;
					i++;
					j++;
				}
				else{
					newargs[j] = args[i];
					j++;
				}
			}
		}
		if (muldivdone && !addsubflag){
			addsubdone = 1;
		}
		if (brackdone && !muldivflag){
			muldivdone = 1;
		}
		if (!brackflag){
			brackdone = 1;
		}
		wordcount = j;
		free(args);
		args = newargs;
	}
	return args[0];
}

const Button buttons[] = {
		{"7",	'7',	{(XMARGIN+ 0 *(XMARGIN+WIDTH)), (YMARGIN+ 1 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 0 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 1 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"8",	'8',	{(XMARGIN+ 1 *(XMARGIN+WIDTH)), (YMARGIN+ 1 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 1 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 1 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"9",	'9',	{(XMARGIN+ 2 *(XMARGIN+WIDTH)), (YMARGIN+ 1 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 2 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 1 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"(",	'(',	{(XMARGIN+ 3 *(XMARGIN+WIDTH)), (YMARGIN+ 1 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 3 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 1 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{")",	')',	{(XMARGIN+ 4 *(XMARGIN+WIDTH)), (YMARGIN+ 1 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 4 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 1 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"4",	'4',	{(XMARGIN+ 0 *(XMARGIN+WIDTH)), (YMARGIN+ 2 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 0 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 2 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"5",	'5',	{(XMARGIN+ 1 *(XMARGIN+WIDTH)), (YMARGIN+ 2 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 1 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 2 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"6",	'6',	{(XMARGIN+ 2 *(XMARGIN+WIDTH)), (YMARGIN+ 2 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 2 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 2 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"/",	'/',	{(XMARGIN+ 3 *(XMARGIN+WIDTH)), (YMARGIN+ 2 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 3 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 2 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"*",	'*',	{(XMARGIN+ 4 *(XMARGIN+WIDTH)), (YMARGIN+ 2 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 4 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 2 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"1",	'1',	{(XMARGIN+ 0 *(XMARGIN+WIDTH)), (YMARGIN+ 3 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 0 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 3 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"2",	'2',	{(XMARGIN+ 1 *(XMARGIN+WIDTH)), (YMARGIN+ 3 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 1 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 3 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"3",	'3',	{(XMARGIN+ 2 *(XMARGIN+WIDTH)), (YMARGIN+ 3 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 2 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 3 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"-",	'-',	{(XMARGIN+ 3 *(XMARGIN+WIDTH)), (YMARGIN+ 3 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 3 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 3 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"+",	'+',	{(XMARGIN+ 4 *(XMARGIN+WIDTH)), (YMARGIN+ 3 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 4 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 3 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"0",	'0',	{(XMARGIN+ 0 *(XMARGIN+WIDTH)), (YMARGIN+ 4 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 0 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 4 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{".",	'.',	{(XMARGIN+ 1 *(XMARGIN+WIDTH)), (YMARGIN+ 4 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 1 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 4 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"<-",	'\b',	{(XMARGIN+ 2 *(XMARGIN+WIDTH)), (YMARGIN+ 4 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 2 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 4 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"c",	'c',	{(XMARGIN+ 3 *(XMARGIN+WIDTH)), (YMARGIN+ 4 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 3 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 4 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
		{"=",	'=',	{(XMARGIN+ 4 *(XMARGIN+WIDTH)), (YMARGIN+ 4 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 4 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 4 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}}
};
void CalculatorInit(void)
{
	// STEPIEN: Assume horizontal display

	// Initialize and turn on LCD and calibrate the touch panel
	BSP_LCD_Init();
	BSP_LCD_DisplayOn();
	BSP_TP_Init();

	// Display welcome message
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(5,5, (uint8_t*)"ELEC3730 Assignment 2",LEFT_MODE);
	BSP_LCD_DisplayStringAt(5,20, (uint8_t*)"Calculator Example",LEFT_MODE);

	// draw buttons
	for (int i = 0; i < 20; i++){
		draw_button(buttons[i]);
	}
	BSP_LCD_DrawRect(XMARGIN, YMARGIN, 320-(2*XMARGIN), HEIGHT);
}

void CalculatorProcess(void)
{
	char* debugmsg = (char*)malloc(sizeof(char)*100);
	// STEPIEN: Assume horizontal display

	uint16_t max_length = 20;
	uint16_t exp_length = 0;
	uint8_t* expression = (uint8_t*) malloc(8 * max_length);
	uint8_t last_char = ' ';

	while (1){
		// getDisplayPoint(&display, Read_Ads7846(), &matrix );
		if (BSP_TP_GetDisplayPoint(&display) == 0){
			for (int i = 0; i < NUM_OF_BUTTONS; i++){
				if (rect_collision(buttons[i].border, display.x, display.y)){
					expression[exp_length] = buttons[i].command;
					last_char = expression[exp_length];
					exp_length++;
				}
			}
		}
		if (exp_length + 2 > max_length){
			max_length *= 2;
			expression = (uint8_t*) realloc(expression, 8 * max_length);
		}
		if (last_char == 'c'){
			max_length = 20;
			exp_length = 0;
			expression = (uint8_t*) malloc(8 * max_length);
		}
		else if (last_char == '='){
			expression[exp_length] = '\0';
			break;
		}
	}
	int arg_count = 0;
	uint8_t* exp_args = parse_expression(exp_length, expression, &arg_count);
	double final_val = eval_expression(arg_count, exp_args);
	sprintf(debugmsg, "%lf", final_val);
	debug(debugmsg);
}
