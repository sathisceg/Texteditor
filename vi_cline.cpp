#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

#include "vi_lib.h"
#include "vi_ds_h.h"

#define gotoxy(x, y) printf("\033[%d;%dH", x, y)
#define clear_screen_fun() printf("\033[H\033[J")

char str[10];
char insert_mode;
char ch;

struct winsize wc;
int screen_widthc,screen_heightc;
int r,c;
bool go_to_normal;
int i=1;

static struct termios initial_settings, new_settings;

void set_mode_C()
{
	 gotoxy(screen_heightc,70);
	 printf("press -n- to go normal mode");
	 gotoxy(screen_heightc,screen_widthc-20);
	 printf("--command line--");
	 
}

void stay_in_line()
{
	gotoxy(screen_heightc-1,0);
}

void clear_line()
{
	gotoxy(screen_heightc,1);
	i=1;
	printf(" ");
	stay_in_line();
}



void init_keyboardc()
{
		tcgetattr(0,&initial_settings);
		new_settings = initial_settings;
		new_settings.c_lflag &= ~ICANON;
		new_settings.c_lflag |= ECHO;
		//new_settings.c_lflag &= ~ISIG;
		new_settings.c_cc[VMIN] = 1;
		new_settings.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &new_settings);

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &wc);
		screen_widthc = wc.ws_col;
		screen_heightc = wc.ws_row;

		set_mode_C();
		gotoxy(screen_heightc-1,0);
		go_to_normal = false;

		// printf("screen_widthc %d \n", screen_widthc);
		// printf("screen_heightc %d \n", screen_heightc);
}


void close_keyboardc()
{
		tcsetattr(0, TCSANOW, &initial_settings);
}

void  commandline_mode()
{


	int th = screen_widthc-20;
	//gotoxy(th,0);

	scanf("%[^\n]%*c",str);
	//printf("%s",str);
    char *token;
    char *rest = str;
    char *args[4];
 	int i=0;

 	clear_screen_fun();

    while ((token = strtok_r(rest, " ", &rest)))
    {
    	args[i]=token;
       // printf("%s\n", args[i]);
        i++;
    }

    args[i]=NULL;
    int j=0;


	pid_t pid;
   
	   	 pid = fork();
		if(pid==0)
		{
			if(execvp(args[0],args)==-1)
			{
					perror("exec");
			}
			
			gotoxy(th,0);			
		}
	


	if(pid > 0)
	{
		set_mode_C();
		gotoxy(screen_heightc-10,0);
		
		gotoxy(3,0);	

		while(1)
		{

			scanf("%c",&ch);

			
			if(ch==110)
			{

			     //printf("in n %d\n",ch);
				go_to_normal = true;
				//cl_to_normal(); //function defined in insert function 
				//ch='i';
				break;
			}
		}	
	}

}

char  start_cline()
{
	 ch = 'q';

	 init_keyboardc();
	 printf(":");
	 do
	 {


	 	scanf("%c",&ch);
	 	//printf("%d",ch);



	 	//printf("in do \n");
	 	if(ch==119)
	 	{
	 		//printf("save file\n");
	 		save_all_nodes();
	 		//go_to_normal = true;
	 		ch = 'q';

	 		//clear_screen_fun();
	 		break;
	 	}
	 	else if(ch==113)
	 	{
	 		//printf("%c",ch);

	 		scanf("%c",&ch);
	 		if(ch==33)
	 		{
	 			scanf("%c",&ch);
	 			gotoxy(screen_heightc-1,3);
	 			printf("%c",ch);
	 			save_all_nodes();
	 			ch = 'q';

	 			break;
	 			
	 		}
	 		else if (check_file_change())
	 		{
	 			gotoxy(screen_heightc-1,5);
	 			printf("file not saved since last changes\n");
	 		}
	 		else
	 		{
	 			ch = 'q';
	 			break;
	 		}
	 	}
	 	else if(ch==33)
	 	{
	 		//printf("command line\n");
	 		//printf("%c",ch);
	 		gotoxy(screen_heightc-1,3);
	 		commandline_mode();
	 	}
	 	else if(ch==110)
	 	{
	 		//printf("in start cline %d\n",ch);
	 		go_to_normal = true;
	 		//go_to_normal =false;
	 		break;
	 	}

		gotoxy(screen_heightc-1,2);


	 }while(1);


	//commandline_mode();
	 if(go_to_normal)
	 {	
	 	//printf("out cline mode\n");
	 	go_to_normal = false;
	 	cl_to_normal();
	 }

	

	close_keyboardc();

	return ch;
}