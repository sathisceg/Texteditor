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

#include "vi_ds_h.h"
#include "vi_lib.h"


#define clear_screen_fun() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", x, y)

// #define up(n) printf("\033[%dA",n)
// #define down(n) printf("\033[%dB",n)
// #define right(n) printf("\033[%dC",n)
// #define left(n) printf("\033[%dD",n)


int check_line_no;
int screenrowi, screencoli;
static struct termios initial_settings, new_settings;
static int peek_character = -1;
struct winsize wi;
int screen_widthi,screen_heighti;
bool load_from_cl;
bool esc_press;
int minrow,maxrow;
bool check_changes=false;


bool check_file_change()
{
	return check_changes;
}
int get_row_i()
{
	return screenrowi;
}

int get_col_i()
{
	return screenrowi;
}

void set_mode_i()
{
	 gotoxy(screen_heighti,screen_widthi-20);
	 printf("--insert mode--");
	 gotoxy(0,0);
	 
}

void clear_mode_i()
{
	 gotoxy(screen_heighti,screen_widthi-20);
	 printf("               ");
}

void init_keyboardi()
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

void close_keyboardi()
{
		tcsetattr(0, TCSANOW, &initial_settings);
}

void initi()
{



		screenrowi = 1;
		screencoli = 1;
		
		esc_press = false;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &wi);
		screen_widthi = wi.ws_col;
		screen_heighti = wi.ws_row;
		minrow =0;
		maxrow = screen_heighti;

		init_keyboardi();	

		set_mode_i();	
		gotoxy(screenrowi,screencoli);




		screenrowi = get_row_n();
		screencoli= get_col_n();

		if(screenrowi>screen_heighti)
			screenrowi =0;
		//printf("row %d\n", screenrowi);
		//printf("col %d\n", screencoli);
		gotoxy(screenrowi,screencoli);
		check_line_no = 0;

}

int kbhit()
{
	char ch;
	int nread;

	if(peek_character != -1)
	return 1;
	
	
	new_settings.c_cc[VMIN]=0;
	tcsetattr(0, TCSANOW, &new_settings);
	
	nread = read(0,&ch,1);

	//printf("\nnread %s\n",nread);
	new_settings.c_cc[VMIN]=1;
	
	
	tcsetattr(0, TCSANOW, &new_settings);
	
	if(nread == 1) {
		peek_character = ch;
	return 1;
	}
	return 0;
}

int readch()
{

	char ch;	
	if(peek_character != -1) {
		ch = peek_character;
		peek_character = -1;
		return ch;
	}		
	return ch;
}


char  start_insert()
{
	char ch = 'c';


	initi();

	// if(load_from_cl==true)
	// {
	// 	load_from_cl==false;
	// 	printf("\033[H\033[J");
	// 	load_from_ds();
	// }


	do
	{
		if(kbhit()) 
		{				
			ch = readch();

			if(ch!=27)
				printf("%c",ch);


			if(screenrowi>(screen_heighti-1))
			{
				minrow++;
				maxrow++;
			}

			if(ch==127)
			{
				//block for backspace
				//printf("backspace pressed\n");

				 // screencoli--;
				 // if(screencoli<2)
				 // 	screencoli++;

				// gotoxy(screenrowi,screencoli-1);
				 //printf("%c",' ');


				//printf(" l %d %d %c\n",screenrowi,screencoli-1,ch);

				 ds_impl(screenrowi,screencoli-1,'$');
				 clear_screen_fun();
				 gotoxy(0,0);
				 load_from_ds(minrow,maxrow);
				 set_mode_i();

				 screencoli--;
				 if(screencoli<1)
				 {					 	
				 	screencoli++;
				 }
				 //screenrowi--;
				 gotoxy((screenrowi),screencoli);

				 check_changes = true;
				 continue;
			}
			else if(ch=='\n')
			{
				//block for enter key press
				++screenrowi;

				screencoli = 1;

				if(screenrowi>=(screen_heighti-2))
				{
					clear_screen_fun();

					int ur = (screenrowi-(screen_heighti));
					gotoxy(0,0);
					load_from_ds(ur,(screenrowi+1));
					set_mode_i();
					gotoxy(screen_heighti-2,screencoli);	
				}
				continue;
			}

		    else if(ch==27)
		    {
		    	//block for esc key to go normal mode

		    	
		    	esc_press = true;

		    	clear_mode_i();


		    	//printf("escape pressed\n");
		    	break;
		    }
		 	else
		 	{	
		        // printf("%c\n",ch);
		 		//put typed character into data structure
		 		check_changes = true;
		        ds_impl(screenrowi,screencoli,ch);
	            screencoli++;				 
	    	}
		}


		if(screencoli == screen_widthi)
		{
		 	++screenrowi;
		 	screencoli = 1;
		}




		if(screenrowi>=screen_heighti)
		{
			gotoxy(screen_heighti-2,screencoli);
		}
		else
		{	
			gotoxy(screenrowi,screencoli);
		}

	}while(1);



	
	
	if(esc_press)
	{
		ch = 'n';
		
		//load_from_ds(2,screenrowi);
		//cl_to_normal();
		esc_press =false;
	}
	
	
	close_keyboardi();

	return ch;
}


// void cl_to_normal()
// {
// 	clear_screen_fun();
// 	gotoxy(1,0);
// 	printf("from cl to normal\n");
// 	//printf("load_from_cl %d",load_from_cl==true);
// 	//load_from_cl = true

// 	//printf("%d %d\n",screenrowi,screencoli);
// 	load_from_ds(0,screenrowi);
// 	set_mode_n();

// 	//	printf("load_from_cl %d",load_from_cl==true);


// }