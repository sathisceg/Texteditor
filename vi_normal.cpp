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

#define clear_screen_fun() printf("\033[H\033[J")

#define gotoxy(x, y) printf("\033[%d;%dH", x, y)



void load_existing_file();

int screenrown = 0, screencoln = 0;
struct winsize w;	
int screen_width,screen_height;		
int a1,a2,a3;
char chn; 

bool file_load_before_insert = true;

char *file_name_n;
int total_lines;
FILE * fn;
int dlimit;



void set_file_name_n(char *filen)
{
    int l = strlen(filen);
    file_name_n = (char *)malloc(sizeof(char)*l);
    strcpy(file_name_n,filen);  

   // printf("file_name_n %s\n",file_name_n);
}

int get_row_n()
{
	return screenrown;
}

int get_col_n()
{
	return screencoln;
}

void set_mode_n()
{
	 gotoxy(screen_height,screen_width-20);
	 printf("--normal mode--");
	 gotoxy(0,0);
}

void clear_mode_n()
{
	 gotoxy(screen_height,screen_width-20);
	 printf("               ");
}

	
void init()
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	 screen_width = w.ws_col;
	 screen_height = w.ws_row;

	 screenrown = get_row_i();
	 screencoln = get_col_i();

	 
	 //gotoxy(1,0);
	  //set_mode_n();	
	  screenrown = 0;


}

void left()
{
		screencoln--;
 		if(screencoln<0)
			screencoln++;
		gotoxy(screenrown,screencoln);
}
// void down()
// {
// 		screenrown++;

// 		// if(screenrown > screen_height-3)
// 		// {
// 		// 	screenrown--;
// 		// }
// 		//gotoxy(screenrown,screencoln);

// 		if(screenrown<(total_lines-screen_height))
// 		{
// 			//screenrown++;
// 			int r = screen_height+screenrown;
// 			clear_screen_fun();
// 			gotoxy(0,0);
// 			load_from_ds(screenrown,r);
// 			set_mode_n();
// 		}	
// 		else
// 		{
// 			//gotoxy(screenrown,screencoln);
// 		}
			
			
// }

// void up()
// {
// 		if((screenrown-screen_height)==0)
// 		{
// 			gotoxy(0,0);
// 		}
// 		else if(screenrown>0)
// 		{
// 			screenrown--;
// 			int r = screenrown-screen_height;
// 			clear_screen_fun();
// 			gotoxy(0,0);
// 			load_from_ds(r,screenrown);
// 			set_mode_n();

// 		}
// 		// screenrown--;
// 		// if(screenrown<0)
// 		// 	screenrown++;

// 		// screenrown--;
// 		// if(screenrown<0)
// 		// {
// 		// 	screenrown++;
// 		// }
		

// 		// if((screenrown-(screen_height-3))>0 )
// 		// {
// 		// 	int r = screenrown-screen_height;
// 		// 	screenrown = screenrown-screen_height;
// 		// 	clear_screen_fun();
// 		// 	gotoxy(0,0);
// 		// 	load_from_ds(r,screenrown);
// 		// 	set_mode_n();
// 		// }
// 		// else
// 		// {
// 		// 	gotoxy(screenrown,screencoln);
// 		// }

// 		// if((screenrown-screen_height) >0 )
// 		// {
// 		// 	int r = screenrown-screen_height;
// 		// 	gotoxy(0,0);
// 		// 	load_from_ds(r,screenrown);
// 		// 	set_mode_n();
// 		// }
		
// }


void down()
{
		screenrown++;

		if(screenrown == total_lines)
			return;

		if(screenrown>(screen_height-2))
		{
			//screenrown--;

			clear_screen_fun();
			int ur = (screenrown-(screen_height-2));
			load_from_ds(ur,(screenrown));
			set_mode_n();
			gotoxy(screen_height-2,screencoln);
		}
		else
		{
			gotoxy(screenrown,screencoln);	
		}
		
}

