/*
20172092
sathiskumar
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>


#include "vi_lib.h"





#define clear_screen_fun() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", x, y)


	



// FILE *input;
// FILE *output;
// FILE *in;
// char *cursor, *clear;
// static FILE *output_stream = (FILE *)0;

char *file_name;

int screenrow =0, screencol = 0;
int choice = 0;
static struct termios initial_settings, new_settings;
char current_mode = 'n';

void init_keyboard()
{
		tcgetattr(0,&initial_settings);
		new_settings = initial_settings;
		new_settings.c_lflag &= ~ICANON;
		new_settings.c_lflag &= ~ECHO;
		//new_settings.c_lflag &= ~ISIG;
		new_settings.c_cc[VMIN] = 1;
		new_settings.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
		tcsetattr(0, TCSANOW, &initial_settings);
}


void initialize()
{

		screenrow = 0;
		screencol = 0;		
		// input = fopen("/dev/tty", "r");
		// output = fopen("/dev/tty", "w");
		
		// if(!input || !output) {
		// 		fprintf(stderr,"Unable to open /dev/tty\n");
		// 	exit(1);
		// }	
		// output_stream = output;
		// in = input;
		//ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		//screen_width = w.ws_col;
		//screen_height = w.ws_row;

		init_keyboard();	
		clear_screen_fun();
		gotoxy(screenrow,screencol);
		//printf("%s",file_name);
		screenrow =0;
		screencol=0;
		gotoxy(screenrow,screencol);


}


int main(int argc,char *argv[])
{
	
	


	if(argv[1]==NULL)
	{
		printf("give file name");
		return 0;
	}

	int l = strlen(argv[1]);

	file_name = (char *)malloc(sizeof(char)*l);
	strcpy(file_name,argv[1]);


	//printf("%s\n",file_name);

	set_file_name_ds(file_name);
	set_file_name_n(file_name);


	initialize();
	// char ch;
	// do
	// {
	// 	scanf("%c",&ch);

	// }while(ch!='q');

	//printf("1\n");

	do
	{
		
		//scanf("%c",&ch);
		if(current_mode == 'n')
		{
			
			//printf("going to normal\n");

			current_mode = start_normal();


		}
		else if(current_mode == 'i')
		{
			
			
			current_mode = start_insert();
		}
		else if(current_mode == ':')
		{

			//printf("in main cl\n");

			current_mode = start_cline();
		}



		//scanf("%c",&current_mode);



	}while(current_mode!='q');

	clear_screen_fun();
	close_keyboard();
	

	return 0;
}