/*
20172092
sathiskumar
*/

#include <stdio.h>
#include <iostream>
#include <map>
#include <iterator>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

map <int, struct Node*> list;
FILE *f;
char *file_name_ds;
int screen_widthd,screen_heightd;
struct winsize wd;
int limit;

int row_ds;
int col_ds;

int get_row_ds()
{
    return row_ds;
}

int get_col_ds()
{
    return col_ds;
}

void load_file_data_into_ds();

void set_file_name_ds(char *filen)
{
    int l = strlen(filen);
    file_name_ds = (char *)malloc(sizeof(char)*l);
    strcpy(file_name_ds,filen);  

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wd);
    screen_widthd = wd.ws_col;
    screen_heightd = wd.ws_row;
    limit = screen_heightd-3;

    //printf("file_name_ds %s\n",file_name_ds);
}

/*
node structure for storing characters
*/
struct Node
{
    int data;
    int r;
    int c;
    struct Node *next;
    struct Node *prev;
};



/*
function for adding character to data structure 
when typed  in insertion mode
*/
struct Node * add(struct Node *head_ref, int r,int c,int new_data)
{
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    new_node->data  = new_data;
    new_node->r  = r;
    new_node->c  = c;

     struct Node*  temp = head_ref;
 
    if (head_ref == NULL)
        head_ref = new_node; 

    else if (head_ref->c > new_node->c) {
        new_node->next = head_ref;
        new_node->next->prev = new_node;
        head_ref = new_node;
        return head_ref;
    }
    else
    {

      
 
        // locate the node after which the new node
        // is to be inserted
        while (temp->next != NULL && 
               temp->next->c < new_node->c)
            temp = temp->next;
 
        /* Make the appropriate links */
        new_node->next = temp->next;
 
        // if the new node is not inserted
        // at the end of the list
        if (temp->next != NULL)
            new_node->next->prev = new_node;
 
        temp->next = new_node;
        new_node->prev = temp;
        return head_ref;
    }
}


/*
function for deleting character from data structure 
when typed in insertion mode
*/
struct Node * remove(struct Node *head_ref, int r,int c)
{

   

    if(head_ref == NULL)
        return NULL;

    else
    { 

            struct Node *temp =NULL;
            if (head_ref->c == c) 
            {
                
                temp = head_ref;
                
                if(head_ref->next!=NULL)
                {        
                    head_ref = head_ref->next;            
                    head_ref->prev = NULL;
                }
                    
                free(temp);
                return head_ref;
            }
            else
            {

               temp = head_ref;
         
                // locate the node after which the new node
                // is to be inserted
                while (temp->next != NULL &&   temp->next->c < c)
                    temp = temp->next;


                if(temp->next->next == NULL)
                {
                    struct Node *dnode = temp->next->next;
                    free(dnode);
                    temp->next =NULL;
                }
                else
                { 
                    struct Node *dnode = temp->next;

                    temp->next  = temp->next->next;
                    temp->next->prev = temp;

                    free(dnode);
                }
                return head_ref;
              }
       } 


}

void printList(struct Node *node)
{
    struct Node *last;


    //printf("\nTraversal in forward direction \n");

    //fprintf(f, "some%s", "Traversal in forward direction");

    while (node != NULL)
    {

       // printf("print\n");

       // printf("%c", (char)(node->data));

        if(((node->data)>=97&&(node->data)<=122)||((node->data)>=65&&(node->data)<=90))
        {
        	//fprintf(f, "welcome!! %c\n", node->data);	
            printf("%c", node->data);
        }
        else
        {
        	// fprintf(f, "%d", node->data);
             printf("%d ",(node->data));
        }

        last = node;
        node = node->next;
    }
 
    printf("\n");
   
}



/*
function for saving data from data structure to file
from command line mode
*/
void save_to_file(struct Node *node)
{
    struct Node *last;

    //printf("\nTraversal in forward direction \n");
    //fprintf(f, "some%s", "Traversal in forward direction");
    while (node != NULL)
    {
       // printf("print\n");
       // printf("%c", (char)(node->data));
        if(((node->data)>=97&&(node->data)<=122)||((node->data)>=65&&(node->data)<=90))
        {
            fprintf(f, "%c", node->data); 
           // printf("%c", node->data);
        }
        else
        {
             fprintf(f, "%d", node->data);
            // printf("%d ",(node->data));
        }
       
        node = node->next;
    }
    fprintf(f, "%c", '\n'); 
    //printf("\n");   

   //load_file_data_into_ds();
}

