//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"

#define WIDTH 55
#define XBUTTONS 5
#define HEIGHT 40
#define YBUTTONS 5
#define XMARGIN (320-(XBUTTONS*WIDTH))/(XBUTTONS+1)
#define YMARGIN (240-(YBUTTONS*HEIGHT))/(YBUTTONS+1)
#define NUM_OF_BUTTONS (XBUTTONS*(YBUTTONS-1))

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
	uint8_t* symbol;
	uint8_t command;
	Rectangle border;
	Rectangle text;
} Button;

void dout(const char* str){
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
	BSP_LCD_DisplayStringAt(b.text.x, b.text.y, b.symbol, LEFT_MODE);
}

uint8_t* parse_expression(uint8_t exp_length, uint8_t* exp, int* argcout){
	char* debugmsg = (char*)malloc(sizeof(char)*100);
	sprintf(debugmsg, "parse_expression entered\n length : %d\n", exp_length);
	dout(debugmsg);
	uint8_t* exp_args_lengths = (uint8_t*)malloc(exp_length * 8);
	uint8_t* exp_args_symbol = (uint8_t*)malloc(exp_length * 8);
	int arg_length = 0;
	int num_args = 0;
	for (int i = 0; i < exp_length; i++){
		sprintf(debugmsg, "exp : %d : %c arg_len : %d num_args : %d\n", i, exp[i], arg_length, num_args);
		dout(debugmsg);
		if (	exp[i] != '+' &&
				exp[i] != '-' &&
				exp[i] != '*' &&
				exp[i] != '/' &&
				exp[i] != '(' &&
				exp[i] != ')' &&
				exp[i] != '=' ){	//this char is a number
			sprintf(debugmsg, "number found\n");
			dout(debugmsg);
			arg_length++;
			if (!(	exp[i+1] != '+' &&
					exp[i+1] != '-' &&
					exp[i+1] != '*' &&
					exp[i+1] != '/' &&
					exp[i+1] != '(' &&
					exp[i+1] != ')' &&
					exp[i+1] != '=' )){		// next char is not a number
				sprintf(debugmsg, "ending number\n");
				dout(debugmsg);
				exp_args_lengths[num_args] = arg_length;
				exp_args_symbol[num_args] = 0;
				arg_length = 0;
			}
		}
		else{
			sprintf(debugmsg, "symbol found\n");
			dout(debugmsg);
			exp_args_lengths[num_args] = 1;
			exp_args_symbol[num_args] = 1;
		}
		if (arg_length == 0){
			num_args++;
		}
	}

	for (int i = 0; i < num_args; i++){
		sprintf(debugmsg, "arg %d length %d\n", i, exp_args_lengths[i]);
		dout(debugmsg);
	}

	sprintf(debugmsg, "number of lengths found: %d, starting tokenising\n", num_args);
	dout(debugmsg);
	//now have list of how long args are
	uint8_t** exp_args = (uint8_t**) malloc(num_args*sizeof(uint8_t));
	int pos = 0;
	for (int i = 0; i < num_args; i++){
		sprintf(debugmsg, "pos: %d exp: %c\n", pos, exp[pos]);
		dout(debugmsg);
		if (exp_args_symbol[i] == 1){
			exp_args[i] = (uint8_t*) malloc(sizeof(uint8_t));
			exp_args[i][0] = exp[pos];
			pos++;
			sprintf(debugmsg, "symbol parsed %c\n", exp_args[i][0]);
			dout(debugmsg);
		}
		else{
			sprintf(debugmsg, "Tokenising number, length: %d\n", exp_args_lengths[i]);
			dout(debugmsg);
			exp_args[i] = (uint8_t*) malloc(sizeof(uint8_t) * exp_args_lengths[i]+1);
			for (int j = 0; j < exp_args_lengths[i]; j++){
				sprintf(debugmsg, "pos: %d exp: %c\n", pos, exp[pos]);
				dout(debugmsg);
				exp_args[i][j] = exp[pos];
				pos++;
			}
			exp_args[i][exp_args_lengths[i]] = '\0';
			sprintf(debugmsg, "number tokenised %s\n", exp_args[i]);
			dout(debugmsg);
		}
	}

	for (int i = 0; i < num_args; i++){
		sprintf(debugmsg, "arg %d : ", i);
		dout(debugmsg);
		for (int j = 0; j < exp_args_lengths[i]; j++){
			sprintf(debugmsg, "%c", exp_args[i][j]);
			dout(debugmsg);
		}
		sprintf(debugmsg, "\n");
		dout(debugmsg);
	}

	sprintf(debugmsg, "Finished tokenising args, Starting parse");
	dout(debugmsg);

	double* exp_array = (double*)malloc(sizeof(double) * num_args);
	for (int i = 0; i < num_args; i++){
		sprintf(debugmsg, "arg: %d\n", i);
		dout(debugmsg);
		if (!exp_args_symbol[i]){
			sprintf(debugmsg, "Parsing number %s\n", exp_args[i]);
			dout(debugmsg);
			exp_array[i] = strtod(exp_args[i]);
		}
		else {
			sprintf(debugmsg, "Parsing symbol %c\n", exp_args[i]);
			dout(debugmsg);
			exp_array[i] = (double) exp_args[i][0];
		}
		sprintf(debugmsg, "Parsed as %lf\n", exp_array[i]);
		dout(debugmsg);
	}
	sprintf(debugmsg, "FINAL ARGUEMENTS\n");
	dout(debugmsg);
	for (int i = 0; i < num_args; i++){
		sprintf(debugmsg, "(%lf : %c) ", exp_array[i], (char) exp_array[i]);
		dout(debugmsg);
		if (i%5 == 0){
			sprintf(debugmsg, "\n");
			dout(debugmsg);
		}
	}

	*argcout = num_args;
	return exp_array;
}