void up()
{
		screenrown--;
		if(screenrown<0)
		{
			screenrown++;
			gotoxy(0,0);
		}
		else if(screenrown<(screen_height-2)) 
		{
			gotoxy(screenrown,screencoln);	
		}
		else
		{
			clear_screen_fun();
			int ur = (screenrown-(screen_height-2));
			load_from_ds(ur,(screenrown));
			set_mode_n();
			gotoxy(0,0);

		}

}

void right()
{
		screencoln++;
		if(screencoln>screen_width)
			screencoln--;
		gotoxy(screenrown,screencoln);
}


int getch(void)
{
	 int ch;
	 struct termios oldt;
	 struct termios newt;
	 tcgetattr(STDIN_FILENO, &oldt); 
	 newt = oldt; 
	 newt.c_lflag &= ~(ICANON | ECHO); 
	 tcsetattr(STDIN_FILENO, TCSANOW, &newt); 
	 ch = getchar(); 
	 tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
	 return ch; 
}

void normal_mode()
{


	fn = fopen(file_name_n, "rt");

	if(fn)
	{
		//file_load_before_insert = true;
		load_existing_file();
	}
	set_mode_n();
	

	do
	{
			 a1 = getch();

			 //printf("%d\n",a1);

			 if(a1 == 27 || a1==104 || a1==106 || a1==107 || a1==108)
			 {

			 	 if(a1==27)
			 	 {
			 	 	a2 = getch();
  				    a3 = getch();


  				     // printf("%d\n",a2);
  				     // printf("%d\n",a3);
  				     // printf("%d\n",a1);
  				 }




				 if(a1==107 || a3 == 65 )
				 {
				 	 up();
				 }
				 else if(a1==106 || a3 == 66)
				 {
				 	down();

				 }
				 else if(a1==108 || a3 == 67)
				 {
				 	 right();
				 }
				 else if(a1==104 || a3 == 68)
				 {
				 	 left();
				 }
				 a3=1;
			 }
			 else if(a1==114)
			 {
			 	//block to replace character 

			 	//printf("r pressed\n");
			 	char t;
			 	scanf("%c",&t);
			 	printf("%c",t);
			 	ds_impl(screenrown,screencoln,t);
			 	//print_nodes();			 	
			 }
			 else if(a1 ==103)
			 {
			 	//block to display first line from data structure

			 	int a4 = getch();
			 	if(a4==103)
			 	{
			 		clear_screen_fun();
			 		int fr = get_first_line(0,screen_height);
			 		set_mode_n();
			 		//gotoxy(0,0);
			 	}	
			 	
			 	//gotoxy(0,0);
			 }
			 else if(a1 ==71)
			 {
			 	//block to display last line from data structure

			 	//printf("last \n");
			 	clear_screen_fun();
			 	int lr = get_last_line();
			 	set_mode_n();
			 	gotoxy(4,0);
			 	// load_from_ds(total_lines-3,total_lines);
			 	// set_mode_n();
			 	// gotoxy(3,0);
			 	
			 }
			 else if(a1==105)
			 {

			 	//block to go insert mode

			 	//printf("insert mode\n");

			 	chn='i';

			 	clear_mode_n();

			 	break;
			 }
			 else if(a1==58)
			 {
			 	chn=':';
			 	break;
			 }



	} while(1);

}


void load_existing_file()
{
	
	//char ch;
	fn = fopen(file_name_n, "rt");
	if (fn)
	{

		load_file_data_into_ds();
		gotoxy(0,0);
		load_from_ds(0,screen_height);
		screenrown = get_row_ds();
		screencoln = get_col_ds();	
		//set_mode_n();	 
		total_lines  = get_row_ds();
		gotoxy(screenrown,screencoln);
	}
	else
	{
		//printf("file not exist\n");

	}

}


char  start_normal()
{
	//char ch = 'i';
	init();
	//set_mode_n();
	normal_mode();
	return chn;
}




/*
	to display content while coming from insert and command line mode
*/
void cl_to_normal()
{
	clear_screen_fun();
	gotoxy(0,0);
	normal_mode();	

}