/*
function to invoke add or delete from data structure
in insertion mode
*/

void ds_impl(int r,int c,char ch)
{
	//printf("%d %d  %c  \n",r,c,ch);
		if(ch=='$')
		{
			//printf("%d %d\n",r,c);

			 struct Node *temp;
	         temp = list[r];

             // if(temp==NULL)
             //    printf("null node\n");	
             if(temp!=NULL)
             {
                list[r] = remove(temp,r,c);
             } 
             else
             {
                return;
             }        
			 
		}	
		else
		 {	

            // printf("%d  \n", r);
            // printf("%d  \n", c);
            // printf("%c  \n", ch);

		 	if(list.count(r)==0)
	     	{
	     
	            //printf("\nt1 adding %d",ch);

	            struct Node *head =NULL;
	            list[r] = add(head,r,c,ch);
	          
	        }
	        else
	        {

	           // printf("\nt2 adding %d",ch);
	            struct Node *temp;
	            temp = list[r];
	            list[r] = add(temp,r,c,ch);



	        }
    	}


	

}


/*
function to invoke saving a file
calls save_to_file for each node.
*/

void save_all_nodes()
{
	f = fopen(file_name_ds, "w");
	

	if (f == NULL)
	{
    	printf("Error opening file!\n");
	}

	//fprintf(f, "some%s", "printnodes");

	//printList(list[4]);


    for(int i=0;i<list.size();i++)
    {
       // printf("i %d \n",i);
        struct Node *stemp  = list[i];
        if(stemp==NULL)
        {
            fprintf(f, "%c", '\n'); 
            continue;
        }
        save_to_file(stemp);
       // printf("\n");
    }

	fclose(f);

}

/*
function to display  only particular range of data structure
using start and end  limits of map
*/
void load_from_ds(int start ,int end)
{




    for(int i=start;i<=end;i++)
     {


            struct Node *node =list[i] ;  

            while (node != NULL)
            {

                if((node->data)>=97&&node->data<=122 || node->data==32)
                {
                    //fprintf(f, "welcome!! %c\n", node->data); 
                    printf("%c", node->data);
                }
                else
                {
                     //fprintf(f, "%d", node->data);
                     printf("%c ",(node->data));
                }
                //last = node;
                node = node->next;
               
            }

        

         printf("\n");
     }   
   

}



/*
function to load data into text buffer 
when the file is already available 

called from normal mode.
*/

void load_file_data_into_ds()
{

	//printf("load_file_data_into_ds\n");


	//map <int, struct Node*> list; 

    FILE *f;
    char ch;
    f=fopen(file_name_ds,"rt");

    // if (f)
    // {

    //     printf("file exist\n");
         
    // }
    
    map <int, struct Node*> list1; 
    struct Node * temp=NULL;

    int i=0;
    int r=0,c=0;
    while((ch=fgetc(f))!=EOF)
    {

        if(ch=='\n')
        {
            
           // printf("new line\n");
                
                r++;
                c=0;
            continue;
        }

        if(list1.count(r)==0)
        {
            //check line is already read else read
                       
                struct Node *head =NULL;
                list1[r] = add(head,r,c,ch);

        }
        else
        {
            //add character to node
             c++;
            temp = list1[r];
            list1[r] = add(temp,r,c,ch);
           
        }
        
    }

    row_ds = r;
    col_ds = c;
    fclose(f);

   // printf("\nloaded");
   // printf("\nsize%d\n",list1.size());

    list = list1;

    // for(int i=0;i<list.size();i++)
    // {
    //    // printf("i %d \n",i);
    //     printList(list[i]);
    //    // printf("\n");
    // }


    // printf("\nsize%d\n",list.size());

}



/*
to display first line of a file
*/

int get_first_line(int start,int end)
{


   // printf("%d\n",list.size());

    load_from_ds(0,end);

    return 0 ;
}


/*
to display last line of a file
*/

int get_last_line()
{

    //printf("%d\n",list.size());

    int sl = (list.size())-3; 
    load_from_ds(sl,list.size());

    return list.size();

}