double eval_expression(uint8_t argc, uint8_t* args){
	char* debugmsg = (char*)malloc(sizeof(char)*100);
	uint8_t wordcount = argc;
	uint8_t brackdone = 0;
	uint8_t muldivdone = 0;
	uint8_t addsubdone = 0;
	uint8_t* newargs;
	int j = 0;
	while (wordcount > 1){
		sprintf(debugmsg, "New Pass, wordcount: %d\n", wordcount);
		dout(debugmsg);
		uint8_t brackflag = 0;    //flags are set to 1 by the end if their respective operation occured.
		uint8_t muldivflag = 0;
		uint8_t addsubflag = 0;
		for (int i = 0; i < wordcount; i++){    // looping through all arguments left to process
			sprintf(debugmsg, "i: %d number: %lf %c\n", i, args[i], (char)args[i]);
			dout(debugmsg);
			j = 0;                      // second iterator to construct the next set of arguments
			newargs = (uint8_t*) malloc (8 * wordcount);    //allocating more than enough space for new arguements;
			if (!brackdone){                // handles brackets
				sprintf(debugmsg, "Bracket Pass\n");
				dout(debugmsg);
				if (args[i] == '('){        // looking for open of bracketed section
					sprintf(debugmsg, "Sub-Expression found\n");
					dout(debugmsg);
					int sublen = 1;         //sub-expression length
					int depthcount = 0;    // incremented if another open bracket is found and decremented when closed (-1 because the first open bracket will make it 0)
					while (!(args[i+sublen] != ')' && depthcount != 0)){    // counting the length of the section in brackets
						sprintf(debugmsg, "Bracket Pass\n");
						dout(debugmsg);
						if (args[i+sublen] == '('){
							sprintf(debugmsg, "depth count increased: %d\n", depthcount);
							dout(debugmsg);
							depthcount++;
						}
						else if (args[i+sublen] == ')'){
							sprintf(debugmsg, "Depth count decreased: %d\n", depthcount);
							dout(debugmsg);
							depthcount--;
						}
						sublen++;
					}
					uint8_t* subargs = (uint8_t*) malloc(8*sublen+1);	// allocating space for the contence inside the brackets
					for (int k = 0; k < sublen; k++){	       // copying contence in to the sub-expression arguments array
					    subargs[k] = args[i];
					    i++;
					}
					subargs[sublen] = '\0';
					sprintf(debugmsg, "Substring complete: %s\n", subargs);
					dout(debugmsg);
					newargs[j] = eval_expression(sublen, subargs);    // recursive call to evaluate the sub-expression
					sprintf(debugmsg, "Substring evaluated: %d\n", newargs[j]);
					dout(debugmsg);
					free(subargs);
					brackflag = 1;
					j++;
				}
			}
			else if (!muldivdone){
				sprintf(debugmsg, "MulDiv Pass\n", exp_args[i]);
				dout(debugmsg);
				if (args[i] == '*'){
					sprintf(debugmsg, "Mul found\n");
					dout(debugmsg);
					newargs[j] = newargs[j] * args[i+1];
					muldivflag = 1;
					i++;
					j++;
				}
				else if (args[i] == '/'){
					sprintf(debugmsg, "Div found\n");
					dout(debugmsg);
					newargs[j] = newargs[j] / args[i+1];
					muldivflag = 1;
					i++;
					j++;
				}
				else{
					sprintf(debugmsg, "MulDiv not found\n");
					dout(debugmsg);
					newargs[j] = args[i];
					j++;
				}
			}
			else{
				sprintf(debugmsg, "AddSub Pass %c\n");
				dout(debugmsg);
				if (args[i] == '+'){
					sprintf(debugmsg, "Add found\n");
					dout(debugmsg);
					newargs[j] = newargs[j] + args[i+1];
					addsubflag = 1;
					i++;
					j++;
				}
				else if (args[i] == '-'){
					sprintf(debugmsg, "Sub found\n", args[i]);
					dout(debugmsg);
					newargs[j] = newargs[j] - args[i+1];
					addsubflag = 1;
					i++;
					j++;
				}
				else{
					sprintf(debugmsg, "AddSub found\n", args[i]);
					dout(debugmsg);
					newargs[j] = args[i];
					j++;
				}
			}
		}
		if (muldivdone && !addsubflag){
			sprintf(debugmsg, "AddSub Done\n");
			dout(debugmsg);
			addsubdone = 1;
		}
		if (brackdone && !muldivflag){
			sprintf(debugmsg, "DivMul Done\n");
			dout(debugmsg);
			muldivdone = 1;
		}
		if (!brackflag){
			sprintf(debugmsg, "Brackets done\n");
			dout(debugmsg);
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
		{" ",	' ',	{(XMARGIN+ 2 *(XMARGIN+WIDTH)), (YMARGIN+ 4 *(YMARGIN+HEIGHT)), WIDTH, HEIGHT}, {(XMARGIN+ 2 *(XMARGIN+WIDTH))+(WIDTH/2)+ 0 , (YMARGIN+ 4 *(YMARGIN+HEIGHT))+(HEIGHT/2)+ 0 , 0, 0}},
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
	printf("%c %c %c %c %c %c", (char)(double) '+', (char)(double) '-', (char)(double) '*', (char)(double) '/', (char)(double) '(', (char)(double) ')');
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
	static uint8_t bounceUp = 0;
	static uint8_t bounceDown = 0;
	while (1){
		sprintf(debugmsg, "new expression\n");
		dout(debugmsg);
		uint8_t pressed = 0;
		while (1){
			// getDisplayPoint(&display, Read_Ads7846(), &matrix );
			uint8_t last_char = ' ';
			if (BSP_TP_GetDisplayPoint(&display) == 0 && pressed == 0){
				bounceDown++;
				if (bounceDown == 25){
					pressed = 1;
					bounceDown = 0;
					for (int i = 0; i < NUM_OF_BUTTONS; i++){
						if (rect_collision(buttons[i].border, display.x, display.y)){
							sprintf(debugmsg, "button pressed at: %d %d : %c\n", display.x, display.y, buttons[i].command);
							dout(debugmsg);
							expression[exp_length] = buttons[i].command;
							expression[exp_length+1] = '\0';
							sprintf(debugmsg, "expression: %s\n", expression);
							dout(debugmsg);
							last_char = expression[exp_length];
							exp_length++;
						}
					}
				}
			}
			else if (BSP_TP_GetDisplayPoint(&display) == 1 && pressed == 1){
				bounceUp++;
				if (bounceUp == 25){
					bounceUp = 0;
					pressed = 0;
				}
			}
			if (exp_length + 2 > max_length){
				sprintf(debugmsg, "doubling expression length");
				dout(debugmsg);
				max_length *= 2;
				expression = (uint8_t*) realloc(expression, 8 * max_length);
			}
			if (last_char == 'c'){
				sprintf(debugmsg, "clearing expression");
				dout(debugmsg);
				max_length = 20;
				exp_length = 0;
				expression = (uint8_t*) malloc(8 * max_length);
				expression[0] = '\0';
			}
			else if (last_char == '='){
				sprintf(debugmsg, "equals sign found!!!");
				dout(debugmsg);
				expression[exp_length+1] = '\0';
				break;
			}
		}
		sprintf(debugmsg, "loop broken\nexpression: %s\n", expression);
		dout(debugmsg);
		int arg_count = 0;
		uint8_t* exp_args = parse_expression(exp_length-1, expression, &arg_count);
		double final_val = eval_expression(arg_count, exp_args);
		sprintf(debugmsg, "%lf", final_val);
		dout(debugmsg);
	}
}
